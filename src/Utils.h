#pragma once

#include <string>
#include <vector>

class Utils {
public:
	/// Split a string <text> everywhere a certain character <ch> is found
	/// store the resulting substring in a vector <strs>
	/// returns the size of the vector
	static unsigned int split(const std::string &text, std::vector<std::string> &strs, char ch) {
		int pos = text.find(ch);
		int initialPos = 0;
		strs.clear();
		
		while (pos != std::string::npos) {
			strs.push_back(text.substr(initialPos, pos - initialPos + 1));
			initialPos = pos + 1;

			pos = text.find(ch, initialPos);
		}

		strs.push_back(text.substr(initialPos, std::min<int>(pos, text.size() - (initialPos + 1))));

		return strs.size();
	}

private:

};
