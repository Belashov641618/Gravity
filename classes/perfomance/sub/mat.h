#pragma once

template<typename type, uint32_t size> class vec;
template<typename type, uint32_t size_x, uint32_t size_y> class mat;


template<typename type, uint32_t size_x, uint32_t size_y> class mat {
protected:
	type data[size_x][size_y];

	/*Функции*/
	

public:

	/*Функции*/
	uint32_t rank_x()																												const restrict(amp, cpu) {
		return size_x;
	}
	uint32_t rank_y()																												const restrict(amp, cpu) {
		return size_y;
	}
	void push_vector(const uint32_t row_num, const vec<type, size_x>& vector)														const restrict(amp, cpu) {
		for (uint32_t i = 0; i < size_x; i++)
			data[i][row_num] = vector(i);
	}
	type det()																														const restrict(amp, cpu) {
		type determinant = 0;

		/*Сейчас этот блок работает только для квдратных матриц а должен работать и для прямоугольных - REWRITE*/
		if (size_x == 2 || size_y == 2) {
			determinant = data[0][0] * data[1][1] - data[1][0] * data[0][1];
			return determinant;
		}

		int c = +1;
		for (uint32_t xi = 0; xi < size_x; xi++) {

			type temp_det;

			if (data[xi][0] == 0) {
				temp_det = 0;
			}
			else {
				mat<type, size_x - 1, size_y - 1> temp;

				for (uint32_t txi = 0; txi < xi; txi++) {
					for (uint32_t tyi = 1; tyi < size_y; tyi++)
						temp.element(txi, tyi - 1) = data[txi][tyi];
				}

				for (uint32_t txi = xi + 1; txi < size_x; txi++) {
					for (uint32_t tyi = 1; tyi < size_y; tyi++)
						temp.element(txi - 1, tyi - 1) = data[txi][tyi];
				}

				temp_det = temp.det();
			}

			//std::cout << temp << "Determinant: " << temp_det << std::endl;

			determinant += temp_det * c * data[xi][0];
			

			c = -c;
		}

		return determinant;
	}

	/*Конструкторы*/
	mat()																															restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for(uint32_t yi = 0; yi < size_y; yi++)
				data[xi][yi] = type();
	}
	mat(const type val)																												restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				data[xi][yi] = val;
	}
	mat(const mat<type, size_x, size_y>& other)																						restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				data[xi][yi] = other.data[xi][yi];
	}

	/*Оператор итерирования*/
	type& element(const uint32_t xi, const uint32_t yi)																				restrict(amp, cpu) {
		return data[xi][yi];
	}
	type& element(const uint32_t id)																								restrict(amp, cpu) {
		return data[id % size_x][id / size_x];
	}
	type get(const uint32_t xi, const uint32_t yi)																					const restrict(amp, cpu) {
		return data[xi][yi];
	}
	type get(const uint32_t id)																										const restrict(amp, cpu) {
		return data[id % size_x][id / size_x];
	}

	/*Операторы равенства*/
	mat<type, size_x, size_y>& operator=(const mat<type, size_x, size_y>& other)													restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				data[xi][yi] = other.data[xi][yi];

		return *this;
	}
	template<uint32_t o_size_x, uint32_t o_size_y> mat<type, size_x, size_y>& operator=(const mat<type, o_size_x, o_size_y>& other)	restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				if (xi >= o_size_x || yi >= o_size_y)
					data[xi][yi] = type();
				else
					data[xi][yi] = other.data[xi][yi];

		return this;
	}

	/*Операторы сложения*/
	mat<type, size_x, size_y> operator+(const mat<type, size_x, size_y>& other)														const restrict(amp, cpu) {
		mat<type, size_x, size_y> res;

		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				res.data[xi][yi] = data[xi][yi] + other.data[xi][yi];

		return res;
	}
	void operator+=(const mat<type, size_x, size_y>& other)																			const restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				data[xi][yi] += other.data[xi][yi];
	}

	/*Операторы вычитания*/
	mat<type, size_x, size_y> operator-(const mat<type, size_x, size_y>& other)														const restrict(amp, cpu) {
		mat<type, size_x, size_y> res;

		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				res.data[xi][yi] = data[xi][yi] - other.data[xi][yi];

		return res;
	}
	void operator-=(const mat<type, size_x, size_y>& other)																			const restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				data[xi][yi] -= other.data[xi][yi];
	}

	/*Операторы умножения на число*/
	mat<type, size_x, size_y> operator*(const type val)																				const restrict(amp, cpu) {
		mat<type, size_x, size_y> res;

		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				res.data[xi][yi] = data[xi][yi] * val;

		return res;
	}
	void operator*=(const type val)																									const restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				data[xi][yi] *= val;
	}
	friend mat<type, size_x, size_y> operator*(const type val, const mat<type, size_x, size_y>& other)								restrict(amp, cpu) {
		mat<type, size_x, size_y> res;

		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				res.elment(xi, yi) = other.get(xi, yi) * val;
			
		return res;
	}
	
	/*Операторы умножения на матрицу*/
	template<uint32_t o_size_y> mat<type, size_x, o_size_y> operator*(const mat<type, size_y, o_size_y>& other)						const restrict(amp, cpu) {
		mat<type, size_x, o_size_y> res;
		
		for (uint32_t x = 0; x < size_x; x++)
			for (uint32_t y = 0; y < o_size_y; y++)
				for (uint32_t w = 0; w < size_y; w++)
					res.element(x, y) = res.element(x, y) + data[x][w] * other.get(w, y);

		return res;
	}


	/*Операторы деления на число*/
	mat<type, size_x, size_y> operator/(const type val)																				const restrict(amp, cpu) {
		mat<type, size_x, size_y> res;

		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				res.data[xi][yi] = data[xi][yi] / val;

		return res;
	}
	void operator/=(const type val)																									const restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				data[xi][yi] /= val;
	}

	/*Операторы сравнения*/
	bool operator==(const mat<type, size_x, size_y>& other)																			const restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for(uint32_t yi = 0; yi < size_y; yi++)
				if (data[xi][yi] != other.data[xi][yi])
					return false;
		return true;
	}
	bool operator!=(const mat<type, size_x, size_y>& other)																			const restrict(amp, cpu) {
		for (uint32_t xi = 0; xi < size_x; xi++)
			for (uint32_t yi = 0; yi < size_y; yi++)
				if (data[xi][yi] == other.data[xi][yi])
					return false;
		return true;
	}
	bool operator< (const mat<type, size_x, size_y>& other)																			const restrict(amp, cpu) {
		return det() <  other.det();
	}
	bool operator<=(const mat<type, size_x, size_y>& other)																			const restrict(amp, cpu) {
		return det() <= other.det();
	}
	bool operator> (const mat<type, size_x, size_y>& other)																			const restrict(amp, cpu) {
		return det() >  other.det();
	}
	bool operator>=(const mat<type, size_x, size_y>& other)																			const restrict(amp, cpu) {
		return det() >= other.det();
	}

	/*Векторное умножение*/


	/*Вывод в консоль*/
	friend std::ostream& operator<<(std::ostream& stream, const mat<type, size_x, size_y>& other) {

		for (uint32_t yi = 0; yi < size_y; yi++) {
			std::cout << "|\t";
			for (uint32_t xi = 0; xi < size_x - 1; xi++)
				std::cout << other.data[xi][yi] << " \t";
			std::cout << other.data[size_x - 1][yi] << "\t|" << std::endl;
		}

		return stream;
	}

};



template<typename type, uint32_t size_x>	class mat<type, size_x, 0> : public mat<type, size_x, 1> {
public:
	
};
template<typename type, uint32_t size_y>	class mat<type, 0, size_y> : public mat<type, 1, size_y> {
public:
	
};
template<typename type>						class mat<type, 0, 0> : public mat<type, 1, 1> {
public:
	
};