#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries

namespace known
{

	#define KNOWN_UNARY_GLM_WRAPPER(TITLE, LOWER)\
	template<typename T, typename A>\
	struct TITLE {\
		using size_type = typename A::size_type;\
		using value_type = T;\
		A a; \
		constexpr explicit TITLE(const A& a):       a(a) {}\
		constexpr          TITLE(const TITLE<T,A>& f): a(f.a) {}\
		constexpr inline auto size() const { return a.size(); }\
		constexpr inline auto operator()(const size_type i ) const { return glm::LOWER(a[i]); }\
		constexpr inline auto operator[](const size_type i ) const { return glm::LOWER(a[i]); }\
	};\
	template<typename T, typename A>\
	constexpr inline auto LOWER(const A a)\
	{\
		return TITLE<T,A>(a);\
	}

	#define KNOWN_UNARY_GLM_OPERATOR(TITLE, LOWER)\
	template<typename A>\
	struct TITLE {\
		using size_type = typename A::size_type;\
		using value_type = typename A::value_type;\
		A a; \
		constexpr explicit TITLE(const A& a):       a(a) {}\
		constexpr          TITLE(const TITLE<A>& f): a(f.a) {}\
		constexpr inline auto size() const { return a.size(); }\
		constexpr inline auto operator()(const size_type i ) const { return glm::LOWER(a[i]); }\
		constexpr inline auto operator[](const size_type i ) const { return glm::LOWER(a[i]); }\
	};\
	template<typename A>\
	constexpr inline auto LOWER(const A a)\
	{\
		return TITLE<T,A>(a);\
	}

	KNOWN_UNARY_GLM_WRAPPER(Length,     length)
	KNOWN_UNARY_GLM_WRAPPER(Normalize,  normalize)

	#undef KNOWN_UNARY_GLM_WRAPPER
	#undef KNOWN_UNARY_GLM_OPERATOR

	#define KNOWN_BINARY_GLM_WRAPPER(TITLE, LOWER)\
	template<typename T, typename A, typename B>\
	struct TITLE {\
		using size_type = typename std::size_t;\
		using value_type = T;\
		A a; B b;\
		constexpr explicit TITLE(const A& a, const B& b): a(a), b(b) {}\
		constexpr          TITLE(const TITLE<T,A,B>& f):     a(f.a), b(f.b) {}\
		constexpr inline auto size() const { return std::max(a.size(), b.size()); }\
		constexpr inline auto operator()(const size_type i ) const { return glm::LOWER(a(i),b(i)); }\
		constexpr inline auto operator[](const size_type i ) const { return glm::LOWER(a(i),b(i)); }\
	};\
	template<typename T, typename A, typename B>\
	constexpr inline auto LOWER(const A a, const B b)\
	{\
		return TITLE<T,A,B>(a, b);\
	}

	#define KNOWN_BINARY_GLM_OPERATOR(TITLE, LOWER)\
	template<typename A, typename B>\
	struct TITLE {\
		using size_type = typename std::size_t;\
		using value_type = typename A::value_type;\
		A a; B b;\
		constexpr explicit TITLE(const A& a, const B& b): a(a), b(b) {}\
		constexpr          TITLE(const TITLE<A,B>& f):     a(f.a), b(f.b) {}\
		constexpr inline auto size() const { return std::max(a.size(), b.size()); }\
		constexpr inline auto operator()(const size_type i ) const { return glm::LOWER(a(i),b(i)); }\
		constexpr inline auto operator[](const size_type i ) const { return glm::LOWER(a(i),b(i)); }\
	};\
	template<typename A, typename B>\
	constexpr inline auto LOWER(const A a, const B b)\
	{\
		return TITLE<A,B>(a, b);\
	}

	KNOWN_BINARY_GLM_WRAPPER(Dot,     dot)
	KNOWN_BINARY_GLM_OPERATOR(Cross,   cross)
	KNOWN_BINARY_GLM_OPERATOR(Reflect, reflect)
	KNOWN_BINARY_GLM_OPERATOR(Refract, refract)

	#undef KNOWN_BINARY_GLM_WRAPPER
	#undef KNOWN_BINARY_GLM_OPERATOR

}

