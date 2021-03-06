
PROJECT ( FLSCAN )

SET ( EXECUTABLE_OUTPUT_PATH
      ${FLSCAN_BINARY_DIR}/bin
      CACHE PATH "install root"
)

cmake_minimum_required (VERSION 3.0)

    MESSAGE ("Using CMake ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}")
    MESSAGE ("*******************************************")
IF (WIN32)
    MESSAGE ("* Generating FLSCAN win32 project files ... *")
ELSE (WIN32)
    MESSAGE ("* Generating FLSCAN unix Makefiles..        *")
ENDIF (WIN32)
    MESSAGE ("*******************************************")

INCLUDE ( ${CMAKE_ROOT}/Modules/FindJPEG.cmake        )
INCLUDE ( ${CMAKE_ROOT}/Modules/FindPNG.cmake         )
INCLUDE ( ${CMAKE_ROOT}/Modules/FindFLTK.cmake        )

# INCLUDE ( ${FLSCAN_SOURCE_DIR}/Modules/FindPNG.cmake  )
INCLUDE ( ${FLSCAN_SOURCE_DIR}/Modules/FindSANE.cmake )

INCLUDE ( ${FLSCAN_SOURCE_DIR}/CMakeFlags.cfg         )

IF ( CMAKE_CONFIGURE_INSTALL_PREFIX )
    SET( CMAKE_INSTALL_PREFIX ${CMAKE_CONFIGURE_INSTALL_PREFIX} )
ENDIF ( CMAKE_CONFIGURE_INSTALL_PREFIX )

MAKE_DIRECTORY( ${EXECUTABLE_OUTPUT_PATH} )

IF ( ${UX_REL} MATCHES "1" ) 
     SET ( CMAKE_CXX_FLAGS "-DOPTIMIZE -O2" )
ELSE ( ${UX_REL} MATCHES "1" ) 
     SET ( CMAKE_CXX_FLAGS "-DDEBUG -g" )
ENDIF ( ${UX_REL} MATCHES "1" )

SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0" )


LINK_DIRECTORIES    ( /usr/X11R6/lib /usr/lib )

INCLUDE_DIRECTORIES ( ${FLTK_INCLUDE_PATH}               )
INCLUDE_DIRECTORIES ( ${FLSCAN_SOURCE_DIR}/build/include )
INCLUDE_DIRECTORIES ( ${SANE_INCLUDE_PATH}               )
INCLUDE_DIRECTORIES ( ${JPEG_INCLUDE_DIR}                )
INCLUDE_DIRECTORIES ( ${PNG_INCLUDE_DIR}                 )

ADD_DEFINITIONS( ${PNG_DEFINITIONS} )
ADD_DEFINITIONS( -D_REENTRANT       )
ADD_DEFINITIONS( -DTHREADED_SAVING  )

LINK_LIBRARIES ( X11                )
LINK_LIBRARIES ( pthread                )
LINK_LIBRARIES ( ${JPEG_LIBRARIES}      )
LINK_LIBRARIES ( ${PNG_LIBRARIES}       )
LINK_LIBRARIES ( ${SANE_LIB_PATH}       )
LINK_LIBRARIES ( ${FLTK_BASE_LIBRARY}   )
LINK_LIBRARIES ( ${FLTK_IMAGES_LIBRARY} )

IF ( CMAKE_HAS_X )
    LINK_LIBRARIES  ( dl Xt m )
    LINK_LIBRARIES  ( ${CMAKE_X_LIBS} )
    ADD_DEFINITIONS ( ${CMAKE_X_CFLAGS} )
ENDIF ( CMAKE_HAS_X )

INSTALL_FILES ( /share/doc/flscan FILES
	README
	INSTALL
)
INSTALL_FILES ( /share/flscan FILES utils/plugin_scanner.sh )

SUBDIRS( build )
