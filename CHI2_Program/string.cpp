//#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include "string.h"

String::String()
{
	//if (this->show_constructor_destructor) std::cout << "\n--empty constructor--\n";
	n = 0;
	str = new char[n + 1];
	str[n] = '\0';
}

String::String(int nstr)
{
	//if (this->show_constructor_destructor) std::cout << "\n--int constructor--\n";
	assert(nstr >= 0);
	n = nstr;
	str = new char[n + 1];
	memset(str, ' ', n);
	str[n] = '\0';
}

String::String(const char* s)
{
	//if (this->show_constructor_destructor) std::cout << "\n--char* constructor--\n";
	n = strlen(s);
	str = new char[n + 1];
	strcpy(str, s);
}

String::String(const String& S)
{
	//if (this->show_constructor_destructor) std::cout << "\n--copy constructor--\n";
	n = S.n;
	str = new char[n + 1];
	strcpy(str, S.str);
}

String::String(String&& S) noexcept
{
	//if (this->show_constructor_destructor) std::cout << "\n--move constructor--\n";
	n = S.n;
	str = S.str;
	S.n = 0;
	S.str = new char[1];
	S.str[0] = '\0';
}

String::String(const char* s, int size)
{
	//if (this->show_constructor_destructor) std::cout << "\n--char* and size constructor--\n";
	assert(size >= 0 && size <= strlen(s));
	/*if (size < 0)
	{
		n = 0;
		str = new char[1];
		str[0] = '\0';
		return;
	}

	if (size > strlen(s))
		size = strlen(s);*/


	n = size;
	str = new char[n + 1];
	memcpy(str, s, n);
	str[n] = '\0';
}

String::~String()
{
	//if (this->show_constructor_destructor) std::cout << "\nDESTROY STRING!\n";
	n = 0;
	delete[] str;
}

String& String::operator = (const String& S)
{
	if (this != &S)
	{
		if (n != S.n)
		{
			delete[] str;
			n = S.n;
			str = new char[n + 1];
		}

		strcpy(str, S.str);
	}

	return *this;
}

String& String::operator= (String&& s) noexcept
{
	if (this != &s)
	{
		delete[] str;
		n = s.n;
		str = s.str;
		s.n = 0;
		s.str = new char[1];
		s.str[0] = '\0';
	}

	return *this;
}


String& String::operator+= (const String& S)
{
	char* temp = new char[n + S.n + 1];
	strcpy(temp, str);
	strcpy(temp + n, S.str);
	delete[] str;
	str = temp;
	n += S.n;

	temp = nullptr;
	return *this;
}


String String::operator () (int i, int j)
{
	if (i > j)
	{
		i += j;
		j = i - j;
		i -= j;
	}
	assert(i >= 0 && j < n);
	/*if (i < 0)
	{
		i = 0;
	}
	if (j >= n)
	{
		j = n - 1;
	}*/

	int size = j - i + 1;
	return String(str + i, size);
}


bool operator == (const String& s1, const String& s2)
{
	return s1.IsEqual(s2);
}

bool operator < (const String& s1, const String& s2)
{
	return s1.IsLessThan(s2);
}

bool operator != (const String& s1, const String& s2)
{
	return !(s1 == s2);
}

bool operator > (const String& s1, const String& s2)
{
	return !(s1 == s2 || s1 < s2);
}

bool operator <= (const String& s1, const String& s2)
{
	return (s1 == s2 || s1 < s2);
}

bool operator >= (const String& s1, const String& s2)
{
	return (s1 == s2 || s1 > s2);
}

String operator + (const String& s1, const String& s2)
{
	String temp(s1);
	temp += s2;
	return temp;
}

ostream& operator<< (ostream& out, const String& S)
{
	out << S.str;
	return out;
}

istream& operator >> (istream& in, String& S)
{
	char str[MAX_SIZE_STRING];
	in >> str;
	//in.getline(str, MAX_SIZE_STRING);

	if (in)
	{
		S = str;
	}

	return in;
}

const char& String::operator[] (int index) const
{
	assert(index >= 0 && index < n);
	return str[index];
}

char& String::operator[](int index)
{
	assert(index >= 0 && index < n);
	return str[index];
}

//String::operator char* ()
//{
//	return str;
//}

size_t String::get_n()
{
	return n;
}
