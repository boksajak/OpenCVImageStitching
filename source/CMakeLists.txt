cmake_minimum_required (VERSION 2.6) 
project (OpenCVImageStitching) 

include(../cmake/macros.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS On)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_SOURCE_DIR})
list(REMOVE_ITEM SUBDIRS "GLSL")
FOREACH(subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()


file(GLOB_RECURSE UTILS_SRCS
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/Utils/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/Utils/*.h)

source_group(Utils FILES ${UTILS_SRCS})

file(GLOB_RECURSE INCS "*.h")
file(GLOB_RECURSE CPPS "*.cpp")

# Additional places where to look for Find*.cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")

# Additional places where to look for libraries
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../sdk"
                      CACHE PATH "Path to dependencies")

IF(MINGW)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
  endif(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_CXX_STANDARD_LIBRARIES -lstdc++)
  set(CMAKE_CXX_FLAGS "-std=c++11 -m64 -fpermissive")
  set(CMAKE_CXX_FLAGS_RELEASE "-fno-exceptions -fno-rtti -ffast-math -ffunction-sections -fomit-frame-pointer -mavx -ftree-vectorize -O3 -fwhole-program -flto")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
ENDIF()

find_package(OpenGL)
find_package(GLEW)
find_package(GLM)
find_package(OpenCV)


message("GLEW: " ${GLEW_LIBRARIES})
message("GLLIB: " ${OPENGL_LIBRARIES})
#message("DevIL: " ${IL_LIBRARIES})
message("OpenCV: " ${OpenCV_LIBS})

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
#include_directories(${IL_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIR})

add_executable(OpenCVImageStitching WIN32 ${CPPS} ${INCS} ${GLSL_SRCS})
target_link_libraries(OpenCVImageStitching ${OpenCV_LIBS} ${GLEW_LIBRARIES} "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64/glu32.lib" "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64/opengl32.lib")
                                                  
add_custom_command(TARGET OpenCVImageStitching POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory                      
        "${CMAKE_SOURCE_DIR}/../sdk/bin/"       
        $<TARGET_FILE_DIR:OpenCVImageStitching>) 


# Set target directory for copying. Can refer to target which hasn't been added yet.
set(COPY_TARGET_DIR $<TARGET_FILE_DIR:OpenCVImageStitching>)
post_build_make_dir_link(OpenCVImageStitching ${PROJECT_SOURCE_DIR}/../assets  ${COPY_TARGET_DIR}/assets)
post_build_make_dir_link(OpenCVImageStitching ${PROJECT_SOURCE_DIR}/GLSL       ${COPY_TARGET_DIR}/shaders)
if(MSVC)
    post_build_make_dir_link(OpenCVImageStitching ${PROJECT_SOURCE_DIR}/../assets  ${PROJECT_BINARY_DIR}/assets)
    post_build_make_dir_link(OpenCVImageStitching ${PROJECT_SOURCE_DIR}/GLSL       ${PROJECT_BINARY_DIR}/shaders)
endif(MSVC)
               


                                                                              