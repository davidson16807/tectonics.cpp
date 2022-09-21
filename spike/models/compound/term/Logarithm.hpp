
namespace compound {
namespace term {

    struct Logarithm {
        float weight;
        constexpr explicit Logarithm(const float weight):
            weight(weight)
        {}
        // zero constructor
        constexpr explicit Logarithm():
            weight(0.0f)
        {}
        constexpr Logarithm(const Logarithm& f):
            weight(f.weight)
        {}
        constexpr float operator()(const float x) const
        {
            return weight*std::log(x);
        }
        constexpr Logarithm& operator*=(const float k)
        {
            weight *= k;
            return *this;
        }
        constexpr Logarithm& operator/=(const float k)
        {
            weight /= k;
            return *this;
        }
    };

    constexpr Logarithm operator*(const Logarithm f, const float k)
    {
        return Logarithm(f.weight*k);
    }
    constexpr Logarithm operator*(const float k, const Logarithm f)
    {
        return Logarithm(f.weight*k);
    }
    constexpr Logarithm operator/(const Logarithm f, const float k)
    {
        return Logarithm(f.weight/k);
    }

}}
