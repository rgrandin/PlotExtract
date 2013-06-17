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


# Configure GUI type for each platform
SET(GUI_TYPE)
IF(APPLE)
    SET(GUI_TYPE MACOSX_BUNDLE)
    SET(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/app_figs/plotextract.icns)
    SET(MACOSX_BUNDLE_ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/app_figs/plotextract.icns)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/app_figs/plotextract.icns
	PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
ENDIF()

IF(WIN32)
    SET(GUI_TYPE WIN32)
ENDIF()



include_directories(${CMAKE_CURRENT_BINARY_DIR})
ADD_EXECUTABLE(${PLOTEXTRACT_NAME} ${GUI_TYPE}
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


# Set options for OS X
#if(APPLE)
#    set(APPS bin)  # paths to executables
#    set(DIRS ${QT_LIBRARY_DIR})  # directories to search for prerequisites
#    INSTALL(CODE "
#       include(BundleUtilities)
#       fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
#       " COMPONENT Runtime)
#
#    FUNCTION(ADD_CUSTOM_EXECUTABLE TARGET HEADERS SOURCES DEPENDENCIES)
#        ADD_EXECUTABLE(${TARGET} MACOSX_BUNDLE ${HEADERS} ${SOURCES} ${ARGN})
#        TARGET_LINK_LIBRARIES(${TARGET} ${DEPENDENCIES})
#
#        INSTALL(TARGETS ${TARGET} BUNDLE DESTINATION .)
#
#        INSTALL(CODE "
#            INCLUDE(BundleUtilities)
#            FIXUP_BUNDLE(${CMAKE_INSTALL_PREFIX}/${TARGET}.app \"\" \"\") 
#        ")
#    ENDFUNCTION(ADD_CUSTOM_EXECUTABLE)
#endif()

SET (PROJECT_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
IF (APPLE)

  # -------- Function to build OS X Stand Alone Bundles -----------------
  function(MakeOSXBundleApp target binary_dir)

    SET (OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT "${binary_dir}/${target} f_OSX_MakeStandAloneBundle.cmake")
    SET (OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT "${binary_dir}/${target}_OSX_MakeStandAloneBundle.sh")

    CONFIGURE_FILE("${PROJECT_RESOURCES_DIR}/OSX_Tools/CompleteBundle.cmake.in"
      "${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT}" @ONLY IMMEDIATE)
    CONFIGURE_FILE("${PROJECT_RESOURCES_DIR}/OSX_Tools/CreateBundle.sh.in"
      "${OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT}" @ONLY IMMEDIATE)
 
   install(SCRIPT "${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT}")
                    
  endfunction(MakeOSXBundleApp)

  IF(CMAKE_BUILD_TYPE MATCHES "Debug")
      MakeOSXBundleApp( "QtTest${EXE_DEBUG_EXTENSION}" ${PROJECT_BINARY_DIR})
  ELSE (CMAKE_BUILD_TYPE MATCHES "Debug")
      MakeOSXBundleApp(QtTest ${PROJECT_BINARY_DIR})
  ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

ENDIF (APPLE)


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


INSTALL(TARGETS ${PLOTEXTRACT_NAME} 
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		BUNDLE DESTINATION bundle
	)




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




