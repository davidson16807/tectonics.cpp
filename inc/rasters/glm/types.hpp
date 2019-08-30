#pragma once

#include "../types.hpp"

namespace rasters
{
	typedef traster<glm::vec<2, bool,         glm::defaultp>> bvec2raster;
	typedef traster<glm::vec<2, int,          glm::defaultp>> ivec2raster;
	typedef traster<glm::vec<2, unsigned int, glm::defaultp>> uvec2raster;
	typedef traster<glm::vec<2, float,        glm::defaultp>>  vec2raster;
	typedef traster<glm::vec<2, double,       glm::defaultp>> dvec2raster;

	typedef traster<glm::vec<3, bool,         glm::defaultp>> bvec3raster;
	typedef traster<glm::vec<3, int,          glm::defaultp>> ivec3raster;
	typedef traster<glm::vec<3, unsigned int, glm::defaultp>> uvec3raster;
	typedef traster<glm::vec<3, float,        glm::defaultp>>  vec3raster;
	typedef traster<glm::vec<3, double,       glm::defaultp>> dvec3raster;

	typedef traster<glm::vec<4, bool,         glm::defaultp>> bvec4raster;
	typedef traster<glm::vec<4, int,          glm::defaultp>> ivec4raster;
	typedef traster<glm::vec<4, unsigned int, glm::defaultp>> uvec4raster;
	typedef traster<glm::vec<4, float,        glm::defaultp>>  vec4raster;
	typedef traster<glm::vec<4, double,       glm::defaultp>> dvec4raster;
}