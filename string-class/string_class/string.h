/* create string class capable of:
string a;
string b(a);
string c("hello");
if (a == "...") -> bool
b = "bye";
*/

#ifndef _STRING
#define _STRING

class string
{
private:

	char* text = nullptr;
	unsigned int size = 0;

public:

	string() {}

	string(const string& string)
	{
		if (string.text != nullptr)
		{
			AllocMem(string.size);
			strcpy_s(text, size, string.text);
		}
	}

	string(const char* string)
	{
		size = strlen(string)+1; // null terminated
		text = new char[size];
		strcpy_s(text, size, string);
	}

	~string()
	{
		if (text != nullptr)
		{
			delete[] text;
		}
	}

	// methods
	string operator = (const string& string)
	{
		if (string.size > 0)
		{
			delete[] text;
			AllocMem(string.size);
			strcpy_s(text, size, string.text);
		}

		return *this;
	}

	bool operator == (const string& string) const
	{
		if (string.text != nullptr && text != nullptr)
			return strcmp(string.text, text) == 0; //0 -> the contents of both strings are equal
		else
			return false;
	}

	bool operator +=(const string& str) 
	{
		bool ret = true;

		if (str.text != nullptr && text != nullptr)
		{
			unsigned int tempSize = strlen(text) + strlen(str.text) + 1;
			char* tempString = text;
			AllocMem(tempSize);

			strcpy_s(text, size, tempString);
			strcat_s(text, size, str.text);

			delete[] tempString;
		}
		else
			ret = false;

		return ret;	
	}

	// print test
	void Print() const
	{
		if (text != nullptr)
			printf_s("%s\n", text);
	}

private:
	
	void AllocMem(const unsigned int memSize)
	{
		if (memSize > 0)
		{
			size = memSize;
			text = new char[size];
		}
	}

};
#endif // !_STRING
