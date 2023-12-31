﻿file(GLOB_RECURSE RENDERER_FILES 
	CONFIGURE_DEPENDS
	Renderer/*.h
	Renderer/*.cpp)

file(GLOB_RECURSE UI_FILES 
	CONFIGURE_DEPENDS
	UI/*.h
	UI/*.cpp)

file(GLOB_RECURSE BODY_FILES
	CONFIGURE_DEPENDS
	Body/*.h
	Body/*.cpp)

file(GLOB_RECURSE SIMULATION_FILES
	CONFIGURE_DEPENDS
	Simulation/*.h
	Simulation/*.cpp)

file(GLOB_RECURSE SHADER_FILES
	CONFIGURE_DEPENDS
	*.glsl
	*.frag
	*.vert)

file(GLOB_RECURSE 3D_FILES
	CONFIGURE_DEPENDS
	*.obj)

file(GLOB_RECURSE 3RD_PARTY
	CONFIGURE_DEPENDS
	A3rdParty/*.h
	A3rdParty/*.cpp)

file(GLOB_RECURSE SUB_APPS
	CONFIGURE_DEPENDS
	SubApp/*.h
	SubApp/*.cpp)

file(GLOB_RECURSE JSON
	CONFIGURE_DEPENDS
	JSON/*.h
	JSON/*.cpp)

add_executable (Pyewton "main.cpp" "Pyewton.cpp" "Pyewton.h" "Files.h" "Files.cpp" "AppData.h" "AppControllers.h" ${BODY_FILES} ${RENDERER_FILES} ${SIMULATION_FILES} ${UI_FILES} ${3RD_PARTY} ${SUB_APPS} ${JSON} )

set_property(TARGET Pyewton PROPERTY CXX_STANDARD 20)

set(shader_dir ${CMAKE_CURRENT_BINARY_DIR}/shader_dir)
set(3dfiles_dir ${CMAKE_CURRENT_BINARY_DIR}/3dfiles)

function(copy_files files_in dir_out)
	foreach( file ${files_in})
		get_filename_component(file_name ${file} NAME)
		message( "Copying " ${file} " to " ${dir_out}/${file_name} "...")
		add_custom_command(
			TARGET Pyewton
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${file} ${dir_out}/${file_name})
		message(done)
	endforeach( file )
endfunction()

copy_files("${SHADER_FILES}" ${shader_dir})
copy_files("${3D_FILES}" ${3dfiles_dir})


find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(folly CONFIG REQUIRED)


target_link_libraries(Pyewton PRIVATE imgui::imgui glad::glad glfw assimp::assimp JsonCpp::JsonCpp Folly::folly)
