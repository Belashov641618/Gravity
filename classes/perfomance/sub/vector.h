#pragma once


#define PERFOMANCE_VECTOR_DEBUG	true



template<typename type> class vector : public std::vector<type> {
private:



public:

	vector() : std::vector<type>() {

	}
	vector(size_t N, type value = type(0)) : std::vector<type>(N, value) {

	}
	vector(const std::initializer_list<type>& other) : std::vector<type>(other) {

	}
	vector(const std::vector<type>& other) : std::vector<type>(other) {

	}


	vector<type>& operator=(type value) {
		*this = vector(this->size(), value);
		return *this;
	}


	friend std::ostream& operator << (std::ostream& stream, const vector<type>& other) {
		if (!other.size())
			return stream;

		stream << "{";
		for (uint16_t i = 0; i < other.size() - 1; i++)
			stream << other[i] << " ";
		stream << other[other.size() - 1] << "}";
		return stream;
	}


	void fill_random(type from = -1e+10, type to = +1e+10, uint64_t seed = perfomance::microseconds_f()) {
		srand(seed);
		for (size_t i = 0; i < (*this).size(); i++)
			(*this)[i] = perfomance::random(from, to);
	}



	/*template<typename fun_type, typename vec_type, typename... args_types> static vector<vec_type> process_for_each_CPU(const fun_type& functor, vector<vec_type>& a, args_types... args) {
		size_t size = a.size();
		vector<vec_type> res(size);

		for (size_t i = 0; i < size; i++) {
			res[i] = functor(a[i], args...);
		}

		return res;
	}
	template<typename fun_type, typename vec_type, typename... args_types> static vector<vec_type> process_for_each_MTH(const fun_type& functor, vector<vec_type>& a, args_types... args) {
		size_t size = a.size();
		vector<vec_type> res(size);

		uint32_t cores_amount = std::thread::hardware_concurrency();

		std::vector<std::thread> thread_pull(cores_amount);

		size_t points_for_one_thread = size / (cores_amount - 1);
		size_t points_for_lst_thread = size % (cores_amount - 1);

		auto lambda = [&](size_t begin, size_t end) {
			for (size_t i = begin; i < end; i++)
				res[i] = functor(a[i], args...);
		};


		for (uint32_t i = 0; i < cores_amount - 1; i++)
			thread_pull[i] = std::thread(lambda, i * points_for_one_thread, i * points_for_one_thread + points_for_one_thread);

		thread_pull[cores_amount - 1] = std::thread(lambda, (cores_amount - 1) * points_for_one_thread, size);


		for (size_t i = 0; i < cores_amount; i++)
			thread_pull[i].join();


		return res;
	}
	template<typename fun_type, typename vec_type, typename... args_types> static vector<vec_type> process_for_each_GPU(const fun_type& functor, vector<vec_type>& a, args_types... args) {
		size_t size = a.size();
		vector<vec_type> res(size);

		concurrency::array_view<vec_type> vec_a(a.size(), a);
		concurrency::array_view<vec_type> vec_c(res.size(), res);

		vec_c.discard_data();

		concurrency::extent<1> ex(size);

		concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp) {
			vec_c[i] = functor(vec_a[i], args...);
			});

		vec_c.synchronize();

		return res;
	}
	template<typename fun_type, typename vec_type, typename... args_types> static vector<vec_type> process_for_each(const fun_type& functor, vector<vec_type>& a, args_types... args) {
		size_t size = a.size();
		vector<vec_type> res(size);

		if (functor.switch_cpu[0] <= size && size <= functor.switch_cpu[1]) {
			for (size_t i = 0; i < size; i++) {
				res[i] = functor(a[i], args...);
			}
		}
		else if (functor.switch_gpu[0] <= size && size <= functor.switch_gpu[1]) {
			concurrency::array_view<vec_type> vec_a(a.size(), a);
			concurrency::array_view<vec_type> vec_c(res.size(), res);

			vec_c.discard_data();

			concurrency::extent<1> ex(size);

			concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp) {
				vec_c[i] = functor(vec_a[i], args...);
				});

			vec_c.synchronize();
		}
		else if (functor.switch_mth[0] <= size && size <= functor.switch_mth[1]) {
			uint32_t cores_amount = std::thread::hardware_concurrency();

			std::vector<std::thread> thread_pull(cores_amount);

			size_t points_for_one_thread = size / (cores_amount - 1);
			size_t points_for_lst_thread = size % (cores_amount - 1);

			auto lambda = [&](size_t begin, size_t end) {
				for (size_t i = begin; i < end; i++)
					res[i] = functor(a[i], args...);
			};


			for (uint32_t i = 0; i < cores_amount - 1; i++)
				thread_pull[i] = std::thread(lambda, i * points_for_one_thread, i * points_for_one_thread + points_for_one_thread);

			thread_pull[cores_amount - 1] = std::thread(lambda, (cores_amount - 1) * points_for_one_thread, size);


			for (size_t i = 0; i < cores_amount; i++)
				thread_pull[i].join();
		}

		return res;
	}*/

};



