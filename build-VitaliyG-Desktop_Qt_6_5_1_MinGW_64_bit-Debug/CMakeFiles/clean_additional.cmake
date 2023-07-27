# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\VitaliyG_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\VitaliyG_autogen.dir\\ParseCache.txt"
  "VitaliyG_autogen"
  )
endif()
