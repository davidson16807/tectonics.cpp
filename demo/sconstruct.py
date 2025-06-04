import os 
import shutil
import glob

cwd = GetLaunchDir()
demo = os.getcwd()
root = os.path.join(demo, os.pardir)
testexe = os.path.join(cwd, 'tests')
testcpp = os.path.join(cwd, 'test.cpp')

with open(testcpp, 'w') as file:
    includes = [
        *glob.glob(os.path.join(cwd, '**/*_specialization.hpp'), recursive=True),
        *glob.glob(os.path.join(cwd, '**/*_test.*pp'), recursive=True),
    ]
    for include in includes:
        localized = include.replace(cwd, '.')
        file.write(f'#include "{localized}"\n')

test = Environment()
is_msvc = test['CC']=='cl'

print(
f'''
Building executable, "tests", under "{cwd}"
using the {test['CC']} compiler
to build a newly created test.cpp file under "{cwd}"
that includes only *_test.cpp files under "{cwd}"
using sconstruct.py under "{demo}".
''')

if is_msvc:
    shutil.copy2(os.path.join(root,*'lib/glew/bin/Release/x64/glew32.dll'.split('/')), cwd)
    shutil.copy2(os.path.join(root,*'lib/glfw/lib-vc2022/glfw3.dll'.split('/')), cwd)

environment = Environment(
    CCFLAGS=(
        ("/std:c++20 /O2 /openmp:llvm /D GLM_FORCE_SWIZZLE" if is_msvc else 
         "-std=c++20 -O2 -Wall -Werror -pedantic-errors -rdynamic -g -D GLM_FORCE_SWIZZLE").split()),
    CPPPATH=[
        *[os.path.join(root,path) for path in 'inc lib src sketch'.split()],
        *([] if not is_msvc else
          [
            os.path.join(root,*'lib/glew/include'.split('/'),''), 
            os.path.join(root,*'lib/glfw/include'.split('/'),'')
          ])
    ],
    # CPPDEFINES=['GLFW_INCLUDE_ES2'],
)

executable = environment.Program(
    target=testexe, source=testcpp,
    LIBPATH=[
        os.path.join(root,*'lib/glew/lib/Release/x64'.split('/'),''),
        os.path.join(root,*'lib/glfw/lib-vc2022'.split('/'),'')
    ],
    LIBS=[
        'glew32 glfw3dll opengl32 gdi32'.split() if is_msvc else
        'glfw GL GLEW'.split()
    ]
)

# works on some linux distros, but doesn't work on Ubuntu
# environment.AddPostAction(executable, testexe.replace(cwd,'.'))

