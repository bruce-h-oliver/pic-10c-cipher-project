#ifndef __BREAKER__
#define __BREAKER__

#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>
#include<unordered_map>
#include<ctime>

#include "Translator.h"

class Breaker {
public:
	/** Default Constructor
	Creates a Breaker with a default-constructed Translator (A=A, B=B, etc.) and a default-constructed ciphertext.
	*/
	Breaker();

	/** String Constructor
	Creates a Breaker with a default-constructed Translator and sets ciphertext to the passed-in value.
	*/
	Breaker(const std::string& ciphertext);

	/** setText
	Sets cipher text to the inputted string
	*/
	void setText(const std::string& newText);

	/** breakCipher
	For a specified number of trials, performs a random walk through possible ciphers, scoring each one and returning the Translator with the best score.	
	*/
	Translator breakCipher(size_t trials);

	/** swapVals
	Swaps two randomly chosen values in the key	
	*/
	void swapVals();

	/** score
	Compares the deciphered text to the frequency chart and returns its score, based on how many times the items appeared in the candidate text.
	*/
	int score();

private:
	static std::unordered_map<std::string, int> frequencyChart;

	std::string ciphertext;
	Translator key;
};

#endif