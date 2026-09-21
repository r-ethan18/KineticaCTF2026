cmake_minimum_required(VERSION 3.22)
project(MySodiumApp LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add custom module path for cmake/FindSodium.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# Tell FindSodium.cmake to look for static libraries
set(sodium_USE_STATIC_LIBS TRUE)
# Find the package using your cmake/FindSodium.cmake
find_package(Sodium REQUIRED)

include_directories(include)

set (ENC_STR_SRC
  src/encrypt-str.cpp
  src/helpers.cpp
)

add_executable(encrypt-str "${ENC_STR_SRC}")
target_link_libraries(encrypt-str PRIVATE sodium)

set (MAIN_SRC
  src/main-lock.cpp
  src/helpers.cpp
  src/backup.cpp
)
add_executable(decrypt-str "${MAIN_SRC}")
target_link_libraries(decrypt-str PRIVATE sodium)

# add_executable(sodium_static src/sodium-test.cpp)
# target_link_libraries(sodium_static PRIVATE sodium)

# # 7. Windows compilation flag
# if(WIN32)
#     target_compile_definitions(sodium_static PRIVATE SODIUM_STATIC=1)
# endif()