namespace vector_calcs {
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each_CPU(const fun_type& functor, vector<vec_type>& a, args_types... args) {

		size_t size = a.size();
		vector<vec_type> res(size);

		for (size_t i = 0; i < size; i++) {
			res[i] = functor(a[i], args...);
		}

		return res;
	}
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each_MTH(const fun_type& functor, vector<vec_type>& a, args_types... args) {

		size_t size = a.size();
		vector<vec_type> res(size);

		uint32_t cores_amount = std::thread::hardware_concurrency();

		std::vector<std::thread> thread_pull(cores_amount);

		size_t points_for_one_thread = size / (cores_amount - 1);
		size_t points_for_lst_thread = size % (cores_amount - 1);

		auto lambda = [&](size_t begin, size_t end) {
			for (size_t i = begin; i < end; i++)
				res[i] = functor(a[i], args...);
		};


		for (uint32_t i = 0; i < cores_amount - 1; i++)
			thread_pull[i] = std::thread(lambda, i * points_for_one_thread, i * points_for_one_thread + points_for_one_thread);

		thread_pull[cores_amount - 1] = std::thread(lambda, (cores_amount - 1) * points_for_one_thread, size);


		for (size_t i = 0; i < cores_amount; i++)
			thread_pull[i].join();


		return res;
	}
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each_GPU(const fun_type& functor, vector<vec_type>& a, args_types... args) {
		
		size_t size = a.size();
		vector<vec_type> res(size);

		concurrency::array_view<vec_type> vec_a(a.size(), a);
		concurrency::array_view<vec_type> vec_c(res.size(), res);

		vec_c.discard_data();

		concurrency::extent<1> ex(size);

		concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp) {
			vec_c[i] = functor(vec_a[i], args...);
			});

		vec_c.synchronize();

		return res;
	}
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each	(const fun_type& functor, vector<vec_type>& a, args_types... args) {
		size_t size = a.size();
		vector<vec_type> res(size);

		if (functor.switch_cpu[0] <= size && size <= functor.switch_cpu[1]) {
			for (size_t i = 0; i < size; i++) {
				res[i] = functor(a[i], args...);
			}
		}
		else if (functor.switch_gpu[0] <= size && size <= functor.switch_gpu[1]) {
			concurrency::array_view<vec_type> vec_a(a.size(), a);
			concurrency::array_view<vec_type> vec_c(res.size(), res);

			vec_c.discard_data();

			concurrency::extent<1> ex(size);

			concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp) {
				vec_c[i] = functor(vec_a[i], args...);
				});

			vec_c.synchronize();
		}
		else if (functor.switch_mth[0] <= size && size <= functor.switch_mth[1]) {
			uint32_t cores_amount = std::thread::hardware_concurrency();

			std::vector<std::thread> thread_pull(cores_amount);

			size_t points_for_one_thread = size / (cores_amount - 1);
			size_t points_for_lst_thread = size % (cores_amount - 1);

			auto lambda = [&](size_t begin, size_t end) {
				for (size_t i = begin; i < end; i++)
					res[i] = functor(a[i], args...);
			};


			for (uint32_t i = 0; i < cores_amount - 1; i++)
				thread_pull[i] = std::thread(lambda, i * points_for_one_thread, i * points_for_one_thread + points_for_one_thread);

			thread_pull[cores_amount - 1] = std::thread(lambda, (cores_amount - 1) * points_for_one_thread, size);


			for (size_t i = 0; i < cores_amount; i++)
				thread_pull[i].join();
		}

		return res;
	}
	

	
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each_CPU(const fun_type& functor, vector<vec_type>& a, vector<vec_type>& b, args_types... args) {

		size_t size = min(a.size(), b.size());
		vector<vec_type> res(size);

		for (size_t i = 0; i < size; i++) {
			res[i] = functor(a[i], b[i], args...);
		}

		return res;
	}
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each_MTH(const fun_type& functor, vector<vec_type>& a, vector<vec_type>& b, args_types... args) {
		
		size_t size = min(a.size(), b.size());
		vector<vec_type> res(size);

		uint32_t cores_amount = std::thread::hardware_concurrency();

		std::vector<std::thread> thread_pull(cores_amount);

		size_t points_for_one_thread = size / (cores_amount - 1);
		size_t points_for_lst_thread = size % (cores_amount - 1);

		auto lambda = [&](size_t begin, size_t end) {
			for(size_t i = begin; i < end; i++)
				res[i] = functor(a[i], b[i], args...);
		};


		for (uint32_t i = 0; i < cores_amount - 1; i++)
			thread_pull[i] = std::thread(lambda, i * points_for_one_thread, i * points_for_one_thread + points_for_one_thread);

		thread_pull[cores_amount - 1] = std::thread(lambda, (cores_amount - 1) * points_for_one_thread, size);


		for (size_t i = 0; i < cores_amount; i++)
				thread_pull[i].join();


		return res;
	}
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each_GPU(const fun_type& functor, vector<vec_type>& a, vector<vec_type>& b, args_types... args) {

		size_t size = min(a.size(), b.size());
		vector<vec_type> res(size);

		concurrency::array_view<vec_type> vec_a(a.size(), a);
		concurrency::array_view<vec_type> vec_b(b.size(), b);
		concurrency::array_view<vec_type> vec_c(res.size(), res);

		vec_c.discard_data();

		concurrency::extent<1> ex(size);

		concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp) {
			vec_c[i] = functor(vec_a[i], vec_b[i], args...);
			});

		vec_c.synchronize();

		return res;
	}
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each	(const fun_type& functor, vector<vec_type>& a, vector<vec_type>& b, args_types... args) {

		size_t size = min(a.size(), b.size());
		vector<vec_type> res(size);

		if (functor.switch_cpu[0] <= size && size <= functor.switch_cpu[1]) {
			for (size_t i = 0; i < size; i++) {
				res[i] = functor(a[i], b[i], args...);
			}
		}
		else if (functor.switch_gpu[0] <= size && size <= functor.switch_gpu[1]) {
			concurrency::array_view<vec_type> vec_a(a.size(), a);
			concurrency::array_view<vec_type> vec_b(b.size(), b);
			concurrency::array_view<vec_type> vec_c(res.size(), res);

			vec_c.discard_data();

			concurrency::extent<1> ex(size);

			concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp) {
				vec_c[i] = functor(vec_a[i], vec_b[i], args...);
				});

			vec_c.synchronize();
		}
		else if (functor.switch_mth[0] <= size && size <= functor.switch_mth[1]) {
			uint32_t cores_amount = std::thread::hardware_concurrency();

			std::vector<std::thread> thread_pull(cores_amount);

			size_t points_for_one_thread = size / (cores_amount - 1);
			size_t points_for_lst_thread = size % (cores_amount - 1);

			auto lambda = [&](size_t begin, size_t end) {
				for (size_t i = begin; i < end; i++)
					res[i] = functor(a[i], b[i], args...);
			};


			for (uint32_t i = 0; i < cores_amount - 1; i++)
				thread_pull[i] = std::thread(lambda, i * points_for_one_thread, i * points_for_one_thread + points_for_one_thread);

			thread_pull[cores_amount - 1] = std::thread(lambda, (cores_amount - 1) * points_for_one_thread, size);


			for (size_t i = 0; i < cores_amount; i++)
				thread_pull[i].join();
		}

		return res;
	}

	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each_CPU(const fun_type& functor, vector<vec_type>& a, vector<vec_type>& b, vector<vec_type>& c, args_types... args) {

		size_t size = min(min(a.size(), b.size()), c.size());
		vector<vec_type> res(size);

		for (size_t i = 0; i < size; i++) {
			res[i] = functor(a[i], b[i], c[i], args...);
		}

		return res;
	}
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each_MTH(const fun_type& functor, vector<vec_type>& a, vector<vec_type>& b, vector<vec_type>& c, args_types... args) {

		size_t size = min(min(a.size(), b.size()), c.size());
		vector<vec_type> res(size);

		uint32_t cores_amount = std::thread::hardware_concurrency();

		std::vector<std::thread> thread_pull(cores_amount);

		size_t points_for_one_thread = size / (cores_amount - 1);
		size_t points_for_lst_thread = size % (cores_amount - 1);

		auto lambda = [&](size_t begin, size_t end) {
			for (size_t i = begin; i < end; i++)
				res[i] = functor(a[i], b[i], c[i], args...);
		};


		for (uint32_t i = 0; i < cores_amount - 1; i++)
			thread_pull[i] = std::thread(lambda, i * points_for_one_thread, i * points_for_one_thread + points_for_one_thread);

		thread_pull[cores_amount - 1] = std::thread(lambda, (cores_amount - 1) * points_for_one_thread, size);


		for (size_t i = 0; i < cores_amount; i++)
			thread_pull[i].join();


		return res;
	}
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each_GPU(const fun_type& functor, vector<vec_type>& a, vector<vec_type>& b, vector<vec_type>& c, args_types... args) {

		size_t size = min(min(a.size(), b.size()), c.size());
		vector<vec_type> res(size);

		concurrency::array_view<vec_type> vec_a(a.size(), a);
		concurrency::array_view<vec_type> vec_b(b.size(), b);
		concurrency::array_view<vec_type> vec_c(c.size(), c);
		concurrency::array_view<vec_type> vec_d(res.size(), res);

		vec_d.discard_data();

		concurrency::extent<1> ex(size);

		concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp) {
			vec_d[i] = functor(vec_a[i], vec_b[i], vec_c[i], args...);
			});

		vec_d.synchronize();

		return res;
	}
	template<typename vec_type, typename fun_type, typename... args_types> vector<vec_type> for_each	(const fun_type& functor, vector<vec_type>& a, vector<vec_type>& b, vector<vec_type>& c, args_types... args) {

		size_t size = min(a.size(), b.size());
		vector<vec_type> res(size);

		if (functor.switch_cpu[0] <= size && size <= functor.switch_cpu[1]) {
			for (size_t i = 0; i < size; i++) {
				res[i] = functor(a[i], b[i], c[i], args...);
			}
		}
		else if (functor.switch_gpu[0] <= size && size <= functor.switch_gpu[1]) {
			concurrency::array_view<vec_type> vec_a(a.size(), a);
			concurrency::array_view<vec_type> vec_b(b.size(), b);
			concurrency::array_view<vec_type> vec_c(c.size(), c);
			concurrency::array_view<vec_type> vec_d(res.size(), res);

			vec_d.discard_data();

			concurrency::extent<1> ex(size);

			concurrency::parallel_for_each(ex, [=](concurrency::index<1> i) restrict(amp) {
				vec_d[i] = functor(vec_a[i], vec_b[i], vec_c[i], args...);
				});

			vec_d.synchronize();
		}
		else if (functor.switch_mth[0] <= size && size <= functor.switch_mth[1]) {
			uint32_t cores_amount = std::thread::hardware_concurrency();

			std::vector<std::thread> thread_pull(cores_amount);

			size_t points_for_one_thread = size / (cores_amount - 1);
			size_t points_for_lst_thread = size % (cores_amount - 1);

			auto lambda = [&](size_t begin, size_t end) {
				for (size_t i = begin; i < end; i++)
					res[i] = functor(a[i], b[i], c[i], args...);
			};


			for (uint32_t i = 0; i < cores_amount - 1; i++)
				thread_pull[i] = std::thread(lambda, i * points_for_one_thread, i * points_for_one_thread + points_for_one_thread);

			thread_pull[cores_amount - 1] = std::thread(lambda, (cores_amount - 1) * points_for_one_thread, size);


			for (size_t i = 0; i < cores_amount; i++)
				thread_pull[i].join();
		}

		return res;
	}

	template<typename fun_type, typename vec_type, typename... args_types> vector<vec_type> custom_CPU	(const fun_type& functor, vector<vec_type>& a, args_types... args) {
		return functor.CPU(a, args...);
	}
	template<typename fun_type, typename vec_type, typename... args_types> vector<vec_type> custom_GPU	(const fun_type& functor, vector<vec_type>& a, args_types... args) {
		return functor.GPU(a, args...);
	}
	template<typename fun_type, typename vec_type, typename... args_types> vector<vec_type> custom		(const fun_type& functor, vector<vec_type>& a, args_types... args) {
		
	}
};


