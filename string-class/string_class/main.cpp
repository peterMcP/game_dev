#include <iostream>
#include "string.h"

void main()
{
	string a;
	string b(a);
	string c("hello");

	string d(c);

	b = a;

	b.Print();
	d.Print();
	
	if (a == c)
	{
		printf_s("d and c strings are equals \n");
	}
	else
	{
		printf_s("d and c strings are not equals \n");
	}

	a = "bye";

	a.Print();

	a += c;

	a += "peter";

	a.Print();

	string e = "hey";
	string f = "byee";

	f.Print();

	f = e;

	f.Print();

	std::cin.get();
}