
#ifndef workplace_T9Trie_h
#define workplace_T9Trie_h

#include <iostream>
#include <vector>

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

class T9Trie {
public:
	virtual void displayTuples(std::ostream &os) = 0;
	virtual void insert(std::string &expression) = 0;
	virtual void autocomplete(std::vector<int> &digits, std::vector<std::string> &expressions, bool best = false) = 0;
	
	virtual ~T9Trie() {
	};
};

#endif
