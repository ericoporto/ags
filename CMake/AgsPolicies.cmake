macro (ags_policies)
	# Don't warn about INTERPROCEDURAL_OPTIMIZATION
	if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.9.0")
		cmake_policy(SET CMP0069 NEW)
	endif()

	# Don't warn about DOWNLOAD_EXTRACT_TIMESTAMP
	if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
	endif()
endmacro()