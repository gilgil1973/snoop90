<block>
  <graph>
    <objectList>
      <object layer="0" correctChecksum="true" autoRead="true" autoParse="true" name="WinDivert1" queueLen="8192" queueTime="1024" enabled="true" priority="0" _class="SnoopWinDivert" filter="udp and outbound and ip.DstAddr!=127.0.0.1" tos="0" flags="0"/>
      <object name="Block1" dropRate="75" _class="SnoopBlock"/>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" sender="WinDivert1" slot="block(SnoopPacket*)" receiver="Block1"/>
    </connectList>
  </graph>
  <coord>
    <object name="WinDivert1" x="-2" y="-143"/>
    <object name="Block1" x="8" y="-104"/>
  </coord>
</block>
