#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <unordered_map>

#include "Translator.h"

// ********************************************
/* HELPER FUNCTION DEFINITIONS */
// ********************************************

// Cleans a string of any character that isn't an alphabetical character or a space, then uppercases every character.
std::string cleanString(const std::string& inputStr) {
	std::string cleaned = inputStr;
	for (size_t i = 0; i < cleaned.size(); ++i) {
		if (!isalpha(cleaned[i]) && cleaned[i] != ' ' && cleaned[i] != '\0')
			cleaned.erase(cleaned.begin() + i);
	}
	for (auto& c : cleaned)
		c = toupper(c);

	return cleaned;
}

// ********************************************
/* TRANSLATOR DEFINITIONS */
// ********************************************

Translator::Translator() 
{
	for (unsigned int i = 0; i < 26; ++i) {
		encryptDict.insert(std::make_pair('A' + i, 'A' + i));
		decryptDict.insert(std::make_pair('A' + i, 'A' + i));
	}
}

Translator::Translator(const std::string& cipher)
{
	this->setCipher(cipher);
}

void Translator::setCipher(const std::string& newCipher)
{
	// Clean cleanCipher to remove punctuation, uppercase everything
	std::string cleanCipher = cleanString(newCipher);
	// Remove spaces
	for (auto iter = cleanCipher.begin(); iter != cleanCipher.end(); ++iter) {
		if (*iter == ' ')
			cleanCipher.erase(iter);
	}

	// Add all characters from cleanCipher into their slots in order
	unsigned int index = 0;
	for (; index < cleanCipher.size(); ++index) {
		encryptDict['A' + index] = cleanCipher[index];
	}
	// If we didn't assign over everything, pick up where we left off and add the remaining characters in alphabetical order
	if (index < 25) {
		// Make a new character variable to add the remaining ones
		char newChar = 'A';
		// Loop through remaining indeces
		for (; index < 26; ++index) {
			// While our character is still in the original string
			while (std::find(cleanCipher.begin(), cleanCipher.end(), newChar) != cleanCipher.end()) {
				// Keep incrementing newChar
				++newChar;
			}
			// Add newChar to the current index and increment it
			encryptDict['A' + index] = newChar++;
		}
	}

	// Update decryptDict
	for (auto iter = encryptDict.begin(); iter != encryptDict.end(); ++iter) {
		decryptDict[iter->second] = iter->first;
	}

	return;
}

std::ostream& Translator::printCipher(std::ostream& out) const {
	out << "Original:  ";
	for (char c = 'A'; c <= 'Z'; ++c) {
		out << c;
	}
	out << "\nEncrypted: " << getCipher() << "\n";

	return out;
}

std::string Translator::getCipher() const {
	std::string cipher = "";
	// Add encrypted characters to the string in alphabetical order
	for (char c = 'A'; c <= 'Z'; ++c)
		cipher += encryptDict.at(c);
	return cipher;
}

std::string Translator::encrypt(const std::string& plaintext) const {
	// Copy the plaintext to a new "encrypted" string, which is cleaned
	std::string encrypted = cleanString(plaintext);

	// For each character in the string
	for (auto iter = encrypted.begin(); iter != encrypted.end(); ++iter) {
		// Set the character to the associated value in the dictionary
		if (*iter != ' ' && *iter != '\0')
			*iter = encryptDict.at(*iter);
		else
			continue;
	}

	return encrypted;
}

std::string Translator::decrypt(const std::string& ciphertext) const {
	// Clean the string just in case
	std::string decrypted = cleanString(ciphertext);

	// For each character in the string
	for (auto iter = decrypted.begin(); iter != decrypted.end(); ++iter) {
		// Set the character to the associated value in the reverse dictionary
		if (*iter != ' ' && *iter != '\0')
			*iter = decryptDict.at(*iter);
		else
			continue;
	}

	return decrypted;
}