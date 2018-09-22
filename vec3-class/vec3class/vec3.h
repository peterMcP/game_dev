#ifndef _VEC3
#define _VEC3

template<typename TYPE>
class vec3
{
public:

	TYPE x, y, z;

public:
	// constructors ---
	vec3() {}
	vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}
	vec3(TYPE X, TYPE Y, TYPE Z) : x(X),y(Y),z(Z) {}

	// ------------------
	// operators

	vec3 operator + (const vec3 &v) const
	{
		vec3 result;

		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;

		return result;
	}

	vec3 operator - (const vec3& v) const
	{
		return vec3(x-v.x, y-v.y, z-v.z);
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

	bool operator == (const vec3& v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	// methods functions ---

	void Print() // test
	{
		std::cout << "x:" << x << " " << "y:" << y << " " << "z:" << z << std::endl;
	};

	void normalize()
	{
		
		TYPE magnitude = sqrt(x * x + y * y + z * z);

		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;

	}

	void SetToZero()
	{
		x = y = z = TYPE(0);
	}

	bool isZero() const
	{
		return (x == 0 && y == 0 && z == 0);
	}

	TYPE distanceTo(const vec3& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;
		TYPE fz = z - v.z;

		return (TYPE)sqrtf(float(fx*fx) + float(fy*fy) + float(fz*fz));
	}

	TYPE distanceToSquared(const vec3& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;
		TYPE fz = z - v.z;

		return (TYPE)(float(fx*fx) + float(fy*fy) + float(fz*fz));
	}

	// ---------------------

};
//
typedef vec3<int> iVec3;
typedef vec3<float> fVec3;
//

#endif // !_VEC3

