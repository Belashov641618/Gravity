#pragma once

#include <iostream>

template<typename type, uint32_t size> class vec {
	
	type data[size];

public:

	/* ������������ */
	vec(const type def_val = type())  restrict(amp, cpu);
	vec(const vec<type, size>& other) restrict(amp, cpu);

	/* ��������� ��������� */
	vec<type, size>& operator= (const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator+=(const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator-=(const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator*=(const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator*=(const type value)			  restrict(amp, cpu);
	vec<type, size>& operator/=(const type value)			  restrict(amp, cpu);

	/* ��������� ��������� */
	bool operator==(const vec<type, size>& other) const restrict(amp, cpu);
	bool operator> (const vec<type, size>& other) const restrict(amp, cpu);
	bool operator>=(const vec<type, size>& other) const restrict(amp, cpu);
	bool operator< (const vec<type, size>& other) const restrict(amp, cpu);
	bool operator<=(const vec<type, size>& other) const restrict(amp, cpu);



	/* ��������� �������������� �������� */
	vec<type, size> operator+(const vec<type, size>& other) const restrict(amp, cpu);
	vec<type, size> operator-(const vec<type, size>& other) const restrict(amp, cpu);
	vec<type, size> operator*(const vec<type, size>& other) const restrict(amp, cpu);
	vec<type, size> operator*(const type value)				const restrict(amp, cpu);
	vec<type, size> operator/(const type value)				const restrict(amp, cpu);

	/* ���������� �������������� ������� */
	static type dot(const vec<type, size>& vec1, const vec<type, size>& vec2) restrict(amp, cpu);

	/* �������������� ������� */
	type dot(const vec<type, size> other) restrict(amp, cpu);

	/* ������������ */
	type& operator[](const uint32_t n)       restrict(amp, cpu);
	type  operator()(const uint32_t n) const restrict(amp, cpu);

	/* ����� */
	friend std::ostream& operator<<(std::ostream& stream, const vec<type, size>& other);

};

#include "vec.cpp"