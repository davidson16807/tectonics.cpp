
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <signal.h>     // signal
#include <execinfo.h>   // backtrace
#include <unistd.h>     // STDERR_FILENO
#include <random>       // rngs
#include <chrono>       // high_resolution_clock

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
    uint id = std::max_element(
        face_midpoints.begin(), 
        face_midpoints.end(), 
        [x](glm::vec3 a, glm::vec3 b) -> bool {
            return glm::dot(glm::normalize(a), glm::normalize(x)) < glm::dot(glm::normalize(b), glm::normalize(x));
        }
    ) - face_midpoints.begin();
    // std::cout << "face: " << id << std::endl;
    uvec3 face = face_vertex_ids[id];
    vec3 a = vertex_positions[face.x];
    vec3 b = vertex_positions[face.y];
    vec3 c = vertex_positions[face.z];
    float a_dx, b_dx, c_dx, min_dx;
    for (uint n = 0; n < N; ++n)
    {
        id *= 3;
        a_dx = glm::distance((a),(x));
        b_dx = glm::distance((b),(x));
        c_dx = glm::distance((c),(x));
        // std::cout << n << " " << a_dx << " " << b_dx << " " << c_dx << std::endl;
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

uint get_vertex_id(
    const uint vertex_tree_id, 
    const uint N, 
    const uvec3s& face_vertex_ids,
    std::unordered_map<glm::uvec2, uint>& midpoints
) {
    float i = vertex_tree_id / pow(3.f,N);
    uint i_n = trunc(i);
    uvec3 face = face_vertex_ids[i_n];
    uint a = face.x;
    uint b = face.y;
    uint c = face.z;
    for (uint n = 0; n < N; ++n)
    {
        i -= i_n;
        i *= 3;
        i_n = trunc(i);
        if (i_n == 0)
        {
            b = midpoints[glm::uvec2(a, b)];
            c = midpoints[glm::uvec2(a, c)];
        } 
        else if (i_n == 1)
        {
            a = midpoints[glm::uvec2(b, a)];
            c = midpoints[glm::uvec2(b, c)];
        }
        else if (i_n == 2)
        {
            a = midpoints[glm::uvec2(c, a)];
            b = midpoints[glm::uvec2(c, b)];
        }
    }
    if      (i_n == 0)   { return a; } 
    else if (i_n == 1)   { return b; } 
    else  /*(i_n == 2)*/ { return c; } 
}
int main(int argc, char const *argv[])
{
    signal(SIGSEGV, handler);   // install our error handler
    signal(SIGABRT, handler);   // install our error handler

    std::chrono::_V2::system_clock::time_point t1, t2;

    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);

    t1 = std::chrono::high_resolution_clock::now();
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "mesh subdivision:    " << std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count() << std::endl;

    t1 = std::chrono::high_resolution_clock::now();
    SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "grid initialization: " << std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count() << std::endl;
    
    vec3s v = vec3s(icosphere.vertex_count);
    mult(glm::rotate(glm::mat4(1.f), glm::radians(30.f), glm::vec3(1.f)), icosphere_mesh.vertices, v);
    uints ids = uints(icosphere.vertex_count);

    t1 = std::chrono::high_resolution_clock::now();
    icosphere.get_values(v, ids);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "grid lookup:         " << std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count() << std::endl;
    
    std::mt19937 generator(2);
    floats a = floats(icosphere.vertex_count);

    t1 = std::chrono::high_resolution_clock::now();
    random(icosphere, generator, a, 10, 0.0001f);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "raster generation:   " << std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count() << std::endl;

    std::cout << to_string(icosphere, a) << std::endl;

    std::cout << "vertex count: " << icosphere.vertex_count << std::endl;

    // floats gradient_in = raster_c;
    // vec3s gradient_out = vec3s(icosphere.vertex_count);
    // std::cout << "calculating gradient" << std::endl;
    // rasters::gradient(icosphere, gradient_in, gradient_out);
    // std::cout << rasters::to_string(icosphere, gradient_out) << std::endl;

    // vec3s divergence_in = gradient_out;
    // floats divergence_out = floats(icosphere.vertex_count);
    // std::cout << "calculating divergence" << std::endl;
    // rasters::divergence(icosphere, divergence_in, divergence_out);
    // std::cout << rasters::to_string(icosphere, divergence_out) << std::endl;

    // vec3s curl_in = gradient_out;
    // vec3s curl_out = vec3s(icosphere.vertex_count);
    // std::cout << "calculating curl" << std::endl;
    // rasters::curl(icosphere, curl_in, curl_out);
    // std::cout << rasters::to_string(icosphere, curl_out) << std::endl;

    // floats laplacian_in = raster_c;
    // floats laplacian_out = floats(icosphere.vertex_count);
    // std::cout << "calculating laplacian" << std::endl;
    // rasters::laplacian(icosphere, laplacian_in, laplacian_out);
    // std::cout << rasters::to_string(icosphere, laplacian_out) << std::endl;

    // std::mt19937 randomizer;
    // std::cout << randomizer() << " " << randomizer() << std::endl;
    // std::cout << randomizer;


}