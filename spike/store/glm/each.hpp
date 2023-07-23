
namespace series
{

	template <glm::length_t L, typename T, glm::qualifier Q>
	void set_x(const Series<glm::vec<L,T,Q>>& a, const Series<T>& values, Series<glm::vec<L,T,Q>>& output )
	{
		if (&a != &output){ series::copy(output, a); }
		for (unsigned int i = 0; i < output.size(); ++i)
		{
			output[i].x = values[i];
		}
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void set_y(const Series<glm::vec<L,T,Q>>& a, const Series<T>& values, Series<glm::vec<L,T,Q>>& output )
	{
		if (&a != &output){ series::copy(output, a); }
		for (unsigned int i = 0; i < output.size(); ++i)
		{
			output[i].y = values[i];
		}
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void set_z(const Series<glm::vec<L,T,Q>>& a, const Series<T>& values, Series<glm::vec<L,T,Q>>& output )
	{
		if (&a != &output){ series::copy(output, a); }
		for (unsigned int i = 0; i < output.size(); ++i)
		{
			output[i].z = values[i];
		}
	}

}