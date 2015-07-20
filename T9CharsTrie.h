
#ifndef workplace_T9CharsTrie_h
#define workplace_T9CharsTrie_h

#include "T9Trie.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

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

class T9CTNode {
public:
	T9CTNode **_charNodes;
	bool _terminal;
	
	T9CTNode(size_t count) {
		if (count > 0) {
			_charNodes = new T9CTNode *[count];
			for (int i = 0; i < count; ++i)
				_charNodes[i] = nullptr;
		} else {
			_charNodes = nullptr;
		}
		_terminal = false;
	}
	~T9CTNode() {
		if (_charNodes) {
			delete [] _charNodes;
			_charNodes = nullptr;
		}
	}
};

class T9CharsTrie : public T9Trie {
	void clear(T9CTNode *&node) {
		if (!node)
			return;
		
		for (int i = 0; i < _chars.size(); ++i)
			clear(node -> _charNodes[i]);
		
		delete node;
		node = nullptr;
	}
	
	void insert(T9CTNode *node, std::string &expression, int idx) {
		if (idx < expression.size()) {
			int childIdx = _charDigitMap.at(expression[idx]);
			if (!node -> _charNodes[childIdx])
				node -> _charNodes[childIdx] = new T9CTNode(_chars.size());
			insert(node -> _charNodes[childIdx], expression, idx + 1);
		} else
			node -> _terminal = true;
	}
	
	void update(std::string &expression) {
		ExpressionInfo &expressionInfo = _expressionsInfo[expression];
		expressionInfo.update(++_timestamp);
	}
	
	void complete(T9CTNode *node, std::string &expression, std::vector<std::string> &expressions, bool best = false) {
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
	
	void autocomplete(T9CTNode *node, std::vector<int> &digits, int idx, std::string &expression, std::vector<std::string> &expressions, bool best = false) {
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
	T9CTNode *_root;
	
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
		
		_root = new T9CTNode(_chars.size());
	}
	
	virtual ~T9CharsTrie() {
		clear(_root);
	}
	
	virtual void insert(std::string &expression) {
		if (expression.size()) {
			update(expression);
			insert(_root, expression, 0);
		}
	}
	
	virtual void autocomplete(std::vector<int> &digits, std::vector<std::string> &expressions, bool best = false) {
		if (digits.size()) {
			expressions.clear();
			std::string expression;
			autocomplete(_root, digits, 0, expression, expressions, best);
		}
	}
	
	virtual void displayTuples(std::ostream &os) {
		for (int i = 0; i < _tupes.size(); ++i) {
			os << i << " : " << _tupes[i] << std::endl;
		}
	}
};

#endif
