
#include "T9CharsTrie.h"
#include "T9TernaryDigitsTrie.h"
#include "T9NaryDigitsTrie.h"

#include <iostream>

void load(T9Trie &trie) {
	std::cout << "T9 Chars Trie loading..." << std::endl;
	
	std::vector<std::string> dictionary = {"git", "github", "git merge", "hit", "history"};
	std::cout << "Atomatic loading..." << std::endl;
	std::cout << dictionary << std::endl;
	for (int i = 0; i < dictionary.size(); ++i) {
		trie.insert(dictionary[i]);
	}
	
	std::cout << "Manual loading: " << std::endl;
	std::string expression;
	do {
		getline(std::cin, expression);
		if (expression.size())
			trie.insert(expression);
	} while (expression.size());
}

void autocomplete(T9Trie &trie) {
	std::string prefix;
	do {
		trie.displayTuples(std::cout);
		std::cout << "Enter the digit T9 prefix for autocomplete: ";
		getline(std::cin, prefix);
		if (prefix.size()) {
			std::cout << std::endl;
			std::cout << "Autocomplete..." << std::endl;
			std::vector<int> digits;
			bool valid = true;
			for (int i = 0; i < prefix.size(); ++i) {
				if (isdigit(prefix[i])) {
					digits.push_back(prefix[i] - '0');
				} else {
					valid = false;
					break;
				}
			}
			
			if (valid) {
				for (int i = 0; i < digits.size(); ++i) {
					std::vector<std::string> expressions;
					std::vector<int> subDigits(digits.begin(), digits.begin() + i + 1);
					trie.autocomplete(subDigits, expressions, false);
					std::cout << "T9 digit prefix: " << subDigits << std::endl;
					std::cout << "Displaying autocomplete expressions..." << std::endl;
					std::cout << expressions << std::endl;
					std::cout << std::endl;
				}
			}
			else
				std::cout << "Your digit T9 prefix containes symbols other then digits. PLease try again..." << std::endl;
			std::cout << std::endl;
		}
	} while (prefix.size());
}

int main() {
//	T9CharsTrie trie;
//	T9TernaryDigitsTrie trie;
	T9NaryDigitsTrie trie;
	
	load(trie);
	autocomplete(trie);
	return 0;
}
