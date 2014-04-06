<loopback>
  <graph>
    <objectList>
      <object autoParse="true" flags="0" layer="0" autoRead="true" queueLen="8192" queueTime="1024" correctChecksum="true" _class="SnoopWinDivert" filter="ip.SrcAddr == 127.0.0.1 or ip.DstAddr == 127.0.0.1" name="WinDivert1" priority="0" tos="0"/>
      <object adapterIndex="1" srcMac="000000-000000" autoParse="true" flags="1" dstMac="000000-000000" snapLen="1600" autoRead="false" _class="SnoopWriteAdapter" filter="" name="WriteAdapter1" readTimeout="1"/>
      <object linkType="1" _class="SnoopBpFilter" filter="" name="BpFilter1"/>
    </objectList>
    <connectList>
      <connect receiver="BpFilter1" sender="WinDivert1" slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect receiver="WriteAdapter1" sender="BpFilter1" slot="copy(SnoopPacket*)" signal="ack(SnoopPacket*)"/>
    </connectList>
  </graph>
  <coord>
    <object name="WinDivert1" x="-76" y="-205"/>
    <object name="WriteAdapter1" x="-84" y="-115"/>
    <object name="BpFilter1" x="-70" y="-158"/>
  </coord>
</loopback>
