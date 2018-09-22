#include <iostream>
#include "string.h"

void main()
{
	string a;
	string b(a);
	string c("hello");

	b = c;

	c.print();
	
	if (b == c)
	{
		std::cout << "b and c strings are equals" << std::endl;
	}
	else
	{
		std::cout << "b and c strings are not equals" << std::endl;
	}

	c = "bye";

	c.print();

	b.print();


	std::cin.get();
}