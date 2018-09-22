/*
Create a new vec3 class with:

- Contains three values x,y,z with templatized type
- Think of at least three handy constructors
- Operators +, -, +=, -=, =, ==
- Methods: normalize(), zero(), is_zero(), distance_to(vec3 …)
- Mind references and const!

*/

#include <iostream>
#include "vec3.h"


void main()
{

	iVec3 vect1(3, 4, 2);
	iVec3 vect1op(1, 1, 1);
	iVec3 vect2(1, 1, 1);

	fVec3 vect31(1, 1, 1);
	fVec3 vect3(4, 7, 4);

	fVec3 vectFloat(5.4f,3.1f,6.7f);

	vec3<char>vectChar(9,7,3);

	vect1.Print();

	// operators test -----

	vect1 = vect1 + vect1op;

	vect1.Print();

	vect1 = vect1 - vect1op;

	vect1.Print();

	vect1 += vect1op;

	vect1.Print();

	vect1 -= vect1op;

	vect1.Print();

	vect1 = vect1op;

	vect1.Print();

	if (vect1 == vect2)
	{
		std::cout << "vect1 and 2 are equals" << std::endl;
	};

	// --------------------
	//methods test

	vectFloat.Print();

	vectFloat.normalize();

	vectFloat.Print();

	vectFloat.SetToZero();

	vectFloat.Print();

	// distance
	float distance = vect31.distanceTo(vect3);

	std::cout << "distanceTo: " << distance << std::endl;


	std::cin.get();
	
}