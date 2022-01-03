#pragma once

/*
1. Определитель матриц
2. Векторное произведение векторов
3. Скалярное произведение векторов
4. Произведение матриц
5. Произведение матрицы на вектор
6. Перевод матрицы в вектор x2
7. Перевод вектора в матрицу x2
8. Перевод вектора в единичный
9. Угол между векторами
*/

#define PI 3.1415926535897932384626433832795

#include "vec.h"
#include "mat.h"

#include <random>

namespace linalg {
	template<typename type>					type det(const mat<type, 2, 2>& matrix)			restrict(amp, cpu) {
		return matrix.get(0, 0) * matrix.get(1, 1) - matrix.get(1, 0) * matrix.get(0, 1);
	}
	template<typename type, uint32_t size>	type det(const mat<type, size, size>& matrix)	restrict(amp, cpu) {
		type determinant = type(0);
		type multiplier = type(+1);

		for (uint32_t i = 0; i < size; i++) {
			mat<type, size - 1, size - 1> temp_mat;

			for (uint32_t x = 0; x < i; x++)
				for (uint32_t y = 1; y < size; y++)
					temp_mat(x, y - 1) = matrix.get(x, y);

			for (uint32_t x = i + 1; x < size; x++)
				for (uint32_t y = 1; y < size; y++)
					temp_mat(x - 1, y - 1) = matrix.get(x, y);

			determinant += multiplier * matrix.get(i, 0) * linalg::det(temp_mat);

			multiplier *= type(-1);
		}

		return determinant;
	}


	template<typename type>  vec<type, 1> cross()																							restrict(amp, cpu) {
		vec<type, 1> result;
		return result;
	}
	template<typename type>  vec<type, 2> cross(const vec<type, 2>& a)																		restrict(amp, cpu) {
		vec<type, 2> result;

		result[0] = +a(1);
		result[1] = -a(0);
	}
	template<typename type>  vec<type, 3> cross(const vec<type, 3>& a, const vec<type, 3>& b)												restrict(amp, cpu) {
		vec<type, 3> result;

		result[0] = a(1) * b(2) - a(2) * b(1);
		result[1] = a(0) * b(2) - a(2) * b(0);
		result[2] = a(0) * b(1) - a(1) * b(0);
	}
	template<typename type>  vec<type, 4> cross(const vec<type, 4>& a, const vec<type, 4>& b, const vec<type, 4>& c)						restrict(amp, cpu) {
		vec<type, 4> result;

		mat<type, 3, 3> m;

		for (uint32_t i = 0; i < 4; i++) {
			for(uint32_t n = 0; n < i; n++) {
				m(n, 0) = a(i);
				m(n, 1) = b(i);
				m(n, 2) = c(i);
			}
			for (uint32_t n = i + 1; n < 4; n++) {
				m(n - 1, 0) = a(i);
				m(n - 1, 1) = b(i);
				m(n - 1, 2) = c(i);
			}
			result[i] = linalg::det(m);
		}

		return result;
	}
	template<typename type>  vec<type, 5> cross(const vec<type, 5>& a, const vec<type, 5>& b, const vec<type, 5>& c, const vec<type, 5>& d)	restrict(amp, cpu) {
		vec<type, 5> result;

		mat<type, 4, 4> m;

		for (uint32_t i = 0; i < 5; i++) {
			for (uint32_t n = 0; n < i; n++) {
				m(n, 0) = a(i);
				m(n, 1) = b(i);
				m(n, 2) = c(i);
				m(n, 3) = d(i);
			}
			for (uint32_t n = i + 1; n < 5; n++) {
				m(n - 1, 0) = a(i);
				m(n - 1, 1) = b(i);
				m(n - 1, 2) = c(i);
				m(n - 1, 3) = d(i);
			}
			result[i] = linalg::det(m);
		}

		return result;
	}


	template<typename type, uint32_t size> type dot(const vec<type, size>& vector1, const vec<type, size>& vector2) restrict(amp, cpu) {
		type result = type(0);
		for (uint32_t i = 0; i < size; i++)
			result += vector1(i) * vector2(i);
		return result;
	}


