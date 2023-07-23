#pragma once


// std libraries
#include <memory>

// 3rd party libraries
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <series/types.hpp>
#include <series/glm/types.hpp>

#include <rasters/components/SpheroidVoronoi/SpheroidVoronoi.hpp>

#include <rasters/entities/Grid/Grid.hpp>

#include <rasters/entities/Grid/Grid_test_utils.hpp>

namespace rasters
{
	/*
	NOTE: any time you have a mesh value (i.e. a tuple of vertices and faces), 
	it is either definitely a spheroid or it is not,
	and there is very little reason after the fact to change your mind.
	Therefore, we initialize SpheroidGrid directly from the mesh, 
	rather than pass a Grid in its constructor. 
	*/
    template<typename Tid=std::uint16_t, typename Tfloat=float>
	struct SpheroidGrid: public Grid<Tid, Tfloat>
	{
		std::shared_ptr<SpheroidVoronoi> voronoi;

		SpheroidGrid(
			const series::Series<glm::vec<3,Tfloat,glm::defaultp>>& vertices, 
			const series::Series<glm::vec<3,Tid, glm::defaultp>>& faces
		):
			Grid<Tid, Tfloat>(vertices, faces),
			voronoi(std::make_shared< SpheroidVoronoi>(
				vertices, 
				series::min(this->metrics->arrow_lengths / 8.f), 
				series::max(this->metrics->arrow_lengths * 1.f)
			))
		{}
		bool operator== (const SpheroidGrid& other) const
		{
		    return (this->structure == other.structure && 
		    	    this->metrics == other.metrics && 
		    	    voronoi == other.voronoi);
		}
		bool operator!= (const SpheroidGrid& other) const
		{
		    return !((*this)==other);
		}
	};
}