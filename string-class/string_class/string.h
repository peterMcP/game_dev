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

	char* text;

public:

	string() {}
	string(const string& string) : text(string.text) {}
	string(const char* string) : text((char*)string) {}

	// methods
	string operator = (const string& string)
	{
		text = string.text;

		return *this;
	}

	bool operator == (const string& string) const
	{
		return (text == string.text);
	}

	// print test
	void print()
	{
		if (text != NULL)
			std::cout << text << std::endl;
	}


};
#endif // !_STRING
