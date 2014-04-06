<httpsniff>
  <graph>
    <objectList>
      <object _class="SnoopWinDivert" filter="(ifIdx!=1) and (tcp.DstPort==80 or tcp.DstPort==443)" priority="0" tos="0" name="WinDivertOutbound" autoParse="true" flags="0" layer="0" queueTime="1024" correctChecksum="true" enabled="true" autoRead="true" queueLen="8192"/>
      <object _class="SnoopBpFilter" filter="dst port 80 or dst port 443" name="BpFilterOutbound" linkType="1"/>
      <object _class="SnoopFlowMgr" ipFlowTimeout="300" name="FlowMgrOutboundProcessFilter" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" checkInterval="1"/>
      <object _class="SnoopProcessFilter" name="ProcessFilterOutbound" showStatus="true" flowMgr="FlowMgrOutboundProcessFilter">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="KakaoTalk.exe" ack="false"/>
          <policy processName="Line.exe" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="firefox.exe" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="iexplore.exe" ack="false"/>
          <policy processName="netclient.exe" ack="false"/>
          <policy processName="netserver.exe" ack="true"/>
          <policy processName="snoopspy.exe" ack="true"/>
          <policy processName="svchost.exe" ack="false"/>
        </policies>
      </object>
      <object _class="SnoopFlowMgr" ipFlowTimeout="300" name="FlowMgrOutbound" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" checkInterval="1"/>
      <object _class="SnoopWinDivert" filter="(ifIdx==1) and (tcp.SrcPort==8080 or tcp.SrcPort==4433)" priority="0" tos="0" name="WinDivertInbound" autoParse="true" flags="0" layer="0" queueTime="1024" correctChecksum="true" enabled="true" autoRead="true" queueLen="8192"/>
      <object _class="SnoopBpFilter" filter="src port 8080 or src port 4433" name="BpFilterInbound" linkType="1"/>
      <object _class="SnoopFlowMgr" ipFlowTimeout="300" name="FlowMgrInboundProcessFilter" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" checkInterval="1"/>
      <object _class="SnoopProcessFilter" name="ProcessFilterInbound" showStatus="true" flowMgr="FlowMgrInboundProcessFilter">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="netserver.exe" ack="true"/>
          <policy processName="snoopspy.exe" ack="true"/>
        </policies>
      </object>
      <object _class="SnoopFlowMgr" ipFlowTimeout="300" name="FlowMgrInbound" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" checkInterval="1"/>
      <object _class="SnoopFlowChange" name="FlowChange1" fromFlowMgr="FlowMgrOutbound" toFlowMgr="FlowMgrInbound" tcpChange="true" udpChange="true">
        <changeItems>
          <item srcIpMask="0.0.0.0" srcIpChangeType="1" srcPortChangeType="1" dstIp="0.0.0.0" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPort="0" dstIpChangeType="1" log="false" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="8080" enabled="true" dstIpFixValue="127.0.0.1" dstPort="80" dstIpMask="0.0.0.0" protocol="0"/>
          <item srcIpMask="0.0.0.0" srcIpChangeType="1" srcPortChangeType="1" dstIp="0.0.0.0" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPort="0" dstIpChangeType="1" log="false" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="4433" enabled="true" dstIpFixValue="127.0.0.1" dstPort="443" dstIpMask="0.0.0.0" protocol="0"/>
        </changeItems>
      </object>
      <object keepAliveTimeout="60000" _class="VHttpProxy" maxContentCacheSize="10485756" name="HttpProxy8080" tcpEnabled="true" sslEnabled="false" disableLoopbackConnection="false">
        <outPolicy host="127.0.0.1" method="1" port="80"/>
        <tcpServer localHost="127.0.0.1" _class="VTcpServer" port="8080" onceWriteSize="0"/>
        <sslServer localHost="" _class="VSslServer" port="4433" certificatePath="D:\project\net\snoop90\bin\certificate\" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" methodType="4"/>
        <inboundDataChange>
          <item log="true" cs="0" syntax="2" pattern="michael" minimal="false" enabled="true" replace="GILBERT"/>
          <item log="true" cs="1" syntax="2" pattern="hacking" minimal="false" enabled="true" replace="FUCKING"/>
        </inboundDataChange>
        <outboundDataChange>
          <item log="true" cs="1" syntax="2" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object keepAliveTimeout="60000" _class="VHttpProxy" maxContentCacheSize="10485756" name="HttpProxy80" tcpEnabled="true" sslEnabled="false" disableLoopbackConnection="true">
        <outPolicy host="" method="1" port="0"/>
        <tcpServer localHost="127.0.0.1" _class="VTcpServer" port="80" onceWriteSize="0"/>
        <sslServer localHost="" _class="VSslServer" port="4433" certificatePath="certificate/" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" methodType="4"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
      <object _class="SnoopWinDivert" filter="ifIdx==1 and outbound and (tcp.SrcPort==80 or tcp.DstPort==80 or tcp.SrcPort==443 or tcp.DstPort==443)" priority="0" tos="0" name="WinDivertProxy" autoParse="true" flags="1" layer="0" queueTime="1024" correctChecksum="true" enabled="true" autoRead="true" queueLen="8192"/>
      <object _class="SnoopDump" name="DumpProxy" linkType="1" filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap"/>
      <object _class="SnoopWriteAdapter" filter="" name="WriteAdapterProxy" autoParse="true" snapLen="1600" adapterIndex="1" flags="1" srcMac="000000-000000" readTimeout="1" enabled="true" autoRead="false" dstMac="000000-000000"/>
      <object keepAliveTimeout="60000" _class="VHttpProxy" maxContentCacheSize="10485756" name="HttpProxy4433" tcpEnabled="false" sslEnabled="true" disableLoopbackConnection="false">
        <outPolicy host="127.0.0.1" method="1" port="443"/>
        <tcpServer localHost="" _class="VTcpServer" port="8080" onceWriteSize="0"/>
        <sslServer localHost="127.0.0.1" _class="VSslServer" port="4433" certificatePath="D:\project\net\snoop90\bin\certificate\" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" methodType="4"/>
        <inboundDataChange>
          <item log="true" cs="1" syntax="2" pattern="hacking" minimal="false" enabled="true" replace="FUCKING"/>
          <item log="true" cs="1" syntax="2" pattern="Michael" minimal="false" enabled="true" replace="GILBERT"/>
        </inboundDataChange>
        <outboundDataChange>
          <item log="false" cs="1" syntax="2" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object keepAliveTimeout="60000" _class="VHttpProxy" maxContentCacheSize="10485756" name="HttpProxy443" tcpEnabled="true" sslEnabled="false" disableLoopbackConnection="true">
        <outPolicy host="" method="2" port="0"/>
        <tcpServer localHost="127.0.0.1" _class="VTcpServer" port="443" onceWriteSize="0"/>
        <sslServer localHost="" _class="VSslServer" port="4433" certificatePath="certificate/" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" methodType="4"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
      <object _class="SnoopArpSpoof" filter="tcp dst port 80 or tcp dst port 443" virtualMac="000000-000000" name="ArpSpoof1" autoParse="true" snapLen="1600" adapterIndex="0" selfRelay="true" infectInterval="1000" flags="1" readTimeout="1" disableAutoRouting="true" enabled="false" autoRead="true">
        <sessionList/>
      </object>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" receiver="DumpProxy" sender="WinDivertProxy" slot="dump(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="WriteAdapterProxy" sender="WinDivertProxy" slot="copy(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="BpFilterOutbound" sender="WinDivertOutbound" slot="check(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="BpFilterInbound" sender="WinDivertInbound" slot="check(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" receiver="FlowMgrOutboundProcessFilter" sender="BpFilterOutbound" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="ProcessFilterOutbound" sender="FlowMgrOutboundProcessFilter" slot="check(SnoopPacket*)"/>
      <connect signal="nak(SnoopPacket*)" receiver="FlowMgrOutbound" sender="ProcessFilterOutbound" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="FlowChange1" sender="FlowMgrOutbound" slot="processFromTo(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" receiver="FlowMgrInboundProcessFilter" sender="BpFilterInbound" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="ProcessFilterInbound" sender="FlowMgrInboundProcessFilter" slot="check(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" receiver="FlowMgrInbound" sender="ProcessFilterInbound" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="FlowChange1" sender="FlowMgrInbound" slot="processToFrom(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="FlowMgrOutbound" sender="ArpSpoof1" slot="process(SnoopPacket*)"/>
    </connectList>
  </graph>
  <coord>
    <object name="WinDivertOutbound" x="-171" y="-143"/>
    <object name="BpFilterOutbound" x="-165" y="-101"/>
    <object name="FlowMgrOutboundProcessFilter" x="-202" y="-64"/>
    <object name="ProcessFilterOutbound" x="-181" y="-25"/>
    <object name="FlowMgrOutbound" x="-161" y="14"/>
    <object name="WinDivertInbound" x="33" y="-142"/>
    <object name="BpFilterInbound" x="37" y="-105"/>
    <object name="FlowMgrInboundProcessFilter" x="6" y="-66"/>
    <object name="ProcessFilterInbound" x="23" y="-27"/>
    <object name="FlowMgrInbound" x="23" y="11"/>
    <object name="FlowChange1" x="-51" y="53"/>
    <object name="HttpProxy8080" x="-116" y="98"/>
    <object name="HttpProxy80" x="-104" y="128"/>
    <object name="WinDivertProxy" x="-51" y="166"/>
    <object name="DumpProxy" x="-98" y="203"/>
    <object name="WriteAdapterProxy" x="-3" y="200"/>
    <object name="HttpProxy4433" x="13" y="97"/>
    <object name="HttpProxy443" x="17" y="127"/>
    <object name="ArpSpoof1" x="-273" y="17"/>
  </coord>
</httpsniff>
