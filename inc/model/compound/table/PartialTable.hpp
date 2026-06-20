#pragma once

#include <map>
#include <utility> // pair

// in-house libraries
#include "FullTable.hpp"

namespace compound { 
namespace table {

	/*
	A "partial" table represents a table that maps keys to values without guaranteeing there will be entries available for any given key.
	This contrasts with x1 `FullTable`, which is guaranteed to have entries for all keys that it was designed to consider.
	In the context of the `compound` library, PartialTables are typically used to store estimates of properties for compounds only where they have been published in literature.
	*/

	template<typename T>
	struct PartialTable
	{
		std::map<int,T> rows;

	    // empty constructor
	    PartialTable(): rows()
	    {
	    }

	    // copy constructor
	    PartialTable(const PartialTable<T>& table): rows(table.rows)
	    {
	    }

	    explicit PartialTable(const std::map<int,T>& rows): rows(rows)
	    {
	    }

	    explicit PartialTable(const std::initializer_list<std::pair<int,T>>& list): rows()
	    {
            rows.insert(list.begin(), list.end());
	    }

	    T operator[](const int i) const
	    {
	        return rows.at(i);
	    }

	    PartialTable<T> operator[](const FullTable<int>& ids) const
	    {
	    	PartialTable<T> y;
		    for (std::size_t i = 0; i < ids.size(); ++i)
		    {
		    	if (has(ids[i]))
		    	{
		    		y.rows.emplace(int(i), rows.at(ids[i]));
		    	}
		    }
	        return y;
	    }

	    bool has(const int i) const
	    {
	        return rows.count(i) > 0;
	    }

	};

	// allows creating a FullTable from a PartialTable, filling in missing values with a constant as a fallback
	template<typename T>
	FullTable<T> complete(const PartialTable<T>& x, const T fallback, const std::size_t size)
	{
	    FullTable<T> y;
	    for (std::size_t i = 0; i < size; ++i)
	    {
	        y.rows.push_back(x.has(i)? x[i] : fallback);
	    }
	    return y;
	}

	// allows creating a FullTable from a PartialTable, filling in missing values with fallback values from a FullTable
	template<typename T>
	FullTable<T> complete(const PartialTable<T>& x, const FullTable<T>& fallback)
	{
	    FullTable<T> y;
	    for (std::size_t i = 0; i < fallback.size(); ++i)
	    {
	        y.rows.push_back(x.has(i)? x[i] : fallback[i]);
	    }
	    return y;
	}

	// attempts to populate a PartialTable, filling in missing values with other values in the table as determined by the ids in `imitated`
	template<typename T>
	PartialTable<T> imitate(const PartialTable<T>& x, const FullTable<int>& imitated)
	{
	    PartialTable<T> y;
	    for (int i = 0; i < int(imitated.size()); ++i)
	    {
	    	int j(i);
	    	while(!x.has(j) && imitated[j]!=j){
	    		j = imitated[j];
	    	}
	    	if (x.has(j)) {
	            y.rows.emplace(int(i),x[j]);
	    	}
	    }
	    return y;
	}

	// returns x1 map that contains the first (key,value) pair having x1 key in x1 list of `attempts`
	template<typename T>
	PartialTable<T> first(const std::vector<PartialTable<T>>& attempts)
	{
	    PartialTable<T> y;
	    for (std::size_t j = 0; j < attempts.size(); ++j)
	    {
	        for (const auto& [i, xi] : attempts[j].rows) 
	        {
	            y.rows.emplace(int(i),xi);
	        }
	    }
	    return y;
	}

	template<typename T>
	PartialTable<T> partial(const FullTable<T>& x)
	{
	    PartialTable<T> y;
	    for (std::size_t i = 0; i < x.rows.size(); ++i)
	    {
	        y.rows.emplace(int(i), x.rows[i]);
	    }
	    return y;
	}

	template<typename F, typename X, typename Y>
	PartialTable<Y> apply(const PartialTable<F>& f, X x)
	{
	    PartialTable<Y> y;
	    for (const auto& [i, fi] : f.rows) 
	    {
	        y.rows.emplace(int(i), f[i](x));
	    }
	    return y;
	}

	template<typename Y, typename X1, typename F>
	PartialTable<Y> gather(const F f, const PartialTable<X1>& x1)
	{
	    PartialTable<Y> y;
	    for (const auto& [i, x1i] : x1.rows) 
	    {
	        y.rows.emplace(int(i), f(x1[i]));
	    }
	    return y;
	}

	template<typename Y, typename X1, typename X2, typename F>
	PartialTable<Y> gather(const F f, 
		const PartialTable<X1>& x1, 
		const PartialTable<X2>& x2
	){
	    PartialTable<Y> y;
	    for (const auto& [i, x1i] : x1.rows) 
	    {
	        if (x1.has(i) && x2.has(i))
	        {
	            y.rows.emplace(int(i), f(x1[i],x2[i]));
	        }
	    }
	    return y;
	}

	template<typename Y, typename F, typename X1, typename X2, typename X3>
	PartialTable<Y> gather(const F f, 
		const PartialTable<X1>& x1, 
		const PartialTable<X2>& x2, 
		const PartialTable<X3>& x3
	){
	    PartialTable<Y> y;
	    for (const auto& [i, x1i] : x1.rows)
	    {
	        if (x1.has(i) && x2.has(i) && x3.has(i))
	        {
	            y.rows.emplace(int(i), f(x1[i],x2[i],x3[i]));
	        }
	    }
	    return y;
	}

