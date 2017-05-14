#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <unordered_map>

#include "Translator.h"
#include "Breaker.h"

// Initialize frequency chart
std::unordered_map<std::string, int> Breaker::frequencyChart;

int main() {

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