#-------------------------------------------------------------------------------
# navigation
#-------------------------------------------------------------------------------

INCLUDE_DIRECTORIES(..
					${Nebula3_SOURCE_DIR}/code/addons/
					${Nebula3_SOURCE_DIR}/code/foundation/
					${Nebula3_SOURCE_DIR}/code/render/
					${Nebula3_SOURCE_DIR}/code/application/
					${Nebula3_SOURCE_DIR}/code/extlibs/
					${Nebula3_SOURCE_DIR}/code/toolkit/toolkitutil
					${Nebula3_SOURCE_DIR}/code/application/basegamefeature
					${Nebula3_SOURCE_DIR}/code/extlibs/recastnavigation/Recast/Include
					${Nebula3_SOURCE_DIR}/code/extlibs/recastnavigation/DebugUtils/Include
					${Nebula3_SOURCE_DIR}/code/extlibs/recastnavigation/Detour/Include
					${Nebula3_SOURCE_DIR}/code/extlibs/recastnavigation/DetourTileCache/Include
					${Nebula3_SOURCE_DIR}/code/extlibs/recastnavigation/DetourCrowd/Include
					${Nebula3_SOURCE_DIR}/code
					)
					
    set(SOURCES_files_nav
		navigationserver.h
		navigationserver.cc		
		path3d.cc
		path3d.h
		crowdmanager.cc
		crowdmanager.h
		navigatedproperty.cc
		navigatedproperty.h
    )
    source_group("navigation" FILES ${SOURCES_files_nav})
	
	set(SOURCES_files_debug_nav		
		debug/detourdebug.h
		debug/detourdebug.cc
		debug/navigationpagehandler.cc
		debug/navigationpagehandler.h
    )
    source_group("navigation\\debug" FILES ${SOURCES_files_debug_nav})
	
	FILE(GLOB SRC "recast/*.cc")
	FILE(GLOB HEAD "recast/*.h")
	LIST(APPEND SRCSREC ${SRC} ${HEAD})
	SOURCE_GROUP("navigation\\recast" FILES ${SRCSREC})


    set(SOURCES_nav
	  ${SOURCES_files_debug_nav}
      ${SOURCES_files_nav}
	  ${SRCSREC}
    )

  set(SOURCES_files
	../stdneb.h
    ../stdneb.cc
  )
	
	SOURCE_GROUP("Precompiled" FILES ${SOURCES_files})
	set(SOURCES_SRCS
	    ${SOURCES_nav}
	    ${SOURCES_files}		
	)

N_WRAP_NIDL_FILES(navigation_NIDLS navigationattributes.nidl)

add_library( navigation STATIC ${SOURCES_SRCS} )
SET_TARGET_PROPERTIES(navigation PROPERTIES FOLDER "N3SDK/addons")
N_SET_COMPILER_DEFAULTS()
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(navigation stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(navigation navigation_NIDLS)
ADD_DEPENDENCIES(navigation_NIDLS application_NIDLS)

