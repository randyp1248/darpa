# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND rscoding_bb_sources
    encode_bb_impl.cc
    decode_bb_impl.cc
)

add_library(gnuradio-rscoding_bb SHARED ${rscoding_bb_sources})
target_link_libraries(gnuradio-rscoding_bb ${Boost_LIBRARIES} ${GRUEL_LIBRARIES} ${GNURADIO_CORE_LIBRARIES})
set_target_properties(gnuradio-rscoding_bb PROPERTIES DEFINE_SYMBOL "gnuradio_rscoding_bb_EXPORTS")

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-rscoding_bb
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_rscoding_bb_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_rscoding_bb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_rscoding_bb.cc
)

add_executable(test-rscoding_bb ${test_rscoding_bb_sources})

target_link_libraries(
  test-rscoding_bb
  ${GNURADIO_CORE_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-rscoding_bb
)

GR_ADD_TEST(test_rscoding_bb test-rscoding_bb)
