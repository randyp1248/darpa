from gnuradio import gr
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser

from gnuradio import digital

import struct

import howto
from gnuradio import gr_unittest

# from current dir
from receive_path import receive_path

class loop_top_block(gr.top_block):
    def __init__(self, demodulator, options, rx_callback):
        gr.top_block.__init__(self)

        self.source = gr.file_source(gr.sizeof_gr_complex, options.samples_file)
        self.rxpath = receive_path(demodulator, rx_callback, options)
        self.connect(self.source, self.rxpath)

def main():
    global file
    
    def rx_callback(ok, payload):
        if ok:
            file.write(payload)

    demods = digital.modulation_utils.type_1_demods()

    parser = OptionParser(option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

    parser.add_option("-m", "--modulation", type="choice", choices=demods.keys(),
                      default='psk',
                      help="Select modulation from :%s [default=%%default]"
                            % (', '.join(demods.keys()),))

    parser.add_option("-s", "--size", type="eng_float", default=1500,
                      help="set packet size [default=%default]")
    parser.add_option("","--samples-file", default=None,
                      help="Output file for modulated samples")
    parser.add_option("","--to-file", default=None,
                      help="Output file for demodulated contents")

    receive_path.add_options(parser, expert_grp)

    for mod in demods.values():
        mod.add_options(expert_grp)
 
    (options, args) = parser.parse_args()

    if len(args) != 0:
       parser.print_help()
       sys.exit(1)

    if options.to_file is not None:
        file = open(options.to_file, "w+")  
 
    tb = loop_top_block(demods[options.modulation], options, rx_callback)

    # do we need to do this for our test bench?
    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: failed to enable realtime scheduling"
    
    tb.start()
    tb.wait()
 

if __name__ == '__main__':
    try: 
        main()
    except KeyboardInterrupt:
        pass
