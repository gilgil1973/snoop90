<http_sniff>
  <graph>
    <objectList>
      <object autoParse="true" priority="0" correctChecksum="true" name="WinDivert1" flags="0" queueTime="1024" _class="SnoopWinDivert" filter="tcp.DstPort==80 or tcp.DstPort==443 or tcp.SrcPort==8080 or tcp.SrcPort==4433" layer="0" autoRead="true" tos="0" queueLen="8192"/>
      <object name="BpFilter1" _class="SnoopBpFilter" filter="dst port 80 or dst port 443" linkType="1"/>
      <object name="BpFilter2" _class="SnoopBpFilter" filter="src port 8080 or src port 4433" linkType="1"/>
      <object name="FlowMgr1" _class="SnoopFlowMgr" checkInterval="1" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300"/>
      <object name="FlowMgr2" _class="SnoopFlowMgr" checkInterval="1" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300"/>
      <object name="ProcessFilter1" showStatus="true" _class="SnoopProcessFilter" flowMgr="FlowMgr1">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="firefox.exe" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="iexplore.exe" ack="false"/>
          <policy processName="netclient.exe" ack="false"/>
          <policy processName="netserver.exe" ack="false"/>
          <policy processName="svchost.exe" ack="false"/>
        </policies>
      </object>
      <object name="ProcessFilter2" showStatus="true" _class="SnoopProcessFilter" flowMgr="FlowMgr2">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="netserver.exe" ack="false"/>
        </policies>
      </object>
      <object toFlowMgr="FlowMgr2" tcpChange="true" name="FlowChange1" _class="SnoopFlowChange" udpChange="true" fromFlowMgr="FlowMgr1">
        <changeItems>
          <item dstPortFixValue="8080" srcPort="0" srcPortChangeType="1" dstIpFixValue="127.0.0.1" srcIpChangeType="1" dstIpMask="0.0.0.0" dstIpChangeType="1" log="false" srcIpMask="0.0.0.0" dstPortChangeType="2" protocol="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstIp="0.0.0.0" enabled="true" srcIp="0.0.0.0" dstPort="80"/>
          <item dstPortFixValue="4433" srcPort="0" srcPortChangeType="1" dstIpFixValue="127.0.0.1" srcIpChangeType="1" dstIpMask="0.0.0.0" dstIpChangeType="1" log="false" srcIpMask="0.0.0.0" dstPortChangeType="2" protocol="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstIp="0.0.0.0" enabled="true" srcIp="0.0.0.0" dstPort="443"/>
        </changeItems>
      </object>
    </objectList>
    <connectList>
      <connect receiver="BpFilter1" signal="captured(SnoopPacket*)" slot="check(SnoopPacket*)" sender="WinDivert1"/>
      <connect receiver="FlowMgr1" signal="ack(SnoopPacket*)" slot="process(SnoopPacket*)" sender="BpFilter1"/>
      <connect receiver="BpFilter2" signal="captured(SnoopPacket*)" slot="check(SnoopPacket*)" sender="WinDivert1"/>
      <connect receiver="FlowMgr2" signal="ack(SnoopPacket*)" slot="process(SnoopPacket*)" sender="BpFilter2"/>
      <connect receiver="ProcessFilter2" signal="processed(SnoopPacket*)" slot="check(SnoopPacket*)" sender="FlowMgr2"/>
      <connect receiver="FlowChange1" signal="ack(SnoopPacket*)" slot="processToFrom(SnoopPacket*)" sender="ProcessFilter2"/>
      <connect receiver="ProcessFilter1" signal="processed(SnoopPacket*)" slot="check(SnoopPacket*)" sender="FlowMgr1"/>
      <connect receiver="FlowChange1" signal="nak(SnoopPacket*)" slot="processFromTo(SnoopPacket*)" sender="ProcessFilter1"/>
    </connectList>
  </graph>
  <coord>
    <object y="-117" name="WinDivert1" x="-66"/>
    <object y="-76" name="BpFilter1" x="-123"/>
    <object y="-76" name="BpFilter2" x="5"/>
    <object y="-35" name="FlowMgr1" x="-126"/>
    <object y="-34" name="FlowMgr2" x="3"/>
    <object y="7" name="ProcessFilter1" x="-137"/>
    <object y="7" name="ProcessFilter2" x="-6"/>
    <object y="50" name="FlowChange1" x="-67"/>
  </coord>
</http_sniff>
