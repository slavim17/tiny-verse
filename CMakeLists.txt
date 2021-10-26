# Full tiny-verse cmake template
#
# Loads all the tinyverse modules in their proper paths and builds them. Where appropriate 

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
# Module Configuration
#
# In order for the repl and compiler to build, the repositories providing the required modules must be set here. This selection is carried over to the C++ code where the FRONTEND, ... macros will be available with the values that are set here. This information is used by the generic repl and compiler to include proper files with the class definitions of the appropriate modules. See the readme for more details. 

set(FRONTEND "tinyc")
set(OPTIMIZER "dummy")
set(BACKEND "dummy")
set(TARGET "dummy")

# Pass the above selection to C++ compiler as macros
add_compile_definitions(FRONTEND=${FRONTEND})
add_compile_definitions(OPTIMIZER=${OPTIMIZER})
add_compile_definitions(BACKEND=${BACKEND})
add_compile_definitions(TARGET=${TARGET})
add_compile_definitions("FRONTEND_${FRONTEND}")
add_compile_definitions("OPTIMIZER_${OPTIMIZER}")
add_compile_definitions("BACKEND_${BACKEND}")
add_compile_definitions("TARGET_${TARGET}")

# We always need the common library which is part of this project (for now at least). Furthermore the dummy modules are added to include path to make sure their definitions are accessible in the same way the other modules are. 
set(TINY_LIBRARIES "libcommon")
set(TINY_INCLUDES "${CMAKE_SOURCE_DIR};${CMAKE_SOURCE_DIR}/dummy")
include_directories("${CMAKE_SOURCE_DIR}/common")

# Projects that will be built as part of the repl/compiler executable
#
# These are separate repositories in tinyverse. They must be cloned in the root folder in folders that are the same as their names here, otherwise the inclusion won't work. If the repositories provide modules (frontend, optimizer, backend, target) they must declare them in their root folder. See the readme for more documentation on repository & module structure. 
#
# If used as a template, comment out the repositories not needed. 
add_subdirectory(common)
add_subdirectory(secd)
add_subdirectory(t86)
add_subdirectory(tinyc)
add_subdirectory(tinycpp)
add_subdirectory(tinylisp)

# Let's be clear about what we are building...
message("tiny modules:")
message("    frontend:  ${FRONTEND}")
message("    optimizer: ${OPTIMIZER}")
message("    backend:   ${BACKEND}")
message("    target:    ${TARGET}")
message("tiny libraries:")
message("    ${TINY_LIBRARIES}")

# Build the tiny repl/compiler
project(tiny)
include_directories(${TINY_INCLUDES})
file(GLOB_RECURSE SRC "main.cpp" "repl/*.cpp" "repl/*.h")
add_executable(tiny ${SRC})
target_link_libraries(tiny ${TINY_LIBRARIES})
