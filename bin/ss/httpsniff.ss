<http_sniff>
  <graph>
    <objectList>
      <object queueLen="8192" queueTime="1024" flags="0" layer="0" priority="0" correctChecksum="true" tos="0" autoParse="true" _class="SnoopWinDivert" name="WinDivert1" filter="tcp.DstPort==80 or tcp.DstPort==443 or tcp.SrcPort==8080 or tcp.SrcPort==4433" autoRead="true"/>
      <object linkType="1" _class="SnoopBpFilter" name="BpFilter1" filter="dst port 80 or dst port 443"/>
      <object linkType="1" _class="SnoopBpFilter" name="BpFilter2" filter="src port 8080 or src port 4433"/>
      <object ipFlowTimeout="300" macFlowTimeout="3600" checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" _class="SnoopFlowMgr" name="FlowMgr1"/>
      <object ipFlowTimeout="300" macFlowTimeout="3600" checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" _class="SnoopFlowMgr" name="FlowMgr2"/>
      <object showStatus="true" _class="SnoopProcessFilter" name="ProcessFilter1" flowMgr="FlowMgr1">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="false" processName="chrome.exe"/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
        </policies>
      </object>
      <object showStatus="true" _class="SnoopProcessFilter" name="ProcessFilter2" flowMgr="FlowMgr2">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
        </policies>
      </object>
      <object udpChange="true" toFlowMgr="FlowMgr2" fromFlowMgr="FlowMgr1" _class="SnoopFlowChange" name="FlowChange1" tcpChange="true">
        <changeItems>
          <item dstIpChangeType="1" protocol="0" srcPortFixValue="0" dstPortChangeType="2" srcPort="0" dstIpMask="0.0.0.0" dstPortFixValue="8080" srcIpMask="0.0.0.0" dstIpFixValue="127.0.0.1" log="false" dstIp="0.0.0.0" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" enabled="true" srcIpChangeType="1" srcPortChangeType="1" dstPort="80"/>
          <item dstIpChangeType="1" protocol="0" srcPortFixValue="0" dstPortChangeType="2" srcPort="0" dstIpMask="0.0.0.0" dstPortFixValue="4433" srcIpMask="0.0.0.0" dstIpFixValue="127.0.0.1" log="false" dstIp="0.0.0.0" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" enabled="true" srcIpChangeType="1" srcPortChangeType="1" dstPort="443"/>
        </changeItems>
      </object>
      <object tcpEnabled="true" sslEnabled="false" _class="VHttpProxy" name="HttpProxy10080" maxContentCacheSize="10485756" keepAliveTimeout="60000">
        <outPolicy method="1" port="80" host="127.0.0.1"/>
        <tcpServer port="8080" localHost="" onceWriteSize="0" _class="VTcpServer"/>
        <sslServer defaultKeyCrtFileName="default.pem" port="4433" methodType="4" localHost="" certificatePath="D:\project\net\snoop90\bin\certificate\" onceWriteSize="0" _class="VSslServer" processConnectMessage="false"/>
        <inboundDataChange>
          <item syntax="2" replace="GILBERT" log="true" cs="0" pattern="michael" minimal="false" enabled="true"/>
          <item syntax="2" replace="FUCKING" log="true" cs="1" pattern="hacking" minimal="false" enabled="true"/>
        </inboundDataChange>
        <outboundDataChange>
          <item syntax="2" replace="Accept-Encoding:      " log="true" cs="1" pattern="Accept-Encoding: gzip," minimal="false" enabled="true"/>
        </outboundDataChange>
      </object>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" slot="check(SnoopPacket*)" sender="WinDivert1" receiver="BpFilter1"/>
      <connect signal="ack(SnoopPacket*)" slot="process(SnoopPacket*)" sender="BpFilter1" receiver="FlowMgr1"/>
      <connect signal="captured(SnoopPacket*)" slot="check(SnoopPacket*)" sender="WinDivert1" receiver="BpFilter2"/>
      <connect signal="ack(SnoopPacket*)" slot="process(SnoopPacket*)" sender="BpFilter2" receiver="FlowMgr2"/>
      <connect signal="processed(SnoopPacket*)" slot="check(SnoopPacket*)" sender="FlowMgr2" receiver="ProcessFilter2"/>
      <connect signal="ack(SnoopPacket*)" slot="processToFrom(SnoopPacket*)" sender="ProcessFilter2" receiver="FlowChange1"/>
      <connect signal="processed(SnoopPacket*)" slot="check(SnoopPacket*)" sender="FlowMgr1" receiver="ProcessFilter1"/>
      <connect signal="nak(SnoopPacket*)" slot="processFromTo(SnoopPacket*)" sender="ProcessFilter1" receiver="FlowChange1"/>
    </connectList>
  </graph>
  <coord>
    <object name="WinDivert1" x="-66" y="-117"/>
    <object name="BpFilter1" x="-124" y="-77"/>
    <object name="BpFilter2" x="4" y="-79"/>
    <object name="FlowMgr1" x="-126" y="-35"/>
    <object name="FlowMgr2" x="3" y="-34"/>
    <object name="ProcessFilter1" x="-137" y="7"/>
    <object name="ProcessFilter2" x="-8" y="7"/>
    <object name="FlowChange1" x="-67" y="50"/>
    <object name="HttpProxy10080" x="-162" y="107"/>
  </coord>
</http_sniff>
