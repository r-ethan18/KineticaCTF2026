cmake_minimum_required(VERSION 3.22)
project(ExceptionalLockChallenge LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ensure release defaults or apply strip flags directly
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add custom module path for cmake/FindSodium.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(sodium_USE_STATIC_LIBS TRUE)
find_package(Sodium REQUIRED)

include_directories(include ${sodium_INCLUDE_DIR})

# --- Target 1: encrypt-str ---
set(ENC_STR_SRC
    src/encrypt-str.cpp
    src/helpers.cpp
)
add_executable(encrypt-str "${ENC_STR_SRC}")
target_link_libraries(encrypt-str PRIVATE sodium)

# --- Target 2: exceptional-locking-mechanism ---
set(MAIN_SRC
    src/main-lock.cpp
    src/validator.cpp
    src/helpers.cpp
    src/backup.cpp
)
add_executable(exceptional-locking-mechanism "${MAIN_SRC}")
target_compile_options(exceptional-locking-mechanism PRIVATE -ffunction-sections -fdata-sections)
target_link_options(exceptional-locking-mechanism PRIVATE -Wl,--gc-sections -Wl,-s)
target_link_libraries(exceptional-locking-mechanism PRIVATE sodium)


# ==========================================
# NixOS Detection & Post-Build Operations
# ==========================================
set(IS_NIXOS FALSE)
if(EXISTS "/etc/NIXOS")
    set(IS_NIXOS TRUE)
elseif(EXISTS "/etc/os-release")
    file(READ "/etc/os-release" OS_RELEASE_DATA)
    if(OS_RELEASE_DATA MATCHES "(N|n)ixos")
        set(IS_NIXOS TRUE)
    endif()
endif()

# Macro to handle multi-directory output and NixOS patching automatically
macro(setup_challenge_outputs TARGET_NAME)
    # 1. Ensure challenge directory exists and copy binary post-build
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/challenge"
        COMMAND ${CMAKE_COMMAND} -E copy 
            "$<TARGET_FILE:${TARGET_NAME}>" 
            "${CMAKE_SOURCE_DIR}/challenge/$<TARGET_FILE_NAME:${TARGET_NAME}>"
        COMMENT "Copying ${TARGET_NAME} to challenge/ directory"
    )

    # 2. Equivalent of your NixOS patchelf snippet
    if(IS_NIXOS)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND patchelf --set-interpreter /lib64/ld-linux-x86-64.so.2 "${CMAKE_SOURCE_DIR}/challenge/$<TARGET_FILE_NAME:${TARGET_NAME}>"
            COMMAND patchelf --remove-rpath "${CMAKE_SOURCE_DIR}/challenge/$<TARGET_FILE_NAME:${TARGET_NAME}>"
            COMMENT "Applying NixOS patchelf fix to challenge/${TARGET_NAME}"
        )
    endif()
endmacro()

# Apply the post-build pipeline to both targets
setup_challenge_outputs(exceptional-locking-mechanism)