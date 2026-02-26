#pragma once
#include <random>

class Random
{
public:
	template<typename T = int>
	static T Generate(T from, T to)
	{
		static std::mt19937 _ins;
		static bool init = false;
		if(!init)
		{
			static std::random_device _rd;
			_ins = std::mt19937(_rd());
			init = !init;
		}

		if (to == 0 || to < from)
			return 0;

		return std::uniform_int_distribution<T>(from, to)(_ins);
	}
};