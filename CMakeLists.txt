cmake_minimum_required(VERSION 3.0.0)
project(rss_nn LANGUAGES C CXX)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)




# set(CMAKE_C_COMPILER             "/usr/bin/clang-11")
# set(CMAKE_C_COMPILER             "/usr/bin/clang")
set(CMAKE_C_FLAGS                "-maes -mavx -mavx2 -march=native -Wall -std=c99")
set(CMAKE_C_FLAGS_DEBUG          "-g")
set(CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE        "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")


set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_COMPILER             "/usr/bin/clang++-11")
# set(CMAKE_CXX_COMPILER             "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS                "-maes -mavx -mavx2 -march=native -Wall")
set(CMAKE_CXX_FLAGS_DEBUG          "-g3 -Wall")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
# set(CMAKE_CXX_FLAGS_RELEASE        "-Ofast")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# set(CMAKE_BUILD_PARALLEL_LEVEL 8)


# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")   # Add more flags here
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -march=native")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Wall")

find_package(OpenSSL REQUIRED)
find_package(CryptoPP REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

option(USE_30 "Use MY_LIB instead of THEIR_LIB" OFF) #enables 60 vscode, should be default for remote
# option(USE_30 "Use MY_LIB instead of THEIR_LIB" ON) #enables 30 vscode
if(USE_30)

  INCLUDE_DIRECTORIES(
      "${CMAKE_SOURCE_DIR}/include"
      "${CMAKE_SOURCE_DIR}/include_30"
      # "${CMAKE_SOURCE_DIR}/include_60"
  )
    #add some compilation flags
else()

  INCLUDE_DIRECTORIES(
    "${CMAKE_SOURCE_DIR}/include"
    # "${CMAKE_SOURCE_DIR}/include_30"
    "${CMAKE_SOURCE_DIR}/include_60"
  )
endif(USE_30)
unset(USE_30 CACHE) # <---- this is the important!!



# set(SOURCE_FILES src/main.c)
file(GLOB SOURCES 
                "src/*.cpp"
                "src/connection/*.cpp"
                "src/benchmarks/*.cpp"
                "src/protocols_basic/*.cpp"
                "src/protocols_composite/*.cpp"
                "src/protocols_rand/*.cpp"
                "src/protocols_ml/*.cpp"
                "src/utilities/*.cpp"
                "src/utilities/*.c"
)

add_executable(rss_nn ${SOURCES})

# add_library(aes_ni STATIC src/utilities/aes_ni.c)
# target_include_directories(aes_ni include/aes_ni.h)



target_link_libraries(rss_nn ${OPENSSL_LIBRARIES} ${CRYPTOPP_LIBRARIES})

# install(TARGETS rss_nn
#         DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)


add_custom_target(clean-all
  make clean && rm -fR ${CMAKE_BINARY_DIR}/CMakeCache.txt
                       ${CMAKE_BINARY_DIR}/cmake_install.cmake
                       ${CMAKE_BINARY_DIR}/Makefile
                       ${CMAKE_BINARY_DIR}/CMakeFiles
             && rmdir  ${CMAKE_BINARY_DIR}/build
)