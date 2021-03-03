#pragma once


// std libraries
#include <memory>

// 3rd party libraries

// in-house libraries
#include <rasters/components/MeshCache/MeshCache.hpp>
#include <rasters/components/LayerScheme/LayerScheme.hpp>
#include <rasters/entities/Grid/Grid.hpp>

namespace rasters
{
	class AbstractLayeredGrid {};

	/* 
	NOTE: we only use inheritence to copy shared pointers within the base class.
	We do not use inheritence to allow use of derived classes in function signatures, 
	preferring function templates instead. This is why our inheritance is marked private,
	and it also explains why we avoid representing other potential inheritence relationships.
	*/
    template<typename Tid=std::uint16_t, typename Tfloat=float>
	struct LayeredGrid: public Grid<Tid, Tfloat>, public AbstractLayeredGrid
	{
		std::shared_ptr<LayerScheme> layering;

		LayeredGrid(
			const Grid<Tid, Tfloat>& base, 
			const float profile_top_height, 
			const float profile_bottom_height,
			const std::size_t layer_count
		):
			Grid<Tid, Tfloat>(base),
			layering(std::make_shared<LayerScheme>(profile_top_height, profile_bottom_height, layer_count))
		{
			assert(this->layering->layer_count < this->cache->vertex_count);
		}
		const std::size_t cell_count(mapping mapping_type) const
		{
			return this->layering->layer_count * (mapping_type == mapping::cell? this->cache->vertex_count : this->cache->arrow_count);
		}
		const std::size_t column_count(mapping mapping_type) const
		{
			return mapping_type == mapping::cell? this->cache->vertex_count : this->cache->arrow_count;
		}
		bool operator== (const LayeredGrid<Tid,Tfloat>& other) const
		{
		    return (this->Grid<Tid, Tfloat>::operator==(other) &&
		    	    layering == other.layering);
		}
		bool operator!= (const LayeredGrid<Tid,Tfloat>& other) const
		{
		    return !((*this)==other);
		}
	};


	
	
}