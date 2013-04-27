#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/kenchin/gnuradio/gnuradio/darpa/gr-correlator_cc/lib
export PATH=/home/kenchin/gnuradio/gnuradio/darpa/gr-correlator_cc/build/lib:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-correlator_cc 
