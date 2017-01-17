#pragma once
#ifndef _STRING_BUFFER_H_
#define _STRING_BUFFER_H_

#include <stdio.h>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>


class string_buffer
{
private:
	static const int default_size = 4096;
	wchar_t* buffer;
	size_t capacity;
	size_t length;

	string_buffer(const string_buffer&);
	string_buffer& operator =(const string_buffer&);
public:
	string_buffer();

	~string_buffer();

	const wchar_t* c_str() const;

	void append(const wchar_t* str, size_t strLen);
};

#endif