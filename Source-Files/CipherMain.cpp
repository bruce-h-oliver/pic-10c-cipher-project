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
	// Create ifstream, opening whatever source file we're using
	//std::ifstream freqStream("quadragram-freq.txt");
	std::ifstream freqStream("full-quadragrams.txt");
	// Set frequency dictionary
	codeBreaker.setFreq(freqStream);
	//codeBreaker.printFreq();

	/*
	codeBreaker.printCipher(std::cout);
	std::cout << "Swapping values...\n";
	codeBreaker.swapVals();
	codeBreaker.printCipher(std::cout);

	std::string exampleString = "The quick brown fox; jumped over the lazy dogs. This is now a much longer example string that simulates a much larger body of English text and, as such, has a more realistic depiction of English letter frequencies.";
	std::cout << "\n\n" << cleanString(exampleString) << "\n";
	std::string encrypted = codeBreaker.encrypt(exampleString);
	std::cout << encrypted << "\n";
	std::cout << codeBreaker.decrypt(encrypted) << "\n\n";

	codeBreaker.setText(encrypted);
	codeBreaker.setCipher(); // Reset cipher to identity
	*/

	// DECRYPTION TIME BOYS

	std::cout << "\n\nDECRYPTION TIME!\n----------------------------------\n";
	std::string goalString = "Call me Ishmael. Some years ago, never mind how long precisely, having little or no money in my purse, and nothing particular to interest me on shore, I thought I would sail about a little and see the watery part of the world."; /*It is a way I have of driving off the spleen and regulating the circulation. Whenever I find myself growing grim about the mouth; whenever it is a damp, drizzly November in my soul; ";*/
	//std::string goalString = "The quick brown fox jumped over the lazy dogs the quick brown fox jumped over the lazy dogs the quick brown fox jumped over the lazy dogs the quick brown fox jumped over the lazy dogs.";
	std::cout << "Goal string: " << cleanString(goalString) << "\n\n";
	Translator encryptor("kanlbcgydosxuwhpjermqtizvf");
	std::string encryptedString = encryptor.encrypt(goalString);
	std::cout << "THE ENCRYPTION: \n";
	encryptor.printCipher(std::cout);
	std::cout << "\nEncrypted string: " << encryptedString << "\n\n";
	std::cout << "Press Enter to start decryption.\n\n";
	std::cin.get();
	
	codeBreaker.breakCipher(encryptedString, 4000);
	std::string decryptedString = codeBreaker.decrypt(encryptedString);
	std::string cleanedGoal = cleanString(goalString);
	std::cout << "Calculated cipher: " << codeBreaker.getCipher() << "\n";
	std::cout << "Actual cipher:     " << encryptor.getCipher() << "\n************\n";
	std::cout << "The decoded text:\n" << decryptedString << "\n************\n";
	std::cout << "The real text: " << cleanedGoal << "\n************\n";

	int numCorrect = 0;
	for (size_t i = 0; i < encryptor.getCipher().size(); ++i) {
		if (encryptor.getCipher()[i] == codeBreaker.getCipher()[i])
			++numCorrect;
	}
	double ratioCorrect = static_cast<double>(numCorrect) / 26.0;
	std::cout << "Correctly decoded letters: " << numCorrect << "\n";
	std::cout << "Ratio correct (key): " << ratioCorrect << "\n";	

	double textScore = 0;
	for (size_t i = 0; i < cleanedGoal.size(); ++i) {
		if (cleanedGoal[i] == decryptedString[i]) {
			++textScore;
		}
	}
	double maxTextScore = cleanedGoal.size();
	std::cout << "Text Characters Matching: " << textScore << "/" << maxTextScore << " = " << (textScore / maxTextScore) * 100.0 << "%\n";

	std::cin.get();

	return 0;
}