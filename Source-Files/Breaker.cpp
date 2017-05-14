#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<algorithm>
#include<unordered_map>
#include<ctime>

#include "Breaker.h"

// Initialize frequency chart
std::unordered_map<std::string, long long> Breaker::frequencyChart;

Breaker::Breaker()
{}

std::ifstream& Breaker::setFreq(std::ifstream& freqStream) {
	if (freqStream) {
		std::cout << "File was found.";
		while (!freqStream.eof()) {
			std::string bigram;
			long long value;
			freqStream >> bigram >> value;
			frequencyChart[bigram] = value;
		}
	}
	else
		std::cout << "FILE NOT FOUND\n";
	return freqStream;
}