
#ifndef workplace_T9NaryDigitsTrie_h
#define workplace_T9NaryDigitsTrie_h

#include "T9Trie.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class T9NDTNode {
public:
	T9NDTNode **_digitNodes;
	
	T9NDTNode(size_t count) {
		if (count > 0) {
			_digitNodes = new T9NDTNode *[count];
			for (int i = 0; i < count; ++i)
				_digitNodes[i] = nullptr;
		} else {
			_digitNodes = nullptr;
		}
	}
	
	~T9NDTNode() {
		if (_digitNodes) {
			delete [] _digitNodes;
			_digitNodes = nullptr;
		}
	}
};

class T9NaryDigitsTrie : public T9Trie {
public:
	T9NDTNode *_root;
	
	std::vector<std::vector<char>> _tupes;
	std::unordered_map<char, int> _charDigitMap;
	
	T9NaryDigitsTrie() {
		_tupes = {
			{' '},
			{',', '.', '?', '!'},
			{'a', 'b', 'c'},
			{'d', 'e', 'f'},
			{'g', 'h', 'i'},
			{'j', 'k', 'l'},
			{'m', 'n', 'o'},
			{'p', 'q', 'r', 's'},
			{'t', 'u', 'v'},
			{'w', 'x', 'y', 'z'}
		};
		for (int i = 0; i < _tupes.size(); ++i)
			for (int j = 0; j < _tupes[i].size(); ++j)
				_charDigitMap[_tupes[i][j]] = i;
		
		_root = new T9NDTNode(_tupes.size());
	}
	
	virtual ~T9NaryDigitsTrie() {
		// TODO:
	}
	
	virtual void insert(std::string &expression) {
		// TODO:
	}
	
	virtual void autocomplete(std::vector<int> &digits, std::vector<std::string> &expressions, bool best = false) {
		// TODO:
	}
	
	virtual void displayTuples(std::ostream &os) {
		for (int i = 0; i < _tupes.size(); ++i) {
			os << i << " : " << _tupes[i] << std::endl;
		}
	}
};

#endif
