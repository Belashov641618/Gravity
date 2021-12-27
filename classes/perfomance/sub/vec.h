#pragma once

template<typename type, uint32_t size> class vec {
protected:

	type data[size];

	/*Функции*/
	type sq_length()																		const restrict(amp, cpu) {
		type res = 0;
		for (uint32_t i = 0; i < size; i++)
			res += data[i] * data[i];

		return res;
	}

public:

	/*Функции*/
	type length()																			const restrict(cpu) {
		type res = 0;

		for (uint32_t i = 0; i < size; i++)
			res += data[i] * data[i];
		
		res = std::sqrt(res);

		return res;
	}
	type length()																			const restrict(amp) {
		type res = 0;

		for (uint32_t i = 0; i < size; i++)
			res += data[i] * data[i];
		
		res = concurrency::precise_math::sqrt(res);

		return res;
	}

	void			normilize()																restrict(amp, cpu) {
		type len = length();
		for (uint32_t i = 0; i < size; i++)
			data[i] /= len;
	}
	vec<type, size>	normilize()																const restrict(amp, cpu) {
		vec<type, size> res;
		type len = length();
		for (uint32_t i = 0; i < size; i++)
			res[i] = data[i] / len;
		return res;
	}

	void			inverse()																restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] = -data[i];
	}
	vec<type, size> inverse_n()																const restrict(amp, cpu) {
		vec<type, size> res;
		for (uint32_t i = 0; i < size; i++)
			res.data[i] = -data[i];
		return res;
	}

	template<typename fun_type> void			each(fun_type function)						restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] = function(data[i]);
	}
	template<typename fun_type> vec<type, size> each(fun_type function)						const restrict(amp, cpu) {
		vec<type, size> res;
		for (uint32_t i = 0; i < size; i++)
			res = function(data[i]);
		return res;
	}



	/*Конструкторы*/
	vec()																					restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] = type();
	}
	vec(const type val)																		restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] = val;
	}
	vec(const vec<type, size>& other)														restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] = other.data[i];
	}
	
	/*Оператор итерирования*/
	type& operator[](const uint32_t num)													restrict(amp, cpu) {
		return data[num];
	}
	type operator()(const uint32_t num)														const restrict(amp, cpu) {
		return data[num];
	}

	/*Операторы равенства*/
	vec<type, size>& operator=(const vec<type, size>& other)								restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] = other.data[i];

		return *this;
	}
	template<uint32_t o_size> vec<type, size>& operator=(const vec<type, o_size>& other)	restrict(amp, cpu) {
		if (size >= o_size) {
			for (uint32_t i = 0; i < o_size; i++)
				data[i] = other.data[i];
			for (uint32_t i = o_size; i < size; i++)
				data[i] = type();
		}
		else {
			for (uint32_t i = 0; i < size; i++)
				data[i] = other.data[i];
		}

		return this;
	}

	/*Операторы сложения*/
	vec<type, size> operator+(const vec<type, size>& other)									const restrict(amp, cpu) {
		vec<type, size> res;
		for (uint32_t i = 0; i < size; i++)
			res.data[i] = data[i] + other.data[i];

		return res;
	}
	void operator+=(const vec<type, size>& other)											const restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] += other.data[i];
	}

	/*Операторы вычитания*/
	vec<type, size> operator-(const vec<type, size>& other)									const restrict(amp, cpu) {
		vec<type, size> res;
		for (uint32_t i = 0; i < size; i++)
			res.data[i] = data[i] - other.data[i];

		return res;
	}
	void operator-=(const vec<type, size>& other)											const restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] -= other.data[i];
	}

	/*Операторы умножения на число*/
	vec<type, size> operator*(const type val)												const restrict(amp, cpu) {
		vec<type, size> res;
		for (uint32_t i = 0; i < size; i++)
			res.data[i] = val * data[i];

		return res;
	}
	void operator*=(const type val)															const restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] *= val;
	}
	friend vec<type, size> operator*(const type val, const vec<type, size>& other)			restrict(amp, cpu) {
		vec<type, size> res;
		for (uint32_t i = 0; i < size; i++)
			res[i] = other(i) * val;
		return res;
	}
	
	/*Операторы умножения на вектор*/
	type operator*(const vec<type, size> other)												const restrict(amp, cpu) {
		type sum = 0;
		for (uint32_t i = 0; i < size; i++)
			sum += data[i] * other.data[i];
		return sum;
	}

	/*Операторы деления на число*/
	vec<type, size> operator/(const type val)												const restrict(amp, cpu) {
		vec<type, size> res;
		for (uint32_t i = 0; i < size; i++)
			res.data[i] = data[i] / val;

		return res;
	}
	void operator/=(const type val)															const restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			data[i] /= val;
	}

	/*Операторы сравнения*/
	bool operator==(const vec<type, size>& other)											const restrict(amp, cpu) {
		for(uint32_t i = 0; i < size; i++)
			if (data[i] != other.data[i])
				return false;
		return true;
	}
	bool operator!=(const vec<type, size>& other)											const restrict(amp, cpu) {
		for (uint32_t i = 0; i < size; i++)
			if (data[i] == other.data[i])
				return false;
		return true;
	}
	bool operator< (const vec<type, size>& other)											const restrict(amp, cpu) {
		return sq_length() <  other.sq_length();
	}
	bool operator<=(const vec<type, size>& other)											const restrict(amp, cpu) {
		return sq_length() <= other.sq_length();
	}
	bool operator> (const vec<type, size>& other)											const restrict(amp, cpu) {
		return sq_length() >  other.sq_length();
	}
	bool operator>=(const vec<type, size>& other)											const restrict(amp, cpu) {
		return sq_length() >= other.sq_length();
	}

	/*Векторное умножение*/


	friend std::ostream& operator<<(std::ostream& stream, const vec<type, size>& other) {

		std::cout << "(";
		for (uint32_t i = 0; i < size - 1; i++)
			std::cout << other.data[i] << ", ";
		std::cout << other.data[size - 1] << ")";

		return stream;
	}

};




