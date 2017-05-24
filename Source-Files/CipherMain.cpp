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
	Breaker codeBreaker("The British Are Coming!", "paul");
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

	std::cin.get();

	return 0;
}