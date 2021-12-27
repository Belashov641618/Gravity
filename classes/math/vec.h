#pragma once

#include <iostream>

template<typename type, uint32_t size> class vec {
	
	type data[size];

public:

	/* Конструкторы */
	vec(const type def_val = type())  restrict(amp, cpu);
	vec(const vec<type, size>& other) restrict(amp, cpu);

	/* Операторы равенства */
	vec<type, size>& operator= (const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator+=(const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator-=(const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator*=(const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator*=(const type value)			  restrict(amp, cpu);
	vec<type, size>& operator/=(const type value)			  restrict(amp, cpu);

	/* Операторы сравнения */
	bool operator==(const vec<type, size>& other) const restrict(amp, cpu);
	bool operator> (const vec<type, size>& other) const restrict(amp, cpu);
	bool operator>=(const vec<type, size>& other) const restrict(amp, cpu);
	bool operator< (const vec<type, size>& other) const restrict(amp, cpu);
	bool operator<=(const vec<type, size>& other) const restrict(amp, cpu);



	/* Операторы математических операций */
	vec<type, size> operator+(const vec<type, size>& other) const restrict(amp, cpu);
	vec<type, size> operator-(const vec<type, size>& other) const restrict(amp, cpu);
	vec<type, size> operator*(const vec<type, size>& other) const restrict(amp, cpu);
	vec<type, size> operator*(const type value)				const restrict(amp, cpu);
	vec<type, size> operator/(const type value)				const restrict(amp, cpu);

	/* Глобальные математические функции */
	static type dot(const vec<type, size>& vec1, const vec<type, size>& vec2) restrict(amp, cpu);

	/* Математические функции */
	type dot(const vec<type, size> other) restrict(amp, cpu);

	/* Инкреминация */
	type& operator[](const uint32_t n)       restrict(amp, cpu);
	type  operator()(const uint32_t n) const restrict(amp, cpu);

	/* Вывод */
	friend std::ostream& operator<<(std::ostream& stream, const vec<type, size>& other);

};

#include "vec.cpp"