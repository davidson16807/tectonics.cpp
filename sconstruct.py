import glob
import os 
cwd = GetLaunchDir()
root = os.getcwd()
testexe = os.path.join(cwd, 'tests')
testcpp = os.path.join(cwd, 'test.cpp')

with open(testcpp, 'w') as file:
    includes = [
        *glob.glob(os.path.join(cwd, '**/*_specialization.hpp'), recursive=True),
        *glob.glob(os.path.join(cwd, '**/*_test.cpp'), recursive=True),
    ]
    for include in includes:
        localized = include.replace(cwd, '.')
        file.write(f'#include "{localized}"\n')

print(
f'''
Building executable, "tests", under "{cwd}"
from newly created test.cpp file under "{cwd}"
that includes only *_test.cpp files under "{cwd}"
using sconstruct.py under "{root}".
''')

environment = Environment(
    CCFLAGS="-std=c++17 -Wall -Werror -pedantic-errors -rdynamic -g -D GLM_FORCE_SWIZZLE",
    CPPPATH=[os.path.join(root,path) for path in 'inc lib src sketch'.split()],
    LIBS='glfw GL GLEW'.split(),
)

executable = environment.Program(target=testexe, source=testcpp)

# works on some linux distros, but doesn't work on Ubuntu
# environment.AddPostAction(executable, testexe.replace(cwd,'.'))
