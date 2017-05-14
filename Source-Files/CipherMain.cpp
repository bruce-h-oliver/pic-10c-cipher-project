#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <unordered_map>

#include "Translator.h"

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