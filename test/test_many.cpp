

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec2.hpp>               // *vec3
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
// #include <many/string_cast.hpp>  
// #include <many/convenience.hpp>  
// #include <many/glm/glm.hpp>         // *vec*s
// #include <many/glm/string_cast.hpp>  
// #include <many/glm/convenience.hpp> //  operators, etc.

using namespace glm;
using namespace many;
