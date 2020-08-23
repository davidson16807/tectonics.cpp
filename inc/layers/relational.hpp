#pragma once

#include "Layers.hpp"

namespace layers
{

	const float LAYER_EPSILON = 1e-4;

	template <std::size_t N, class T>
	bool equal(const Layers<T,N>& a, const T b, T threshold = T(LAYER_EPSILON))
	{
		bool out(true);
		T diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out &= diff*diff <= threshold;
		}
		return out;
	}
	template <std::size_t N, class T>
	bool notEqual(const Layers<T,N>& a, const T b, T threshold = T(LAYER_EPSILON))
	{
		bool out(false);
		T diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out |= diff*diff > threshold;
		}
		return out;
	}
	template <std::size_t N, class T>
	bool equal(const Layers<T,N>& a, const Layers<T,N>& b, T threshold = T(LAYER_EPSILON))
	{
		if (a.size() != b.size())
		{
			return false;
		}
		bool out(true);
		T diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out &= diff*diff <= threshold;
		}
		return out;
	}
	template <std::size_t N, class T>
	bool notEqual(const Layers<T,N>& a, const Layers<T,N>& b, T threshold = T(LAYER_EPSILON))
	{
		if (a.size() != b.size())
		{
			return true;
		}
		bool out(false);
		T diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out |= diff*diff > threshold;
		}
		return out;
	}



	template <std::size_t N, class T>
	void equal(const Layers<T,N>& a, const T b, Layers<bool,N>& out, T threshold = T(LAYER_EPSILON))
	{
		T diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out[i] = diff*diff <= threshold;
		}
	}
	template <std::size_t N, class T>
	void notEqual(const Layers<T,N>& a, const T b, Layers<bool,N>& out, T threshold = T(LAYER_EPSILON))
	{
		T diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out[i] = diff*diff > threshold;
		}
	}
	template <std::size_t N, class T>
	void equal(const Layers<T,N>& a, const Layers<T,N>& b, Layers<bool,N>& out, T threshold = T(LAYER_EPSILON))
	{
		T diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out[i] = diff*diff <= threshold;
		}
	}
	template <std::size_t N, class T>
	void notEqual(const Layers<T,N>& a, const Layers<T,N>& b, Layers<bool,N>& out, T threshold = T(LAYER_EPSILON))
	{
		T diff(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out[i] = diff*diff > threshold;
		}
	}






	template <std::size_t N>
	bool equal(const Layers<bool,N>& a, const bool b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] == b;
		}
		return out;
	}
	template <std::size_t N>
	bool notEqual(const Layers<bool,N>& a, const bool b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] != b;
		}
		return out;
	}
	template <std::size_t N>
	bool equal(const Layers<bool,N>& a, const Layers<bool,N>& b)
	{
		if (a.size() != b.size())
		{
			return false;
		}
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] == b[i];
		}
		return out;
	}
	template <std::size_t N>
	bool notEqual(const Layers<bool,N>& a, const Layers<bool,N>& b)
	{
		if (a.size() != b.size())
		{
			return true;
		}
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] != b[i];
		}
		return out;
	}



	template <std::size_t N>
	void equal(const Layers<bool,N>& a, const bool b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] == b;
		}
	}
	template <std::size_t N>
	void notEqual(const Layers<bool,N>& a, const bool b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] != b;
		}
	}
	template <std::size_t N>
	void equal(const Layers<bool,N>& a, const Layers<bool,N>& b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] == b[i];
		}
	}
	template <std::size_t N>
	void notEqual(const Layers<bool,N>& a, const Layers<bool,N>& b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] != b[i];
		}
	}






	template <std::size_t N>
	bool equal(const Layers<int,N>& a, const int b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] == b;
		}
		return out;
	}
	template <std::size_t N>
	bool notEqual(const Layers<int,N>& a, const int b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] != b;
		}
		return out;
	}
	template <std::size_t N>
	bool equal(const Layers<int,N>& a, const Layers<int,N>& b)
	{
		if (a.size() != b.size())
		{
			return false;
		}
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] == b[i];
		}
		return out;
	}
	template <std::size_t N>
	bool notEqual(const Layers<int,N>& a, const Layers<int,N>& b)
	{
		if (a.size() != b.size())
		{
			return true;
		}
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] != b[i];
		}
		return out;
	}



	template <std::size_t N>
	void equal(const Layers<int,N>& a, const int b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] == b;
		}
	}
	template <std::size_t N>
	void notEqual(const Layers<int,N>& a, const int b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] != b;
		}
	}
	template <std::size_t N>
	void equal(const Layers<int,N>& a, const Layers<int,N>& b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] == b[i];
		}
	}
	template <std::size_t N>
	void notEqual(const Layers<int,N>& a, const Layers<int,N>& b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] != b[i];
		}
	}






	template <std::size_t N>
	bool equal(const Layers<unsigned int,N>& a, const unsigned int b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] == b;
		}
		return out;
	}
	template <std::size_t N>
	bool notEqual(const Layers<unsigned int,N>& a, const unsigned int b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] != b;
		}
		return out;
	}
	template <std::size_t N>
	bool equal(const Layers<unsigned int,N>& a, const Layers<unsigned int,N>& b)
	{
		if (a.size() != b.size())
		{
			return false;
		}
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] == b[i];
		}
		return out;
	}
	template <std::size_t N>
	bool notEqual(const Layers<unsigned int,N>& a, const Layers<unsigned int,N>& b)
	{
		if (a.size() != b.size())
		{
			return true;
		}
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] != b[i];
		}
		return out;
	}



	template <std::size_t N>
	void equal(const Layers<unsigned int,N>& a, const unsigned int b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] == b;
		}
	}
	template <std::size_t N>
	void notEqual(const Layers<unsigned int,N>& a, const unsigned int b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] != b;
		}
	}
	template <std::size_t N>
	void equal(const Layers<unsigned int,N>& a, const Layers<unsigned int,N>& b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] == b[i];
		}
	}
	template <std::size_t N>
	void notEqual(const Layers<unsigned int,N>& a, const Layers<unsigned int,N>& b, Layers<bool,N>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] != b[i];
		}
	}





	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void greaterThan(const Layers<T,N>& a, const T2 b, Layers<bool,N>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai > bi; }, out); 
	}
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void greaterThanEqual(const Layers<T,N>& a, const T2 b, Layers<bool,N>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai >= bi; }, out); 
	}
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void lessThan(const Layers<T,N>& a, const T2 b, Layers<bool,N>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai < bi; }, out); 
	}
	template <std::size_t N, class T, class T2, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	void lessThanEqual(const Layers<T,N>& a, const T2 b, Layers<bool,N>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai <= bi; }, out); 
	}



	template <std::size_t N, class T, class T2>
	void greaterThan(const Layers<T,N>& a, const Layers<T2,N>& b, Layers<bool,N>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai > bi; }, out); 
	}
	template <std::size_t N, class T, class T2>
	void greaterThanEqual(const Layers<T,N>& a, const Layers<T2,N>& b, Layers<bool,N>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai >= bi; }, out); 
	}
	template <std::size_t N, class T, class T2>
	void lessThan(const Layers<T,N>& a, const Layers<T2,N>& b, Layers<bool,N>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai <= bi; }, out); 
	}
	template <std::size_t N, class T, class T2>
	void lessThanEqual(const Layers<T,N>& a, const Layers<T2,N>& b, Layers<bool,N>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai < bi; }, out); 
	}

	
	template <std::size_t N>
	bool all(const Layers<bool,N>& a)
	{
		bool out = true;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i];
		}
		return out;
	}
	template <std::size_t N>
	bool any(const Layers<bool,N>& a)
	{
		bool out = false;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i];
		}
		return out;
	}
	template <std::size_t N>
	bool none(const Layers<bool,N>& a)
	{
		bool out = false;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i];
		}
		return !out;
	}

}
