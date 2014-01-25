Snoop Component Suite version 9.0
==================================


Introduction
------------

Snoop Component Suite version 9.0 is a core library of network sniffer security tool.
It supports the following network environment.

  1. Capture
       Adapter         : winpcap wrapping class of capturing live nic adapter.
	   File            : winpcap wrapping class of capturing from pcap file.
	   SourcePcap      : winpcap wrapping class of base winpcap feature.
	   SnoopRemote     : winpcap wrapping class of capturing from remote host.
	   SnoopVirtualNat : virtual class of nat device.
	   SnoopWinDivert  : windivert wrapping cass.

  2. Filter
       BpFilter        : berkley packet filter
	   ProcessFilter   : process packet filter

  3. Process
       Block           : block packets
	   Delay           : delay packets
	   Dump            : dump packet into pcap file
	   TcpBlock        : block tcp packets using RST, FIN and PSH flags
	   WriteAdapter    : copy packet into another adapter


Installation
------------


About
-----


License
-------
