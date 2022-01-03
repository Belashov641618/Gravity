#pragma once

#include <iostream>
#include <iomanip>
#include <io.h>
#include <fcntl.h>

#define mat_left_up_angle		L"┌"
#define mat_left_down_angle		L"└"
#define mat_right_up_angle		L"┐"
#define mat_right_down_angle	L"┘"
#define mat_walls				L"│"

#define mat_open_scope			"{"
#define mat_close_scope			"}"
#define mat_separator			", "

#include <cmath>
#include <amp_math.h>

#include "vec.h"

template<typename type, uint32_t size_x, uint32_t size_y> class mat {
protected:

	type data[size_x * size_y];

public:
	
	mat(std::initializer_list<std::initializer_list<type>> list)	restrict(cpu);
	mat(std::initializer_list<vec<type, size_x>> list)				restrict(cpu);

	/* Конструкторы */
	mat()										restrict(amp, cpu);
	mat(const type def_val)						restrict(amp, cpu);
	mat(const mat<type, size_x, size_y>& other) restrict(amp, cpu);
	
	/* Операторы равенства */
	mat<type, size_x, size_y>& operator= (const mat<type, size_x, size_y>& other)	restrict(amp, cpu);

	mat<type, size_x, size_y>& operator+=(const mat<type, size_x, size_y>& other)	restrict(amp, cpu);
	mat<type, size_x, size_y>& operator-=(const mat<type, size_x, size_y>& other)	restrict(amp, cpu);

	mat<type, size_x, size_y>& operator*=(const type value)							restrict(amp, cpu);
	mat<type, size_x, size_y>& operator/=(const type value)							restrict(amp, cpu);

	/* Операторы сравнения */
	bool operator==(const mat<type, size_x, size_y>& other) const restrict(amp, cpu);
	bool operator!=(const mat<type, size_x, size_y>& other) const restrict(amp, cpu);
	
	/* Операторы математических операций */
	mat<type, size_x, size_y> operator+(const mat<type, size_x, size_y>& other)	const restrict(amp, cpu);
	mat<type, size_x, size_y> operator-(const mat<type, size_x, size_y>& other)	const restrict(amp, cpu);

	mat<type, size_x, size_y> operator*(const type value) const restrict(amp, cpu);
	mat<type, size_x, size_y> operator/(const type value) const restrict(amp, cpu);
	friend mat<type, size_x, size_y> operator*(const type value, const mat<type, size_x, size_y>& mat) restrict(amp, cpu) {
		return mat * value;
	}
	
	/* Инкреминация */
	type& operator()(const uint32_t x, const uint32_t y) restrict(amp, cpu);
	type& operator()(const uint32_t n) restrict(amp, cpu);
	type get(const uint32_t x, const uint32_t y) const restrict(amp, cpu);
	type get(const uint32_t n) const restrict(amp, cpu);
	
	/* Вывод */
	friend std::ostream& operator<<(std::ostream& stream, const mat<type, size_x, size_y>& mat) {
		stream << mat_open_scope;
		for(uint32_t y = 0; y < size_y - 1; y++) {
			stream << mat_open_scope;
			for (uint32_t x = 0; x < size_x - 1; x++)
				stream << mat.data[x * size_y + y] << mat_separator;
			stream << mat.data[(size_x - 1) * size_y + y] << mat_close_scope << mat_separator;
		}
		stream << mat_open_scope;
		for (uint32_t x = 0; x < size_x - 1; x++)
			stream << mat.data[x * size_y + size_y - 1] << mat_separator;
		stream << mat.data[(size_x - 1) * size_y + size_y - 1] << mat_close_scope << mat_close_scope;
		return stream;
	}
	void print() const;

};


template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>::mat(std::initializer_list<std::initializer_list<type>> list)	restrict(cpu) {
	for (uint32_t y = 0; y < list.size() && y < size_y; y++) {
		for (uint32_t x = 0; x < (list.begin()[y]).size() && x < size_x; x++)
			data[x * size_y + y] = (list.begin()[y]).begin()[x];
		for (uint32_t x = (list.begin()[y]).size(); x < size_x; x++)
			data[x * size_y + y] = type();
	}
	for (uint32_t y = list.size(); y < size_y; y++)
		for (uint32_t x = 0; x < size_x; x++)
			data[x * size_y + y] = type();
}
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>::mat(std::initializer_list<vec<type, size_x>> list)				restrict(cpu) {
	for (uint32_t y = 0; y < list.size() && y < size_y; y++)
		for (uint32_t x = 0; x < size_x; x++)
			data[x * size_y + y] = (list.begin()[y])(x);
	for (uint32_t y = list.size(); y < size_y; y++)
		for (uint32_t x = 0; x < size_x; x++)
			data[x * size_y + y] = type();
}

/* Конструкторы */
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>::mat()											restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			data[x * size_y + y] = type();
}
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>::mat(const type def_val)						restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			data[x * size_y + y] = def_val;
}
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>::mat(const mat<type, size_x, size_y>& other)	restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			data[x * size_y + y] = other.data[x * size_y + y];
}

