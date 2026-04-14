

namespace field
{

	template<typename id, typename scalar, typename position>
	class Monopole
	{

		const position position_sum;
		const scalar weight_sum;

		Monopole(
			const position position_sum,
			const scalar weight_sum,
		): 
			position_sum(position_sum),
			weight_sum(weight_sum)
		{}

		// zero constuctor
		Monopole(): 
			position_sum(0),
			weight_sum(0)
		{}

	    inline Monopole<id,scalar,precision>& operator+=(const Monopole<id,scalar,position>& other) noexcept
	    {
	    	position_sum += other.position_sum;
	    	weight_sum += other.weight_sum;
	        return *this;
	    }

	    inline Monopole<id,scalar,precision>& operator-=(const Monopole<id,scalar,position>& other) noexcept
	    {
	    	position_sum -= other.position_sum;
	    	weight_sum -= other.weight_sum;
	        return *this;
	    }

	    inline position center() const
	    {
	    	return position_sum/weight_sum;
	    }

	};

}
