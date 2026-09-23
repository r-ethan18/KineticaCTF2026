cmake_minimum_required(VERSION 3.22)
project(MySodiumApp LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add custom module path for cmake/FindSodium.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# Tell FindSodium.cmake to look for static libraries
set(sodium_USE_STATIC_LIBS TRUE)
# Find the package using your cmake/FindSodium.cmake
find_package(Sodium REQUIRED)

include_directories(include ${sodium_INCLUDE_DIR})

set (ENC_STR_SRC
  src/encrypt-str.cpp
  src/helpers.cpp
)

add_executable(encrypt-str "${ENC_STR_SRC}")
target_link_libraries(encrypt-str PRIVATE sodium)

set (MAIN_SRC
  src/main-lock.cpp
  src/validator.cpp
  src/helpers.cpp
  src/backup.cpp
)
add_executable(exceptional-locking-mechanism "${MAIN_SRC}")
target_compile_options(exceptional-locking-mechanism PRIVATE -ffunction-sections -fdata-sections)
target_link_options(exceptional-locking-mechanism PRIVATE -Wl,--gc-sections)
target_link_libraries(exceptional-locking-mechanism PRIVATE sodium)


