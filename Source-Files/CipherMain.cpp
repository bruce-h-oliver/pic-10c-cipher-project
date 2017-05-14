#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <unordered_map>

#include "Translator.h"
#include "Breaker.h"

int main() {
	
	// Create Breaker
	Breaker codeBreaker;
	// Create ifstream, opening bigram-freq.txt
	std::ifstream freqStream("bigram-freq.txt");
	// Set frequency dictionary
	codeBreaker.setFreq(freqStream);

	Translator newDict("bcdefghijklmnopqrstuvwxyza");
	newDict.printCipher(std::cout);

	std::string exampleString = "The quick brown fox; jumped over the lazy dog.";
	std::cout << "\n\n" << cleanString(exampleString);
	std::string encryptedBaby = newDict.encrypt(exampleString);
	std::cout << "\n" << encryptedBaby;
	std::cout << "\n" << newDict.decrypt(encryptedBaby);

	std::cin.get();

	return 0;
}