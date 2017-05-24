#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<algorithm>
#include<unordered_map>

#include "Breaker.h"

// Initialize frequency chart
std::unordered_map<std::string, long long> Breaker::frequencyChart;


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
	// Current score
	long long score = 0;

	// For each element in frequencyChart
	for (const auto& entry : frequencyChart) {
		// Create working tally of number of times it appears
		int tally = 0;
		// Create working string to chop down as we find elements
		std::string chopped = workingDecrypt;
		// Search for the substring in the string and add to tally every time it appears
		while (auto iter = chopped.find(entry.first) != std::string::npos) {
			chopped.erase(iter, iter + 1);
			++tally;
		}
		// Multiply tally by that entry's frequency and add it to the score
		score += tally * entry.second;
	}

	// Return final score
	return score;
}