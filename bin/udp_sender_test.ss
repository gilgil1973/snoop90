<udp_sender_test>
  <graph>
    <objectList>
      <object enabled="true" autoRead="true" autoParse="true" queueLen="8192" name="WinDivert1" correctChecksum="true" queueTime="1024" priority="0" flags="0" tos="0" layer="0" _class="SnoopWinDivert" filter="udp and outbound and ip.DstAddr!=127.0.0.1"/>
      <object linkType="1" name="BpFilter1" _class="SnoopBpFilter" filter=""/>
      <object macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="FlowMgr1" _class="SnoopFlowMgr" checkInterval="1"/>
      <object headerSize="2" writer="WinDivert1" dscr="GGGG" name="UdpSender1" addChunkCount="5" _class="SnoopUdpSender" flowMgr="FlowMgr1"/>
      <object enabled="true" autoRead="false" autoParse="false" srcMac="000000-000000" snapLen="1600" dstMac="000000-000000" name="WriteAdapter1" readTimeout="1" flags="1" _class="SnoopWriteAdapter" filter=""/>
      <object dropRate="75" name="Block1" _class="SnoopBlock"/>
    </objectList>
    <connectList>
      <connect slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)" sender="WinDivert1" receiver="BpFilter1"/>
      <connect slot="process(SnoopPacket*)" signal="ack(SnoopPacket*)" sender="BpFilter1" receiver="FlowMgr1"/>
      <connect slot="merge(SnoopPacket*)" signal="processed(SnoopPacket*)" sender="FlowMgr1" receiver="UdpSender1"/>
      <connect slot="copy(SnoopPacket*)" signal="merged(SnoopPacket*)" sender="UdpSender1" receiver="WriteAdapter1"/>
      <connect slot="block(SnoopPacket*)" signal="merged(SnoopPacket*)" sender="UdpSender1" receiver="Block1"/>
    </connectList>
  </graph>
  <coord>
    <object x="-53" y="-163" name="WinDivert1"/>
    <object x="-49" y="-125" name="BpFilter1"/>
    <object x="-54" y="-86" name="FlowMgr1"/>
    <object x="-61" y="-46" name="UdpSender1"/>
    <object x="42" y="-47" name="WriteAdapter1"/>
    <object x="-45" y="-7" name="Block1"/>
  </coord>
</udp_sender_test>