/* Операторы равенства */
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>& mat<type, size_x, size_y>::operator= (const mat<type, size_x, size_y>& other)		restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			data[x * size_y + y] = other.data[x * size_y + y];
	return *this;
}

template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>& mat<type, size_x, size_y>::operator+=(const mat<type, size_x, size_y>& other)		restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			data[x * size_y + y] += other.data[x * size_y + y];
	return *this;
}
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>& mat<type, size_x, size_y>::operator-=(const mat<type, size_x, size_y>& other)		restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			data[x * size_y + y] -= other.data[x * size_y + y];
	return *this;
}

template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>& mat<type, size_x, size_y>::operator*=(const type value)							restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			data[x * size_y + y] *= value;
	return *this;
}
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y>& mat<type, size_x, size_y>::operator/=(const type value)							restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			data[x * size_y + y] /= value;
	return *this;
}

/* Операторы сравнения */
template<typename type, uint32_t size_x, uint32_t size_y> bool mat<type, size_x, size_y>::operator==(const mat<type, size_x, size_y>& other) const restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			if (data[x * size_y + y] != other.data[x * size_x + y])
				return false;
	return true;
}
template<typename type, uint32_t size_x, uint32_t size_y> bool mat<type, size_x, size_y>::operator!=(const mat<type, size_x, size_y>& other) const restrict(amp, cpu) {
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			if (data[x * size_y + y] != other.data[x * size_x + y])
				return true;
	return false;
}

/* Операторы математических операций */
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y> mat<type, size_x, size_y>::operator+(const mat<type, size_x, size_y>& other)	const restrict(amp, cpu) {
	mat<type, size_x, size_y> result;
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			result.data[x * size_y + y] = data[x * size_y + y] + other.data[x * size_y + y];
	return result;
}
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y> mat<type, size_x, size_y>::operator-(const mat<type, size_x, size_y>& other)	const restrict(amp, cpu) {
	mat<type, size_x, size_y> result;
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			result.data[x * size_y + y] = data[x * size_y + y] - other.data[x * size_y + y];
	return result;
}

template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y> mat<type, size_x, size_y>::operator*(const type value)							const restrict(amp, cpu) {
	mat<type, size_x, size_y> result;
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			result.data[x * size_y + y] = data[x * size_y + y] * value;
	return result;
}
template<typename type, uint32_t size_x, uint32_t size_y> mat<type, size_x, size_y> mat<type, size_x, size_y>::operator/(const type value)							const restrict(amp, cpu) {
	mat<type, size_x, size_y> result;
	for (uint32_t x = 0; x < size_x; x++)
		for (uint32_t y = 0; y < size_y; y++)
			result.data[x * size_y + y] = data[x * size_y + y] / value;
	return result;
}

/* Инкреминация */
template<typename type, uint32_t size_x, uint32_t size_y> type& mat<type, size_x, size_y>::operator()(const uint32_t x, const uint32_t y) restrict(amp, cpu) {
	return data[x * size_y + y];
}
template<typename type, uint32_t size_x, uint32_t size_y> type& mat<type, size_x, size_y>::operator()(const uint32_t n) restrict(amp, cpu) {
	return data[n];
}
template<typename type, uint32_t size_x, uint32_t size_y> type mat<type, size_x, size_y>::get(const uint32_t x, const uint32_t y) const restrict(amp, cpu) {
	return data[x * size_y + y];
}
template<typename type, uint32_t size_x, uint32_t size_y> type mat<type, size_x, size_y>::get(const uint32_t n) const restrict(amp, cpu) {
	return data[n];
}

/* Вывод */
template<typename type, uint32_t size_x, uint32_t size_y> void mat<type, size_x, size_y>::print() const {
	auto mode = _setmode(_fileno(stdout), _O_U16TEXT);

	std::wcout << mat_left_up_angle;
	for (uint32_t x = 0; x < size_x - 1; x++) {
		std::wcout << std::setw(9) << std::setprecision(2) << std::scientific << get(x, 0);
		std::wcout << mat_separator;
	}
	std::wcout << std::setw(9) << std::setprecision(2) << std::scientific << get(size_x - 1, 0);
	std::wcout << mat_right_up_angle << std::endl;

	for (uint32_t y = 1; y < size_y - 1; y++) {
		std::wcout << mat_walls;
		for (uint32_t x = 0; x < size_x - 1; x++) {
			std::wcout << std::setw(9) << std::setprecision(2) << std::scientific << get(x, y); 
			std::wcout << mat_separator;
		}
		std::wcout << std::setw(9) << std::setprecision(2) << std::scientific << get(size_x - 1, y);
		std::wcout << mat_walls << std::endl;
	}

	std::wcout << mat_left_down_angle;
	for (uint32_t x = 0; x < size_x - 1; x++) {
		std::wcout << std::setw(9) << std::setprecision(2) << std::scientific << get(x, size_y - 1);
		std::wcout << mat_separator;
	}
	std::wcout << std::setw(9) << std::setprecision(2) << std::scientific << get(size_x - 1, size_y - 1);
	std::wcout << mat_right_down_angle << std::endl;

	_setmode(_fileno(stdout), mode);
}