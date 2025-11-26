function(collect_source_recursive DIRECTORY_PATH EXTENSION OUT_VARIABLE)
  if(NOT IS_DIRECTORY "${DIRECTORY_PATH}")
    message(FATAL_ERROR "Directory ${DIRECTORY_PATH} does not exist")
  endif()

  file(GLOB_RECURSE found_files
    "${DIRECTORY_PATH}/*${EXTENSION}"
  )

  set(${OUT_VARIABLE} "${found_files}" PARENT_SCOPE)
endfunction()