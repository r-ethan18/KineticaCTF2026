cmake_minimum_required(VERSION 3.22)
project(MySodiumApp LANGUAGES C CXX)

# Add custom module path for cmake/FindSodium.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# Tell FindSodium.cmake to look for static libraries
set(sodium_USE_STATIC_LIBS TRUE)
# Find the package using your cmake/FindSodium.cmake
find_package(Sodium REQUIRED)


# 5. Target configuration
add_executable(sodium_static src/sodium-test.cpp)

# 6. Link against the imported target created by FindSodium.cmake
target_link_libraries(sodium_static PRIVATE sodium)

# 7. Windows compilation flag
if(WIN32)
    target_compile_definitions(sodium_static PRIVATE SODIUM_STATIC=1)
endif()
