#pragma once

#include "Common.h"

#include <iostream>
#include <sstream>
#include <iterator>



/// <summary>
/// Gets the file's name from a path
/// </summary>
/// <param name="path">Path of the file</param>
/// <returns>File's name retrieved from the given path</returns>
std::string GetFileNameFromPath(std::string path);


/// <summary>
/// Template for splitting
/// </summary>
/// <see cref="https://stackoverflow.com/a/236803"/>
template<typename Out>
void Split(const std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}


/// <summary>
/// Splits a string by a delimiter
/// </summary>
/// <param name="s">String to split</param> 
/// <param name="delimiter">Delimiter used to split</param>
/// <returns>The string splitted</returns>
std::vector<std::string> Split(const std::string &s, char delimiter);


/// <summary>
/// Gets the hexadecimal value of a character
/// </summary>
/// <see cref="https://stackoverflow.com/a/3790707"/>
/// <param name="c">Character to convert</param>
/// <returns>Hexadecimal value of the character</returns>
unsigned char GetHexValue(unsigned char c);


/// <summary>
/// Gets the hexadecimal value of a character
/// </summary>
/// <see cref="https://stackoverflow.com/a/3790707"/>
/// <param name="c">Character to convert</param>
/// <returns>Hexadecimal value of the character</returns>
std::string ConvertHexToAscii(const std::string& in);