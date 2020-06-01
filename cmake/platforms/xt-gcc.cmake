set (CMAKE_SYSTEM_NAME "Generic"             CACHE STRING "")
set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER CACHE STRING "")
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER CACHE STRING "")
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER CACHE STRING "")

include (CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER   ("${CROSS_PREFIX}xcc" GNU)
CMAKE_FORCE_CXX_COMPILER ("${CROSS_PREFIX}xc++" GNU)

# vim: expandtab:ts=2:sw=2:smartindent
