echo "inc:" `find ../inc/ -name '*.*pp' ! -name "*_test.*pp" -exec cat {} \; | wc -l`
echo "src:" `find ../src/ -name '*.*pp' ! -name "*_test.*pp" -exec cat {} \; | wc -l`
echo "spike:" `find ../spike/ -name '*.*pp' ! -name "*_test.*pp" -exec cat {} \; | wc -l`
echo "glsl:" `find ../../tectonics.glsl/ -type f -name '*.glsl' ! -name "*_test.*pp" -exec cat {} \; | wc -l`
echo "js precompiled:" `find ../../tectonics.js/precompiled -name '*.js' ! -name "*_test.*pp" -exec cat {} \; | wc -l`
echo "js noncompiled:" `find ../../tectonics.js/noncompiled -name '*.js' ! -name "*_test.*pp" -exec cat {} \; | wc -l`
echo "tools:" `find ../../glsl_tools/ -name '*.py' ! -name "*_test.*pp" -exec cat {} \; | wc -l`
