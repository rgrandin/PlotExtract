# Based on tutorial at: http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects


cmake_minimum_required(VERSION 2.8)


set(PLOTEXTRACT_NAME "plotextract")                       # Define name of application


set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)


SET(PLOTEXTRACT_CXX_11 ON CACHE BOOL "Enable C++-11 Support")
SET(PLOTEXTRACT_OPENMP ON CACHE BOOL "Enable OpenMP Support")


# Set GCC compiler flags
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if(PLOTEXTRACT_CXX_11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")    # Enable C++11 features
        add_definitions(-DCXX11)
    endif()

    if(PLOTEXTRACT_OPENMP)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")      # Enable OpenMP
    endif()
endif()


# Set MSVC compiler flags
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if(PLOTEXTRACT_CXX_11)
        add_definitions(-DCXX11)
    endif()

    if(PLOTEXTRACT_OPENMP)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")      # Enable OpenMP
    endif()
endif()



# Add general-use C++ classes
set(GEN_CLASSES_DIR "GEN_CLASSES_NOTFOUND" CACHE STRING "Path to CMakeLists.txt for General-Use C++ classes.")
add_subdirectory(${GEN_CLASSES_DIR} "${CMAKE_CURRENT_BINARY_DIR}/genCppClasses")



# Define source directory and add it to the directories to be included
set(PLOTEXTRACT_DIR ${CMAKE_CURRENT_SOURCE_DIR})        # Define directory containing source files
include_directories(${PLOTEXTRACT_DIR})


# Require Qt4
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})



# Header files to be MOC'd
SET(PLOTEXTRACT_HEADERS
        clickablepixmap.h
        credits.h
        plotextract.h
   )

QT4_WRAP_CPP(PLOTEXTRACT_HEADERS_MOC ${PLOTEXTRACT_HEADERS})






# Define source files to be compiled into object files.  Most of the *.cpp files are not
# listed here because they are templated and thus directly included in their headers.
set(PLOTEXTRACT_SOURCE ${PLOTEXTRACT_DIR}/clickablepixmap.cpp
                           ${PLOTEXTRACT_DIR}/credits.cpp
                           ${PLOTEXTRACT_DIR}/main.cpp
                           ${PLOTEXTRACT_DIR}/plotextract.cpp
    )



ADD_EXECUTABLE(${PLOTEXTRACT_NAME} ${PLOTEXTRACT_SOURCES}
    ${PLOTEXTRACT_HEADERS_MOC})
TARGET_LINK_LIBRARIES(${PLOTEXTRACT_NAME} ${QT_LIBRARIES})





