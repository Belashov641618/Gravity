#pragma once

#include <iostream>

#include <thread>
#include <cmath>

#include <amp.h>
#include <amp_math.h>

#include <Windows.h>

#include <typeinfo>
#include <typeindex>

#include <vector>
#include <string>

#include <random>
#include <fstream>
#include <chrono>

#include <assert.h>


namespace perfomance {


	/*Hard math functions*/
	void liner_fit(float& A, float& B, const std::vector<float>& X, const std::vector<float>& Y) {

		float x2 = 0;
		float x = 0;
		float xy = 0;
		float y = 0;

		for (uint32_t i = 0; i < X.size(); i++) {
			x2	+= X[i] * X[i];
			x	+= X[i];
			xy	+= X[i] * Y[i];
			y	+= Y[i];
		}

		float d		= x2 * X.size() - x * x;
		float da	= xy * X.size() - x * y;
		float db	= x2 * y - x * xy;

		A = da / d;
		B = db / d;

	}


	/*Time functions*/
	uint64_t zero_time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	uint64_t nanoseconds() {
		return (std::chrono::high_resolution_clock::now().time_since_epoch().count() - zero_time);
	}
	uint64_t microseconds() {
		return (std::chrono::high_resolution_clock::now().time_since_epoch().count() - zero_time) / 1000;
	}
	uint64_t milliseconds() {
		return (std::chrono::high_resolution_clock::now().time_since_epoch().count() - zero_time) / 1000000;
	}
	uint64_t seconds() {
		return (std::chrono::high_resolution_clock::now().time_since_epoch().count() - zero_time) / 1000000000;
	}

	uint64_t nanoseconds_f() {
		return (std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}
	uint64_t microseconds_f() {
		return (std::chrono::high_resolution_clock::now().time_since_epoch().count()) / 1000;
	}
	uint64_t milliseconds_f() {
		return (std::chrono::high_resolution_clock::now().time_since_epoch().count()) / 1000000;
	}
	uint64_t seconds_f() {
		return (std::chrono::high_resolution_clock::now().time_since_epoch().count()) / 1000000000;
	}

	template<typename function_type, typename... args_types> uint64_t calculate_duration_time(function_type function, args_types... args) {

		uint64_t Timer[2];

		Timer[0] = nanoseconds_f();
			
		function(args);

		Timer[1] = nanoseconds_f();

		uint64_t Duration = Timer[1] - Timer[0];

		const uint64_t multyplier = 5;

		if		(Duration > multyplier*1000000000)	std::cout << Duration / 1000000000	<< " seconds";
		else if (Duration > multyplier*1000000)		std::cout << Duration / 1000000		<< " milliseconds";
		else if (Duration > multyplier*1000)		std::cout << Duration / 1000		<< " microseconds";
		else										std::cout << Duration << " nanoseconds";

		return Duration;
	}


	/*Random functions*/
	float random(float from, float to) {
		return (float(rand()) / float(RAND_MAX)) * (to - from) + from;
	}


	#include "perfomance/mat.h"
	#include "perfomance/vector.h"
	#include "perfomance/vec.h"

}