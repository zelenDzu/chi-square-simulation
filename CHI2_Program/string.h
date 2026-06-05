#pragma once
#include <cstring>
#include <iostream>
#include <cassert>
using namespace std;

const size_t MAX_SIZE_STRING = 128;


class String
{
	char* str;
	int n;
	//static bool show_constructor_destructor;
	int IsEqual(const String& S) const
	{
		return strcmp(str, S.str) == 0;
	}
	int IsLessThan(const String& S) const
	{
		return (strcmp(str, S.str) < 0);
	}

public:
	String();
	explicit String(int nstr);
	String(const char* s);
	String(const String& S);
	String(String&& S) noexcept;
	~String();

	String(const char* s, int size);
	size_t get_n();

	String& operator = (const String& S);
	String& operator = (String&& s) noexcept;
	String& operator += (const String& S);
	String operator () (int i, int j); //���������� ���������
	//operator char* (); 

	friend bool operator == (const String& s1, const String& s2);
	friend bool operator < (const String& s1, const String& s2);
	friend bool operator != (const String& s1, const String& s2);
	friend bool operator > (const String& s1, const String& s2);
	friend bool operator <= (const String& s1, const String& s2);
	friend bool operator >= (const String& s1, const String& s2);

	friend String operator + (const String& s1, const String& s2);
	friend ostream& operator << (ostream& out, const String& S);
	friend istream& operator >> (istream& in, String& S);

	const char& operator[] (int index) const;
	char& operator[](int index);
};