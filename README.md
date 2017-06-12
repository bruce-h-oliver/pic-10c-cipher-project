##-------------- MCMC Cipher-Breaker Tool --------------
### Created by Bruce Oliver
#### Date: 6/13/17
#### Language Used: C++ 14
#### PIC 10C Spring 2017 Final Project

---

Welcome to my MCMC Cipher-Breaker Tool! In this README file, I'll be walking you through exactly how the tool works, the concepts behind it, and how best to use it.

### How to Use the Program

1. **MOVE THE FILES "full-quadragrams.txt" AND "monogram-freq.txt" INTO YOUR WORKING DIRECTORY.** This is very important; the program will not be able to read in frequency data otherwise.
2. Compile and run the program.
3. Wait for the frequency data to finish compiling; it usually takes around 20 seconds (we're using a lot of reference data).
4. Follow the prompts and select your choices with regards to what you want to decode. If this is your first time running the program, I suggest using the list of pre-chosen strings, as they have a confirmed high success rate of decryption.
5. When choosing the number of trials to run, please note that the shorter the string, the more trials you will likely need to run. Longer strings can be solved in 2000-4000 trials, but shorter strings will require closer to 8000-10000 trials. 
6. Wait for the decryption to finish! The program will print out its progress every 100 trials, so you can keep track of the current state of the decryption.
7. Once the program is finished, it will display the decoded string, along with how successful its decryption was (if you encoded a plain string to begin with).
8. If it didn't correctly decrypt the string, answer 'y' to the prompt to start the decryption over, optionally changing the number of trials to run. Otherwise, answer 'n' and the program will exit.

### Introduction to Monoalphabetic Substitution Ciphers

The art of encoding secret messages such that your allies can read them, but your enemies can't, has been worked on since ancient history. Monoalphabetic Substitution ciphers are one of the earliest forms of cryptography invented, meaning they're fairly easy to understand (and, incidentally, fairly easy to crack). 

*(From now on, I'm going to be referring to monoalphabetic substitution ciphers as simply "substitution ciphers," although polyalphabetic substitution ciphers (which are much more difficult to crack) do exist: Look up the Vigenère cipher if you're interested.)*

The basic idea behind a substitution cipher is, as the name implies, to substitute each letter of the alphabet for another letter using a pre-decided cipher. To encrypt, you substitute letters according to the cipher; to decrypt, you substitute the letters back according to the inverse of the cipher.

One of the most famous variants of a substitution cipher is the **Caesar cipher.** A Caesar cipher works by picking a *shift,* rather than an explicit cipher. The encryption is then performed by shifting each letter to the right by the amount indicated by the shift.

For example, a Caesar cipher with a shift of 3 looks like this:

ABCDEFGHIJKLMNOPQRSTUVWXYZ  
DEFGHIJKLMNOPQRSTUVWXYZABC

The top row is the plaintext character, the bottom row is the encrypted character. In this case, A maps to D, B maps to E, C maps to F, and so on.

Using this key, the plaintext string:  
HELLO HOW ARE YOU DOING TODAY  
Encrypts to:  
KHOOR KRZ DUH BRX GRLQJ WRGDB

Of course, a substitution cipher doesn't have to be a Caesar cipher -- any rearrangement of the alphabet mapped against the original works as a substitution key. 

The simplest method would be to randomly rearrange the characters, but a third method (beyond Caesar ciphers and random rearrangement) is to pick a 'keyword' and build your cipher from there.

In a keyword cipher, you pick any word with no repeated letters, write it out, then fill in the remaining letters of the alphabet after it. For example, the keyword ZEBRA would give you the encryption:

ABCDEFGHIJKLMNOPQRSTUVWXYZ  
ZEBRACDFGHIJKLMNOPQSTUVWXY

The problem with keyword ciphers is that if you don't pick a keyword with characters late enough into the alphabet, you'll get letters that map to themselves, which is usually undesirable. 

That's it for the overview of substitution ciphers! Now, let's go over the logic of how to break them.

### Markov Chain Monte Carlo (MCMC) Methods

To be completely honest, I don't still fully understand all of the theoretical details behind a Markov chain. 

What I can tell you, however, that a Markov chain is any set of probabilistic states where the state at time *t* depends only on the state at time *t-1*. Furthermore, a Monte Carlo algorithm is any algorithm that uses random selection to try to arrive at a desired state.

A Markov Chain Monte Carlo method, in our case, is a method that performs a *random walk* through a certain space, eventually converging to the correct answer based on the probabilities of the states it arrives at.

Or something.

Like I said, I don't really get it in the general case. However, I *can* explain how the algorithm works in the specific case of cipher breaking.

#### Ciphers and English Letter Distribution

The major problem when breaking ciphers is the sheer number of possible keys that could have encrypted the text: 26!, or 4.03 x 10^26. Brute-forcing and going through every key randomly would take far too long, and someone would have to look at it at every point to make sure it wasn't already solved.

However, this assumes a completely flat probability distribution. We can do a bit better than this. Specifically, we can harness the statistical probabilities of certain letters appearing in English. 

For example, E is the most common letter in English, followed by T, followed by A, and so on. You probably know a few of these if you've played Hangman. 

We can take advantage of this statistical distribution when attempting to crack a cipher. If the most common letter in the encrypted text is Q, then it's most likely that E is mapped to Q in the cipher. One letter down!

Unfortunately, single-letter frequencies aren't usually enough to solve ciphers by themselves, especially when the ciphertext is relatively short. So, we can kick it up a notch, and look at bigrams, trigrams, or (in our case) quadgrams.

As you may have guessed, a bigram is a two-letter permutation. The most common bigram in English is TH, followed by HE. Trigrams are three-letter permutations; in English, the most common is THE, followed by AND. Quadgrams, as you can guess, are four-letter permutations. The most common quadgram in English is TION, followed by NTHE.

#### How My Cipher Solving Algorithm Works

Our ultimate goal is to create a **score function**, which can look at a piece of decrypted text and give it a "score" that indicates how much it looks like English. In our case, we use quadgram frequencies to score text. We compile a massive table of quadgrams in English, storing the frequency associated with them (as a logarithm, to avoid too many small numbers). When we get our decrypted text, we look up each quadgram in it, compare it to our table of English quadgrams, and add its log probability to our score. Once we've gone through every quadgram, we have our final score as a sum of log probabilities (which is the same as multiplying all the probabilities, then taking the logarithm). 

At the end of the day, the more English-like quadgrams appear in our deciphered text, the higher its score should be, since higher-frequency quadgrams contribute more to a score. The maximized score, one at which we can't go any higher, should theoretically be plain English text.

So, now that we have a score function, we can use our MCMC method. It's fairly simple: we start with some random key, then we swap two of the letters in that key. We then decrypt the text using the key, and score that text. If the score is higher than the previous score, we keep the new key. If the score was lower, we reject the new key and keep the old one. This is the essence of an MCMC random walk: randomly swapping to new positions, but with weighted probabilities, so that we eventually arrive at a maximum.

One last detail: in my program, we don't start with "some random key." Remember when I said that single-letter frequencies could get you a few letters, but weren't great for solving ciphers outright? Well, it turns out, performing a single-letter "attack" at the beginning gives you a better starting position from which to perform the MCMC random walk. A single-letter attack doesn't involve any random walks — it simply consists of swapping the most frequent letter in the ciphertext with the most frequent letter in English, the second most frequent letter with the second most frequent letter in English, and so on. This single-attack is only really effective for larger strings of text, but it often gets you E for free, so why not?

### Implementation Details

My cipher breaker is coded on inheritance model: The **Breaker** object is what breaks ciphers, but it inherits from the **Translator** object, which simply encrypts and decrypts text using ciphers (implemented as unordered_maps).

#### Translator Member Variables

* std::unordered_map<char, char> **encryptDict**  
*Contains the mapping from each English character (stored as key) to each encrypted character (stored as value).*
* std::unordered_map<char, char> **decryptDict**  
*Contains the mapping from each encrypted character (stored as key) to each English character (stored as value).*

Any function that updates encryptDict will also update decryptDict with the inverse values, ensuring that they are always inverses of each other.

#### Translator Member Functions

* **Translator()** 
*Constructs a translator with an identity key: each letter is mapped to itself.*
* **Translator(const std::string&)**    
*Constructs a translator using the passed-in string. Does this by calling setCipher on the passed-in string.*
* **Translator(int)**  
*Takes several codes to construct other forms of ciphers.*  
***1 through 25:** Creates a Caesar cipher with the inputted integer as the shift.*  
***Any other int:** Creates a randomly shuffled cipher.*  
*Once the cipher is created as a string, it calls setCipher using that string.*
* **setCipher**  
*Sets cipher to current string. If string is less than 26 characters, it fills in the remaining characters up to 26, creating a keyword cipher.*
* **printCipher**  
*Prints out the current cipher to the passed-in ostream object, in the format:*  
Original:  ABCDEFGHIJKLMNOPQRSTUVWXYZ  
Encrypted: NOPQRSTUVWXYZABCDEFGHIJKLM
* **getCipher**  
*Returns the current cipher as a 26-character string. The returned string can be used with setCipher of another Translator to copy the cipher.*
* **encrypt**  
*Returns the encrypted version of the passed-in string, based on the current cipher, without changing the original string.*
* **decrypt**  
*Returns the decrypted version of the passed in string, using the inverse of the current cipher, without changing the original string.* 
*Calling decrypt(encrypt(some_string) from the same translator object is an identity, and returns the same string as passed in.*

#### Breaker Member Variables

* static std::unordered_map<std::string, double> **frequencyChart**  
*Stores every English n-gram (my program uses quadgrams, but you can substitute your own frequency data with different sized n-grams) mapped to its log frequency. MUST be compiled using setFreq at the beginning of the program.*
* std::string **ciphertext**  
*The encrypted text that the Breaker will try to decrypt.*
* std::string **bestCipher**  
*The current best cipher, stored as a 26-character string. On initialization, will simply be an empty string; will be updated as breakCipher() runs, until the function finishes and it holds the calculated best cipher.*

#### Breaker Member Functions

* **Breaker()**  
*Creates a Breaker with a default-constructed Translator (A=A, B=B, etc.) and a default-constructed ciphertext.*
* **Breaker(const std::string&)**  
*Creates a Breaker with a default-constructed Translator and sets ciphertext to the passed-in value.*
* **Breaker(const std::string&, const std::string&)**  
*Creates a Breaker, sets ciphertext to first passed-in string, then constructs Translator with second string.*
* **setFreq**  
*Sets the frequency dictionary's values by reading in from a specified filestream. You must run this function with whatever frequency data you plan to use before the Breaker will work.*
* **setText**  
*Sets ciphertext to the inputted string.*
* **swapVals**  
*Swaps two randomly chosen values in the cipher, updating both encryptDict and decryptDict.*
* **score**  
*Deciphers ciphertext based on current cipher, compares that to the frequency chart, and returns its score.*  
*Score is used as a measure of similarity to English text. The higher the score (closer to 0), the more the text resembles English.*  
*Score is based on the sum of the frequencies of all n-grams in the deciphered text, in the form of the cumulative log of their probabilities. Will always be negative.*
* **singleFreqAttack**  
*Uses frequencies of single letters and substitutes highest frequency letter in the ciphertext with highest frequency letter in English, second highest with second highest, and so on.*  
*Only modifies the encryptDict and decryptDict from Translator.*
* **breakCipher**  
*For a specified number of trials, performs a random walk through possible ciphers, scoring each one and returning the key (as a 26-char string) with the best score.*  
*Once the function has been completed, the encryptDict and decryptDict will match the best cipher, so the object can be used to decode the string with decrypt().*

#### Other Functions

* **cleanString**  
*Cleans a passed-in string of all characters other than alphabetical characters and spaces, and uppercases all alphabetical characters. This ensures that the string can be properly decoded by Translators/Breakers.*  
*This function is run on any string passed into the program before it is decoded, so if you're decoding your own string, all punctuation will be cleaned. Therefore, even if you type in the string normally, everything will still work.*