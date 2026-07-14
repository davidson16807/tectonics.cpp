"field" contains classes of callable objects with closed form overloads of `operator()` with the form ℝ²→𝕋 and ℝ³→𝕋 for some arbitrary type 𝕋.
It is meant to represent the concept of a "field" in the context of physics, which continuously maps each point in a space with a value. 

"field" uses `glm` as a requirement for all functionality.

"field" contains the following folders, which are organized by functionality:

* **harmonics** for fields that represent harmonics, such as spherical harmonics, discrete cosine transforms, etc.
* **noise** for fields that represent procedural noise, such as Perlin noise, Worley Noise, etc.
* **poles** for fields that represent mathematical poles, such as those used to represent light, gravity, inverse distance weighting, etc.