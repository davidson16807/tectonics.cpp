#pragma once

#include <cmath>

#include "Layers.hpp"
#include "relational.hpp"
#include "common.hpp"

/*
"convenience.hpp" contains functions that pass output as return values rather than using using output parameters
This provides convenience at the expense of performance, since now we have to call constructors for the new objects. 
See https://codeyarns.com/2010/10/21/c-return-value-versus-output-parameter/ for more info.
It is important to keep these functions separate from the rest of the library for two reasons:
 1.) It encourages good practice, since you have to explicitly opt-in to less performant convenience functions.
 2.) It provides a nice itemization of functions that will have to be created if you subclass Layers<T,N> 
*/

namespace layers
{
	template <std::size_t N, class T1, typename F>
	inline Layers<T1,N> transform(const Layers<T1,N>& a, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, f, out); 
		return out;
	}


	template <std::size_t N, class T1, class T2, typename F>
	inline Layers<T1,N> transform(const Layers<T1,N>& a, const Layers<T2,N>& b, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, f, out); 
		return out;
	}
	template <std::size_t N, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	inline Layers<T1,N> transform(const Layers<T1,N>& a, const T2 b, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, f, out); 
		return out;
	}
	template <std::size_t N, class T1, class T2, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractLayers, T1>::value, int> = 0>
	inline Layers<T1,N> transform(const T1 a, const Layers<T2,N>& b, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, f, out); 
		return out;
	}




	template <std::size_t N, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractLayers, F>::value, int> = 0>
	inline Layers<T1,N> transform(const Layers<T1,N>& a, const Layers<T2,N>& b, const Layers<T3,N>& c, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, c, f, out); 
		return out;
	}
	template <std::size_t N, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractLayers, T3>::value && !std::is_base_of<AbstractLayers, F>::value, int> = 0>
	inline Layers<T1,N> transform(const Layers<T1,N>& a, const Layers<T2,N>& b, const T3 c, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, c, f, out); 
		return out;
	}
	template <std::size_t N, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value && !std::is_base_of<AbstractLayers, F>::value, int> = 0>
	inline Layers<T1,N> transform(const Layers<T1,N>& a, const T2 b, const Layers<T3,N>& c, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, c, f, out); 
		return out;
	}
	template <std::size_t N, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value && !std::is_base_of<AbstractLayers, T3>::value && !std::is_base_of<AbstractLayers, F>::value, int> = 0>
	inline Layers<T1,N> transform(const Layers<T1,N>& a, const T2 b, const T3 c, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, c, f, out); 
		return out;
	}
	template <std::size_t N, class T1, class T2, class T3, typename F, 
		std::enable_if_t<!std::is_base_of<AbstractLayers, T1>::value && !std::is_base_of<AbstractLayers, F>::value, int> = 0>
	inline Layers<T1,N> transform(const T1 a, const Layers<T2,N>& b, const Layers<T3,N>& c, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, c, f, out); 
		return out;
	}
	template <std::size_t N, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<AbstractLayers, T1>::value && !std::is_base_of<AbstractLayers, T3>::value && !std::is_base_of<AbstractLayers, F>::value, int> = 0>
	inline Layers<T1,N> transform(const T1 a, const Layers<T2,N>& b, const T3 c, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, c, f, out); 
		return out;
	}
	template <std::size_t N, class T1, class T2, class T3, typename F,
		std::enable_if_t<!std::is_base_of<AbstractLayers, T1>::value && !std::is_base_of<AbstractLayers, T2>::value && !std::is_base_of<AbstractLayers, F>::value, int> = 0>
	inline Layers<T1,N> transform(const T1 a, const T2 b, const Layers<T3,N>& c, F f)
	{
		Layers<T1,N> out(T1(0));
		layers::transform(a, b, c, f, out); 
		return out;
	}





	/// Returns x if x >= 0; otherwise, it returns -x.
	template <std::size_t N, class T>
	Layers<T,N> abs(const Layers<T,N>& a)
	{
		return layers::transform(a, [](T ai){ return ai >= 0? ai : -ai; });
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template <std::size_t N, class T>
	Layers<T,N> sign(const Layers<T,N>& a)
	{
		return layers::transform(a, [](T ai){ return (T(0) < ai) - (ai < T(0)); });
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template <std::size_t N, class T>
	Layers<T,N> floor(const Layers<T,N>& a)
	{
		return layers::transform(a, std::floor);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template <std::size_t N, class T>
	Layers<T,N> trunc(const Layers<T,N>& a)
	{
		return layers::transform(a, std::trunc);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template <std::size_t N, class T>
	Layers<T,N> round(const Layers<T,N>& a)
	{
		return layers::transform(a, std::round);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template <std::size_t N, class T>
	Layers<T,N> ceil(const Layers<T,N>& a)
	{
		layers::transform(a, std::ceil);
	}


}