#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<algorithm>
#include<unordered_map>
#include<cmath>

#include "Breaker.h"

// Initialize frequency chart
std::unordered_map<std::string, double> Breaker::frequencyChart;

// Default Constructor
Breaker::Breaker()
{}

// String Constructor (sets ciphertext)
Breaker::Breaker(const std::string& ciphertext)
	: ciphertext(ciphertext)
{}

// Two-String Constructor (sets ciphertext and underlying cipher)
Breaker::Breaker(const std::string& ciphertext, const std::string& cipher)
	: ciphertext(ciphertext)
{
	setCipher(cipher);
}

// String, String Constructor (sets ciphertext, then Translator's key)

// Sets frequency dictionary from a passed-in file stream, in format PAIR[space]FREQUENCY
std::ifstream& Breaker::setFreq(std::ifstream& freqStream) {
	frequencyChart.reserve(400000);
	if (freqStream) {
		std::cout << "File was found.\n";
		while (!freqStream.eof()) {
			std::string ngram;
			double freq;
			freqStream >> ngram >> freq;
			frequencyChart[ngram] = freq;
		}

		// Set each frequency to its log probability
		// First, get total # of n-grams by summing all frequencies
		double nGramSum = 0;
		for (const auto& entry : frequencyChart)
			nGramSum += entry.second;

		std::cout << "Total sum: " << nGramSum << "\n";

		// Next, iterate through and change all frequencies to their log probabilities: log(freq/total)
		for (auto iter = frequencyChart.begin(); iter != frequencyChart.end(); ++iter) {
			iter->second = log(iter->second / nGramSum);
		}
	}
	else
		std::cout << "FILE NOT FOUND\n";
	return freqStream;
}

// Sets ciphertext to passed-in string
void Breaker::setText(const std::string& newText) {
	ciphertext = newText;
}

// Swaps two random values in cipher (requires seeding rand in main)
void Breaker::swapVals() {
	// Get two random (different) letters
	char letter1 = 'A' + (rand() % decryptDict.size());
	char letter2 = 'A' + (rand() % decryptDict.size());
	// Keep trying until they're different
	while (letter1 == letter2) {
		letter2 = 'A' + (rand() % 26);
	}

	// Get iterators to those letters in decryptDict
	auto iter1 = decryptDict.find(letter1);
	auto iter2 = decryptDict.find(letter2);

	// Swap their values
	std::swap(iter1->second, iter2->second);
	// Put those new keys into the corresponding values in encryptDict
	encryptDict.at(iter1->second) = iter1->first;
	encryptDict.at(iter2->second) = iter2->first;
	
	return;
}

// Decodes ciphertext and scores it based on frequency dict
double Breaker::score() {
	// Decode ciphertext using current decryptDict
	std::string workingDecrypt = decrypt(ciphertext);
	// Remove spaces
	for (auto iter = workingDecrypt.begin(); iter != workingDecrypt.end(); ++iter) {
		if (*iter == ' ')
			workingDecrypt.erase(iter);
	}
	//std::cout << "\nWorking Decrypt: " << workingDecrypt << "\n";

	// Current score
	double score = 0;

	// Get the size of the n-gram as a size_t in a really hacky way
	// THIS MEANS IT ONLY SUPPORTS FREQUENCY CHARTS WITH ALL SUBSTRINGS THE SAME SIZE
	auto iter = frequencyChart.begin();
	size_t gramSize = iter->first.size();

	// For each substring that's gram-sized in the working decryption...
	for (size_t i = 0; i < workingDecrypt.size() - gramSize + 1; ++i) {
		// Get the substring
		std::string subgram = workingDecrypt.substr(i, gramSize);
		// Look it up in the frequency table
		auto theGramIt = frequencyChart.find(subgram);
		// If we found it
		if (theGramIt != frequencyChart.end()) {
			// Add its log probability to the score (summing log probabilities = multiplying probabilities)
			score += theGramIt->second;
		}
		// If we didn't find it
		else {
			// Add a floored log probability to the score (1/9E8 ~~ totalGrams)
			score += log(1.0 / 900000000.0);
		}
	}
	
	return score;
}

void Breaker::singleFreqAttack() {
	// Create multimap of frequencies of English characters
	std::multimap<double, char> engMap = std::multimap<double, char>();
	// Create another multimap for frequencies in the decoded string
	std::multimap<double, char> textMap = std::multimap<double, char>();

	// Read in english letters from monogram-freq.txt
	std::ifstream monoStream("monogram-freq.txt");
	// Add letters to the multimap, keyed (and therefore ordered) by their frequency
	while (!monoStream.eof()) {
		char theChar;
		double theFreq;
		monoStream >> theChar >> theFreq;
		engMap.insert( std::make_pair(theFreq, theChar) );
	}

	/*
	std::cout << "English Frequency Map: {\n";
	for (auto iter = engMap.rbegin(); iter != engMap.rend(); ++iter)
	std::cout << "Frequency: " << iter->first << ", Character: " << iter->second << "\n";
	std::cout << "}\n";	
	*/

	// For each character in the alphabet
	for (char ch = 'A'; ch <= 'Z'; ++ch) {
		// Count that letter's frequency in the ciphertext
		double letFreq = 0;
		for (auto iter = ciphertext.begin(); iter != ciphertext.end(); ++iter)
			if (*iter == ch) {
				++letFreq;
			}
		// Add that letter to the text multimap, sorted by its frequency
		textMap.insert(std::make_pair(letFreq, ch));
	}

	/*
	std::cout << "Cipher Text Frequency Map: {\n";
	for (auto iter = textMap.rbegin(); iter != textMap.rend(); ++iter)
		std::cout << "Frequency: " << iter->first << ", Character: " << iter->second << "\n";
	std::cout << "}\n";
	*/

	// Go through both maps at the same time, in order from biggest to smallest (i.e. using r-iterators)
	auto engIter = engMap.rbegin();
	auto textIter = textMap.rbegin();
	for (; engIter != engMap.rend() && textIter != textMap.rend(); ++engIter, ++textIter) {
		// Get the engMap character and the textMap character at that frequency order
		char engChar = engIter->second;
		char textChar = textIter->second;

		// In decryptDict, make the textChar map to the correct engChar
		decryptDict.at(textChar) = engChar;
		// Make sure encryptDict matches the identity
		encryptDict.at(engChar) = textChar;
	}
}

std::string Breaker::breakCipher(std::string toDecode, size_t trials) {
	// Set text as the inputted string
	setText(toDecode);

	// Start with a single-frequency attack
	singleFreqAttack();

	// Set best score to whatever the current key decodes as
	double bestScore = score();
	// Set best key to the current cipher
	std::string bestKey = getCipher();

	// For as many trials as input,
	for (size_t i = 0; i < trials; ++i) {
		// Set new key to a random two-value swap of the current translator
		swapVals();
		std::string newKey = getCipher();
		// Score the new key
		double newScore = score();

		// If the new score was higher than (or equal to) the best score, set the key to the best key
		if (newScore >= bestScore) {
			bestScore = newScore;
			bestKey = newKey;
		}
		
		// Reset cipher to best key
		setCipher(bestKey);

		// If we're at a multiple of 100, check progress
		if (i % 100 == 0) {
			std::cout << "Trial " << i << "\n";
			std::cout << "Best Score: " << bestScore << "\n";
			std::cout << "Best Key: " << bestKey << "\n";
			std::cout << "Decoded Text:" << decrypt(ciphertext) << "\n";
			std::cout << "--------------------\n";
		}
	}
	
	// Return (and set as cipher) the best key we found
	setCipher(bestKey);
	return bestKey;
}