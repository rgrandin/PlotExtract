# Based on tutorial at: http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects

# ==============================================================================
#
#   Set basic options
#
# ==============================================================================

# Require CMake version 2.8+
cmake_minimum_required(VERSION 2.8)

# Set name ofproject
set(PLOTEXTRACT_NAME "plotextract")

# Set default build type
set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)


# Control build options and feature support
SET(PLOTEXTRACT_CXX_11 ON CACHE BOOL "Enable C++-11 Support")
SET(PLOTEXTRACT_OPENMP ON CACHE BOOL "Enable OpenMP Support")








# ==============================================================================
#
#   Set compiler flags
#
# ==============================================================================


# GCC
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if(PLOTEXTRACT_CXX_11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")    # Enable C++11 features
        add_definitions(-DCXX11)
    endif()

    if(PLOTEXTRACT_OPENMP)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")      # Enable OpenMP
    endif()
endif()


# MSVC
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if(PLOTEXTRACT_CXX_11)
        add_definitions(-DCXX11)
    endif()

    if(PLOTEXTRACT_OPENMP)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")      # Enable OpenMP
    endif()
endif()



# ==============================================================================
#
#   Add general-use C++ classes
#
# ==============================================================================

# The following must be manually set.  An improved solution would be to configure
# the general-use classes to be found automatically using 'FIND_PACKAGE', but this
# manual configuration must be used for now.
set(GEN_CLASSES_INCLUDE "GEN_CLASSES_NOTFOUND" CACHE STRING
        "Path to include files for General-Use C++ classes.")
set(GEN_CLASSES_LIB_DIR "GEN_CLASSES_LIB_DIR_NOTFOUND" CACHE STRING
        "Path to library for General-Use C++ classes.")
set(GEN_CLASSES_LIBS "GEN_CLASSES_LIBS_NOTFOUND" CACHE STRING
        "Library for General-Use C++ classes.")

# Add include and linking info to build
link_directories(${GEN_CLASSES_LIB_DIR})
include_directories(${GEN_CLASSES_INCLUDE})








# ==============================================================================
#
#   Add current source dir to include directories
#
# ==============================================================================

include_directories(${CMAKE_CURRENT_SOURCE_DIR})







# ==============================================================================
#
#   Configure Qt support
#
# ==============================================================================

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


# UI files
SET(PLOTEXTRACT_UI
        credits.ui
        plotextract.ui
   )
QT4_WRAP_UI(PLOTEXTRACT_HEADER_FORMS ${PLOTEXTRACT_UI})


# Resources
SET(PLOTEXTRACT_QRC
        app_figs.qrc
   )
QT4_ADD_RESOURCES(PLOTEXTRACT_RESOURCES ${PLOTEXTRACT_QRC})








# ==============================================================================
#
#   Define sources to be compiled
#
# ==============================================================================

# Define source files to be compiled into object files.  Most of the *.cpp files are not
# listed here because they are templated and thus directly included in their headers.
set(PLOTEXTRACT_SOURCES ${PLOTEXTRACT_DIR}clickablepixmap.cpp
                        ${PLOTEXTRACT_DIR}credits.cpp
                        ${PLOTEXTRACT_DIR}main.cpp
                        ${PLOTEXTRACT_DIR}plotextract.cpp
    )







# ==============================================================================
#
#   Add executable to the build & link against required libraries
#
# ==============================================================================

include_directories(${CMAKE_CURRENT_BINARY_DIR})
ADD_EXECUTABLE(${PLOTEXTRACT_NAME} ${PLOTEXTRACT_SOURCES}
                                   ${PLOTEXTRACT_HEADERS_MOC}
                                   ${PLOTEXTRACT_HEADER_FORMS}
                                   ${PLOTEXTRACT_RESOURCES}
              )
TARGET_LINK_LIBRARIES(${PLOTEXTRACT_NAME}
                            ${QT_LIBRARIES}
                            ${GEN_CLASSES_LIBS}
                     )





# ==============================================================================
#
#   Install rules
#
# ==============================================================================

INSTALL(TARGETS ${PLOTEXTRACT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)





# ==============================================================================
#
#   Configure CPack
#
# ==============================================================================
INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Plot digitization program.")
SET(CPACK_PACKAGE_VENDOR "Robert Grandin")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "PlotExtract ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_PACKAGE_EXECUTABLES "plotextract" "PlotExtract")

include(CPack)




