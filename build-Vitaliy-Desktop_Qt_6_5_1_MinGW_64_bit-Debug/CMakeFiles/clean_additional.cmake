# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Vitaliy_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Vitaliy_autogen.dir\\ParseCache.txt"
  "Vitaliy_autogen"
  )
endif()
