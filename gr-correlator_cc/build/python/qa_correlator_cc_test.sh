#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/kenchin/gnuradio/gnuradio/darpa/gr-correlator_cc/python
export PATH=/home/kenchin/gnuradio/gnuradio/darpa/gr-correlator_cc/build/python:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=/home/kenchin/gnuradio/gnuradio/darpa/gr-correlator_cc/build/swig:$PYTHONPATH
/usr/bin/python /home/kenchin/gnuradio/gnuradio/darpa/gr-correlator_cc/python/qa_correlator_cc.py 
