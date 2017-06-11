#ifndef __TRANSLATOR__
#define __TRANSLATOR__

#include<iostream>
#include<string>
#include<unordered_map>
#include<ctime>
#include<cstdlib>

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
	Does this by calling setCipher() with the passed-in string.
	*/
	Translator(const std::string& cipher);

	/** Int Constructor
	Takes several ints as codes to construct ciphers. 
	0 (or any int not in range 1-25): Constructs a random cipher
	1-25: Constructs a Caeser cipher with the indicated int as the shift
	*/
	Translator(int code);

	/** Virtual Destructor
	It's virtual!
	*/
	virtual ~Translator() = default;

	/** setCipher
	Changes the translator's cipher to match the indicated string.
	If the passed-in string has fewer than 26 characters, the unmapped values will be mapped to the remaining characters in alphabetical order.
	Reassigns the values in decryptDict to the associated keys in encryptDict to maintain bijection.
	Defaults to empty string, which means the cipher is reset to the identity cipher.
	Param newCipher: the string used to set the cipher.
	*/
	void setCipher(const std::string& newCipher = "");

	/** printCipher
	Prints the translator's cipher to the passed-in ostream object.
	Param out: the ostream that the cipher should be printed to.
	Return: the ostream object that was passed in, as a reference.
	*/
	std::ostream& printCipher(std::ostream& out) const;

	/** getCipher
	Returns the translator's cipher as a string.
	*/
	std::string getCipher() const;

	/** encrypt
	Encrypts a passed-in string and returns the encrypted version (without changing the original string).
	Param plaintext: Plain English text to be encrypted.
	Return: Encrypted string, based on current encryption key.
	*/
	std::string encrypt(const std::string& plaintext) const;

	/** decrypt
	Decrypts a passed-in encrypted string using the current decryption key, returning the decryption without changing original string.
	Param ciphertext: Encrypted text to be decrypted.
	Return: Decrypted string, based on current decryption key.
	*/
	std::string decrypt(const std::string& ciphertext) const;

protected:
	std::unordered_map<char, char> encryptDict; // Dictionary which maps original_char (key) -> cipher_char (value)
	std::unordered_map<char, char> decryptDict; // Dictionary which maps cipher_char (key) -> original_char (value)
};

#endif