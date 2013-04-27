from gnuradio import gr
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser

from gnuradio import digital

import struct

# from current dir
from transmit_path import transmit_path

class loop_top_block(gr.top_block):
    def __init__(self, modulator, options):
        gr.top_block.__init__(self)

        print "Saving samples to '%s'.\n" % (options.samples_file)
        self.sink = gr.file_sink(gr.sizeof_gr_complex, options.samples_file)

        self.txpath = transmit_path(modulator, options)
        self.connect(self.txpath, self.sink)

def main():
    global file
    
    def send_pkt(payload='', eof=False):
        return tb.txpath.send_pkt(payload, eof)

    mods = digital.modulation_utils.type_1_mods()

    parser = OptionParser(option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

    parser.add_option("-m", "--modulation", type="choice", choices=mods.keys(),
                      default='psk',
                      help="Select modulation from :%s [default=%%default]"
                            % (', '.join(mods.keys()),))

    parser.add_option("-s", "--size", type="eng_float", default=1500,
                      help="set packet size [default=%default]")
    parser.add_option("","--from-file", default=None,
                      help="Use input file for packet contents")
    parser.add_option("","--samples-file", default=None,
                      help="Output file for modulated samples")

    transmit_path.add_options(parser, expert_grp)
    for mod in mods.values():
        mod.add_options(expert_grp)

    (options, args) = parser.parse_args()

    if len(args) != 0:
       parser.print_help()
       sys.exit(1)

    if options.from_file is not None: 
        source_file = open(options.from_file, 'r')
  
    tb = loop_top_block(mods[options.modulation], options)

    # do we need to do this for our test bench?
    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: failed to enable realtime scheduling"
    
    tb.start()
 
    pkt_size = int(options.size)
    
    while True:
        data = source_file.read(pkt_size)
        if data == '':
            break;

        #payload = struct.pack('!H', pktno & 0xffff) + data
        payload = data
        send_pkt(payload)

if __name__ == '__main__':
    try: 
        main()
    except KeyboardInterrupt:
        pass
