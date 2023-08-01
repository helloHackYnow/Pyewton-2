add_executable(renderer "test.cpp" "test.h" "Renderer.cpp" "Renderer.h" "Shader/Shader.h" "Shader/Shader.cpp" "Mesh.h" "Mesh.cpp" "Model.h" "Model.cpp" "Camera.h" "stb_image.h")


set(shaders_files "fragment.glsl" "vertex.glsl" "grid_vs.glsl" "grid_fs.glsl")

set(models "rock.obj" "rock.mtl" "rock.png") 

foreach( shader ${shaders_files})
	add_custom_command(
	TARGET renderer
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy C:/Dev/projects/Pyewton/Pyewton/Renderer/${shader} ${CMAKE_CURRENT_BINARY_DIR}/${shader})
	message("Copy of " ${shader} " completed")
endforeach( shader )

foreach( model ${models})
	add_custom_command(
	TARGET renderer
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy C:/Dev/projects/Pyewton/Pyewton/Renderer/${model} ${CMAKE_CURRENT_BINARY_DIR}/${model})
	message("Copy of " ${model} " completed")
endforeach( model )

find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(renderer PRIVATE glm::glm glfw glad::glad assimp::assimp)