# - Try to find the WIRING_PI library, libwiringPi.so
# Once done this defines
#
#  WIRING_PI_FOUND - system has libwiringPi.so
#  WIRING_PI_INCLUDE_DIR - the libwiringPi include directory
#  WIRING_PI_LIBRARIES - Link these to use libwiringPi
#
# Copyright (c) 2016, Magnus W. Eriksen
#
# Based on FindUSBRelayDevice
# Copyright (c) 2015, Magnus W. Eriksen
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING.BSD file.

if (WIRING_PI_INCLUDE_DIR AND WIRING_PI_LIBRARIES)

  	# in cache already
  	set(WIRING_PI_FOUND TRUE)
    message(STATUS "Found WIRING_PI: ${WIRING_PI_LIBRARIES}")

else (WIRING_PI_INCLUDE_DIR AND WIRING_PI_LIBRARIES)

  	find_path(WIRING_PI_INCLUDE_DIR wiringPi.h
      	HINTS ${/usr/local/include}
    )

	find_library(WIRING_PI_LIBRARIES NAMES libwiringPi.so
    	HINTS ${/usr/local/lib}
    )

    set(CMAKE_REQUIRED_INCLUDES ${WIRING_PI_INCLUDE_DIR})
    set(CMAKE_REQUIRED_LIBRARIES ${WIRING_PI_LIBRARIES})

    if(WIRING_PI_INCLUDE_DIR AND WIRING_PI_LIBRARIES)
        set(WIRING_PI_FOUND TRUE)
    else(WIRING_PI_INCLUDE_DIR AND WIRING_PI_LIBRARIES)
        set(WIRING_PI_FOUND FALSE)
    endif (WIRING_PI_INCLUDE_DIR AND WIRING_PI_LIBRARIES)

    if(WIRING_PI_FOUND)
        message(STATUS "Found WIRING_PI: ${WIRING_PI_LIBRARIES}")
    else(WIRING_PI_FOUND)
        message(FATAL_ERROR "WIRING_PI: Not Found!
        The library must be compiled from source,
        see indi_wiringpi_gpio/INSTALL for instructions")
    endif(WIRING_PI_FOUND)

	mark_as_advanced(WIRING_PI_INCLUDE_DIR WIRING_PI_LIBRARIES)

endif (WIRING_PI_INCLUDE_DIR AND WIRING_PI_LIBRARIES)
