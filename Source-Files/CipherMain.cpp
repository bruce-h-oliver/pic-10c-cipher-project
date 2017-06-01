#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <unordered_map>

#include "Translator.h"
#include "Breaker.h"

int main() {
	
	// Seed random function
	std::srand(std::time(nullptr));

	// Create Breaker
	Breaker codeBreaker = Breaker("Example string", "paul");
	// Create ifstream, opening bigram-freq.txt
	std::ifstream freqStream("bigram-freq.txt");
	// Set frequency dictionary
	codeBreaker.setFreq(freqStream);

	

	codeBreaker.printCipher(std::cout);
	std::cout << "Swapping values...\n";
	codeBreaker.swapVals();
	codeBreaker.printCipher(std::cout);

	std::string exampleString = "The quick brown fox; jumped over the lazy dogs.";
	std::cout << "\n\n" << cleanString(exampleString) << "\n";
	std::string encrypted = codeBreaker.encrypt(exampleString);
	std::cout << encrypted << "\n";
	std::cout << codeBreaker.decrypt(encrypted);
	
	codeBreaker.setText(encrypted);
	std::cout << "\n\nGoodScore: " << codeBreaker.score();
	codeBreaker.setText(exampleString);
	std::cout << "\nBad Score: " << codeBreaker.score();

	codeBreaker.setCipher(""); // Reset cipher to identity
	codeBreaker.singleFreqAttack();


	/*
	// DECRYPTION TIME BOYS

	std::cout << "\n\nDECRYPTION TIME!\n\n";
	std::string goalString = "Call me Ishmael. Some years ago, never mind how long precisely, having little or no money in my purse, and nothing particular to interest me on shore";
	std::cout << "Goal string: " << goalString << "\n";
	Translator encryptor("gorky");
	std::string encryptedString = encryptor.encrypt(goalString);
	std::cout << "Encryption: \n";
	encryptor.printCipher(std::cout);
	std::cout << "Encrypted string: " << encryptedString << "\n\n";

	codeBreaker.breakCipher(encryptedString, 1000);
	std::cout << "Calculated cipher: " << codeBreaker.getCipher() << "\n";
	std::cout << "Actual cipher:     " << encryptor.getCipher() << "\n";
	std::cout << "The decoded text: " << codeBreaker.decrypt(encryptedString) << "\n";
	int numCorrect = 0;
	for (size_t i = 0; i < encryptor.getCipher().size(); ++i) {
		if (encryptor.getCipher()[i] == codeBreaker.getCipher()[i])
			++numCorrect;
	}
	double ratioCorrect = static_cast<double>(numCorrect) / 26.0;
	std::cout << "Correctly decoded letters: " << numCorrect << "\n";
	std::cout << "Ratio correct: " << ratioCorrect << "\n";

	*/

	std::cin.get();

	return 0;
}