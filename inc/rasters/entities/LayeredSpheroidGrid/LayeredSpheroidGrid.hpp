#pragma once


// std libraries
#include <memory>

// 3rd party libraries
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <series/types.hpp>
#include <series/glm/types.hpp>

#include <rasters/components/SpheroidVoronoi/SpheroidVoronoi.hpp>
#include <rasters/components/LayerScheme/LayerScheme.hpp>

#include <rasters/entities/LayeredGrid/LayeredGrid.hpp>
#include <rasters/entities/SpheroidGrid/SpheroidGrid.hpp>

namespace rasters
{
	/* 
	NOTE: we only use inheritence to copy shared pointers within the base class.
	We do not use inheritence to allow use of derived classes in function signatures, 
	preferring function templates instead. This is why our inheritance is marked private,
	and it also explains why we avoid representing other potential inheritence relationships.
	*/
    template<typename Tid=std::uint16_t, typename Tfloat=float>
	struct LayeredSpheroidGrid: public SpheroidGrid<Tid, Tfloat>
	{
		std::shared_ptr<LayerScheme> layering;

		LayeredSpheroidGrid(
			const SpheroidGrid<Tid, Tfloat>& base, 
			const float profile_top_height, 
			const float profile_bottom_height, 
			const float layer_count
		):
			SpheroidGrid<Tid, Tfloat>(base),
			layering(std::make_shared<LayerScheme>(profile_top_height, profile_bottom_height, layer_count))
		{}
		const std::size_t cell_count(mapping mapping_type) const
		{
			return this->layering->layer_count * (mapping_type == mapping::cell? this->structure->vertex_count : this->structure->arrow_count);
		}
		const std::size_t column_count(mapping mapping_type) const
		{
			return mapping_type == mapping::cell? this->structure->vertex_count : this->structure->arrow_count;
		}
		bool operator== (const LayeredSpheroidGrid& other) const
		{
		    return (this->SpheroidGrid<Tid, Tfloat>::operator==(other) &&
		    	    layering == other.layering);
		}
		bool operator!= (const LayeredSpheroidGrid& other) const
		{
		    return !((*this)==other);
		}
	};
}