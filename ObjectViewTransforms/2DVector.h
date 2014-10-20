#include <string>

class TwoDVector
{
public:
	TwoDVector(float X, float Z) : x(X), z(Z) {}

	float x;
	float z;

	float crossProduct(TwoDVector);
	TwoDVector subtract(TwoDVector);
	TwoDVector add(TwoDVector);
	TwoDVector scale(float);
	float mag();
	std::string toString();
};

// Return this x v
float TwoDVector::crossProduct(TwoDVector v)
{
	return x * v.z - z * v.x;
}

// Return this - v
TwoDVector TwoDVector::subtract(TwoDVector v)
{
	return TwoDVector(x - v.x, z - v.z);
}

// Return this + v
TwoDVector TwoDVector::add(TwoDVector v)
{
	return TwoDVector(x + v.x, z + v.z);
}

// Return this * scale
TwoDVector TwoDVector::scale(float scale)
{
	return TwoDVector(scale * x, scale * z);
}

std::string TwoDVector::toString()
{
	return std::string("x: " + std::to_string(x) + " z: " + std::to_string(z));
}

float TwoDVector::mag()
{
	return sqrt(pow(x, 2) + pow(z, 2));
}
