#include <sstream>
#include "imstring.h"

using namespace std;

string trim_leading_whitespace(string str) {
	while (str.size() && isspace(str[0])) { str.erase(0, 1); }
	return str;
}

string remove_comments(string str, char delim) {
	// remove all characters after the first comment character
	size_t comment = str.size();
	for (size_t i=0; i<str.size(); i++) { 
		if (str[i] == delim) {
			comment = i;
			break;
	}}

	// remove all characters after & including the comment
	while (str.size() > comment) { str.erase(str.size()-1, 1); }
	return str;
}

bool is_string_empty(std::string str) {
	bool empty = true;
	for (size_t i=0; i<str.size(); i++) { empty = empty && isspace(str[i]); }
	return empty;
}