template<typename fun_type> void calculate_pefomance_1_vector_functor_curve(const fun_type& functor, size_t from = 1, size_t to = 100000, size_t step = 100) {

	std::ofstream file("perfomance_vector_curves.txt");

	uint64_t duration[3];

	uint64_t time_t[4];

	for (size_t size = from; size <= to; size += step) {
		
		vector<float> a(size); a.fill_random(-100000, +100000);

		vector<float> c1;
		vector<float> c2;
		vector<float> c3;
		vector<float> c4;

		time_t[0] = perfomance::microseconds_f();
		c1 = perfomance::vector_calcs::for_each_CPU(functor, a);
		time_t[1] = perfomance::microseconds_f();
		c2 = perfomance::vector_calcs::for_each_MTH(functor, a);
		time_t[2] = perfomance::microseconds_f();
		c3 = perfomance::vector_calcs::for_each_GPU(functor, a);
		time_t[3] = perfomance::microseconds_f();

		if (c1 != c2 || c2 != c1) {
		}

		duration[0] = (time_t[1] - time_t[0]);
		duration[1] = (time_t[2] - time_t[1]);
		duration[2] = (time_t[3] - time_t[2]);

		file << size << " " << duration[0] << " " << duration[1] << " " << duration[2] << std::endl;

		std::cout << "Calculating curve points: " <<  float(size) / float(to - from) * 100 << "%" << "                 \r";
	}
	std::cout << "Calculating curve points: " << 100 << "%           " << std::endl;
}
template<typename fun_type> void calculate_pefomance_2_vector_functor_curve(const fun_type& functor, size_t from = 1, size_t to = 100000, size_t step = 100) {

	std::ofstream file("perfomance_vector_curves.txt");

	uint64_t duration[3];

	uint64_t time_t[4];

	for (size_t size = from; size <= to; size += step) {

		vector<float> a(size); a.fill_random(-100000, +100000);
		vector<float> b(size); b.fill_random(-100000, +100000);

		vector<float> c1;
		vector<float> c2;
		vector<float> c3;
		vector<float> c4;

		time_t[0] = perfomance::microseconds_f();
		c1 = perfomance::vector_calcs::for_each_CPU(functor, a, b);
		time_t[1] = perfomance::microseconds_f();
		c2 = perfomance::vector_calcs::for_each_MTH(functor, a, b);
		time_t[2] = perfomance::microseconds_f();
		c3 = perfomance::vector_calcs::for_each_GPU(functor, a, b);
		time_t[3] = perfomance::microseconds_f();

		if (c1 != c2 || c2 != c1) {
		}

		duration[0] = (time_t[1] - time_t[0]);
		duration[1] = (time_t[2] - time_t[1]);
		duration[2] = (time_t[3] - time_t[2]);

		file << size << " " << duration[0] << " " << duration[1] << " " << duration[2] << std::endl;

		std::cout << "Calculating curve points: " << float(size) / float(to - from) * 100 << "%" << "                 \r";
	}
	std::cout << "Calculating curve points: " << 100 << "%           " << std::endl;
}



#define PerfomanceVectorFunctorBegin(name)				\
struct perfomance_vector_functor_##name {				\
	uint32_t switch_cpu[2] = {0,		INFINITE};		\
	uint32_t switch_mth[2] = {INFINITE,	INFINITE};		\
	uint32_t switch_gpu[2] = {INFINITE,	INFINITE}		\

#define PerfomanceVectorFunctorEnd(name)				\
} ##name												\



#define PefomanceVectorCustomFunctorBegin(name)			\
struct perfomance_vector_custom_functor_##name {		\
	uint32_t switch_trig = INFINITE						\
	
#define PerfomanceVectorCustomFunctorEnd(name)			\
} ##name												\



