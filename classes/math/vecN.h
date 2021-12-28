#pragma once

template<typename type> class vec1 : public vec<type, 1> {
public:

	vec1()							restrict(amp, cpu) : vec<type, 1>()			{}
	vec1(const type val)			restrict(amp, cpu) : vec<type, 1>(val)		{}
	vec1(const vec<type, 1>& other) restrict(amp, cpu) : vec<type, 1>(other)	{}

	/*vec1(const type x) restrict(amp, cpu) {
		this->data[0] = x;
	}*/

	void set(const type x) restrict(amp, cpu) {
		this->data[0] = x;
	}

	/*Векторное умножение*/
	static vec1<type> cross() restrict(amp, cpu) {
		return vec1<type>();
	}

};



template<typename type> class vec2 : public vec<type, 2> {
public:

	vec2()							restrict(amp, cpu) : vec<type, 2>() {}
	vec2(const type val)			restrict(amp, cpu) : vec<type, 2>(val) {}
	vec2(const vec<type, 2>& other) restrict(amp, cpu) : vec<type, 2>(other) {}

	vec2(const type x, const type y) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
	}

	void set(const type x, const type y) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
	}

	/*Векторное умножение*/
	static vec2<type> cross(const vec2<type>& a) restrict(amp, cpu) {
		vec2<type> res;
		res[0] = +a[1];
		res[1] = -a[0];
		return res;
	}

};



template<typename type> class vec3 : public vec<type, 3> {
public:

	vec3()							restrict(amp, cpu) : vec<type, 3>() {}
	vec3(const type val)			restrict(amp, cpu) : vec<type, 3>(val) {}
	vec3(const vec<type, 3>& other) restrict(amp, cpu) : vec<type, 3>(other) {}

	vec3(const type x, const type y, const type z) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
	}

	void set(const type x, const type y, const type z) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
	}

	/*Векторное умножение*/
	static vec3<type> cross(const vec3<type>& a, const vec3<type>& b) restrict(amp, cpu) {
		vec3<type> res;
		res[0] = a[1] * b[2] - b[1] * a[2];
		res[1] = a[0] * b[2] - b[0] * a[2];
		res[2] = a[0] * b[1] - b[0] * a[1];
		return res;
	}

};



template<typename type> class vec4 : public vec<type, 4> {
public:

	vec4()							restrict(amp, cpu) : vec<type, 4>() {}
	vec4(const type val)			restrict(amp, cpu) : vec<type, 4>(val) {}
	vec4(const vec<type, 4>& other) restrict(amp, cpu) : vec<type, 4>(other) {}

	vec4(const type x, const type y, const type z, const type w) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
		this->data[3] = w;
	}

	void set(const type x, const type y, const type z, const type w) restrict(amp, cpu) {
		this->data[0] = x;
		this->data[1] = y;
		this->data[2] = z;
		this->data[3] = w;
	}

	/*Векторное умножение*/
	static vec4<type> cross(const vec4<type>& a, const vec4<type>& b, const vec4<type>& c) restrict(amp, cpu) {
		vec4<type> res;
		
		return res;
	}

};