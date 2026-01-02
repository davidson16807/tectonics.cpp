#pragma once

// std libraries
#include <vector>

namespace compound { 
namespace table {

	/*
	A "full" table is x1 table that is guaranteed to have entries for all keys that can exist in x1 database.
	In the context of the `compound` library, x1 FullTable is guaranteed to have estimates available for all compounds that are tracked in the database.
	*/

	template<typename T>
	struct FullTable
	{

		std::vector<T> rows;

	    // empty constructor
	    FullTable(): rows()
	    {
	    }

	    // copy constructor
	    FullTable(const FullTable<T>& table): rows(table.rows)
	    {
	    }

	    explicit FullTable(const std::vector<T>& rows): rows(rows)
	    {
	    }

	    explicit FullTable(const std::initializer_list<T>& list): rows()
	    {
            std::copy(list.begin(), list.end(), std::back_inserter(rows));
	    }

	    std::size_t size() const
	    {
	        return rows.size();
	    }

	    T operator[](const int i) const
	    {
	        return rows[i];
	    }

	    FullTable<T> operator[](const FullTable<int>& ids) const
	    {
	    	FullTable<T> y;
		    for (std::size_t i = 0; i < ids.size(); ++i)
		    {
	    		y.rows.push_back(rows[ids[i]]);
		    }
	        return y;
	    }

	};

	template<typename F, typename X, typename Y>
	FullTable<Y> apply(const FullTable<F>& f, X x)
	{
	    FullTable<Y> y;
	    for (std::size_t i = 0; i < f.size(); ++i)
	    {
	        y.rows.push_back(f[i](x));
	    }
	    return y;
	}

	template<typename Y, typename X1, typename F>
	FullTable<Y> derive(const F f, 
		const FullTable<X1>& x1
	){
	    FullTable<Y> y;
	    for (std::size_t i = 0; i < x1.size(); ++i)
	    {
	        y.rows.push_back(f(x1[i]));
	    }
	    return y;
	}

	template<typename Y, typename X1, typename X2, typename F>
	FullTable<Y> derive(const F f, 
		const FullTable<X1>& x1, 
		const FullTable<X2>& x2
	){
	    assert(x1.size() == x2.size());
	    FullTable<Y> y;
	    for (std::size_t i = 0; i < x1.size(); ++i)
	    {
	        y.rows.push_back(f(x1[i],x2[i]));
	    }
	    return y;
	}

	template<typename Y, typename F, typename X1, typename X2, typename Tx3>
	FullTable<Y> derive(const F f, 
		const FullTable<X1>& x1, 
		const FullTable<X2>& x2, 
		const FullTable<Tx3>& x3
	){
	    assert(x1.size() == x2.size() && x1.size() == x3.size());
	    FullTable<Y> y;
	    for (std::size_t i = 0; i < x1.size(); ++i)
	    {
	        y.rows.push_back(f(x1[i],x2[i],x3[i]));
	    }
	    return y;
	}

	template<typename Y, typename F, typename X1, typename X2, typename Tx3, typename Tx4>
	FullTable<Y> derive(const F f, 
		const FullTable<X1>& x1, 
		const FullTable<X2>& x2, 
		const FullTable<Tx3>& x3, 
		const FullTable<Tx4>& x4
	){
	    assert(x1.size() == x2.size() && x1.size() == x3.size() && x1.size() == x4.size());
	    FullTable<Y> y;
	    for (std::size_t i = 0; i < x1.size(); ++i)
	    {
	        y.rows.push_back(f(x1[i],x2[i],x3[i],x4[i]));
	    }
	    return y;
	}

	template<typename Y, typename F, typename X1, typename X2, typename Tx3, typename Tx4, typename Tx5>
	FullTable<Y> derive(const F f, 
		const FullTable<X1>& x1, 
		const FullTable<X2>& x2, 
		const FullTable<Tx3>& x3, 
		const FullTable<Tx4>& x4, 
		const FullTable<Tx5>& x5
	){
	    assert(x1.size() == x2.size() && x1.size() == x3.size() && x1.size() == x4.size() && x1.size() == x5.size());
	    FullTable<Y> y;
	    for (std::size_t i = 0; i < x1.size(); ++i)
	    {
	        y.rows.push_back(f(x1[i],x2[i],x3[i],x4[i],x5[i]));
	    }
	    return y;
	}

