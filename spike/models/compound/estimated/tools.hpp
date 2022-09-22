#pragma once

// std libraries
#include <vector>
#include <map>

namespace compound { 
namespace estimated {

    // allows creating a vector from a map, fills in missing values with a constant as a fallback
    template<typename T>
    std::vector<T> complete(const std::map<int,T>& x, const T fallback, const std::size_t size)
    {
        std::vector<T> y;
        for (std::size_t i = 0; i < size; ++i)
        {
            y.push_back(x.count(i)>0? x.at(i) : fallback);
        }
        return y;
    }

    // allows creating a vector from a map, fills in missing values with fallback values from a vector
    template<typename T>
    std::vector<T> complete(const std::map<int,T>& x, const std::vector<T>& fallback)
    {
        std::vector<T> y;
        for (std::size_t i = 0; i < fallback.size(); ++i)
        {
            y.push_back(x.count(i)>0? x.at(i) : fallback[i]);
        }
        return y;
    }

    // returns a map that contains the first (key,value) pair having a key in a list of `attempts`
    template<typename T>
    std::map<int,T> first(const std::vector<std::map<int,T>>& attempts)
    {
        std::map<int,T> y;
        for (std::size_t j = 0; j < attempts.size(); ++j)
        {
            for (const auto& [i, xi] : attempts[j]) 
            {
                y.emplace(int(i),xi);
            }
        }
        return y;
    }

    // useful when deriving values using both vectors and maps as arguments, e.g. mapY = populate(f, mapA, map(vectorA), mapB)
    template<typename T>
    std::map<int,T> map(std::vector<T>& x)
    {
        std::map<int,T> y;
        for (std::size_t i = 0; i < x.size(); ++i)
        {
            y.emplace(int(i), x[i]);
        }
        return y;
    }

    template<typename Ty, typename Tx1, typename F>
    std::vector<Ty> derive(const F f, const std::vector<Tx1>& x1)
    {
        std::vector<Ty> y;
        for (std::size_t i = 0; i < x1.size(); ++i)
        {
            Tx1 x1i = x1[i];
            y.push_back(f(x1i));
        }
        return y;
    }

    template<typename Ty, typename Tx1, typename Tx2, typename F>
    std::vector<Ty> derive(const F f, const std::vector<Tx1>& x1, const std::vector<Tx2>& x2)
    {
        assert(x1.size() == x2.size());
        std::vector<Ty> y;
        for (std::size_t i = 0; i < x1.size(); ++i)
        {
            Tx1 x1i = x1[i];
            Tx2 x2i = x2[i];
            y.push_back(f(x1i,x2i));
        }
        return y;
    }

    template<typename Ty, typename F, typename Tx1, typename Tx2, typename Tx3>
    std::vector<Ty> derive(const F f, const std::vector<Tx1>& x1, const std::vector<Tx2>& x2, const std::vector<Tx3>& x3)
    {
        assert(x1.size() == x2.size() && x1.size() == x3.size());
        std::vector<Ty> y;
        for (std::size_t i = 0; i < x1.size(); ++i)
        {
            Tx1 x1i = x1[i];
            Tx2 x2i = x2[i];
            Tx3 x3i = x3[i];
            y.push_back(f(x1i,x2i,x3i));
        }
        return y;
    }

    template<typename Ty, typename F, typename Tx1, typename Tx2, typename Tx3, typename Tx4>
    std::vector<Ty> derive(const F f, const std::vector<Tx1>& x1, const std::vector<Tx2>& x2, const std::vector<Tx3>& x3, const std::vector<Tx4>& x4)
    {
        assert(x1.size() == x2.size() && x1.size() == x3.size() && x1.size() == x4.size());
        std::vector<Ty> y;
        for (std::size_t i = 0; i < x1.size(); ++i)
        {
            Tx1 x1i = x1[i];
            Tx2 x2i = x2[i];
            Tx3 x3i = x3[i];
            Tx4 x4i = x4[i];
            y.push_back(f(x1i,x2i,x3i,x4i));
        }
        return y;
    }

