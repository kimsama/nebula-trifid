#-------------------------------------------------------------------------------
# posteffect
#-------------------------------------------------------------------------------



INCLUDE_DIRECTORIES(${Nebula3_SOURCE_DIR}/code
					${Nebula3_SOURCE_DIR}/code/addons/
					${Nebula3_SOURCE_DIR}/code/foundation/
					${Nebula3_SOURCE_DIR}/code/extlibs/
					${Nebula3_SOURCE_DIR}/code/render/	
					${Nebula3_SOURCE_DIR}/code/application
					${Nebula3_SOURCE_DIR}/code/application/basegamefeature					
					${Nebula3_SOURCE_DIR}/code/application/graphicsfeature
					${Nebula3_SOURCE_DIR}/code
					)


        set(SOURCES_files_posteffect__rt__params
		  rt/params/aoparams.cc
		  rt/params/aoparams.h
          rt/params/colorparams.cc
          rt/params/colorparams.h
          rt/params/depthoffieldparams.cc
          rt/params/depthoffieldparams.h
          rt/params/fogparams.cc
          rt/params/fogparams.h
          rt/params/hdrparams.cc
          rt/params/hdrparams.h
          rt/params/lightparams.cc
          rt/params/lightparams.h
          rt/params/parambase.cc
          rt/params/parambase.h
          rt/params/skyparams.cc
          rt/params/skyparams.h
		  rt/params/generalparams.cc
          rt/params/generalparams.h		  
        )
        source_group("posteffect\\rt\\params" FILES ${SOURCES_files_posteffect__rt__params})

        set(SOURCES_posteffect__rt__params
          ${SOURCES_files_posteffect__rt__params}
        )

      set(SOURCES_files_posteffect__rt
        rt/posteffecthandler.cc
        rt/posteffecthandler.h
        rt/posteffectrtplugin.cc
        rt/posteffectrtplugin.h
        rt/posteffectserver.cc
        rt/posteffectserver.h
      )
      source_group("posteffect\\rt" FILES ${SOURCES_files_posteffect__rt})

      set(SOURCES_posteffect__rt
        ${SOURCES_posteffect__rt__params}
        ${SOURCES_files_posteffect__rt}
      )

    set(SOURCES_files_posteffect
      posteffectentity.cc
      posteffectentity.h
      posteffectmanager.cc
      posteffectmanager.h
	  posteffectregistry.cc
	  posteffectregistry.h
      posteffectrendermodule.cc
      posteffectrendermodule.h
	  posteffectfeatureunit.cc
	  posteffectfeatureunit.h
    )
    source_group("posteffect" FILES ${SOURCES_files_posteffect})

    set(SOURCES_posteffect
      ${SOURCES_posteffect__rt}
      ${SOURCES_files_posteffect}
    )

	set(SOURCES_files
		../stdneb.h
		../stdneb.cc
	)
	SOURCE_GROUP("Precompiled" FILES ${SOURCES_files})
	SET(P_NIDL_FILES posteffectprotocol.nidl)
	N_WRAP_NIDL_FILES(posteffect_NIDLS ${P_NIDL_FILES})
    
	set(SOURCES_
		${SOURCES_posteffect}
		${SOURCES_files}
	)


  add_library( posteffect STATIC ${SOURCES_} )
  add_dependencies(posteffect posteffect_NIDLS)
SET_TARGET_PROPERTIES(posteffect PROPERTIES FOLDER "N3SDK/addons")

N_SET_COMPILER_DEFAULTS()
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(posteffect stdneb.h stdneb.cc ../)
ENDIF()
 
