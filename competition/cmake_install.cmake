# Install script for directory: /home/randyp/school/capstone/gnuradio/darpa/competition

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "uhd_examples")
  IF(EXISTS "$ENV{DESTDIR}/home/randyp/school/capstone/gnuradio/darpa/competition/tags_demo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/randyp/school/capstone/gnuradio/darpa/competition/tags_demo")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/randyp/school/capstone/gnuradio/darpa/competition/tags_demo"
         RPATH "")
  ENDIF()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/randyp/school/capstone/gnuradio/darpa/competition/tags_demo")
FILE(INSTALL DESTINATION "/home/randyp/school/capstone/gnuradio/darpa/competition" TYPE EXECUTABLE FILES "/home/randyp/school/capstone/gnuradio/darpa/competition/tags_demo")
  IF(EXISTS "$ENV{DESTDIR}/home/randyp/school/capstone/gnuradio/darpa/competition/tags_demo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/randyp/school/capstone/gnuradio/darpa/competition/tags_demo")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/randyp/school/capstone/gnuradio/darpa/competition/tags_demo")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "uhd_examples")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/randyp/school/capstone/gnuradio/darpa/competition/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/randyp/school/capstone/gnuradio/darpa/competition/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
