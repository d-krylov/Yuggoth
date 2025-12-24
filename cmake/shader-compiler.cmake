function(compile_shaders SHADER_FILES SHADER_SPIRV_DIR)
  set(GLSL_FLAGS --target-env vulkan1.3)
  foreach(SHADER ${SHADER_FILES})
    file(RELATIVE_PATH RELATIVE_SHADER_PATH "${PROJECT_SOURCE_DIR}/assets/shaders/" ${SHADER})
    set(SHADER_OUTPUT "${SHADER_SPIRV_DIR}/${RELATIVE_SHADER_PATH}.spv")
    get_filename_component(SHADER_OUTPUT_DIR ${SHADER_OUTPUT} DIRECTORY)
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})
    list(APPEND SPIRV_FILES ${SHADER_OUTPUT})
    add_custom_command(
      OUTPUT ${SHADER_OUTPUT}
      COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V -c ${GLSL_FLAGS} ${SHADER} -o ${SHADER_OUTPUT}
      DEPENDS ${SHADER}
      COMMENT "Compiling ${SHADER} to ${SHADER_OUTPUT}"
    )
  endforeach()
  add_custom_target(compile_shaders_target ALL DEPENDS ${SPIRV_FILES})
endfunction()

function(compile_slang_shaders SHADER_FILES SHADER_SPIRV_DIR)
  foreach(SHADER ${SHADER_FILES})
    file(RELATIVE_PATH RELATIVE_SHADER_PATH "${PROJECT_SOURCE_DIR}/assets/shaders_slang/" ${SHADER})

    get_filename_component(EXTENSION1 ${SHADER} LAST_EXT)
    get_filename_component(WITHOUT_EXTENSION1 ${SHADER} NAME_WLE)
    get_filename_component(EXTENSION2 ${WITHOUT_EXTENSION1} LAST_EXT)

    if(EXTENSION2 STREQUAL ".vert") 
      set(PROFILE vs_6_6)
    elseif(EXTENSION2 STREQUAL ".frag") 
      set(PROFILE ps_6_6)
    elseif(EXTENSION2 STREQUAL ".comp") 
      set(PROFILE cs_6_6)
    else()
      message(FATAL_ERROR "Unsupported shader stage: ${WITHOUT_EXTENSION1}")
    endif()

    set(SHADER_OUTPUT "${SHADER_SPIRV_DIR}/${RELATIVE_SHADER_PATH}.spv")
    get_filename_component(SHADER_OUTPUT_DIR ${SHADER_OUTPUT} DIRECTORY)
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

    list(APPEND SPIRV_FILES ${SHADER_OUTPUT})

    add_custom_command(
      OUTPUT ${SHADER_OUTPUT}
      COMMAND slangc
        ${SHADER}
        -target spirv
        -profile ${PROFILE}
        -entry main
        -fvk-use-entrypoint-name
        -o ${SHADER_OUTPUT}
      DEPENDS ${SHADER}
      COMMENT "Compiling ${SHADER} → ${SHADER_OUTPUT}"
    )
  endforeach()

  add_custom_target(compile_slang_shaders_target ALL DEPENDS ${SPIRV_FILES})
endfunction()