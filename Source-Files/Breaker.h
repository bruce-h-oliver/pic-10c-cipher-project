#ifndef __BREAKER__
#define __BREAKER__

#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>
#include<unordered_map>
#include<cstdlib>
#include<ctime>

#include "Translator.h"

class Breaker : public Translator {
public:
	/** Default Constructor
	Creates a Breaker with a default-constructed Translator (A=A, B=B, etc.) and a default-constructed ciphertext.
	*/
	Breaker();

	/** String Constructor
	Creates a Breaker with a default-constructed Translator and sets ciphertext to the passed-in value.
	*/
	Breaker(const std::string& ciphertext);

	/** String, String Constructor
	Creates a Breaker, sets ciphertext to first passed-in string, then constructs Translator with second string.
	*/
	Breaker(const std::string& ciphertext, const std::string& cipher);

	/** setFreq
	Sets the frequency dictionary's values by reading in from a specified file.
	*/
	std::ifstream& setFreq(std::ifstream& freqStream);

	/** setText
	Sets cipher text to the inputted string
	*/
	void setText(const std::string& newText);

	/** swapVals
	Swaps two randomly chosen values in the key	
	*/
	void swapVals();

	/** score
	Deciphers ciphertext based on current key, compares that to the frequency chart, returns its score.
	// Score is based on how many times the items appeared in the candidate text.
	*/
	long long score();

	/** breakCipher
	For a specified number of trials, performs a random walk through possible ciphers, scoring each one and returning the Translator with the best score.
	*/
	Translator breakCipher(size_t trials);

	// Print freqDict
	// FOR DEBUGGING
	void printFreq() {
		for (auto iter = frequencyChart.begin(); iter != frequencyChart.end(); ++iter)
			std::cout << iter->first << ", " << iter->second << "\n";
	}

private:
	static std::unordered_map<std::string, long long> frequencyChart;
	std::string ciphertext;
	std::string bestCipher; // Cipher, as a string, that has scored the highest so far
};

#endif