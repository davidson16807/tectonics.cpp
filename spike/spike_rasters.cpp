
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <signal.h>     // signal
#include <execinfo.h>   // backtrace
#include <unistd.h>     // STDERR_FILENO
#include <random>       // rngs

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/string_cast.hpp>  
#include <many/morphologic.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/string_cast.hpp>  
#include <many/glm/convenience.hpp> //  operators, etc.

#include <rasters/mesh.hpp>
#include <rasters/string_cast.hpp>  
#include <rasters/random.hpp>  
#include <rasters/morphologic.hpp>
#include <rasters/glm/glm.hpp>
#include <rasters/glm/string_cast.hpp>  
#include <rasters/glm/vector_calculus.hpp>

// #include "academics/tectonics.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

void handler(int sig) {
      void *array[10];
      size_t size;

      // get void*'s for all entries on the stack
      size = backtrace(array, 10);

      // print out all the frames to stderr
      fprintf(stderr, "Error: signal %d:\n", sig);
      backtrace_symbols_fd(array, size, STDERR_FILENO);
      exit(1);
}

uint get_vertex_tree_id(
    const vec3 x, 
    const uint N, 
    const vec3s&  face_midpoints, 
    const uvec3s& face_vertex_ids,
    const vec3s&  vertex_positions
) {
    uint id = std::min_element(
        face_midpoints.begin(), 
        face_midpoints.end(), 
        [x](glm::vec3 a, glm::vec3 b) -> bool {
            return glm::distance(a, x) < glm::distance(b, x);
        }
    ) - face_midpoints.begin();
    uvec3 face = face_vertex_ids[id];
    vec3 a = vertex_positions[face.x];
    vec3 b = vertex_positions[face.y];
    vec3 c = vertex_positions[face.z];
    float a_dx, b_dx, c_dx, min_dx;
    for (uint n = 0; n < N; ++n)
    {
        id *= 3;
        a_dx = glm::distance(a,x);
        b_dx = glm::distance(b,x);
        c_dx = glm::distance(c,x);
        min_dx = min(a_dx, min(b_dx, c_dx));
        if (a_dx == min_dx)
        {
            b = glm::normalize((a+b)/2.f);
            c = glm::normalize((a+c)/2.f);
            id += 0;
        } 
        else if (b_dx == min_dx)
        {
            a = glm::normalize((b+a)/2.f);
            c = glm::normalize((b+c)/2.f);
            id += 1;
        }
        else if (c_dx == min_dx)
        {
            a = glm::normalize((c+a)/2.f);
            b = glm::normalize((c+b)/2.f);
            id += 2;
        }
    }
    return id;
}

