#pragma once
#include <cstdio>
#include <cstring>

class DString
{
public:
	DString()
	{
		cStr = new char[1]{'\0'};
	}
	DString(const char* cstring, int clenght)
	{
		char* tcStr = new char[clenght + 1];
		memcpy(tcStr, cstring, sizeof(char) * clenght);
		lenght = clenght + 1;
		tcStr[clenght] = 0;
		cStr = tcStr;
	}

	DString(char charstring[])
	{
		for (int i = 0; true; ++i)
		{
			if (charstring[i] == '\0')
			{
				lenght = i;
				break;
			}
		}
		char* tcStr = new char[lenght];
		memcpy(tcStr, &charstring, sizeof(char) * lenght);
		cStr = tcStr;
	}

	friend bool operator==(const DString& lhs, const DString& rhs)
	{
		return lhs.lenght == rhs.lenght && strcmp(lhs.cStr, rhs.cStr) == 0;
	}

	friend bool operator==(const DString& lhs, const char rhs[])
	{
		return strcmp(lhs.cStr, rhs) == 0;
	}

	~DString()
	{
		delete cStr;
	}

	void PrintString()
	{
		printf("[DString] '%s' \n", cStr);
	}

	signed int GetLenght()const { return lenght; }

	const char* GetCStr()const { return cStr; }

private:
	const char* cStr = nullptr;
	signed int lenght = 0;
};

