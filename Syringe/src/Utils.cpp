#include "Utils.h"


std::string GetFileNameFromPath(std::string path) {
	std::size_t found = path.find_last_of("\\\\");
	return path.substr(found + 1);
}


std::vector<std::string> Split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	Split(s, delim, std::back_inserter(elems));
	return elems;
}


// C++98 guarantees that '0', '1', ... '9' are consecutive.
// It only guarantees that 'a' ... 'f' and 'A' ... 'F' are
// in increasing order, but the only two alternative encodings
// of the basic source character set that are still used by
// anyone today (ASCII and EBCDIC) make them consecutive.
unsigned char GetHexValue(unsigned char c) {
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('a' <= c && c <= 'f')
		return c - 'a' + 10;
	else if ('A' <= c && c <= 'F')
		return c - 'A' + 10;
	else abort();
}


std::string ConvertHexToAscii(const std::string& in) {
	std::string result;
	result.reserve(in.length() / 2);

	for (std::string::const_iterator p = in.begin(); p != in.end(); p++)
	{
		unsigned char c = GetHexValue(*p);
		p++;
		if (p == in.end()) break; // incomplete last digit - should report error
		c = (c << 4) + GetHexValue(*p); // + takes precedence over <<
		result.push_back(c);
	}

	return result;
}