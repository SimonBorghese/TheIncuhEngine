cmake_minimum_required(VERSION 3.10)

project(IncuhEngine VERSION 1.0)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} ${SOURCES} "include/*.hpp")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} ${SOURCES} "include/*.h")


add_executable(IncuhEngine ${SOURCES} "src/extra/glad.c")

set(OpenGL_GL_PREFERENCE LEGACY)

# Find all the packages we need
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(fmt REQUIRED)
find_package(assimp REQUIRED)
# Have PhysX installed, or else
# find_package(PHYSX REQUIRED)

# Set their include directories or something
target_include_directories(IncuhEngine PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(IncuhEngine PUBLIC ${OPENGL_INCLUDE_DIR})
link_directories("libs")
link_directories("libs/physx")
target_link_directories(IncuhEngine PUBLIC "libs/physx")
target_link_directories(IncuhEngine PUBLIC "libs")


# link the libraries
target_link_libraries(IncuhEngine ${SDL2_LIBRARIES})
target_link_libraries(IncuhEngine ${OPENGL_LIBRARIES})
target_link_libraries(IncuhEngine ${CMAKE_DL_LIBS})
target_link_libraries(IncuhEngine assimp)
target_link_libraries(IncuhEngine pthread)
target_link_libraries(IncuhEngine zlibstatic)
target_link_libraries(IncuhEngine PhysX_static_64 PhysXCharacterKinematic_static_64 PhysXPvdSDK_static_64 PhysXExtensions_static_64 PhysXCooking_static_64 PhysXFoundation_static_64 PhysXVehicle_static_64 PhysXCommon_static_64)
 #LIB =  -lPhysX_static_64 -lPhysXCharacterKinematic_static_64 -lPhysXPvdSDK_static_64 -lPhysXExtensions_static_64 -lPhysXCooking_static_64 -lPhysXFoundation_static_64  -lPhysXVehicle_static_64 -lPhysXCommon_static_64
target_link_libraries(IncuhEngine fmt)
target_link_libraries(IncuhEngine asound)
target_link_libraries(IncuhEngine soloud)

include_directories("include/IncuhEngine")
include_directories("include/IncuhEngine/World")
include_directories("include/IncuhEngine/Objects")
include_directories("include/IncuhEngine/Engine")
include_directories("include/thirdparty")
include_directories("include/thirdparty/physx")
include_directories("include/thirdparty/imgui")
include_directories("include/IncuhEngine/BSP")
include_directories("/usr/include")


set(CMAKE_CXX_STANDARD 13)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

add_compile_definitions(NDEBUG)