template<typename type = float> class vec1 : public vec<type, 1> {
public:

	type& x() restrict(amp, cpu) { return this->data[0]; }

	type& a() restrict(amp, cpu) { return this->data[0]; }


	void set(const type x) restrict(amp, cpu) {
		this->data[0] = x;
	}

	vec1(const type x) restrict(amp, cpu) {
		this->data[0] = x;
	}

	vec1()							restrict(amp, cpu) : vec<type, 1>()			{}
	//vec1(const type val)			restrict(amp, cpu) : vec<type, 1>(val)		{}
	vec1(const vec<type, 1>& other) restrict(amp, cpu) : vec<type, 1>(other)	{}
	vec1(const vec1<type>& other)	restrict(amp, cpu) : vec<type, 1>(other)	{}


	/*Векторное умножение*/
	static vec<type, 1> cross() restrict(amp, cpu) {
		return vec1<type>();
	}
};


template<typename type = float> class vec2 : public vec<type, 2> {
public:

	type& x() restrict(amp, cpu) { return this->data[0]; }
	type& y() restrict(amp, cpu) { return this->data[1]; }

	type& a() restrict(amp, cpu) { return this->data[0]; }
	type& b() restrict(amp, cpu) { return this->data[1]; }


	void set(const type x, const type y) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
	}

	vec2(const type x, const type y) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
	}

	vec2()							restrict(amp, cpu) : vec<type, 2>()			{}
	vec2(const type val)			restrict(amp, cpu) : vec<type, 2>(val)		{}
	vec2(const vec<type, 2>& other) restrict(amp, cpu) : vec<type, 2>(other)	{}
	vec2(const vec2<type>& other)	restrict(amp, cpu) : vec<type, 2>(other)	{}


	/*Векторное умножение*/
	static vec<type, 2> cross(vec2<type>& a) restrict(amp, cpu) {
		vec2<type> res;
		res.x() = +a.y();
		res.y() = -a.x();
		return res;
	}
};


