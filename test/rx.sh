#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/tritons/Tritons_Darpa/gnuradio/darpa/test
export PATH=/home/tritons/Tritons_Darpa/gnuradio/darpa/test:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=/home/tritons/Tritons_Darpa/gnuradio/darpa/gr-correlator_cc/build/swig:$PYTHONPATH
/usr/bin/python ./rx.py