int main(int argc, char const *argv[])
{
    signal(SIGSEGV, handler);   // install our error handler
    signal(SIGABRT, handler);   // install our error handler

    /* 
    "diamond" is a simple 2d grid for testing raster operations 
     that do not require spatial awareness (e.g. arithmetic on scalar fields)
    It looks like this:
         2  
        /|\ 
       3-0-1
        \|/ 
         4   
    */
    Grid diamond = 
        Grid(
            vec3s({
                    vec3( 0, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 0, 1, 0),
                    vec3(-1, 0, 0),
                    vec3( 0,-1, 0)
                }),
            uvec3s({
                    uvec3(0,1,2),
                    uvec3(0,1,4),
                    uvec3(0,3,2),
                    uvec3(0,3,4)
                })
        );
    /*
    "tetrahedron" is a simple 3d grid for testing basic raster operations
     that require spatial awareness without requiring a particular shape.
     (e.g. gradient, divergence, laplacian)
    */
    std::shared_ptr<Grid> tetrahedron = 
        std::make_shared<Grid>(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    /*
    "octahedron" is a simple 3d grid for testing raster operations that require 
    something comparable to a unit sphere (e.g. nearest neighbor lookups using SpheroidGridVoronoi)
    */
    SpheroidGrid octahedron = 
        SpheroidGrid(meshes::octahedron.vertices, meshes::octahedron.faces);

    /*
    "icosahedron" is a simple 3d grid for testing rasters with a large number of vertices
    */
    SpheroidGrid icosahedron = 
        SpheroidGrid(meshes::icosahedron.vertices, meshes::icosahedron.faces);

    SpheroidGridVoronoi voronoi_test(
            vec3s({
                    normalize(vec3( 1, 0, 0)),
                    normalize(vec3( 0, 1, 0)),
                    normalize(vec3( 0, 0, 1)),
                    normalize(vec3(-1, 0, 0)),
                    normalize(vec3( 0,-1, 0)),
                    normalize(vec3( 0, 0,-1)),
                    normalize(vec3(-1,-1,-1)),
                    normalize(vec3( 1,-1,-1)),
                    normalize(vec3(-1, 1,-1)),
                    normalize(vec3( 1, 1,-1)),
                    normalize(vec3(-1,-1, 1)),
                    normalize(vec3( 1,-1, 1)),
                    normalize(vec3(-1, 1, 1)),
                    normalize(vec3( 1, 1, 1)),
                    normalize(vec3( 1, 1, 1))
                }),
            1./100.,
            3./100.
        );

    floats a = floats({1,2,3,4,5});
    floats b = floats({1,1,2,3,5});
    floats c = floats({4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                       4,2,5,INFINITY,9,4,6,2,8,1,5,3,7,5,8,5,6,6,7,6,
                       1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                       4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7,5,
                       4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                       4,2,5,9,9,9,9,9,9,9,9,9,9,9,8,5,6,6,7,6,
                       1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                       4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7});

    std::string stra = to_string(a);
    std::string strb = to_string(b);
    std::string strc = to_string(c);
    std::cout << strc << std::endl;

    vec2s v2d = vec2s({
        vec2(-1,-1),
        vec2(-1, 1),
        vec2(-1,-0.1),
        vec2(-1, 0.1),
        vec2(-1, 0.0),
    });
    vec3s v3d = vec3s({
        vec3(-1,-1,-1),
        vec3( 0,-1,-1),
        vec3( 1,-1,-1),
        vec3(-1, 0,-1),
        vec3( 0, 0,-1),
        vec3( 1, 0,-1),
        vec3(-1, 1,-1),
        vec3( 0, 1,-1),
        vec3( 1, 1,-1),

        vec3(-1,-1, 0),
        vec3( 0,-1, 0),
        vec3( 1,-1, 0),
        vec3(-1, 0, 0),
        vec3( 0, 0, 0),
        vec3( 1, 0, 0),
        vec3(-1, 1, 0),
        vec3( 0, 1, 0),
        vec3( 1, 1, 0),

        vec3(-1,-1, 1),
        vec3( 0,-1, 1),
        vec3( 1,-1, 1),
        vec3(-1, 0, 1),
        vec3( 0, 0, 1),
        vec3( 1, 0, 1),
        vec3(-1, 1, 1),
        vec3( 0, 1, 1),
        vec3( 1, 1, 1),
    });
    std::string str_v2d = to_string(v2d);
    std::string str_v3d = to_string(v3d, glm::mat3x2(1,1,0, 
                                                     0,1,1));
    std::cout << str_v2d << std::endl;
    std::cout << str_v3d << std::endl;

    floats raster_a = floats({0,1,2,3,4,5});
    std::string str_raster_a = to_string(octahedron, raster_a);
    std::cout << str_raster_a << std::endl;

    vec2s raster_v2 = vec2s({
        vec2( 0,-1),
        vec2( 0, 1),
        vec2(-1, 0),
        vec2( 1, 0),
        vec2(-1,-1),
        vec2( 1, 1),
    });
    vec3s raster_v3 = vec3s({
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
        vec3( 0, 1, 0),
    });

    std::string str_raster_v2 = rasters::to_string(octahedron, raster_v2);
    std::cout << str_raster_v2 << std::endl;

    std::string str_raster_v3 = rasters::to_string(octahedron, raster_v3);
    std::cout << str_raster_v3 << std::endl;

    std::mt19937 generator(2);
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    std::unordered_map<glm::uvec2, unsigned int> midpoints {};
    icosphere_mesh = meshes::subdivide(icosphere_mesh, midpoints); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

    std::cout << "starting" << std::endl;
    for (int i = 0; i < 40000; ++i)
    {
        get_vertex_tree_id(
            normalize(vec3(1,1,1)),     // x
            100,                          // N
            icosphere.face_midpoints,   // 
            icosphere.face_vertex_ids,  // 
            icosphere.vertex_positions  //
        );
    }
    std::cout << "ending" << std::endl;

    std::cout << icosphere_mesh.vertices.size() << std::endl;
    floats raster_b = floats(icosphere_mesh.vertices.size());
    random(icosphere, generator, raster_b, 10, 0.0001f);
    std::string str_raster_b = to_string(icosphere, raster_b);
    std::cout << str_raster_b << std::endl;

    // floats raster_c = floats(icosphere_mesh.vertices.size());
    // random(icosphere, generator, raster_c);
    // std::string str_raster_c = to_string(icosphere, raster_c);
    // std::cout << str_raster_c << std::endl;

    // floats gradient_in = raster_c;
    // vec3s gradient_out = vec3s(icosphere_mesh.vertices.size());
    // std::cout << "calculating gradient" << std::endl;
    // rasters::gradient(icosphere, gradient_in, gradient_out);
    // std::cout << rasters::to_string(icosphere, gradient_out) << std::endl;

    // std::mt19937 randomizer;
    // std::cout << randomizer() << " " << randomizer() << std::endl;
    // std::cout << randomizer;


}