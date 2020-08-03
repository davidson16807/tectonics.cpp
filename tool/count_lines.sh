
echo "inc:" `find ../inc/ -name '*.*pp' -exec cat {} \; | wc -l`
echo "src:" `find ../src/ -name '*.*pp' -exec cat {} \; | wc -l`
echo "spike:" `find ../spike/ -name '*.*pp' -exec cat {} \; | wc -l`
echo "glsl:" `find ../../tectonics.glsl/ -name '*.glsl' -exec cat {} \; | wc -l`
echo "js precompiled:" `find ../../tectonics.js/precompiled -name '*.js' -exec cat {} \; | wc -l`
echo "js noncompiled:" `find ../../tectonics.js/noncompiled -name '*.js' -exec cat {} \; | wc -l`
echo "tools:" `find ../../glsl_tools/ -name '*.py' -exec cat {} \; | wc -l`
