#ifndef __BREAKER__
#define __BREAKER__

#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<vector>

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

	Param ciphertext: The string to be set as the ciphertext (encrypted text).
	Param cipher: The string to be set as the cipher. String must be between 0 and 26 characters, alphabetical, with no repeated characters.
	*/
	Breaker(const std::string& ciphertext, const std::string& cipher);

	/** setFreq
	Sets the frequency dictionary's values by reading in from a specified file.
	*/
	std::ifstream& setFreq(std::ifstream& freqStream);

	/** setText
	Sets ciphertext to the inputted string.

	Param newText: sets internal variable cipherText to the contents of this string.
	Return: none.
	*/
	void setText(const std::string& newText);

	/** swapVals
	Swaps two randomly chosen values in the key.
	Only 

	Params: None.
	Returns: None.
	*/
	void swapVals();

	/** score
	Deciphers ciphertext based on current key, compares that to the frequency chart, and returns its score.
	Score is used as a measure of similarity to English text. The higher the score (closer to 0), the more the text resembles English.
	Score is based on how many times the items appeared in the candidate text, in the form of the log of its probability. Will always be negative.

	Params: None.
	Returns: The score of the decoded text using the current key as a (negative) double.
	*/
	double score();

	/** singleFreqAttack
	Uses frequencies of single letters and substitutes highest frequency letter in the ciphertext with highest frequency letter in English, 
	second highest with second highest, and so on.
	Only modifies the member variables of the Breaker (the encryptDict and decryptDict from Translator).
	*/
	void singleFreqAttack();

	/** breakCipher
	For a specified number of trials, performs a random walk through possible ciphers, 
	scoring each one and returning the key (as a 26-char string) with the best score.
	Once the function has been completed, the encryptDict and decryptDict will match the best cipher, so the object can be used to decode the string.

	Param toDecode: The string to be decoded. Will be set as the Breaker's ciphertext immediately.
	Param trials: The number of times to loop the swapping of random values. Recommended values: 5000-10000. 
	Return: The best key found at the end of the loop, in the form of a 26-character string. 
		If that string is input into a Breaker or Translator object using setCipher(), it will correctly decode the encrypted string (if the encryption was solved in the first place).
	*/
	std::string breakCipher(std::string toDecode, size_t trials);

	// Print freqDict
	// FOR DEBUGGING
	void printFreq() {
		for (auto iter = frequencyChart.begin(); iter != frequencyChart.end(); ++iter)
			std::cout << iter->first << ", " << iter->second << "\n";
	}

private:
	static std::unordered_map<std::string, double> frequencyChart;

	std::string ciphertext;
	std::string bestCipher; // Cipher, as a string, that has scored the highest so far
};

#endif