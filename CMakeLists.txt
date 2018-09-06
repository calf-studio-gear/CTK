
cmake_minimum_required (VERSION 2.6)

project (CTK)

set(CTK_VERSION_MAJOR "0")
set(CTK_VERSION_MINOR "0")
set(CTK_VERSION_PATCH "1")

set(CTK_VERSION "${CTK_VERSION_MAJOR}.${CTK_VERSION_MINOR}.${CTK_VERSION_PATCH}")

option(CTK_RELEASE_BUILD   "Build for releasing"                                        ON  )
option(CTK_DEBUG_MAIN      "Build with debugging prints for main UI functionality"      OFF )
option(CTK_DEBUG_DRAW      "Build with debugging prints for rendering and compositing"  OFF )
option(CTK_DEBUG_EVENT     "Build with debugging prints for event handling"             OFF )
option(CTK_DEBUG_LAYOUT    "Build with debugging prints for layout purposes"            OFF )
option(CTK_TEST            "Build test program"                                         ON  )


################################################################################
### Code compilation starts here
################################################################################

ADD_DEFINITIONS( "-DCTK_VERSION_STRING=\"${CTK_VERSION}\"" )

include_directories ("${PROJECT_SOURCE_DIR}/CTK/")

find_package(PkgConfig)

IF(CTK_DEBUG_MAIN)
  ADD_DEFINITIONS( "-DDEBUG_MAIN" )
ENDIF()
IF(CTK_DEBUG_DRAW)
  ADD_DEFINITIONS( "-DDEBUG_DRAW" )
ENDIF()
IF(CTK_DEBUG_EVENT)
  ADD_DEFINITIONS( "-DDEBUG_EVENT" )
ENDIF()
IF(CTK_DEBUG_LAYOUT)
  ADD_DEFINITIONS( "-DDEBUG_LAYOUT" )
ENDIF()

ADD_DEFINITIONS( "-DPUGL_HAVE_CAIRO" )
pkg_check_modules(CAIRO cairo REQUIRED)
include_directories( ${CAIRO_INCLUDE_DIRS})
link_directories   ( ${CAIRO_LIBRARY_DIRS})

pkg_check_modules(X11 x11 REQUIRED)
include_directories( ${X11_INCLUDE_DIRS} )
link_directories   ( ${X11_LIBRARY_DIRS} )

SET(CMAKE_CXX_FLAGS "-msse -msse2 -mfpmath=sse -g") # -fsanitize=address

FILE(GLOB src CTK/*.cxx CTK/pugl/pugl_x11.c)

ADD_LIBRARY( CTK STATIC ${src} )

IF(CTK_TEST)
  ADD_EXECUTABLE( CTKDemo demo.cxx main.cxx)
  target_link_libraries( CTKDemo CTK  )
  target_link_libraries( CTKDemo ${CAIRO_LIBRARIES}  )
  target_link_libraries( CTKDemo ${X11_LIBRARIES}    )
ENDIF()


