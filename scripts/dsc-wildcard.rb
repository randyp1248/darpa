# To run with default defProperty values:
# > omf-5.4 exec dsc-wildcard.rb
# To override defProperty values from command line:
# > omf-5.4 exec dsc-wildcard.rb -- --team1 dsc-teamA --team2 dsc-teamC --runtime 20
defProperty('team1','dsc-teamA','Team 1 nodes')
defProperty('team2','dsc-teamB','Team 2 nodes')

defProperty('freq', '1700000000', "Center frequency")
defProperty('server', 'localhost', "name of pkt server")
defProperty('port', '5123', "pkt server port")
defProperty('runtime', 100, "Run time (s)")

# Two topologies for the competitive tournament

team1 = Topology["system:topo:#{property.team1}"]
team2 = Topology["system:topo:#{property.team2}"]

defApplication('test:app:bot1_rx', 'bot1_rx.py') { |a|
  a.version(2, 0, 4)
  a.shortDescription = ""
  a.description = ""
  a.path = "export LC_ALL=C;/root/gnuradio/gr-digital/examples/narrowband/bot1_rx.py"
  a.defProperty('args', "Argument list", nil,
                {:dynamic => false, :type => :string})
  a.defProperty('freq', "center frequency in Hz", '-f',
                {:dynamic => false, :type => :string})
  a.defProperty('rx-gain', "receive gain in dB", '--rx-gain', 
                {:dynamic => false, :type => :string})
  a.defProperty('bitrate', "specify bitrate", '-r',
                {:dynamic => false, :type => :string})
  a.defProperty('modulation', "modulation: psk, cpm, qpsk, dqpsk, gfsk,qam, dbpsk, bpsk, gmsk [default=psk]", '-m', 
                {:dynamic => false, :type => :string})
  a.defProperty('constellation-points', "set constellation - power of two for psk, power of 4 for QAM [default=16]", '-p', 
                {:dynamic => false, :type => :string})
  a.defProperty('server', "server adress", '-s',
                {:dynamic => false, :type => :string})
  a.defProperty('port', "port number", '-o',
                {:dynamic => false, :type => :string})
}

defApplication('test:app:bot1_tx', 'bot1_tx.py') { |a|
  a.version(2, 0, 4)
  a.shortDescription = ""
  a.description = ""
  a.path = "export LC_ALL=C;/root/gnuradio/gr-digital/examples/narrowband/bot1_tx.py"
  a.defProperty('args', "Argument list", nil,
                {:dynamic => false, :type => :string})
  a.defProperty('freq', "center frequency in Hz", '-f',
                {:dynamic => false, :type => :string})
  a.defProperty('tx-gain', "transmit gain in dB", '--tx-gain', 
                {:dynamic => false, :type => :string})
  a.defProperty('tx-amplitude', "transmitter digital amplitude [0,1)  [default=0.25]", '--tx-amplitude', 
                {:dynamic => false, :type => :string})
  a.defProperty('bitrate', "specify bitrate", '-r',
                {:dynamic => false, :type => :string})
  a.defProperty('modulation', "modulation: psk, cpm, qpsk, dqpsk, gfsk,qam, dbpsk, bpsk, gmsk [default=psk]", '-m', 
                {:dynamic => false, :type => :string})
  a.defProperty('constellation-points', "set constellation - power of two for psk, power of 4 for QAM [default=16]", '-p',
                {:dynamic => false, :type => :string})
  a.defProperty('server', "server adress", '-s',
                {:dynamic => false, :type => :string})
  a.defProperty('port', "port number", '-o',
                {:dynamic => false, :type => :string})
  a.defProperty('burst', "number of packets in each burst", '-b',
                {:dynamic => false, :type => :string})
  a.defProperty('sleep', "msec between bursts", '-t',
                {:dynamic => false, :type => :string})
}

defGroup('rx_node1', team1.getNodeByIndex(0).to_s ) { |n|
 n.addApplication('test:app:bot1_rx') { |app|
   app.setProperty('freq', property.freq)
   app.setProperty('rx-gain','30')
   app.setProperty('modulation','bpsk')
   app.setProperty('bitrate','1.25M')
   app.setProperty('server','10.10.0.10')
   app.setProperty('port','5125')
 }
}

defGroup('tx_node1', team1.getNodeByIndex(1).to_s ) { |n|
 n.addApplication('test:app:bot1_tx') { |app|
   app.setProperty('freq', property.freq)
   app.setProperty('tx-gain','30')
   app.setProperty('modulation','bpsk')
   app.setProperty('bitrate','1.25M')
   app.setProperty('tx-amplitude','0.5')
   app.setProperty('server','10.10.0.10')  
   app.setProperty('port','5123')
   app.setProperty('burst','100')
   app.setProperty('sleep','1000')
 }
}

defGroup('rx_node2', team2.getNodeByIndex(0).to_s ) { |n|
 n.addApplication('test:app:bot1_rx') { |app|
   app.setProperty('freq', property.freq)
   app.setProperty('rx-gain','30')
   app.setProperty('modulation','dqpsk')
   app.setProperty('bitrate','2.5M')
   app.setProperty('server','10.10.0.10')  
   app.setProperty('port','5125')
 }
}

defGroup('tx_node2', team2.getNodeByIndex(1).to_s ) { |n|
 n.addApplication('test:app:bot1_tx') { |app|
   app.setProperty('freq', property.freq)
   app.setProperty('tx-gain','30')
   app.setProperty('modulation','dqpsk')
   app.setProperty('bitrate','2.5M')
   app.setProperty('tx-amplitude','0.5')
   app.setProperty('server','10.10.0.10')  
   app.setProperty('port','5123')
   app.setProperty('burst','100')
   app.setProperty('sleep','2000')
 }
}

#onEvent(:ALL_UP) { 
onEvent(:ALL_UP_AND_INSTALLED) { |event|
  info "Give machines some time to warm up" 
  wait 1

  info "Start packet server"
  consoleExec("/usr/local/bin/pkt_server_bot --port 5123 --duration #{property.runtime+5} --team1 #{property.team1} --team2 #{property.team2} &")

  wait 2
  info "Start benchmark_rx,tx"
  allGroups.startApplications

  wait 5
  info "send signal to packet server to start serving packets"
  consoleExec("/usr/local/bin/dsc_send_start.py &") # may be rename this signal
  
  wait property.runtime
  
  info "Stop eveything"
  allGroups.stopApplications

  wait  2
  consoleExec("ssh root@#{team1.getNodeByIndex(0).to_s} 'pkill python' ") # Just in case kill rx side
  consoleExec("ssh root@#{team2.getNodeByIndex(0).to_s} 'pkill python' ") # Just in case kill rx side
  
  info "Finish it." 
  Experiment.done
}
