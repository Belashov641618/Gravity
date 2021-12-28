#pragma once

#include <iostream>

#define vec_open_scope	"{"
#define vec_close_scope "}"
#define vec_separator	", "

#include <cmath>
#include <amp_math.h>



template<typename type, uint32_t size> class vec {
protected:

	type data[size];

public:

	vec(std::initializer_list<type> list) restrict(cpu);

	/* Конструкторы */
	vec()								restrict(amp, cpu);
	vec(const type def_val)				restrict(amp, cpu);
	vec(const vec<type, size>& other)	restrict(amp, cpu);

	/* Операторы равенства */
	vec<type, size>& operator= (const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator+=(const vec<type, size>& other) restrict(amp, cpu);
	vec<type, size>& operator-=(const vec<type, size>& other) restrict(amp, cpu);
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

	vec<type, size> operator*(const type value)	const restrict(amp, cpu);
	vec<type, size> operator/(const type value)	const restrict(amp, cpu);
	friend vec<type, size> operator*(const type value, const vec<type, size>& vec) restrict(amp, cpu) {
		return vec * value;
	}

	type operator*(const vec<type, size>& other) const restrict(amp, cpu);

	/* Глобальные математические функции */

	/* Математические функции */
	type dot(const vec<type, size> other) restrict(amp, cpu);
	type len() const restrict(cpu);
	type len() const restrict(amp);
	vec<type, size> dir() const restrict(amp, cpu);

	/* Инкреминация */
	type& operator[](const uint32_t n)			restrict(amp, cpu);
	type  operator()(const uint32_t n)	  const restrict(amp, cpu);

	/* Вывод */
	friend std::ostream& operator<<(std::ostream& stream, const vec<type, size>& vec) restrict(cpu) {
		stream << vec_open_scope;
		for (uint32_t i = 0; i < size - 1; i++)
			stream << vec(i) << vec_separator;
		stream << vec(size - 1) << vec_close_scope;
		return stream;
	}

}; 



template<typename type, uint32_t size> vec<type, size>::vec(std::initializer_list<type> list) restrict(cpu) {
	for (uint32_t i = 0; i < list.size() && i < size; i++)
		data[i] = list.begin()[i];
	for (uint32_t i = list.size(); i < size; i++)
		data[i] = type();
}

/* Конструкторы */
template<typename type, uint32_t size> vec<type, size>::vec() restrict(amp, cpu) {
	for (uint32_t i = 0; i < size; i++)
		data[i] = type();
}
template<typename type, uint32_t size> vec<type, size>::vec(const type def_val) restrict(amp, cpu) {
	for (uint32_t i = 0; i < size; i++)
		data[i] = def_val;
}
template<typename type, uint32_t size> vec<type, size>::vec(const vec<type, size>& other) restrict(amp, cpu) {
	for (uint32_t i = 0; i < size; i++)
		data[i] = other.data[i];
}


/* Операторы равенства */
template<typename type, uint32_t size> vec<type, size>& vec<type, size>::operator= (const vec<type, size>& other) restrict(amp, cpu) {
	for (uint32_t i = 0; i < size; i++)
		data[i] = other.data[i];
	return *this;
}
template<typename type, uint32_t size> vec<type, size>& vec<type, size>::operator+=(const vec<type, size>& other) restrict(amp, cpu) {
	for (uint32_t i = 0; i < size; i++)
		data[i] += other.data[i];
	return *this;
}
template<typename type, uint32_t size> vec<type, size>& vec<type, size>::operator-=(const vec<type, size>& other) restrict(amp, cpu) {
	for (uint32_t i = 0; i < size; i++)
		data[i] -= other.data[i];
	return *this;
}
template<typename type, uint32_t size> vec<type, size>& vec<type, size>::operator*=(const type value)			  restrict(amp, cpu) {
	for (uint32_t i = 0; i < size; i++)
		data[i] *= value;
	return *this;
}
template<typename type, uint32_t size> vec<type, size>& vec<type, size>::operator/=(const type value)			  restrict(amp, cpu) {
	for (uint32_t i = 0; i < size; i++)
		data[i] /= value;
	return *this;
}


/* Операторы сравнения */
template<typename type, uint32_t size> bool vec<type, size>::operator==(const vec<type, size>& other) const restrict(amp, cpu) {
	for (uint32_t i = 0; i < size; i++)
		if (data[i] != other.data[i])
			return false;
	return true;
}
template<typename type, uint32_t size> bool vec<type, size>::operator> (const vec<type, size>& other) const restrict(amp, cpu) {
	return (this->len() > other.len());
}
template<typename type, uint32_t size> bool vec<type, size>::operator>=(const vec<type, size>& other) const restrict(amp, cpu) {
	return (this->len() >= other.len());
}
template<typename type, uint32_t size> bool vec<type, size>::operator< (const vec<type, size>& other) const restrict(amp, cpu) {
	return (this->len() < other.len());
}
template<typename type, uint32_t size> bool vec<type, size>::operator<=(const vec<type, size>& other) const restrict(amp, cpu) {
	return (this->len() <= other.len());
}


/* Операторы математических операций */
template<typename type, uint32_t size> vec<type, size> vec<type, size>::operator+(const vec<type, size>& other) const restrict(amp, cpu) {
	vec<type, size> result;
	for (uint32_t i = 0; i < size; i++)
		result[i] = data[i] + other.data[i];
	return result;
}
template<typename type, uint32_t size> vec<type, size> vec<type, size>::operator-(const vec<type, size>& other) const restrict(amp, cpu) {
	vec<type, size> result;
	for (uint32_t i = 0; i < size; i++)
		result[i] = data[i] - other.data[i];
	return result;
}

template<typename type, uint32_t size> vec<type, size> vec<type, size>::operator*(const type value)	const restrict(amp, cpu) {
	vec<type, size> result;
	for (uint32_t i = 0; i < size; i++)
		result[i] = data[i] * value;
	return result;
}
template<typename type, uint32_t size> vec<type, size> vec<type, size>::operator/(const type value)	const restrict(amp, cpu) {
	vec<type, size> result;
	for (uint32_t i = 0; i < size; i++)
		result[i] = data[i] / value;
	return result;
}

template<typename type, uint32_t size> type vec<type, size>::operator*(const vec<type, size>& other) const restrict(amp, cpu) {
	type result = type();
	for (uint32_t i = 0; i < size; i++)
		result += data[i] * other.data[i];
	return result;
}


/* Глобальные математические функции */


/* Математические функции */
template<typename type, uint32_t size> type vec<type, size>::dot(const vec<type, size> other) restrict(amp, cpu) {
	type result = type();
	for (uint32_t i = 0; i < size; i++)
		result += data[i] * other.data[i];
	return result;
}
template<typename type, uint32_t size> type vec<type, size>::len() const restrict(cpu) {
	type result = type();
	for (uint32_t i = 0; i < size; i++)
		result += data[i] * data[i];
	return sqrt(result);
}
template<typename type, uint32_t size> type vec<type, size>::len() const restrict(amp) {
	type result = type();
	for (uint32_t i = 0; i < size; i++)
		result += data[i] * data[i];
	return concurrency::precise_math::sqrt(result);
}
template<typename type, uint32_t size> vec<type, size> vec<type, size>::dir() const restrict(amp, cpu) {
	return *this / len();
}


/* Инкреминация */
template<typename type, uint32_t size> type& vec<type, size>::operator[](const uint32_t n)       restrict(amp, cpu) {
	return data[n];
}
template<typename type, uint32_t size> type  vec<type, size>::operator()(const uint32_t n) const restrict(amp, cpu) {
	return data[n];
}




#include "vecN.h"