#include "CSVParser.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

CSVParser::CSVParser(const char* File)
{
	FILE* filePtr = fopen(File, "r");
	fseek(filePtr, 0, SEEK_END);
	lenght = ftell(filePtr);
	fseek(filePtr, 0, SEEK_SET);
	str = new char[lenght]; // Warning source string has no \0
	lenght = fread(str, sizeof(char), lenght, filePtr);
	currentCharIndex = 0;
	xCursor = yCursor = 0;
	fclose(filePtr);
}

int CSVParser::ReadInt()
{
	char* data = nullptr;
	ReadNextElement(data);
	if (data == nullptr)return -2;

	const int value = atoi(data);
	delete[] data;
	return value;
}

char* CSVParser::ReadChar()
{
	char* data = nullptr;
	ReadNextElement(data);
	return data;
}

int CSVParser::NextColumn()
{
	if (!endOfRow)
	{
		if (ReachedEndOfData())
		{
			//This is cursed
			return ++yCursor;
		}
		printf("[FileParser] tried to goto next column without reading all data in row");
		assert(false && "[FileParser] tried to goto next column without reading all data in row");
	}

	//currentCharIndex++;
	endOfRow = false;
	if (xMax < xCursor)
		xMax = xCursor;
	xCursor = 0;
	return ++yCursor;
}

int CSVParser::ReadNextElement(char*& data)
{
	if (endOfRow)
	{
		assert(false && "Attempted to read element after reaching end of line");
		return -1;
	}

	if (ReachedEndOfData())
	{
		printf("[FileParser] Reached end of data on row '%i'\n", yCursor);
		//	assert(false && "Reached end of data");
		return-1;
	}

	const unsigned int readStart = currentCharIndex;

	do
	{
		//std::cout << currentCharIndex << " :: '" << str[currentCharIndex] << "'" << std::endl;
		currentCharIndex++;
	} while (str[currentCharIndex] != ',' && str[currentCharIndex] != '\n' && !ReachedEndOfData());

	const unsigned int elmLenght = currentCharIndex - readStart;

	if (str[currentCharIndex] == '\n')
	{
		endOfRow = true;
	}

	char* cstr = new char[elmLenght + 1];
	memcpy(cstr, &str[readStart], sizeof(char) * elmLenght);
	cstr[elmLenght] = 0;

	data = cstr;

	//Skip over element end
	currentCharIndex++;
	//Reached next element
	xCursor++;

	return elmLenght;
}