	template<typename type, uint32_t size_x, uint32_t size_xy, uint32_t size_y> mat<type, size_x, size_y>	product(const mat<type, size_x, size_xy>& matrix1, const mat<type, size_xy, size_y>& matrix2)	restrict(amp, cpu) {
		mat<type, size_x, size_y> result;
		for (uint32_t x = 0; x < size_x; x++)
			for (uint32_t y = 0; y < size_y; y++)
				for (uint32_t n = 0; n < size_xy; n++)
					result(x, y) += matrix1.get(x, n) * matrix2.get(n, y);
		return result;
	}
	template<typename type, uint32_t size> mat<type, size, size>											product(const mat<type, 1, size>& matrix, const vec<type, size>& vector)						restrict(amp, cpu) {
		mat<type, size, size> result;
		for (uint32_t x = 0; x < size; x++)
			for (uint32_t y = 0; y < size; y++)
				result(x, y) = matrix.get(0, y) * vector(x);
		return result;
	}


	template<typename type, uint32_t size> vec<type, size> to_vec(const mat<type, size, 1>& matrix) restrict(amp, cpu) {
		vec<type, size> result;
		for (uint32_t i = 0; i < size; i++)
			result[i] = matrix.get(i, 0);
		return result;
	}
	template<typename type, uint32_t size> vec<type, size> to_vec(const mat<type, 1, size>& matrix) restrict(amp, cpu) {
		vec<type, size> result;
		for (uint32_t i = 0; i < size; i++)
			result[i] = matrix.get(0, i);
		return result;
	}

	template<typename type, uint32_t size> mat<type, 1, size> to_col(const vec<type, size>& vector) restrict(amp, cpu) {
		mat<type, 1, size> result;
		for (uint32_t i = 0; i < size; i++)
			result(0, i) = vector(i);
		return result;
	}
	template<typename type, uint32_t size> mat<type, size, 1> to_row(const vec<type, size>& vector) restrict(amp, cpu) {
		mat<type, size, 1> result;
		for (uint32_t i = 0; i < size; i++)
			result(i, 0) = vector(i);
		return result;
	}


	template<typename type, uint32_t size> vec<type, size> dir(const vec<type, size>& vector) restrict(amp, cpu) {
		return vector / vector.len();
	}


	template<typename type, uint32_t size> type angle(const vec<type, size>& vector1, const vec<type, size>& vector2) restrict(cpu) {
		return acos(linalg::dot(vector1.dir(), vector2.dir()));
	}
	template<typename type, uint32_t size> type angle(const vec<type, size>& vector1, const vec<type, size>& vector2) restrict(amp) {
		return concurrency::precise_math::acos(linalg::dot(vector1.dir(), vector2.dir()));
	}

	template<typename type, uint32_t size> type cosbtw(const vec<type, size>& vector1, const vec<type, size>& vector2) restrict(amp, cpu) {
		return linalg::dot(vector1.dir(), vector2.dir());
	}


	template<typename type> vec<type, 1> random_vec1() restrict(cpu);
	template<typename type> vec<type, 2> random_vec2() restrict(cpu);
	template<typename type> vec<type, 3> random_vec3(const type len_min = 0., const type len_max = 1., const type azi_min = 0., const type azi_max = 2. * PI, const type ver_min = 0., const type ver_max = PI) restrict(cpu) {
		type azi = (type(rand()) / type(RAND_MAX)) * (azi_max - azi_min) + azi_min;
		type ver = (type(rand()) / type(RAND_MAX)) * (ver_max - ver_min) + ver_min;
		type len = (type(rand()) / type(RAND_MAX)) * (len_max - len_min) + len_min;

		return vec<type, 3>({
			len * cos(ver) * cos(azi),
			len * cos(ver) * sin(azi),
			len * sin(ver)
			});
	}
	template<typename type> vec<type, 4> random_vec4() restrict(cpu);

	template<typename type> type random(const type min, const type max) restrict(cpu) {
		return (type(rand()) / type(RAND_MAX)) * (max - min) + min;
	}
};