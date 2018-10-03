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
	
	if (d == c)
	{
		printf_s("d and c strings are equals \n");
	}
	else
	{
		printf_s("d and c strings are not equals");
	}

	a = "bye";

	a.Print();

	//c.Print();

	std::cin.get();
}