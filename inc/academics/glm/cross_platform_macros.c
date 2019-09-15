// NOTE: these macros are here to allow porting the code between several languages

#ifdef CPP
#define vec2 glm::vec2
#define vec3 glm::vec3
#define vec4 glm::vec4
#define mat2 glm::mat2
#define mat3 glm::mat3
#define mat4 glm::mat4
#elif JS
#define vec2 glm.vec2
#define vec3 glm.vec3
#define vec4 glm.vec4
#define mat2 glm.mat2
#define mat3 glm.mat3
#define mat4 glm.mat4
#endif