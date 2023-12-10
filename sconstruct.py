import glob
import os 
cwd = GetLaunchDir()
root = os.getcwd()

print(
f'''
Building executable, "test", under "{cwd}"
using *_test.cpp files under "{cwd}"
and sconstruct.py under "{root}".
'''
)

env = Environment(
	CCFLAGS="-std=c++17 -Wall -Werror -pedantic-errors -rdynamic -g -D GLM_FORCE_SWIZZLE",
	CPPPATH=[os.path.join(root,path) for path in 'inc lib src spike'.split()],
	LIBS='glfw GL GLEW'.split(),
)

env.Program(target=os.path.join(cwd, 'test'), source=glob.glob(os.path.join(cwd, "*_test.hpp")))
env.AddPostAction(executable, './test')
