<http_sniff>
  <graph>
    <objectList>
      <object _class="SnoopWinDivert" queueTime="1024" priority="0" correctChecksum="true" tos="0" flags="0" layer="0" autoRead="true" autoParse="true" name="WinDivert1" queueLen="8192" filter="tcp.DstPort==80 or tcp.DstPort==443 or tcp.SrcPort==8080 or tcp.SrcPort==4433"/>
      <object _class="SnoopBpFilter" linkType="1" name="BpFilter1" filter="dst port 80 or dst port 443"/>
      <object _class="SnoopBpFilter" linkType="1" name="BpFilter2" filter="src port 8080 or src port 4433"/>
      <object _class="SnoopFlowMgr" ipFlowTimeout="300" macFlowTimeout="3600" checkInterval="1" tcpFlowTimeout="300" udpFlowTimeout="300" name="FlowMgr1"/>
      <object _class="SnoopFlowMgr" ipFlowTimeout="300" macFlowTimeout="3600" checkInterval="1" tcpFlowTimeout="300" udpFlowTimeout="300" name="FlowMgr2"/>
      <object _class="SnoopProcessFilter" flowMgr="FlowMgr1" showStatus="true" name="ProcessFilter1">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="iexplore.exe" ack="false"/>
          <policy processName="netclient.exe" ack="false"/>
          <policy processName="netserver.exe" ack="false"/>
          <policy processName="svchost.exe" ack="false"/>
        </policies>
      </object>
      <object _class="SnoopProcessFilter" flowMgr="FlowMgr2" showStatus="true" name="ProcessFilter2">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="netserver.exe" ack="false"/>
        </policies>
      </object>
      <object _class="SnoopFlowChange" fromFlowMgr="FlowMgr1" udpChange="true" toFlowMgr="FlowMgr2" name="FlowChange1" tcpChange="true">
        <changeItems>
          <item srcIp="0.0.0.0" srcPortFixValue="0" dstPortChangeType="2" srcIpFixValue="127.0.0.1" dstPortFixValue="8080" srcPort="0" dstIpMask="0.0.0.0" log="false" srcIpMask="0.0.0.0" srcIpChangeType="1" dstIpChangeType="1" dstIpFixValue="127.0.0.1" enabled="true" srcPortChangeType="1" dstPort="80" protocol="0" dstIp="0.0.0.0"/>
          <item srcIp="0.0.0.0" srcPortFixValue="0" dstPortChangeType="2" srcIpFixValue="127.0.0.1" dstPortFixValue="4433" srcPort="0" dstIpMask="0.0.0.0" log="false" srcIpMask="0.0.0.0" srcIpChangeType="1" dstIpChangeType="1" dstIpFixValue="127.0.0.1" enabled="true" srcPortChangeType="1" dstPort="443" protocol="0" dstIp="0.0.0.0"/>
        </changeItems>
      </object>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" slot="check(SnoopPacket*)" receiver="BpFilter1" sender="WinDivert1"/>
      <connect signal="ack(SnoopPacket*)" slot="process(SnoopPacket*)" receiver="FlowMgr1" sender="BpFilter1"/>
      <connect signal="captured(SnoopPacket*)" slot="check(SnoopPacket*)" receiver="BpFilter2" sender="WinDivert1"/>
      <connect signal="ack(SnoopPacket*)" slot="process(SnoopPacket*)" receiver="FlowMgr2" sender="BpFilter2"/>
      <connect signal="processed(SnoopPacket*)" slot="check(SnoopPacket*)" receiver="ProcessFilter2" sender="FlowMgr2"/>
      <connect signal="ack(SnoopPacket*)" slot="processToFrom(SnoopPacket*)" receiver="FlowChange1" sender="ProcessFilter2"/>
      <connect signal="processed(SnoopPacket*)" slot="check(SnoopPacket*)" receiver="ProcessFilter1" sender="FlowMgr1"/>
      <connect signal="nak(SnoopPacket*)" slot="processFromTo(SnoopPacket*)" receiver="FlowChange1" sender="ProcessFilter1"/>
    </connectList>
  </graph>
  <coord>
    <object x="-65" y="-117" name="WinDivert1"/>
    <object x="-123" y="-76" name="BpFilter1"/>
    <object x="5" y="-76" name="BpFilter2"/>
    <object x="-126" y="-35" name="FlowMgr1"/>
    <object x="3" y="-34" name="FlowMgr2"/>
    <object x="-137" y="7" name="ProcessFilter1"/>
    <object x="-6" y="7" name="ProcessFilter2"/>
    <object x="-67" y="50" name="FlowChange1"/>
  </coord>
</http_sniff>
