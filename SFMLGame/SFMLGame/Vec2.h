#pragma once

template <typename T>
class Vec2
{
public:
	T x = 0;
	T y = 0;

	Vec2();
	Vec2(T xin, T yin);

	bool operator == (const Vec2<T>& rhs) const;
	bool operator != (const Vec2<T>& rhs) const;

	Vec2<T> operator + (const Vec2<T>& rhs) const;
	Vec2<T> operator - (const Vec2<T>& rhs) const;
	Vec2<T> operator / (const T val) const;
	Vec2<T> operator * (const T val) const;

	void operator - ();
	void operator += (const Vec2<T>& rhs);
	void operator -= (const Vec2<T>& rhs);

	void operator += (const T val);
	void operator -= (const T val);
	void operator /= (const T val);
	void operator *= (const T val);

	float distance(const Vec2<T>& rhs) const;
	float length() const;

	void print() const;
	void normalize();

};

typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2u;
typedef Vec2<float> Vec2f;