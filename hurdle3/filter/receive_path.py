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
import filter_swig as filter

# /////////////////////////////////////////////////////////////////////////////
#                              receive path
# /////////////////////////////////////////////////////////////////////////////

class circular_buffer_block(gr.gr_basic_block):
    def __init__(self):
#        gr.gr_basic_block.__init__(self, "receive_path",
#                                gr.io_signature(1, 1, 4),
#                                gr.io_signature(0, 0, 0))
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
        
        # Design filter to get actual channel we want
        sw_decim = 1
        chan_coeffs = gr.firdes.low_pass (1.0,                  # gain
                                          sw_decim * self.samples_per_symbol(), # sampling rate
                                          self._chbw_factor,    # midpoint of trans. band
                                          0.5,                  # width of trans. band
                                          gr.firdes.WIN_HANN)   # filter type
        self.channel_filter = gr.fft_filter_ccc(sw_decim, chan_coeffs)
        
        # receiver
        self.packet_receiver = \
            digital.demod_pkts(self.demodulator,
                               access_code=None,
                               callback=self._rx_callback,
                               threshold=-1)

        # Carrier Sensing Blocks
        alpha = 0.001
        thresh = 30   # in dB, will have to adjust
        self.probe_lp = gr.probe_avg_mag_sqrd_c(thresh,alpha)
        self.probe_hp = gr.probe_avg_mag_sqrd_c(thresh,alpha)

        # Display some information about the setup
        if self._verbose:
            self._print_verbage()

        low_pass_taps = [
   -0.0401, 
    0.0663, 
    0.0468, 
   -0.0235, 
   -0.0222, 
    0.0572, 
    0.0299, 
   -0.1001, 
   -0.0294, 
    0.3166, 
    0.5302, 
    0.3166, 
   -0.0294, 
   -0.1001, 
    0.0299, 
    0.0572, 
   -0.0222, 
   -0.0235, 
    0.0468, 
    0.0663,
   -0.0401]

        high_pass_taps = [
   -0.0389,
   -0.0026, 
    0.0302, 
    0.0181,
   -0.0357,
   -0.0394,
    0.0450,
    0.0923,
   -0.0472,
   -0.3119,
    0.5512,
   -0.3119,
   -0.0472,
    0.0923,
    0.0450,
   -0.0394,
   -0.0357,
    0.0181,
    0.0302,
   -0.0026,
   -0.0389]

        #self.lp = filter.adaptive_fir_ccf("lp", 1, low_pass_taps)
        #self.hp = filter.adaptive_fir_ccf("hp", 1, high_pass_taps)

        self.lp = gr.fft_filter_ccc(1, low_pass_taps)
        self.hp = gr.fft_filter_ccc(1, high_pass_taps)

        self.power_low_pass = gr.complex_to_mag_squared()
        self.power_high_pass = gr.complex_to_mag_squared()

        self.power_low_pass_buf = circular_buffer_block()
        self.power_high_pass_buf = circular_buffer_block()

        self.connect(self, self.lp)
        self.connect(self.lp, self.probe_lp)
        self.connect(self, self.hp)
        self.connect(self.hp, self.probe_hp)
        #self.connect(self.power_low_pass, self.power_low_pass_buf)
        #self.connect(self, self.lp, self.power_low_pass, self.power_low_pass_buf)
        #self.connect(self, self.hp, self.power_high_pass, self.power_high_pass_buf)

	# connect block input to channel filter
	#self.connect(self, self.channel_filter)

        # connect the channel input filter to the carrier power detector
        #self.connect(self.channel_filter, self.probe)

        # connect channel filter to the packet receiver
        #self.connect(self.channel_filter, self.packet_receiver)

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