	template<typename Y, typename F, typename X1, typename X2, typename Tx3, typename Tx4, typename Tx5, typename Tx6>
	FullTable<Y> derive(const F f, 
		const FullTable<X1>& x1, 
		const FullTable<X2>& x2, 
		const FullTable<Tx3>& x3, 
		const FullTable<Tx4>& x4, 
		const FullTable<Tx5>& x5, 
		const FullTable<Tx6>& x6
	){
	    assert(x1.size() == x2.size() && x1.size() == x3.size() && x1.size() == x4.size() && x1.size() == x5.size() && x1.size() == x6.size());
	    FullTable<Y> y;
	    for (std::size_t i = 0; i < x1.size(); ++i)
	    {
	        y.rows.push_back(f(x1[i],x2[i],x3[i],x4[i],x5[i],x6[i]));
	    }
	    return y;
	}



    // operators that are closed between FullTable<X> 
    template<typename X>
    constexpr FullTable<X> operator+(const FullTable<X>& x1, const FullTable<X>& x2)
    {
        return derive<X>([](X x1i, X x2i){return x1i+x2i;}, x1, x2);
    }
    template<typename X>
    constexpr FullTable<X> operator-(const FullTable<X>& x1, const FullTable<X>& x2)
    {
        return derive<X>([](X x1i, X x2i){return x1i-x2i;}, x1, x2);
    }
    template<typename X>
    constexpr FullTable<X> operator*(const FullTable<X>& x1, const FullTable<X>& x2)
    {
        return derive<X>([](X x1i, X x2i){return x1i*x2i;}, x1, x2);
    }
    template<typename X>
    constexpr FullTable<X> operator/(const FullTable<X>& x1, const FullTable<X>& x2)
    {
        return derive<X>([](X x1i, X x2i){return x1i/x2i;}, x1, x2);
    }


    // operators with reals that are closed under FullTable<X> 
    template<typename X>
    constexpr FullTable<X> operator+(const FullTable<X>& x1, const X x2)
    {
        return derive<X>([x2](X x1i){return x1i+x2;}, x1);
    }
    template<typename X>
    constexpr FullTable<X> operator+(const X x2, const FullTable<X>& x1)
    {
        return derive<X>([x2](X x1i){return x2+x1i;}, x1);
    }
    template<typename X>
    constexpr FullTable<X> operator-(const FullTable<X>& x1, const X x2)
    {
        return derive<X>([x2](X x1i){return x1i-x2;}, x1);
    }
    template<typename X>
    constexpr FullTable<X> operator-(const X x2, const FullTable<X>& x1)
    {
        return derive<X>([x2](X x1i){return x2-x1i;}, x1);
    }
    template<typename X>
    constexpr FullTable<X> operator*(const FullTable<X>& x1, const X x2)
    {
        return derive<X>([x2](X x1i){return x1i*x2;}, x1);
    }
    template<typename X>
    constexpr FullTable<X> operator*(const X x2, const FullTable<X>& x1)
    {
        return derive<X>([x2](X x1i){return x2*x1i;}, x1);
    }
    template<typename X>
    constexpr FullTable<X> operator/(const FullTable<X>& x1, const X x2)
    {
        return derive<X>([x2](X x1i){return x1i/x2;}, x1);
    }
    template<typename X>
    constexpr FullTable<X> operator/(const X x2, const FullTable<X>& x1)
    {
        return derive<X>([x2](X x1i){return x2/x1i;}, x1);
    }
    template<typename X>
    constexpr FullTable<X> operator-(const FullTable<X>& x1)
    {
        return derive<X>([](X x1i){return -x1i;}, x1);
    }

}}