template<typename type = float> class vec3 : public vec<type, 3> {
public:

	type& x() restrict(amp, cpu) { return this->data[0]; }
	type& y() restrict(amp, cpu) { return this->data[1]; }
	type& z() restrict(amp, cpu) { return this->data[2]; }

	type& a() restrict(amp, cpu) { return this->data[0]; }
	type& b() restrict(amp, cpu) { return this->data[1]; }
	type& c() restrict(amp, cpu) { return this->data[2]; }


	void set(const type x, const type y, const type z) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
	}

	vec3(const type x, const type y, const type z) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
	}


	vec3()							restrict(amp, cpu) : vec<type, 3>() {}
	vec3(const type val)			restrict(amp, cpu) : vec<type, 3>(val) {}
	vec3(const vec<type, 3>& other) restrict(amp, cpu) : vec<type, 3>(other) {}
	vec3(const vec3<type>& other)	restrict(amp, cpu) : vec<type, 3>(other) {}


	/*Векторное умножение*/
	static vec<type, 3> cross(vec3<type>& a, vec3<type>& b) restrict(amp, cpu) {
		vec3<type> res;
		res.x() = a.y() * b.z() - b.y() * a.z();
		res.y() = a.x() * b.z() - b.x() * a.z();
		res.z() = a.x() * b.y() - b.x() * a.y();
		return res;
	}
};


template<typename type = float> class vec4 : public vec<type, 4> {
public:

	type& x() restrict(amp, cpu) { return this->data[0]; }
	type& y() restrict(amp, cpu) { return this->data[1]; }
	type& z() restrict(amp, cpu) { return this->data[2]; }
	type& w() restrict(amp, cpu) { return this->data[3]; }

	type& a() restrict(amp, cpu) { return this->data[0]; }
	type& b() restrict(amp, cpu) { return this->data[1]; }
	type& c() restrict(amp, cpu) { return this->data[2]; }
	type& d() restrict(amp, cpu) { return this->data[3]; }


	void set(const type x, const type y, const type z, const type w) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
		this->data[3] = w;
	}

	vec4(const type x, const type y, const type z, const type w) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
		this->data[3] = w;
	}

	vec4()							restrict(amp, cpu) : vec<type, 4>() {}
	vec4(const type val)			restrict(amp, cpu) : vec<type, 4>(val) {}
	vec4(const vec<type, 4>& other) restrict(amp, cpu) : vec<type, 4>(other) {}
	vec4(const vec4<type>& other)	restrict(amp, cpu) : vec<type, 4>(other) {}


	/*Векторное умножение*/
	static vec<type, 4> cross(vec4<type>& a, vec4<type>& b, vec4<type>& c) restrict(amp, cpu) {
		vec4<type> res;
		
		perfomance::mat<type, 3, 3> mx;
		perfomance::mat<type, 3, 3> my;
		perfomance::mat<type, 3, 3> mz;
		perfomance::mat<type, 3, 3> mw;

		for (uint32_t i = 1; i < 4; i++) {
			mx.element(0, i - 1) = a[i];
			mx.element(1, i - 1) = b[i];
			mx.element(2, i - 1) = c[i];
		}

		for (uint32_t i = 0; i < 1; i++) {
			my.element(0, i) = a[i];
			my.element(1, i) = b[i];
			my.element(2, i) = c[i];
		}
		for (uint32_t i = 2; i < 4; i++) {
			my.element(0, i - 1) = a[i];
			my.element(1, i - 1) = b[i];
			my.element(2, i - 1) = c[i];
		}

		for (uint32_t i = 0; i < 2; i++) {
			mz.element(0, i) = a[i];
			mz.element(1, i) = b[i];
			mz.element(2, i) = c[i];
		}
		for (uint32_t i = 3; i < 4; i++) {
			mz.element(0, i - 1) = a[i];
			mz.element(1, i - 1) = b[i];
			mz.element(2, i - 1) = c[i];
		}
			
		for (uint32_t i = 0; i < 3; i++) {
			mw.element(0, i) = a[i];
			mw.element(1, i) = b[i];
			mw.element(2, i) = c[i];
		}

		res.x() = mx.det();
		res.y() = my.det();
		res.z() = mz.det();
		res.w() = mw.det();

		return res;
	}
};


