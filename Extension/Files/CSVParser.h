#pragma once

class CSVParser
{
public:
	CSVParser(const char* File);

	~CSVParser()
	{
		delete[] str;
	}

	CSVParser(const CSVParser& Other) = delete;
	CSVParser(CSVParser&& Other) = delete;
	CSVParser& operator=(const CSVParser& Other) = delete;

	/// <summary>
	/// Reads the next row and returns value as int
	/// </summary>
	/// <returns>Int value found, -2 if no value was read</returns>
	int ReadInt();
	/// <summary>
	/// Reads the next row and returns value as string
	/// </summary>
	/// <returns>Data read from this column,You are responsible for deleting this item</returns>
	char* ReadChar();
	/// <summary>
	/// Call this once you are at the end of the row.
	/// Will throw an error when we are not done reading data in this row.
	/// </summary>
	/// <returns>The new column we are reading</returns>
	int NextColumn();

	bool ReachedEndOfData() const { return currentCharIndex >= lenght; }
	bool ReachedEndOfRow() const { return endOfRow || ReachedEndOfData(); }

	const unsigned short GetRows() const { return xMax; }
	const unsigned short GetColmns() const { return yCursor; }
private:
	int ReadNextElement(char*& data);

private:
	bool endOfRow = false;
	/// <summary>
	/// The amount of characters in this file
	/// </summary>
	unsigned int lenght = 0;
	/// <summary>
	/// Start of the data found in this file.
	/// </summary>
	char* str = nullptr;
	/// <summary>
	/// The current index of the file we are reading.
	/// </summary>
	unsigned int currentCharIndex = 0;
	//The Current row we are parsing
	unsigned short xCursor = 0;

	unsigned short xMax = 0;
	//The Current column
	unsigned short yCursor = 0;
};
