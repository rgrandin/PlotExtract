# Based on tutorial at: http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects

# ==============================================================================
#
#   Set basic options
#
# ==============================================================================

# Set name ofproject
project(PlotExtrat)

# Require CMake version 2.8+
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set name to be used for executable
set(PLOTEXTRACT_NAME "plotextract")

# Set default build type
set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)


# Control build options and feature support
SET(USE_CXX_11 ON CACHE BOOL "Enable C++-11 Support Globally")
SET(USE_OPENMP ON CACHE BOOL "Enable OpenMP Support Globally")
SET(USE_QT ON CACHE BOOL "Enable Qt Functionality Globally" FORCE)
SET(USE_VTK OFF CACHE BOOL "Enable VTK Library Use Globally")










# ==============================================================================
#
#   Set compiler flags
#
# ==============================================================================


# GCC
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    
    if(CMAKE_COMPILER_IS_GNUCXX AND USE_CXX_11)
        SET(ENABLE_CXX11 "-std=c++11")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ENABLE_CXX11}")
        add_definitions(-DCXX11)
    endif()


    if(PLOTEXTRACT_OPENMP)
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")      # Enable OpenMP
        SET_TARGET_PROPERTIES(${PLOTEXTRACT_NAME}  PROPERTIES 
                COMPILE_FLAGS "-fopenmp"
                LINK_FLAGS "-fopenmp"
                )
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
if(WIN32)
	set(QT_USE_QTMAIN TRUE)
endif()
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})



# Header files to be MOC'd
SET(PLOTEXTRACT_HEADERS
        clickablepixmap.h
        credits.h
        plotextract.h
)
SET(QT_HEADERS ${QT_HEADERS} ${PLOTEXTRACT_HEADERS})
QT4_WRAP_CPP(PLOTEXTRACT_HEADERS_MOC ${QT_HEADERS})


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
#   Add general-use C++ classes
#
# ==============================================================================

# User specifies the path to CMakeLists.txt for General-Use C++ classes
set(GENCLASSES_SOURCE_DIR "GENCLASSES_NOTFOUND" CACHE PATH 
            "Path General-Use C++ Classes CMakeLists.txt")

# Add the directory as a subdirectory of the build using the 2-argument form
# of 'add_subdirectory'.  This lets us use code located outside of the local source
# directory without having to outright copy it or deal with symbolic links.
add_subdirectory(${GENCLASSES_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/gen-classes)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/gen-classes)






# ==============================================================================
#
#   Define sources to be compiled
#
# ==============================================================================

# Sources local to this directory
set(PLOTEXTRACT_SOURCES ${PLOTEXTRACT_DIR}clickablepixmap.cpp
                        ${PLOTEXTRACT_DIR}credits.cpp
                        ${PLOTEXTRACT_DIR}main.cpp
                        ${PLOTEXTRACT_DIR}plotextract.cpp
    )
    
# Combine into master 'SOURCES' variable
set(SOURCES ${SOURCES} ${PLOTEXTRACT_SOURCES})






# ==============================================================================
#
#   Add executable to the build & link against required libraries
#
# ==============================================================================

# Configure RPATH for library-loading on Linux
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# Add "../lib" directory relative to executable to library search path.  This
# makes the executable portable.
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN/../lib/'" )



include_directories(${CMAKE_CURRENT_BINARY_DIR})
ADD_EXECUTABLE(${PLOTEXTRACT_NAME} WIN32 MACOSX_BUNDLE
								${SOURCES}
                                ${PLOTEXTRACT_HEADERS_MOC}
                                ${PLOTEXTRACT_HEADER_FORMS}
                                ${PLOTEXTRACT_RESOURCES}
            )
TARGET_LINK_LIBRARIES(${PLOTEXTRACT_NAME} ${QT_LIBRARIES})
                    

                    
                    



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

# Copy needed Qt DLL files into INSTALL destination
if ( NOT Q_WS_MAC)
    if (DEFINED QT_QMAKE_EXECUTABLE)
        SET (QTLIBLIST QtCore QtGui)

		IF(MSVC)
			FOREACH(qtlib ${QTLIBLIST})
				INSTALL(FILES ${QT_BINARY_DIR}/${qtlib}4.dll DESTINATION bin)
			ENDFOREACH()
		ENDIF()

    endif(DEFINED QT_QMAKE_EXECUTABLE)
endif()



# ==============================================================================
#
#   Configure CPack
#
# ==============================================================================
INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Plot digitization program.")
SET(CPACK_PACKAGE_NAME "PlotExtract")
SET(CPACK_PACKAGE_VENDOR "Robert Grandin")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "PlotExtract ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_PACKAGE_EXECUTABLES "plotextract" "PlotExtract")

include(CPack)




