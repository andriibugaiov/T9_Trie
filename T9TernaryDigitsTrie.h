
#ifndef workplace_T9TernaryDigitsTrie_h
#define workplace_T9TernaryDigitsTrie_h

#include "T9Trie.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>

class T9TDTNode {
public:
	T9TDTNode *_left;
	T9TDTNode *_middle;
	T9TDTNode *_right;
	
	int _digit;
	
	std::list<std::string> _expressions;
	
	T9TDTNode(int digit) {
		_left = nullptr;
		_middle = nullptr;
		_right = nullptr;
		
		_digit = digit;
	}
};

class T9TernaryDigitsTrie : public T9Trie {
	void clear(T9TDTNode *&node) {
		if (!node)
			return;
	
		clear(node -> _left);
		clear(node -> _middle);
		clear(node -> _right);
		
		delete node;
		node = nullptr;
	}
	
	void insert(T9TDTNode *&node, std::string &expression, int idx) {
		int digit = _charDigitMap.at(expression[idx]);
		if (!node) {
			node = new T9TDTNode(digit);
			if (idx < expression.size() - 1) {
				insert(node -> _middle, expression, idx + 1);
			} else {
				(node -> _expressions).push_front(expression);
			}
		} else if (digit < node -> _digit) {
			insert(node -> _left, expression, idx);
		} else if (node -> _digit < digit) {
			insert(node -> _right, expression, idx);
		} else {
			if (idx < expression.size() - 1) {
				insert(node -> _middle, expression, idx + 1);
			} else {
				(node -> _expressions).push_front(expression);
			}
		}
	}
	
	void autocomplete(T9TDTNode *node, std::vector<int> &digits, int idx, std::vector<std::string> &expressions, bool best = false) {
		int digit = digits[idx];
		if (!node) {
			return;
		} else if (digit < node -> _digit) {
			autocomplete(node -> _left, digits, idx, expressions, best);
		} else if (node -> _digit < digit) {
			autocomplete(node -> _right, digits, idx, expressions, best);
		} else {
			if (idx < digits.size() - 1) {
				autocomplete(node -> _middle, digits, idx + 1, expressions, best);
			} 
		}
		
		if (expressions.empty())
			if (!(node -> _expressions).empty()) {
				std::list<std::string>::iterator itr = (node -> _expressions).begin();
				while (itr != (node -> _expressions).end()) {
					expressions.push_back(*(itr++));
				}
			}
	}
public:
	T9TDTNode *_root;
	
	std::vector<std::vector<char>> _tuples;
	std::unordered_map<char, int> _charDigitMap;
	
	T9TernaryDigitsTrie() {
		_tuples = {
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
		for (int i = 0; i < _tuples.size(); ++i)
			for (int j = 0; j < _tuples[i].size(); ++j)
				_charDigitMap[_tuples[i][j]] = i;
		
		_root = nullptr;
	}
	
	virtual ~T9TernaryDigitsTrie() {
		clear(_root);
	}
	
	virtual void insert(std::string &expression) {
		if (!expression.empty()) {
			insert(_root, expression, 0);
		}
	}
	
	virtual void autocomplete(std::vector<int> &digits, std::vector<std::string> &expressions, bool best = false) {
		if (!digits.empty()) {
			expressions.clear();
			autocomplete(_root, digits, 0, expressions, best);
		}
	}
	
	virtual void displayTuples(std::ostream &os) {
		for (int i = 0; i < _tuples.size(); ++i) {
			os << i << " : " << _tuples[i] << std::endl;
		}
	}
};

#endif
