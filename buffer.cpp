#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cstdio>

#include "buffer.h"

using namespace std;

bool InputBuffer::EndOfInput()
{
	//If the input file is empty, return EOF
	if (!input_buffer.empty())
		return false;
	else
		return cin.eof();
}

char InputBuffer::UngetChar(char c)
{
	//If it's not the EOF, place the buffer back one space
	if (c != EOF)
		input_buffer.push_back(c);;
	return c;
}

void InputBuffer::GetChar(char& c)
{
	//Parse the next character
	if (!input_buffer.empty()) {
		c = input_buffer.back();
		input_buffer.pop_back();
	}
	else {
		cin.get(c);
	}
}

string InputBuffer::UngetString(string s)
{
	//Place the buffer before the read in string
	for (int i = 0; i < s.size(); i++)
		input_buffer.push_back(s[s.size() - i - 1]);
	return s;
}