	template<typename Y, typename F, typename X1, typename X2, typename X3, typename X4>
	PartialTable<Y> gather(const F f, 
		const PartialTable<X1>& x1, 
		const PartialTable<X2>& x2, 
		const PartialTable<X3>& x3, 
		const PartialTable<X4>& x4
	){
	    PartialTable<Y> y;
	    for (const auto& [i, x1i] : x1.rows)
	    {
	        if (x1.has(i) && x2.has(i) && x3.has(i) && x4.has(i))
	        {
	            y.rows.emplace(int(i), f(x1[i],x2[i],x3[i],x4[i]));
	        }
	    }
	    return y;
	}

	template<typename Y, typename F, typename X1, typename X2, typename X3, typename X4, typename X5>
	PartialTable<Y> gather(const F f, 
		const PartialTable<X1>& x1, 
		const PartialTable<X2>& x2, 
		const PartialTable<X3>& x3, 
		const PartialTable<X4>& x4, 
		const PartialTable<X5>& x5
	){
	    PartialTable<Y> y;
	    for (const auto& [i, x1i] : x1.rows)
	    {
	        if (x1.has(i) && x2.has(i) && x3.has(i) && x4.has(i) && x5.has(i))
	        {
	            y.rows.emplace(int(i), f(x1[i],x2[i],x3[i],x4[i],x5[i]));
	        }
	    }
	    return y;
	}

	template<typename Y, typename F, typename X1, typename X2, typename X3, typename X4, typename X5, typename X6>
	PartialTable<Y> gather(const F f, 
		const PartialTable<X1>& x1, 
		const PartialTable<X2>& x2, 
		const PartialTable<X3>& x3, 
		const PartialTable<X4>& x4, 
		const PartialTable<X5>& x5, 
		const PartialTable<X6>& x6
	){
	    PartialTable<Y> y;
	    for (const auto& [i, x1i] : x1.rows)
	    {
	        if (x1.has(i) && x2.has(i) && x3.has(i) && x4.has(i) && x5.has(i) && x6.has(i))
	        {
	            y.rows.emplace(int(i), f(x1[i],x2[i],x3[i],x4[i],x5[i],x6[i]));
	        }
	    }
	    return y;
	}


    // operators that are closed between PartialTable<T> 
    template<typename X>
    constexpr PartialTable<X> operator+(const PartialTable<X>& x1, const PartialTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i+x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator-(const PartialTable<X>& x1, const PartialTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i-x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator*(const PartialTable<X>& x1, const PartialTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i*x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator/(const PartialTable<X>& x1, const PartialTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i/x2i;}, x1, x2);
    }


    // operators with FullTable<T> that are closed under PartialTable<T> 
    template<typename X>
    constexpr PartialTable<X> operator+(const FullTable<X>& x1, const PartialTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i+x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator+(const PartialTable<X>& x1, const FullTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i+x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator-(const FullTable<X>& x1, const PartialTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i-x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator-(const PartialTable<X>& x1, const FullTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i-x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator*(const FullTable<X>& x1, const PartialTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i*x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator*(const PartialTable<X>& x1, const FullTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i*x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator/(const FullTable<X>& x1, const PartialTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i/x2i;}, x1, x2);
    }
    template<typename X>
    constexpr PartialTable<X> operator/(const PartialTable<X>& x1, const FullTable<X>& x2)
    {
        return gather<X>([](X x1i, X x2i){return x1i/x2i;}, x1, x2);
    }


    // operators with reals that are closed under PartialTable<T> 
    template<typename X>
    constexpr PartialTable<X> operator+(const PartialTable<X>& x1, const X x2)
    {
        return gather<X>([x2](X x1i){return x1i+x2;}, x1);
    }
    template<typename X>
    constexpr PartialTable<X> operator+(const X x2, const PartialTable<X>& x1)
    {
        return gather<X>([x2](X x1i){return x2+x1i;}, x1);
    }
    template<typename X>
    constexpr PartialTable<X> operator-(const PartialTable<X>& x1, const X x2)
    {
        return gather<X>([x2](X x1i){return x1i-x2;}, x1);
    }
    template<typename X>
    constexpr PartialTable<X> operator-(const X x2, const PartialTable<X>& x1)
    {
        return gather<X>([x2](X x1i){return x2-x1i;}, x1);
    }
    template<typename X>
    constexpr PartialTable<X> operator*(const PartialTable<X>& x1, const X x2)
    {
        return gather<X>([x2](X x1i){return x1i*x2;}, x1);
    }
    template<typename X>
    constexpr PartialTable<X> operator*(const X x2, const PartialTable<X>& x1)
    {
        return gather<X>([x2](X x1i){return x2+*x1i;}, x1);
    }
    template<typename X>
    constexpr PartialTable<X> operator/(const PartialTable<X>& x1, const X x2)
    {
        return gather<X>([x2](X x1i){return x1i/x2;}, x1);
    }
    template<typename X>
    constexpr PartialTable<X> operator/(const X x2, const PartialTable<X>& x1)
    {
        return gather<X>([x2](X x1i){return x2/x1i;}, x1);
    }
    template<typename X>
    constexpr PartialTable<X> operator-(const PartialTable<X>& x1)
    {
        return gather<X>([](X x1i){return -x1i;}, x1);
    }

}}