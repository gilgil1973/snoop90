<http_sniff>
  <graph>
    <objectList>
      <object layer="0" autoRead="true" queueLen="8192" _class="SnoopWinDivert" filter="tcp" priority="0" correctChecksum="true" autoParse="true" name="WinDivert1" tos="0" queueTime="1024" flags="1024"/>
      <object checkInterval="1" _class="SnoopFlowMgr" macFlowTimeout="3600" udpFlowTimeout="300" name="FlowMgr1" tcpFlowTimeout="300" ipFlowTimeout="300"/>
      <object showStatus="true" _class="SnoopProcessFilter" flowMgr="FlowMgr1" name="ProcessFilter1">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="true" processName="chrome.exe"/>
          <policy ack="true" processName="firefox.exe"/>
          <policy ack="true" processName="iexplore.exe"/>
          <policy ack="true" processName="netclient.exe"/>
          <policy ack="false" processName="snoopspy.exe"/>
          <policy ack="false" processName="tvnviewer.exe"/>
        </policies>
      </object>
      <object _class="SnoopBpFilter" filter="tcp dst port 80 or tcp dst port 443" name="BpFilter1" linkType="1"/>
      <object _class="SnoopBpFilter" filter="tcp src port 8080 or tcp src port 4433" name="BpFilter2" linkType="1"/>
      <object outFlowMgr="FlowMgr1" inCapture="WinDivert1" _class="SnoopFlowChange" name="FlowChange1" inFlowMgr="FlowMgr1" outCapture="WinDivert1" tcpChange="true" udpChange="false">
        <changeItems>
          <item dstIpChangeType="1" dstIpMask="0.0.0.0" srcPortChangeType="1" protocol="0" srcIpMask="0.0.0.0" srcPortFixValue="0" dstPortFixValue="8080" dstIpFixValue="192.168.10.11" dstIp="0.0.0.0" dstPortChangeType="2" srcIp="0.0.0.0" enabled="true" dstPort="80" log="true" srcIpFixValue="0.0.0.0" srcIpChangeType="0" srcPort="0"/>
          <item dstIpChangeType="1" dstIpMask="0.0.0.0" srcPortChangeType="1" protocol="0" srcIpMask="0.0.0.0" srcPortFixValue="0" dstPortFixValue="4433" dstIpFixValue="192.168.10.11" dstIp="0.0.0.0" dstPortChangeType="2" srcIp="0.0.0.0" enabled="true" dstPort="443" log="true" srcIpFixValue="0.0.0.0" srcIpChangeType="0" srcPort="0"/>
        </changeItems>
      </object>
    </objectList>
    <connectList>
      <connect slot="process(SnoopPacket*)" receiver="FlowMgr1" signal="captured(SnoopPacket*)" sender="WinDivert1"/>
      <connect slot="check(SnoopPacket*)" receiver="ProcessFilter1" signal="processed(SnoopPacket*)" sender="FlowMgr1"/>
      <connect slot="processInOut(SnoopPacket*)" receiver="FlowChange1" signal="ack(SnoopPacket*)" sender="BpFilter1"/>
      <connect slot="processOutIn(SnoopPacket*)" receiver="FlowChange1" signal="ack(SnoopPacket*)" sender="BpFilter2"/>
      <connect slot="check(SnoopPacket*)" receiver="BpFilter1" signal="ack(SnoopPacket*)" sender="ProcessFilter1"/>
      <connect slot="check(SnoopPacket*)" receiver="BpFilter2" signal="nak(SnoopPacket*)" sender="ProcessFilter1"/>
    </connectList>
  </graph>
  <coord>
    <object name="WinDivert1" x="-53" y="-227"/>
    <object name="FlowMgr1" x="-51" y="-190"/>
    <object name="ProcessFilter1" x="-62" y="-153"/>
    <object name="BpFilter1" x="-81" y="-114"/>
    <object name="BpFilter2" x="-8" y="-115"/>
    <object name="FlowChange1" x="-59" y="-70"/>
  </coord>
</http_sniff>
