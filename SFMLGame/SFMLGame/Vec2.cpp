#include "Vec2.h"

#include <iostream>
#include <math.h>

template <typename T>
Vec2<T>::Vec2()
{}

template <typename T>
Vec2<T>::Vec2(T xin, T yin)
	:x(xin), y(yin)
{}

template <typename T>
bool Vec2<T>::operator == (const Vec2<T>& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

template <typename T>
bool Vec2<T>::operator != (const Vec2<T>& rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

template <typename T>
Vec2<T> Vec2<T>::operator + (const Vec2<T>& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

template <typename T>
Vec2<T> Vec2<T>::operator - (const Vec2<T>& rhs) const
{
	return Vec2<T>(x - rhs.x, y - rhs.y);
}

template <typename T>
Vec2<T> Vec2<T>::operator / (const T val) const
{
	return Vec2<T>(x / val, y / val);
}

template <typename T>
Vec2<T> Vec2<T>::operator * (const T val) const
{
	return Vec2<T>(x * val, y * val);
}

template <typename T>
void Vec2<T>::operator - ()
{
	x *= -1;
	y *= -1;
}

template <typename T>
void Vec2<T>::operator += (const Vec2<T>& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

template <typename T>
void Vec2<T>::operator -= (const Vec2<T>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

template <typename T>
void Vec2<T>::operator += (const T val)
{
	x += val;
	y += val;
}

template <typename T>
void Vec2<T>::operator -= (const T val)
{
	x -= val;
	y -= val;
}

template <typename T>
void Vec2<T>::operator /= (const T val)
{
	x /= val;
	y /= val;
}

template <typename T>
void Vec2<T>::operator *= (const T val)
{
	x *= val;
	y *= val;
}

template <typename T>
float Vec2<T>::distance(const Vec2<T>& rhs) const
{
	return sqrt((rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y));
}

template <typename T>
float Vec2<T>::length() const
{
	return sqrt(x * x + y * y);
}

template <typename T>
void Vec2<T>::print() const
{
	std::cout << x << " " << y << "\n";
}

template <typename T>
void Vec2<T>::normalize()
{
	float len = length();
	if (len != 0)
	{
		x /= len;
		y /= len;
	}
}

template class Vec2<int>;
template class Vec2<unsigned int>;
template class Vec2<float>;