This folder contains test code for functionality that can only be demonstrated using 3d graphics. You can think of it as the "demoscene" folder. We would eventually like to present this functionality in a manner similar to tectonics.js using WebAssembly and WebGL. However, since the turnaround time needed to achieve that is untenable, we instead build small 3d demoscenes for each unit of work. 

We must pick a graphics library to render these demos. In order to do so, we make the following observations:

1.) Since the library is meant to be cross platform, we should limit ourselves to libraries that support cross platform development. 
2.) Since we know we must eventually support WebGL, the library should allow us to limit functionality to that used within the OpenGL 3.3 standard. 
3.) Since the whole purpose of creating demos is to minimize turnaround time, the graphics library should not be any indication of our choice for the final product. This is because any attempt to predict the library we use will effectively require us to decide now what that library is. This will ineviteably paralyze us, since we lack the information needed to make that decision, and the only good way to procure that information is from experimentation such as that provided by demos.
4.) Since we do want minimize turnaround time, the graphics library we use for demos should minimize setup and compile time: it should have a minimal footprint, minimal requirements, minimal scope, and minimal installation effort. 

Only a few mainstream C++ libraries appear to support the requirements for observations 1.) and 2.). These include FreeGLUT, Qt, SFML, SDL, and GLFW. Here is some relevant reading material comparing the few:

https://news.ycombinator.com/item?id=19677201
https://gamedev.stackexchange.com/questions/32595/freeglut-vs-sdl-vs-glfw

FreeGLUT does not appear to be popular due to its age. Qt is so large that I will dismiss it out of hand. That leaves SFML, SDL, and GLFW, which I have attempted to order from largest to smallest. Since I prefer lightweight solutions, I choose GLFW. GLFW does not have any support for audio, but that is a non issue to me, since I do not intend to use audio for demos. If I choose to continue with GLFW when developing a final product, I may likely prefer a non-bundled audio solution, anyways. 
