# Full tiny-verse cmake template
#
# Loads all the tinyverse modules in their proper paths and builds them. Where appropriate 

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
# Module Configuration
#
# In order for the repl and compiler to build, the repositories providing the required modules must be set here. This selection is carried over to the C++ code where the FRONTEND, ... macros will be available with the values that are set here. This information is used by the generic repl and compiler to include proper files with the class definitions of the appropriate modules. See the readme for more details. 

set(FRONTEND "tinyc")
set(OPTIMIZER "tvlm")
set(BACKEND "dummy")
set(TARGET "dummy")

# Projects that will be built as part of the repl/compiler executable
#
# These are separate repositories in tinyverse. They must be cloned in the root folder in folders that are the same as their names here, otherwise the inclusion won't work. If the repositories provide modules (frontend, optimizer, backend, target) they must declare them in their root folder. See the readme for more documentation on repository & module structure. 
#
# If used as a template, comment out the repositories not needed. 
set(TINY_MODULES "tinyc;tinycpp;tinylisp;tvlm;secd;t86")



# Pass the above selection to C++ compiler as macros
add_compile_definitions(FRONTEND=${FRONTEND})
add_compile_definitions(OPTIMIZER=${OPTIMIZER})
add_compile_definitions(BACKEND=${BACKEND})
add_compile_definitions(TARGET=${TARGET})
add_compile_definitions("FRONTEND_${FRONTEND}")
add_compile_definitions("OPTIMIZER_${OPTIMIZER}")
add_compile_definitions("BACKEND_${BACKEND}")
add_compile_definitions("TARGET_${TARGET}")

# make sure common is always added as even we need it
set(TINY_MODULES "common;${TINY_MODULES}")
# We always need the common library which is part of this project (for now at least). Furthermore the dummy modules are added to include path to make sure their definitions are accessible in the same way the other modules are. 
set(TINY_LIBRARIES "")
include_directories("${CMAKE_SOURCE_DIR};${CMAKE_SOURCE_DIR}/dummy")


# first include all module directories and if the modules are not present clone them
foreach(MODULE IN LISTS TINY_MODULES)
    if(NOT IS_DIRECTORY "${CMAKE_SOURCE_DIR}/${MODULE}")
        message("Module ${MODULE} not found - cloning...")
        execute_process(
            COMMAND git clone "git@github.com:tiny-verse/${MODULE}"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
    endif()
    include_directories("${CMAKE_SOURCE_DIR}/${MODULE}")
endforeach()

foreach(MODULE IN LISTS TINY_MODULES)
    message("Configuring module ${MODULE}")
    add_subdirectory("${MODULE}")
endforeach()

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
file(GLOB_RECURSE SRC "main.cpp" "repl/*.cpp" "repl/*.h")
add_executable(tiny ${SRC})
target_link_libraries(tiny ${TINY_LIBRARIES})
