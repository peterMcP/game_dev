/*
Create a new vec3 class with:

- Contains three values x,y,z with templatized type
- Think of at least three handy constructors
- Operators +, -, +=, -=, =, ==
- Methods: normalize(), zero(), is_zero(), distance_to(vec3 …)
- Mind references and const!

*/

#include <iostream>


template<typename T>
class vec3
{
public:

	T x, y, z;

public:
	// constructors ---
	vec3() {}
	vec3(const vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	vec3(T X, T Y, T Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	// ------------------

	// operators

	vec3 operator + (const vec3 &v) //const
	{
		vec3 result;

		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;

		return result;
	}

	vec3 operator - (const vec3& v)
	{
		vec3 result;

		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;

		return result;
	}

	vec3 operator += (const vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	vec3 operator -= (const vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	vec3 operator = (const vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	bool operator == (const vec3& v)
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	// methods functions ---

	void Print() 
	{
		std::cout << "x:" << x << " " << "y:" << y << " " << "z:" << z << std::endl;
	};

	void normalize()//const vec3& v)
	{
		T magnitude;

		magnitude = sqrt(x * x + y * y + z * z);
		
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
		
	}

	void SetToZero()
	{
		x = y = z = 0;
		//return *this;
	}

	bool isZero()
	{
		return (x == 0 && y == 0 && z == 0);
	}

	T distanceTo(const vec3& v)
	{
		T fx = x - v.x;
		T fy = y - v.y;
		T fz = z - v.z;

		return (T)sqrtf(float(fx*fx) + float(fy*fy) + float(fz*fz));
	}

	// ---------------------

};
//
typedef vec3<int> iVec3;
typedef vec3<float> fVec3;
//


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