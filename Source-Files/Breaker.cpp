#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<algorithm>
#include<unordered_map>

#include "Breaker.h"

// Initialize frequency chart
std::unordered_map<std::string, long long> Breaker::frequencyChart;

// HELPER FUNCTION:
// Converts uppercase letter to a size_t index (A = 0, B = 1... Z = 25)
size_t letterToIndex(char C) {
	return static_cast<size_t>(C) - 65;
}

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
	if (freqStream) {
		std::cout << "File was found.\n";
		while (!freqStream.eof()) {
			std::string bigram;
			long long value;
			freqStream >> bigram >> value;
			frequencyChart[bigram] = value;
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
long long Breaker::score() {
	// Decode ciphertext using current decryptDict
	std::string workingDecrypt = decrypt(ciphertext);
	// Remove spaces
	std::remove_if(workingDecrypt.begin(), workingDecrypt.end(), isspace);

	/*
	CURRENTLY, I DON'T REMOVE SPACES FROM THE WORKING DECRYPTION.
	THIS MAY AFFECT BIGRAM FREQUENCY DATA, DEPENDING ON HOW BIGRAM FREQUENCY WAS COMPUTED IN MY DATA SET.

	HOWEVER, I'M ASSUMING BIGRAMS /WITHIN/ WORDS ARE MORE FREQUENT (AND STATISTICALLY SIGNIFICANT) THAN THOSE /BETWEEN/ WORDS,
	AND THEREFORE NOT REMOVING SPACES WOULD LIKELY MAKE CLOSER-TO-ENGLISH TEXT SCORE HIGHER.
	*/

	// Current score
	long long score = 0;

	// For each element in frequencyChart
	for (const auto& entry : frequencyChart) {
		// Create working tally of number of times it appears
		int tally = 0;
		// Find first instance
		size_t thePos = workingDecrypt.find(entry.first, 0);

		while (thePos != std::string::npos) {
			++tally;
			thePos = workingDecrypt.find(entry.first, thePos + 1);
		}

		/*
		// Create working string to chop down as we find elements
		std::string chopped = workingDecrypt;
		// Search for the substring in the string and add to tally every time it appears
		while (size_t pos = chopped.find(entry.first) != std::string::npos) {
			std::cout << "Current chopped string: " << chopped << std::endl;
			chopped.erase(pos, entry.first.size());
			++tally;
		}
		*/

		// Multiply tally by that entry's frequency and add it to the score
		score += tally * entry.second;
	}

	// Return final score
	return score;
}

void Breaker::singleFreqAttack() {
	std::vector<std::pair<char, long long>> freqVec(26);
	// Read in english letters from monogram-freq.txt
	std::ifstream monoStream("monogram-freq.txt");
	while (!monoStream.eof()) {
		for (size_t i = 0; i < freqVec.size(); ++i)
			monoStream >> freqVec[i].first >> freqVec[i].second;
	}

	/*
	std::cout << "\n{";
	for (auto& x : freqVec)
		std::cout << x.first << ", " << x.second << "\n";
	std::cout << std::endl;
	*/

	// Find frequencies of each letter in the encrypted string

}

std::string Breaker::breakCipher(std::string toDecode, size_t trials) {
	// Set text as the inputted string
	setText(toDecode);
	// Set best score to 0
	long long bestScore = 0;
	// Set best key to the current cipher
	std::string bestKey = getCipher();

	// For as many trials as input,
	for (size_t i = 0; i < trials; ++i) {
		// Set new key to a random two-value swap of the current translator
		swapVals();
		std::string newKey = getCipher();
		// Score the new key
		long long newScore = score();

		// If the new score was higher than the best score, set the key to the best key
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