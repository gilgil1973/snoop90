<udp_receiver_test>
  <graph>
    <objectList>
      <object _class="SnoopWinDivert" filter="udp and inbound and ip.DstAddr!=127.0.0.1" correctChecksum="true" priority="0" autoParse="true" flags="0" name="WinDivert1" layer="0" autoRead="true" queueTime="1024" enabled="true" queueLen="8192" tos="0"/>
      <object _class="SnoopBpFilter" filter="" linkType="1" name="BpFilter1"/>
      <object _class="SnoopFlowMgr" ipFlowTimeout="300" name="FlowMgr1" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" checkInterval="1"/>
      <object _class="SnoopUdpReceiver" autoBlockOnSplit="true" dscr="GGGG" name="UdpReceiver1" headerSize="2" flowMgr="FlowMgr1" writer="WinDivert1"/>
      <object _class="SnoopWriteAdapter" filter="" autoParse="false" snapLen="1600" flags="1" readTimeout="1" srcMac="000000-000000" name="WriteAdapter1" autoRead="false" dstMac="000000-000000" enabled="true"/>
    </objectList>
    <connectList>
      <connect slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="BpFilter1" sender="WinDivert1"/>
      <connect slot="process(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="FlowMgr1" sender="BpFilter1"/>
      <connect slot="split(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="UdpReceiver1" sender="FlowMgr1"/>
      <connect slot="copy(SnoopPacket*)" signal="splitted(SnoopPacket*)" receiver="WriteAdapter1" sender="UdpReceiver1"/>
    </connectList>
  </graph>
  <coord>
    <object name="WinDivert1" x="-55" y="-163"/>
    <object name="BpFilter1" x="-49" y="-125"/>
    <object name="FlowMgr1" x="-54" y="-86"/>
    <object name="UdpReceiver1" x="-66" y="-50"/>
    <object name="WriteAdapter1" x="-67" y="-8"/>
  </coord>
</udp_receiver_test>
