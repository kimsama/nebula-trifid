#-------------------------------------------------------------------------------
# ExtLibs
#-------------------------------------------------------------------------------


IF(NOT N_BUILD_FIRSTTIME)


    IF(MSVC)
        add_subdirectory(libjpeg)
        add_subdirectory(libpng) 
        add_subdirectory(libfreetype) 
    ENDIF()
	
	IF(N_BUILD_NVTT)
		add_subdirectory(nvidia-texture-tools)
	ENDIF()

    add_subdirectory(lua) 
	
    add_subdirectory(recastnavigation/cmake)	
		SET_TARGET_PROPERTIES(DebugUtils PROPERTIES FOLDER "extlibs/recast")
		SET_TARGET_PROPERTIES(Detour PROPERTIES FOLDER "extlibs/recast")
		SET_TARGET_PROPERTIES(DetourCrowd PROPERTIES FOLDER "extlibs/recast")
		SET_TARGET_PROPERTIES(DetourTileCache PROPERTIES FOLDER "extlibs/recast")
		SET_TARGET_PROPERTIES(Recast PROPERTIES FOLDER "extlibs/recast")
		
    add_subdirectory(bullet) 
    add_subdirectory(tinyxml) 
    add_subdirectory(sqlite3) 
    add_subdirectory(zlib) 
    add_subdirectory(imgui)
	add_subdirectory(CubeMapGen)
	add_subdirectory(stackwalker)
	
	#DevIL
	set(LIBJPEG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libjpeg)
	#set(NVTT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nvidia-texture-tools)
	set(LIBPNG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libpng)
	add_subdirectory(DevIL/DevIL)
		SET_TARGET_PROPERTIES(DevIL PROPERTIES FOLDER "extlibs/DevIL")
		SET_TARGET_PROPERTIES(ILU PROPERTIES FOLDER "extlibs/DevIL")
		SET_TARGET_PROPERTIES(ILUT PROPERTIES FOLDER "extlibs/DevIL")
		
			
	add_subdirectory(raknet)
		SET_TARGET_PROPERTIES(RakNetLibStatic PROPERTIES FOLDER "extlibs")
	
	add_subdirectory(miniupnp/miniupnpc)
		SET_TARGET_PROPERTIES(upnpc-static PROPERTIES FOLDER "extlibs")
	
	set(JANSSON_STATIC_CRT ON CACHE BOOL "Link the static CRT libraries")
	set(JANSSON_EXAMPLES OFF CACHE BOOL "Compile example applications")
	set(JANSSON_BUILD_DOCS OFF CACHE BOOL "Build documentation (uses python-sphinx).")
	set(JANSSON_WITHOUT_TESTS ON CACHE BOOL "Don't build tests ('make test' to execute tests)")
	add_subdirectory(jansson)
		SET_TARGET_PROPERTIES(jansson PROPERTIES FOLDER "extlibs")
	add_subdirectory(base64)
	
	
    add_subdirectory(librocket)    

    IF(N_BUILD_QT)
        add_subdirectory(libqimg)
    ENDIF()

    IF(N_BUILD_OGL4)			
        add_subdirectory(glew)		
			SET_TARGET_PROPERTIES(glew PROPERTIES FOLDER "extlibs/glew")
        add_subdirectory(SOIL)
		
		SET(ANYFX_FOLDER "extlibs/anyfx")
        add_subdirectory(anyfx)
			SET_TARGET_PROPERTIES(compiler PROPERTIES FOLDER "extlibs/anyfx")
			SET_TARGET_PROPERTIES(mcpp PROPERTIES FOLDER "extlibs/anyfx")
			SET_TARGET_PROPERTIES(antlr PROPERTIES FOLDER "extlibs/anyfx")
			SET_TARGET_PROPERTIES(api PROPERTIES FOLDER "extlibs/anyfx")
			SET_TARGET_PROPERTIES(util PROPERTIES FOLDER "extlibs/anyfx")
			SET_TARGET_PROPERTIES(EASTL PROPERTIES FOLDER "extlibs/anyfx")
		
		set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
		set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
		set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
		set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")		        
		add_subdirectory(glfw)	
			SET_TARGET_PROPERTIES(glfw PROPERTIES FOLDER "extlibs/glfw")
    ELSEIF(N_BUILD_DX11)
        add_subdirectory(Effects11)
    ENDIF()

ELSE()
    add_subdirectory(tinyxml) 
    add_subdirectory(zlib)
ENDIF()
