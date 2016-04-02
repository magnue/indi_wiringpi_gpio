cmake_minimum_required(VERSION 2.6)
PROJECT(indi_wiringpi_gpio CXX)

cmake_policy(SET CMP0003 NEW)
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

##################  Paths  ###################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(UDEVRULES_INSTALL_DIR "/lib/udev/rules.d" CACHE STRING "Base directory for udev rules")
set(DATA_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/indi/")


IF(APPLE)
set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
ENDIF(APPLE)

##################  Includes  ################################

include (MacroBoolTo01)

FIND_PACKAGE(INDI REQUIRED)
FIND_PACKAGE(INDIDriver REQUIRED)
FIND_PACKAGE(WiringPi REQUIRED)

macro_bool_to_01(INDI_DRIVER_LIBRARIES HAVE_INDIDRIVER_LIB)
macro_bool_to_01(WIRING_PI_FOUND HAVE_WIRING_PI_H)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

include_directories( ${INDI_INCLUDE_DIR})
include_directories( ${CMAKE_CURRENT_BINARY_DIR})
include_directories( ${CMAKE_SOURCE_DIR}/wiringpi_gpio)

################ USB RELAY2 ##############################

set(wiringpi_gpio_SRCS
        ${CMAKE_SOURCE_DIR}/wiringpi_gpio/wiringpi_gpio.cpp
        ${CMAKE_SOURCE_DIR}/wiringpi_gpio/systeminfo.cpp
        ${CMAKE_SOURCE_DIR}/wiringpi_gpio/systeminfo_updater.cpp
        ${CMAKE_SOURCE_DIR}/wiringpi_gpio/wiringpi_interface.cpp
        ${CMAKE_SOURCE_DIR}/wiringpi_gpio/wiringpi_updater.cpp
   )

add_executable(indi_wiringpi_gpio ${wiringpi_gpio_SRCS})
target_link_libraries(indi_wiringpi_gpio ${INDI_DRIVER_LIBRARIES} ${WIRING_PI_LIBRARIES})
install(TARGETS indi_wiringpi_gpio RUNTIME DESTINATION bin )
install(FILES ${CMAKE_SOURCE_DIR}/wiringpi_gpio/99-wiringpi.rules DESTINATION ${UDEVRULES_INSTALL_DIR})
install( FILES ${CMAKE_SOURCE_DIR}/wiringpi_gpio/indi_wiringpi_gpio.xml DESTINATION ${DATA_INSTALL_DIR})

###########################################################
