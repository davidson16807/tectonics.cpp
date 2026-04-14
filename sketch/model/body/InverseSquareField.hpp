

namespace body
{

	/*
	`InverseSquareField` represents a mathematical field of arbitrary dimensionality
	where the value at each point is the weighted sum of the inverse square of distances to particles.
	It is an implementation of the Barnes-Hut algorithm.
	`InverseSquareField` can be used to efficiently implement inverse distance weighting, 
	gravity simulations, charged particle simulations, light propagation, boids, etc.
	In inverse distance weighting, the weight associated with a sample is the measurement taken for the sample.
	In gravity simulation, the weight associated with a body is its standard gravitational parameter.
	In charged particle simulation, the weight associated with a particle is its charge times the coulomb constant.
	In light propagation, the weight associated with a light source is its luminosity.
	*/
	template<int dimension_count, typename id, typename scalar, glm::qualifier quality = glm::defaultp>
	class InverseSquareField
	{

		using vector = glm::vec<dimension_count, scalar, quality>;
		using ivector = glm::vec<dimension_count, id, quality>;

		const OrthantIndexing<dimension_count, id, quality> orthants;
		const vector grid_origin;
		const scalar grid_width;
		const id level_count;
		const id orthant_count;
		const id cell_count;
		std::vector<Aggregate<id,scalar,vector>> orthtree;

	public:

		InverseSquareField(
			const vector grid_origin,
			const scalar grid_width,
			const scalar min_cell_width
		):
			orthants(),
			grid_origin(grid_origin),
			grid_width(grid_width),
			level_count(std::ceil(std::log2(grid_width/min_cell_width))), 
			orthant_count(std::pow(id(2),dimension_count)), 
			cell_count(std::pow(orthant_count,level_count+1)),
			orthtree(cell_count, Aggregate<id,scalar,vector>())
		{}

		void clear()
		{
			std::fill(orthtree.begin(), orthtree.end(), Aggregate<id,scalar,vector>());
		}

		/*
		`add` modifies the field to also consider a particle of given `position` and `weight`
		*/
		void add(const vector position, const scalar weight)
		{
			id memory_id(0);
			id neighbor_id, orthant_id;
			vector cell_center(grid_origin);
			scalar cell_width(grid_width);
			ivector orthant;
			for (int level_id = 1; level_id < level_count; ++level_id)
			{
				orthant = glm::greaterThan(position-cell_center,vector(0));
				orthant_id = orthants.memory_id(orthant);
				// add contributions to each orthant of the current level that do not contain the particle
				for (neighbor_id = 0; neighbor_id < orthant_count; ++neighbor_id)
				{
					if (neighbor_id != orthant_id)
					{
						orthtree[orthant_count * memory_id + neighbor_id] += Aggregate<id,scalar,vector>(position, weight);
					}
				}
				memory_id = orthant_count * memory_id + orthant_id;
				cell_center += cell_width * (vector(orthant)-vector(0.5));
				cell_width /= scalar(2);
			}
		}

		/*
		`sample` returns the value acting at a given `position_` in the field
		*/
		scalar sample(const vector position) const
		{
			id memory_id(0);
			id orthant_id;
			Aggregate<id,scalar,vector> aggregate;
			vector cell_center(grid_origin);
			scalar cell_width(grid_width);
			scalar value(0);
			ivector orthant;
			for (int level_id = 1; level_id < level_count; ++level_id)
			{
				orthant = glm::greaterThan(position-cell_center,vector(0));
				orthant_id = orthants.memory_id(orthant);
				aggregate = orthtree[memory_id];
				value += aggregate.factor_sum / glm::distance2(aggregate.center(), position);
				memory_id = orthant_count * memory_id + orthant_id;
				cell_center += cell_width * (vector(orthant)-vector(0.5));
				cell_width /= scalar(2);
			}
			return value;
		}

	};

}
