# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/NekoIDE_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/NekoIDE_autogen.dir/ParseCache.txt"
  "CMakeFiles/neko_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/neko_autogen.dir/ParseCache.txt"
  "NekoIDE_autogen"
  "neko_autogen"
  )
endif()
