<loopback>
  <graph>
    <objectList>
      <object queueLen="8192" correctChecksum="true" queueTime="1024" _class="SnoopWinDivert" filter="ip.SrcAddr == 127.0.0.1 or ip.DstAddr == 127.0.0.1" priority="0" tos="0" name="WinDivert1" flags="1" layer="0" autoParse="true" autoRead="true"/>
      <object adapterIndex="1" snapLen="1600" _class="SnoopWriteAdapter" filter="" readTimeout="1" name="WriteAdapter1" flags="1" autoParse="true" autoRead="false"/>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" receiver="WriteAdapter1" sender="WinDivert1" slot="output(SnoopPacket*)"/>
    </connectList>
  </graph>
  <coord>
    <object x="-75" y="-204" name="WinDivert1"/>
    <object x="-84" y="-157" name="WriteAdapter1"/>
  </coord>
</loopback>
