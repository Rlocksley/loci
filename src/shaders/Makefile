
Shaders:
	./glslc --target-env=vulkan1.3 chitReflectionShader.rchit -o chitReflectionShader.spv
	./glslc --target-env=vulkan1.3 chitShader.rchit -o chitShader.spv
	./glslc --target-env=vulkan1.3 chitShadowShader.rchit -o chitShadowShader.spv
	
	./glslc --target-env=vulkan1.3 genShader.rgen -o genShader.spv

	./glslc --target-env=vulkan1.3 missReflectionShader.rmiss -o missReflectionShader.spv
	./glslc --target-env=vulkan1.3 missShader.rmiss -o missShader.spv
	./glslc --target-env=vulkan1.3 missShadowShader.rmiss -o missShadowShader.spv


.PHONY:	loci_compile_shaders

loci_compile_shaders: Shaders