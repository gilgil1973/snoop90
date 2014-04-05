<loopback>
  <graph>
    <objectList>
      <object queueLen="8192" name="WinDivert1" queueTime="1024" tos="0" priority="0" _class="SnoopWinDivert" filter="ip.SrcAddr == 127.0.0.1 or ip.DstAddr == 127.0.0.1" flags="0" layer="0" autoParse="true" autoRead="true" correctChecksum="true"/>
      <object name="WriteAdapter1" _class="SnoopWriteAdapter" filter="" snapLen="1600" flags="1" readTimeout="1" autoParse="true" autoRead="false" adapterIndex="1"/>
      <object name="BpFilter1" _class="SnoopBpFilter" filter="tcp or icmp or (udp and not udp port 8908)" linkType="1"/>
    </objectList>
    <connectList>
      <connect receiver="WriteAdapter1" slot="output(SnoopPacket*)" signal="ack(SnoopPacket*)" sender="BpFilter1"/>
      <connect receiver="BpFilter1" slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)" sender="WinDivert1"/>
    </connectList>
  </graph>
  <coord>
    <object name="WinDivert1" x="-76" y="-205"/>
    <object name="WriteAdapter1" x="-84" y="-115"/>
    <object name="BpFilter1" x="-82" y="-158"/>
  </coord>
</loopback>
