#ifndef __TRANSLATOR__
#define __TRANSLATOR__

#include<iostream>
#include<string>
#include<unordered_map>

// Helper function: cleanString
// Removes all non-alphabetical/space characters from a string, then uppercases all characters.
std::string cleanString(const std::string& inputStr);

class Translator {
public:
	/** Default Constructor
	Constructs a translator with all 26 characters (A-Z) mapped to their identities (A-Z); constructs the same for the reverse dictionary.
	*/ 
	Translator();

	/** Cipher Constructor
	Constructs a translator with all 26 characters (A-Z) mapped to the indicated characters. 
	If the passed-in string has fewer than 26 characters, the unmapped values will be mapped to the remaining characters in alphabetical order.
	Reassigns the values in decryptDict to the associated keys in encryptDict
	*/
	Translator(const std::string& cipher);

	/** Virtual Destructor
	It's virtual!
	*/
	virtual ~Translator() = default;

	/** setCipher
	Changes the translator's cipher to match the indicated string
	*/
	void setCipher(const std::string& newCipher);

	/** printCipher
	Prints the translator's cipher to the passed-in ostream object.
	*/
	std::ostream& printCipher(std::ostream& out) const;

	/** getCipher
	Returns the translator's cipher as a string.
	*/
	std::string getCipher() const;

	/** encrypt
	Encrypts a passed-in string and returns the encrypted version (without changing the original).
	*/
	std::string encrypt(const std::string& plaintext) const;

	/** decrypt
	Decrypts a passed-in encrypted string by reversing the cipher values stored in encryptDict.
	*/
	std::string decrypt(const std::string& ciphertext) const;

protected:
	std::unordered_map<char, char> encryptDict;
	std::unordered_map<char, char> decryptDict;
};

#endif