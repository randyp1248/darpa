#!/usr/bin/env python
#
# Copyright 2005-2007,2011 Free Software Foundation, Inc.
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
# 

from gnuradio import gr, gru
from gnuradio import eng_notation
from gnuradio import digital

import copy
import sys
import collections

# /////////////////////////////////////////////////////////////////////////////
#                              receive path
# /////////////////////////////////////////////////////////////////////////////


class circular_buffer_block(gr.hier_block2):
    def __init__(self, args):
        gr.block.__init__(self, name="circular_buffer_block", in_sig=[numpy.float32], out_sig=[])
        self.set_auto_consume(False)
        self.deque = collections.deque(maxlen=100)

    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = noutput_items

    def work(self, input_items, output_items):
        #buffer references
        in0 = input_items[0][:len(output_items[0])]
        deque.append(in0)
        
        #consume the inputs
        self.consume(0, 1) #consume port 0 input

    def sum(self):
        s=0.0;
        for i in xrange(100):
            s += deque(i)
        return s;


class receive_path(gr.hier_block2):
    def __init__(self, demod_class, rx_callback, options):
        gr.hier_block2.__init__(self, "receive_path",
                                gr.io_signature(1, 1, gr.sizeof_gr_complex),
                                gr.io_signature(0, 0, 0))


        low_pass_taps = [
            -0.011857823032443,
            -0.128315103082374,
            -0.170385954727615,
            0.000440143383084,
            0.325831110288658,
            0.495630065579598,
            0.325831110288658,
            0.000440143383084,
            -0.170385954727615,
            -0.128315103082374,
            -0.011857823032443]

        high_pass_taps = [
            0.011857823032443,
            -0.128315103082374,
            0.170385954727615,
            0.000440143383085,
            -0.325831110288658,
            0.495630065579598,
            -0.325831110288658,
            0.000440143383085,
            0.170385954727615,
            -0.128315103082374,
            0.011857823032443]

        self.lp = gr.filter.fir_filter_ccf(1, low_pass_taps)
        self.hp = gr.filter.fir_filter_ccf(1, high_pass_taps)

        power_low_pass = gr.blocks.complex_to_mag_squared()
        power_high_pass = gr.blocks.complex_to_mag_squared()

        power_low_pass_buf = circular_buffer_block()
        power_high_pass_buf = circular_buffer_block()

        self.connect(self, self.lp, low_power, power_low_pass_buf)
        self.connect(self, self.hp, high_power, power_high_pass_buf)
        
        options = copy.copy(options)    # make a copy so we can destructively modify

        self._verbose     = options.verbose
        self._bitrate     = options.bitrate  # desired bit rate

        self._rx_callback = rx_callback  # this callback is fired when a packet arrives
        self._demod_class = demod_class  # the demodulator_class we're using

        self._chbw_factor = options.chbw_factor # channel filter bandwidth factor

        # Get demod_kwargs
        demod_kwargs = self._demod_class.extract_kwargs_from_options(options)

        # Build the demodulator
        self.demodulator = self._demod_class(**demod_kwargs)

        # Make sure the channel BW factor is between 1 and sps/2
        # or the filter won't work.
        if(self._chbw_factor < 1.0 or self._chbw_factor > self.samples_per_symbol()/2):
            sys.stderr.write("Channel bandwidth factor ({0}) must be within the range [1.0, {1}].\n".format(self._chbw_factor, self.samples_per_symbol()/2))
            sys.exit(1)
        
        # Display some information about the setup
        if self._verbose:
            self._print_verbage()


    def bitrate(self):
        return self._bitrate

    def samples_per_symbol(self):
        return self.demodulator._samples_per_symbol

    def differential(self):
        return self.demodulator._differential

    def carrier_sensed(self):
        """
        Return True if we think carrier is present.
        """
        #return self.probe.level() > X
        return self.probe.unmuted()

    def carrier_threshold(self):
        """
        Return current setting in dB.
        """
        return self.probe.threshold()

    def set_carrier_threshold(self, threshold_in_db):
        """
        Set carrier threshold.

        @param threshold_in_db: set detection threshold
        @type threshold_in_db:  float (dB)
        """
        self.probe.set_threshold(threshold_in_db)
    
        
    def add_options(normal, expert):
        """
        Adds receiver-specific options to the Options Parser
        """
        if not normal.has_option("--bitrate"):
            normal.add_option("-r", "--bitrate", type="eng_float", default=400e3,
                              help="specify bitrate [default=%default].")
        normal.add_option("-v", "--verbose", action="store_true", default=False)
        expert.add_option("-S", "--samples-per-symbol", type="float", default=2,
                          help="set samples/symbol [default=%default]")
        expert.add_option("", "--log", action="store_true", default=False,
                          help="Log all parts of flow graph to files (CAUTION: lots of data)")
        expert.add_option("", "--chbw-factor", type="float", default=1.0,
                          help="Channel bandwidth = chbw_factor x signal bandwidth [defaut=%default]")

    # Make a static method to call before instantiation
    add_options = staticmethod(add_options)


    def _print_verbage(self):
        """
        Prints information about the receive path
        """
        print "\nReceive Path:"
        print "modulation:      %s"    % (self._demod_class.__name__)
        print "bitrate:         %sb/s" % (eng_notation.num_to_str(self._bitrate))
        print "samples/symbol:  %.4f"    % (self.samples_per_symbol())
        print "Differential:    %s"    % (self.differential())

