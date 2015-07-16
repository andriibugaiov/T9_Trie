//
//  T9CharsTrie.h
//  workplace
//
//  Created by Andrii Bugaiov on 2015-07-16.
//  Copyright (c) 2015 MyCompany. All rights reserved.
//

#ifndef workplace_T9CharsTrie_h
#define workplace_T9CharsTrie_h

#include <iostream>

#include <string>
#include <vector>
#include <unordered_map>

template<typename T> std::ostream& operator<< (std::ostream& os, const std::vector<T>& v) {
	os << "[";
	size_t last = v.size() - 1;
	for(size_t i = 0; i < v.size(); ++i) {
		os << v[i];
		if (i != last)
			os << ", ";
	}
	os << "]";
	return os;
}

// mark -

class ExpressionInfo {
public:
	long long _frequency;
	long long _timestamp;
	
	ExpressionInfo() {
		_frequency = 0;
		_timestamp = 0;
	}
	
	void update(long long timestamp) {
		_frequency += 1;
		_timestamp = timestamp;
	}
	
	bool operator<(const ExpressionInfo &expressionInfo) {
		if (_frequency < expressionInfo._frequency)
			return true;
		else if (expressionInfo._frequency < _frequency)
			return false;
		else if (_timestamp < expressionInfo._timestamp)
			return true;
		else
			return false;
	}
};

class TrieNode {
public:
	TrieNode **_charNodes;
	bool _terminal;
	
	TrieNode(unsigned long count) {
		if (count > 0) {
			_charNodes = new TrieNode *[count];
			for (int i = 0; i < count; ++i)
				_charNodes[i] = nullptr;
		} else {
			_charNodes = nullptr;
		}
		_terminal = false;
	}
	~TrieNode() {
		if (_charNodes) {
			delete [] _charNodes;
			_charNodes = nullptr;
		}
	}
};

class T9CharsTrie {
	void clear(TrieNode *&node) {
		for (int i = 0; i < _chars.size(); ++i)
			if (node -> _charNodes[i])
				clear(node -> _charNodes[i]);
		
		delete node;
		node = nullptr;
	}
	
	void insert(TrieNode *node, std::string &expression, int idx) {
		if (idx < expression.size()) {
			int childIdx = _charDigitMap.at(expression[idx]);
			if (!node -> _charNodes[childIdx])
				node -> _charNodes[childIdx] = new TrieNode(_chars.size());
			insert(node -> _charNodes[childIdx], expression, idx + 1);
		} else
			node -> _terminal = true;
	}
	
	void update(std::string &expression) {
		ExpressionInfo &expressionInfo = _expressionsInfo[expression];
		expressionInfo.update(++_timestamp);
	}
	
	void complete(TrieNode *node, std::string &expression, std::vector<std::string> &expressions, bool best = false) {
		for (int i = 0; i < _chars.size(); ++i) {
			if (node -> _charNodes[i]) {
				expression.push_back(_chars[i]);
				complete(node -> _charNodes[i], expression, expressions, best);
				expression.pop_back();
			}
		}
		if (node -> _terminal) {
			if (!best || (best && !expressions.size())) {
				expressions.push_back(expression);
			} else {
				if (_expressionsInfo.at(expressions[0]) < _expressionsInfo.at(expression)) {
					expressions.pop_back();
					expressions.push_back(expression);
				}
			}
		}
	}
	
	void autocomplete(TrieNode *node, std::vector<int> &digits, int idx, std::string &expression, std::vector<std::string> &expressions, bool best = false) {
		if (idx < digits.size()) {
			std::vector<char> &tuple = _tupes[digits[idx]];
			for (int i = 0; i < tuple.size(); ++i) {
				int childIdx = _charDigitMap.at(tuple[i]);
				if (node -> _charNodes[childIdx]) {
					expression.push_back(tuple[i]);
					autocomplete(node -> _charNodes[childIdx], digits, idx + 1, expression, expressions, best);
					expression.pop_back();
				}
			}
		} else if (expression.size()) {
			complete(node, expression, expressions, best);
		}
	}
public:
	TrieNode *_root;
	
	std::vector<std::vector<char>> _tupes;
	std::vector<char> _chars;
	std::unordered_map<char, int> _charDigitMap;
	
	long long _timestamp;
	std::unordered_map<std::string, ExpressionInfo> _expressionsInfo;
	
	T9CharsTrie() {
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
				_chars.push_back(_tupes[i][j]);
		
		for (int i = 0; i < _chars.size(); ++i)
			_charDigitMap[_chars[i]] = i;
		
		_root = new TrieNode(_chars.size());
	}
	
	~T9CharsTrie() {
		clear(_root);
	}
	
	void insert(std::string &expression) {
		update(expression);
		insert(_root, expression, 0);
	}
	
	void autocomplete(std::vector<int> &digits, std::vector<std::string> &expressions, bool best = false) {
		if (digits.size()) {
			std::string expression;
			autocomplete(_root, digits, 0, expression, expressions, best);
		}
	}
	
	void displayTuples(std::ostream &os) {
		for (int i = 0; i < _tupes.size(); ++i) {
			os << i << " : " << _tupes[i] << std::endl;
		}
	}
};

#endif