    template<typename Ty, typename F, typename Tx1, typename Tx2, typename Tx3, typename Tx4, typename Tx5>
    std::vector<Ty> derive(const F f, const std::vector<Tx1>& x1, const std::vector<Tx2>& x2, const std::vector<Tx3>& x3, const std::vector<Tx4>& x4, const std::vector<Tx5>& x5)
    {
        assert(x1.size() == x2.size() && x1.size() == x3.size() && x1.size() == x4.size() && x1.size() == x5.size());
        std::vector<Ty> y;
        for (std::size_t i = 0; i < x1.size(); ++i)
        {
            Tx1 x1i = x1[i];
            Tx2 x2i = x2[i];
            Tx3 x3i = x3[i];
            Tx4 x4i = x4[i];
            Tx5 x5i = x5[i];
            y.push_back(f(x1i,x2i,x3i,x4i,x5i));
        }
        return y;
    }

    template<typename Ty, typename Tx1, typename F>
    std::map<int,Ty> attempt(const F f, const std::map<int,Tx1>& x1)
    {
        std::map<int,Ty> y;
        for (const auto& [i, x1i] : x1) 
        {
            y.emplace(int(i), f(x1i));
        }
        return y;
    }

    template<typename Ty, typename Tx1, typename Tx2, typename F>
    std::map<int,Ty> attempt(const F f, const std::map<int,Tx1>& x1, const std::map<int,Tx2>& x2)
    {
        std::map<int,Ty> y;
        for (const auto& [i, x1i] : x1) 
        {
            if (x1.count(i)>0 && x2.count(i)>0)
            {
                Tx2 x2i = x2.at(i);
                y.emplace(int(i), f(x1i,x2i));
            }
        }
        return y;
    }

    template<typename Ty, typename F, typename Tx1, typename Tx2, typename Tx3>
    std::map<int,Ty> attempt(const F f, const std::map<int,Tx1>& x1, const std::map<int,Tx2>& x2, const std::vector<Tx3>& x3)
    {
        std::map<int,Ty> y;
        for (const auto& [i, x1i] : x1)
        {
            if (x1.count(i)>0 && x2.count(i)>0 && x3.count(i)>0)
            {
                Tx2 x2i = x2.at(i);
                Tx3 x3i = x3.at(i);
                y.emplace(int(i), f(x1i,x2i,x3i));
            }
        }
        return y;
    }

    template<typename Ty, typename F, typename Tx1, typename Tx2, typename Tx3, typename Tx4>
    std::map<int,Ty> attempt(const F f, const std::map<int,Tx1>& x1, const std::map<int,Tx2>& x2, const std::map<int,Tx3>& x3, const std::map<int,Tx4>& x4)
    {
        std::map<int,Ty> y;
        for (const auto& [i, x1i] : x1)
        {
            if (x1.count(i)>0 && x2.count(i)>0 && x3.count(i)>0 && x4.count(i)>0)
            {
                Tx2 x2i = x2.at(i);
                Tx3 x3i = x3.at(i);
                Tx4 x4i = x4.at(i);
                y.emplace(int(i), f(x1i,x2i,x3i,x4i));
            }
        }
        return y;
    }

    template<typename Ty, typename F, typename Tx1, typename Tx2, typename Tx3, typename Tx4, typename Tx5>
    std::map<int,Ty> attempt(const F f, const std::map<int,Tx1>& x1, const std::map<int,Tx2>& x2, const std::map<int,Tx3>& x3, const std::map<int,Tx4>& x4, const std::map<int,Tx5>& x5)
    {
        std::map<int,Ty> y;
        for (const auto& [i, x1i] : x1)
        {
            if (x1.count(i)>0 && x2.count(i)>0 && x3.count(i)>0 && x4.count(i)>0 && x5.count(i)>0)
            {
                Tx2 x2i = x2.at(i);
                Tx3 x3i = x3.at(i);
                Tx4 x4i = x4.at(i);
                Tx4 x5i = x5.at(i);
                y.emplace(int(i), f(x1i,x2i,x3i,x4i));
            }
        }
        return y;
    }
}}