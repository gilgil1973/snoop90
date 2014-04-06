<httpsniff>
  <graph>
    <objectList>
      <object autoRead="true" tos="0" flags="0" queueLen="8192" layer="0" _class="SnoopWinDivert" filter="tcp.DstPort==80 or tcp.DstPort==443 or tcp.SrcPort==8080 or tcp.SrcPort==4433" autoParse="true" name="WinDivert1" priority="0" correctChecksum="true" queueTime="1024"/>
      <object linkType="1" _class="SnoopBpFilter" filter="dst port 80 or dst port 443" name="BpFilter1"/>
      <object linkType="1" _class="SnoopBpFilter" filter="src port 8080 or src port 4433" name="BpFilter2"/>
      <object macFlowTimeout="3600" checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" _class="SnoopFlowMgr" name="FlowMgr1" ipFlowTimeout="300"/>
      <object macFlowTimeout="3600" checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" _class="SnoopFlowMgr" name="FlowMgr2" ipFlowTimeout="300"/>
      <object flowMgr="FlowMgr1" _class="SnoopProcessFilter" name="ProcessFilter1" showStatus="true">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="false" processName="chrome.exe"/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="false" processName="iexplore.exe"/>
          <policy ack="false" processName="netclient.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
          <policy ack="false" processName="svchost.exe"/>
        </policies>
      </object>
      <object flowMgr="FlowMgr2" _class="SnoopProcessFilter" name="ProcessFilter2" showStatus="true">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
        </policies>
      </object>
      <object _class="SnoopFlowChange" toFlowMgr="FlowMgr2" name="FlowChange1" tcpChange="true" fromFlowMgr="FlowMgr1" udpChange="true">
        <changeItems>
          <item dstIpMask="0.0.0.0" enabled="true" srcIpChangeType="1" dstPort="80" log="false" srcIpMask="0.0.0.0" dstIpChangeType="1" dstIpFixValue="127.0.0.1" srcPortChangeType="1" protocol="0" srcPortFixValue="0" srcPort="0" srcIpFixValue="127.0.0.1" dstPortFixValue="8080" dstPortChangeType="2" dstIp="0.0.0.0" srcIp="0.0.0.0"/>
          <item dstIpMask="0.0.0.0" enabled="true" srcIpChangeType="1" dstPort="443" log="false" srcIpMask="0.0.0.0" dstIpChangeType="1" dstIpFixValue="127.0.0.1" srcPortChangeType="1" protocol="0" srcPortFixValue="0" srcPort="0" srcIpFixValue="127.0.0.1" dstPortFixValue="4433" dstPortChangeType="2" dstIp="0.0.0.0" srcIp="0.0.0.0"/>
        </changeItems>
      </object>
      <object sslEnabled="false" maxContentCacheSize="10485756" _class="VHttpProxy" keepAliveTimeout="60000" name="HttpProxy8080" tcpEnabled="true">
        <outPolicy host="127.0.0.1" method="1" port="8081"/>
        <tcpServer localHost="127.0.0.1" onceWriteSize="0" _class="VTcpServer" port="8080"/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" localHost="" onceWriteSize="0" _class="VSslServer" methodType="4" port="4433" certificatePath="D:\project\net\snoop90\bin\certificate\"/>
        <inboundDataChange>
          <item pattern="michael" minimal="false" enabled="true" log="true" replace="GILBERT" syntax="2" cs="0"/>
          <item pattern="hacking" minimal="false" enabled="true" log="true" replace="FUCKING" syntax="2" cs="1"/>
        </inboundDataChange>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" log="true" replace="Accept-Encoding:      " syntax="2" cs="1"/>
        </outboundDataChange>
      </object>
      <object sslEnabled="true" maxContentCacheSize="10485756" _class="VHttpProxy" keepAliveTimeout="60000" name="HttpProxy8081" tcpEnabled="true">
        <outPolicy host="" method="1" port="0"/>
        <tcpServer localHost="127.0.0.1" onceWriteSize="0" _class="VTcpServer" port="8081"/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" localHost="" onceWriteSize="0" _class="VSslServer" methodType="4" port="4433" certificatePath="certificate/"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
      <object autoRead="true" tos="0" flags="0" queueLen="8192" layer="0" _class="SnoopWinDivert" filter="ifIdx==1 and (tcp.SrcPort==8081 or tcp.DstPort==8081 or tcp.SrcPort==4434 or tcp.DstPort==4434)" autoParse="true" name="WinDivertProxy" priority="0" correctChecksum="true" queueTime="1024"/>
      <object linkType="1" filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap" _class="SnoopDump" name="DumpProxy"/>
      <object dstMac="000000-000000" autoRead="false" readTimeout="1" adapterIndex="1" flags="1" _class="SnoopWriteAdapter" filter="" autoParse="true" name="WriteAdapterProxy" snapLen="1600" srcMac="000000-000000"/>
      <object sslEnabled="true" maxContentCacheSize="10485756" _class="VHttpProxy" keepAliveTimeout="60000" name="HttpProxy4433" tcpEnabled="false">
        <outPolicy host="127.0.0.1" method="1" port="4434"/>
        <tcpServer localHost="" onceWriteSize="0" _class="VTcpServer" port="8080"/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" localHost="127.0.0.1" onceWriteSize="0" _class="VSslServer" methodType="4" port="4433" certificatePath="D:\project\net\snoop90\bin\certificate\"/>
        <inboundDataChange>
          <item pattern="hacking" minimal="false" enabled="true" log="true" replace="FUCKING" syntax="2" cs="1"/>
        </inboundDataChange>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" log="true" replace="Accept-Encoding:      " syntax="2" cs="1"/>
        </outboundDataChange>
      </object>
      <object sslEnabled="false" maxContentCacheSize="10485756" _class="VHttpProxy" keepAliveTimeout="60000" name="HttpProxy4434" tcpEnabled="true">
        <outPolicy host="" method="2" port="0"/>
        <tcpServer localHost="127.0.0.1" onceWriteSize="0" _class="VTcpServer" port="4434"/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" localHost="" onceWriteSize="0" _class="VSslServer" methodType="4" port="4433" certificatePath="certificate/"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
    </objectList>
    <connectList>
      <connect slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="BpFilter1" sender="WinDivert1"/>
      <connect slot="process(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="FlowMgr1" sender="BpFilter1"/>
      <connect slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="BpFilter2" sender="WinDivert1"/>
      <connect slot="process(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="FlowMgr2" sender="BpFilter2"/>
      <connect slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="ProcessFilter2" sender="FlowMgr2"/>
      <connect slot="processToFrom(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="FlowChange1" sender="ProcessFilter2"/>
      <connect slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="ProcessFilter1" sender="FlowMgr1"/>
      <connect slot="processFromTo(SnoopPacket*)" signal="nak(SnoopPacket*)" receiver="FlowChange1" sender="ProcessFilter1"/>
      <connect slot="dump(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="DumpProxy" sender="WinDivertProxy"/>
      <connect slot="copy(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="WriteAdapterProxy" sender="WinDivertProxy"/>
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
    <object name="HttpProxy8080" x="-115" y="91"/>
    <object name="HttpProxy8081" x="-113" y="128"/>
    <object name="WinDivertProxy" x="-70" y="164"/>
    <object name="DumpProxy" x="-113" y="205"/>
    <object name="WriteAdapterProxy" x="-26" y="203"/>
    <object name="HttpProxy4433" x="-13" y="93"/>
    <object name="HttpProxy4434" x="-12" y="126"/>
  </coord>
</httpsniff>
