# Optionally find and configure OpenCV dependency

option( KWIVER_ENABLE_OPENCV
  "Enable OpenCV dependent code and plugins (Arrows)"
  ${fletch_ENABLED_OpenCV}
  )

set( USE_OPENCV False )
if( KWIVER_ENABLE_OPENCV )
  find_package( OpenCV REQUIRED )
  include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})

  if( OpenCV_VERSION VERSION_GREATER "2.4.6" )
    set( USE_OPENCV True )
    if( OpenCV_VERSION VERSION_GREATER "4.0" OR
        OpenCV_VERSION VERSION_EQUAL "4.0")
      message(STATUS "Found OPENCV 4.x")
      set (KWIVER_OPENCV_VERSION_MAJOR 4)
    elseif( OpenCV_VERSION VERSION_GREATER "3.0" OR
            OpenCV_VERSION VERSION_EQUAL "3.0")
      message(STATUS "Found OPENCV 3.x")
      set (KWIVER_OPENCV_VERSION_MAJOR 3)
    else()
      message(STATUS "Found OPENCV 2.x")
      set (KWIVER_OPENCV_VERSION_MAJOR 2)
    endif()

  else()
    message( FATAL_ERROR "OpenCV version must be at least 2.4.6" )
  endif()
endif( KWIVER_ENABLE_OPENCV )

  ##
