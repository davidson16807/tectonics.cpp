
namespace body
{

	template<typename id, typename scalar, typename position>
	class Aggregate
	{

		const position position_sum;
		const scalar weight_sum;

		Aggregate(
			const position position_sum,
			const scalar weight_sum,
		): 
			position_sum(position_sum),
			weight_sum(weight_sum)
		{}

		// zero constuctor
		Aggregate(): 
			position_sum(0),
			weight_sum(0)
		{}

	    inline Aggregate<id,scalar,precision>& operator+=(const Aggregate<id,scalar,position>& other) noexcept
	    {
	    	position_sum += other.position_sum;
	    	weight_sum += other.weight_sum;
	        return *this;
	    }

	    inline Aggregate<id,scalar,precision>& operator-=(const Aggregate<id,scalar,position>& other) noexcept
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

