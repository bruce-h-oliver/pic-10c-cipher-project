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
	Breaker codeBreaker = Breaker();
	// Create ifstream, opening full-quadragrams.txt (best decoding performance)
	std::ifstream freqStream("full-quadragrams.txt");
	// Set frequency dictionary
	codeBreaker.setFreq(freqStream);

	// Create translator (cipher to be set later)
	Translator encryptor = Translator();

	// Choice variables:
	char alreadyEncrypted = '0';
	char ownStringChar = '0';
	char preselectedChoice = '0';
	char cipherChoice = '0';

	// Other UI variables
	int caesarShift = 0;
	std::string userCipher = "";
	size_t numTrials = 5000;

	// Possible strings to decrypt, and final string (goalString)
	std::string userEncrypted = "";
	std::string userPlain = "";

	std::string mobyDick = "Call me Ishmael. Some years ago, never mind how long precisely, having little or no money in my purse, and nothing particular to interest me on shore, I thought I would sail about a little and see the watery part of the world.";
	std::string oliverTwist = "For a long time after it was ushered into this world of sorrow and trouble, by the parish surgeon, it remained a matter of considerable doubt whether the child would survive to bear any name at all; in which case it is somewhat more than probable that these memoirs would never have appeared;";
	std::string candide = "His countenance was a true picture of his soul. He combined a true judgment with simplicity of spirit, which was the reason, I apprehend, of his being called Candide. The old servants of the family suspected him to have been the son of the Baron's sister, by a good, honest gentleman of the neighborhood, whom that young lady would never marry because he had been able to prove only seventy one quarterings.";
	std::string breadBook = "Time was when a peasant family could consider the corn which it grew, or the woollen garments woven in the cottage, as the products of its own toil. But even then this way of looking at things was not quite correct. There were the roads and the bridges made in common, the swamps drained by common toil. Even in those days a peasant family could not live alone, but was dependent in a thousand ways on the village or the commune.";

	std::string plainString = "";
	std::string encryptedString = "";

	// --------------------------------
	// USER INTERFACE:
	// --------------------------------
	std::cout << "--------------------------------------------------\n" << 
		"Welcome to the MCMC Cipher-Breaker Tool!\n--------------------------------------------------\n";
	// Already encrypted vs. plain string
	std::cout << "Would you like to: \n\ta) input an already-encrypted string, or \n\tb) would you like the program to encrypt a string for you?\n";
	while (alreadyEncrypted < 'a' || alreadyEncrypted > 'b') {
		std::cout << "Please select your choice using the associated letter [a, b]: ";
		std::cin >> alreadyEncrypted;
	}
	if (alreadyEncrypted == 'a') {
		std::cout << "Please input the encrypted string: ";
		std::cin.ignore();
		std::getline(std::cin, userEncrypted);
		// Set encrypted string to user input
		encryptedString = userEncrypted;
		std::cout << "\n-------------------------\n";
	}
	else {
		// Using own string vs. using pre-selected string
		std::cout << "Would you like to: \n\ta) input your own string, or \n\tb) would you like to select from a list of pre-selected strings?\n";
		std::cout << "PLEASE NOTE: The tool works best with strings of 250 characters or greater.\n";
		while (ownStringChar < 'a' || ownStringChar > 'b') {
			std::cout << "Please select your choice using the associated letter [a, b]: ";
			std::cin >> ownStringChar;
		}
		if (ownStringChar == 'a') {
			std::cout << "Please input your English string: ";
			std::cin.ignore();
			std::getline(std::cin, userPlain);
			plainString = userPlain;
		}
		else {
			std::cout << "Your choices for preselected strings:\n";
			std::cout << "\ta) An excerpt from \"Moby Dick\" by Herman Melville.\n";
			std::cout << "\tb) An excerpt from \"Oliver Twist\" by Charles Dickens.\n";
			std::cout << "\tc) An excerpt from \"Candide\" by Voltaire (English translation).\n";
			std::cout << "\td) An excerpt from \"The Conquest of Bread\" by Paul Kropotkin.\n";
			while (preselectedChoice < 'a' || preselectedChoice > 'd') {
				std::cout << "Please select your choice using the associated letter [a, b, c, d]: ";
				std::cin >> preselectedChoice;
			}

			switch (preselectedChoice) {
			case 'a':
				plainString = mobyDick;
				break;
			case 'b' :
				plainString = oliverTwist;
				break;
			case 'c':
				plainString = candide;
				break;
			case 'd':
				plainString = breadBook;
				break;
			default:
				plainString = mobyDick;
			}
		}

		// Choosing a cipher
		std::cout << "-------------------------\n";
		std::cout << "Would you like to: \n\ta) randomly select a cipher, \n\tb) use a Caesar cipher, or \n\tc) use a keyword cipher/input your own cipher?\n";
		while (cipherChoice < 'a' || cipherChoice > 'c') {
			std::cout << "Select your choice from above by its letter [a, b, c]: ";
			std::cin >> cipherChoice;
		}
		switch (cipherChoice) {
		case 'a':
			std::cout << "Randomizing cipher...\n";
			encryptor = Translator(0);
			break;
		case 'b':
			while (caesarShift < 1 || caesarShift > 25) {
				std::cout << "Please indicate the shift for the Caesar cipher (any number from 1-25): ";
				std::cin >> caesarShift;
			}
			encryptor = Translator(caesarShift);
			break;
		case 'c':
			std::cout << "Keywords must be only alphabetical, and can be anywhere from 1 to 26 characters, \n\tas long as there are no repeated characters.\n" <<
				"For example, the keyword \'zebra\' corresponds\n\y to the cipher ZEBRACDFGHIJKLMNOPQSTUVWXY.\n" <<
				"Similarly, if you are using your own full cipher, \n\tit can be no more than 26 characters long.\n";
			std::cout << "Please input the keyword, or full cipher, that you wish to use: ";
			std::cin >> userCipher;
			encryptor.setCipher(userCipher);
		}

		std::cout << "-------------------------\n";
		std::cout << "Goal string: " << cleanString(plainString) << "\n\n";

		encryptedString = encryptor.encrypt(plainString);

		std::cout << "THE ENCRYPTION: \n";
		encryptor.printCipher(std::cout);

	}

	std::cout << "\nEncrypted string: " << encryptedString << "\n\n";

	// Choosing number of trials
	std::cout << "For best results, we recommend running the decryptor for 4,000-8,000 trials.\n";
	std::cout << "Please input the number of trials you would like to run: ";
	std::cin >> numTrials;
	std::cout << "\nPress Enter to start decryption.\n";
	std::cin.ignore();
	std::cin.get();
	
	codeBreaker.breakCipher(encryptedString, numTrials);

	std::string decryptedString = codeBreaker.decrypt(encryptedString);
	std::string cleanedGoal = cleanString(plainString);

	std::cout << "\n************\n" << "The Final Decoded Text:\n" << decryptedString << "\n************\n";
	if (alreadyEncrypted == 'a') {
		std::cout << "Calculated Cipher: " << codeBreaker.getCipher() << "\n";
	}

	if (alreadyEncrypted != 'a') {
		std::cout << "The Real Text:\n" << cleanedGoal << "\n************\n";
		std::cout << "Calculated Cipher: " << codeBreaker.getCipher() << "\n";
		std::cout << "Actual Cipher:     " << encryptor.getCipher() << "\n************\n";

		double numCorrect = 0;
		for (size_t i = 0; i < encryptor.getCipher().size(); ++i) {
			if (encryptor.getCipher()[i] == codeBreaker.getCipher()[i])
				++numCorrect;
		}
		double ratioCorrect = numCorrect / 26.0;
		std::cout << "Correctly Decoded Letters: " << static_cast<int>(numCorrect) << "\n";
		std::cout << "Ratio Correct (Cipher): " << ratioCorrect << "\n";

		double textScore = 0;
		for (size_t i = 0; i < cleanedGoal.size(); ++i) {
			if (cleanedGoal[i] == decryptedString[i]) {
				++textScore;
			}
		}
		double maxTextScore = cleanedGoal.size();
		std::cout << "Text Characters Matching: " << textScore << "/" << maxTextScore << " = " << (textScore / maxTextScore) * 100.0 << "%\n";
	}
	

	std::cin.get();

	return 0;
}