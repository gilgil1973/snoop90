<httpsniff>
  <graph>
    <objectList>
      <object queueTime="1024" tos="0" flags="0" layer="0" enabled="false" autoRead="true" correctChecksum="true" queueLen="8192" name="WinDivertOutbound" _class="SnoopWinDivert" autoParse="true" filter="(ifIdx!=1) and (tcp.DstPort==80 or tcp.DstPort==443)" priority="0"/>
      <object linkType="1" name="BpFilterOutbound" _class="SnoopBpFilter" filter="dst port 80 or dst port 443"/>
      <object checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="FlowMgrOutboundProcessFilter" _class="SnoopFlowMgr" macFlowTimeout="3600"/>
      <object flowMgr="FlowMgrOutboundProcessFilter" showStatus="false" name="ProcessFilterOutbound" _class="SnoopProcessFilter">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="KakaoTalk.exe" ack="false"/>
          <policy processName="Line.exe" ack="false"/>
          <policy processName="Wireshark.exe" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="clover.exe" ack="false"/>
          <policy processName="firefox.exe" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="iexplore.exe" ack="false"/>
          <policy processName="netclient.exe" ack="false"/>
          <policy processName="netserver.exe" ack="true"/>
          <policy processName="netsession_win.exe" ack="false"/>
          <policy processName="snoopspy.exe" ack="true"/>
          <policy processName="svchost.exe" ack="false"/>
        </policies>
      </object>
      <object checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="FlowMgrOutbound" _class="SnoopFlowMgr" macFlowTimeout="3600"/>
      <object queueTime="1024" tos="0" flags="0" layer="0" enabled="true" autoRead="true" correctChecksum="true" queueLen="8192" name="WinDivertInbound" _class="SnoopWinDivert" autoParse="true" filter="(ifIdx==1) and (tcp.SrcPort==8080 or tcp.SrcPort==4433)" priority="0"/>
      <object linkType="1" name="BpFilterInbound" _class="SnoopBpFilter" filter="src port 8080 or src port 4433"/>
      <object checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="FlowMgrInboundProcessFilter" _class="SnoopFlowMgr" macFlowTimeout="3600"/>
      <object flowMgr="FlowMgrInboundProcessFilter" showStatus="false" name="ProcessFilterInbound" _class="SnoopProcessFilter">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="netserver.exe" ack="true"/>
          <policy processName="snoopspy.exe" ack="true"/>
        </policies>
      </object>
      <object checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="FlowMgrInbound" _class="SnoopFlowMgr" macFlowTimeout="3600"/>
      <object udpChange="false" name="FlowChange1" _class="SnoopFlowChange" toFlowMgr="FlowMgrInbound" fromFlowMgr="FlowMgrOutbound" tcpChange="true">
        <changeItems>
          <item dstIpFixValue="127.0.0.1" log="true" srcPortFixValue="0" dstPortFixValue="8080" enabled="true" dstIpMask="0.0.0.0" dstPort="80" srcIpFixValue="127.0.0.1" srcIpMask="0.0.0.0" srcPortChangeType="1" protocol="1" srcPort="0" dstIp="0.0.0.0" srcIp="0.0.0.0" srcIpChangeType="1" dstPortChangeType="2" dstIpChangeType="1"/>
          <item dstIpFixValue="127.0.0.1" log="true" srcPortFixValue="0" dstPortFixValue="4433" enabled="true" dstIpMask="0.0.0.0" dstPort="443" srcIpFixValue="127.0.0.1" srcIpMask="0.0.0.0" srcPortChangeType="1" protocol="1" srcPort="0" dstIp="0.0.0.0" srcIp="0.0.0.0" srcIpChangeType="1" dstPortChangeType="2" dstIpChangeType="1"/>
        </changeItems>
      </object>
      <object tcpEnabled="true" sslEnabled="false" disableLoopbackConnection="false" maxContentCacheSize="10485756" name="HttpProxy8080" _class="VHttpProxy" keepAliveTimeout="60000">
        <outPolicy host="127.0.0.1" port="80" method="1"/>
        <tcpServer onceWriteSize="0" localHost="127.0.0.1" _class="VTcpServer" port="8080"/>
        <sslServer onceWriteSize="0" localHost="" certificatePath="D:\project\net\snoop90\bin\certificate\" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" methodType="4" port="4433"/>
        <inboundDataChange>
          <item syntax="2" log="true" pattern="michael" minimal="false" enabled="true" replace="GILBERT" cs="0"/>
          <item syntax="2" log="true" pattern="hacking" minimal="false" enabled="true" replace="FUCKING" cs="1"/>
        </inboundDataChange>
        <outboundDataChange>
          <item syntax="2" log="true" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " cs="1"/>
        </outboundDataChange>
      </object>
      <object tcpEnabled="true" sslEnabled="false" disableLoopbackConnection="true" maxContentCacheSize="10485756" name="HttpProxy80" _class="VHttpProxy" keepAliveTimeout="60000">
        <outPolicy host="" port="0" method="1"/>
        <tcpServer onceWriteSize="0" localHost="127.0.0.1" _class="VTcpServer" port="80"/>
        <sslServer onceWriteSize="0" localHost="" certificatePath="certificate/" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" methodType="4" port="4433"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
      <object queueTime="1024" tos="0" flags="1" layer="0" enabled="true" autoRead="true" correctChecksum="true" queueLen="8192" name="WinDivertProxy" _class="SnoopWinDivert" autoParse="true" filter="ifIdx==1 and outbound and (tcp.SrcPort==80 or tcp.DstPort==80 or tcp.SrcPort==443 or tcp.DstPort==443)" priority="0"/>
      <object linkType="1" filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap" name="DumpProxy" _class="SnoopDump"/>
      <object flags="1" enabled="true" readTimeout="1" adapterIndex="1" srcMac="000000-000000" autoRead="false" dstMac="000000-000000" name="WriteAdapterProxy" _class="SnoopWriteAdapter" autoParse="true" filter="" snapLen="1600"/>
      <object tcpEnabled="false" sslEnabled="true" disableLoopbackConnection="false" maxContentCacheSize="10485756" name="HttpProxy4433" _class="VHttpProxy" keepAliveTimeout="60000">
        <outPolicy host="127.0.0.1" port="443" method="1"/>
        <tcpServer onceWriteSize="0" localHost="" _class="VTcpServer" port="8080"/>
        <sslServer onceWriteSize="0" localHost="127.0.0.1" certificatePath="D:\project\net\snoop90\bin\certificate\" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" methodType="4" port="4433"/>
        <inboundDataChange>
          <item syntax="2" log="true" pattern="hacking" minimal="false" enabled="true" replace="FUCKING" cs="1"/>
          <item syntax="2" log="true" pattern="Michael" minimal="false" enabled="true" replace="GILBERT" cs="1"/>
        </inboundDataChange>
        <outboundDataChange>
          <item syntax="2" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " cs="1"/>
        </outboundDataChange>
      </object>
      <object tcpEnabled="true" sslEnabled="false" disableLoopbackConnection="true" maxContentCacheSize="10485756" name="HttpProxy443" _class="VHttpProxy" keepAliveTimeout="60000">
        <outPolicy host="" port="0" method="2"/>
        <tcpServer onceWriteSize="0" localHost="127.0.0.1" _class="VTcpServer" port="443"/>
        <sslServer onceWriteSize="0" localHost="" certificatePath="certificate/" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" methodType="4" port="4433"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
      <object flags="1" enabled="true" readTimeout="1" adapterIndex="0" autoRead="true" disableAutoRouting="true" name="ArpSpoof1" _class="SnoopArpSpoof" autoParse="true" filter="tcp dst port 80 or tcp dst port 443" selfRelay="true" snapLen="1600" virtualMac="000000-000000" infectInterval="1000">
        <sessionList>
          <session senderIp="192.168.10.2" targetMac="002E00-1040F8" senderMac="002666-4F9E3F" targetIp="192.168.10.1"/>
        </sessionList>
      </object>
      <object queueTime="1024" tos="0" SubIfIdx="0" flags="1" layer="0" enabled="true" autoRead="false" correctChecksum="true" queueLen="8192" Direction="0" IfIdx="1" name="WriteWinDivert1" _class="SnoopWriteWinDivert" autoParse="true" filter="true" changeDivertAddr="true" priority="0"/>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" receiver="DumpProxy" slot="dump(SnoopPacket*)" sender="WinDivertProxy"/>
      <connect signal="captured(SnoopPacket*)" receiver="WriteAdapterProxy" slot="copy(SnoopPacket*)" sender="WinDivertProxy"/>
      <connect signal="captured(SnoopPacket*)" receiver="BpFilterOutbound" slot="check(SnoopPacket*)" sender="WinDivertOutbound"/>
      <connect signal="captured(SnoopPacket*)" receiver="BpFilterInbound" slot="check(SnoopPacket*)" sender="WinDivertInbound"/>
      <connect signal="ack(SnoopPacket*)" receiver="FlowMgrOutboundProcessFilter" slot="process(SnoopPacket*)" sender="BpFilterOutbound"/>
      <connect signal="processed(SnoopPacket*)" receiver="ProcessFilterOutbound" slot="check(SnoopPacket*)" sender="FlowMgrOutboundProcessFilter"/>
      <connect signal="nak(SnoopPacket*)" receiver="FlowMgrOutbound" slot="process(SnoopPacket*)" sender="ProcessFilterOutbound"/>
      <connect signal="processed(SnoopPacket*)" receiver="FlowChange1" slot="processFromTo(SnoopPacket*)" sender="FlowMgrOutbound"/>
      <connect signal="ack(SnoopPacket*)" receiver="FlowMgrInboundProcessFilter" slot="process(SnoopPacket*)" sender="BpFilterInbound"/>
      <connect signal="processed(SnoopPacket*)" receiver="ProcessFilterInbound" slot="check(SnoopPacket*)" sender="FlowMgrInboundProcessFilter"/>
      <connect signal="ack(SnoopPacket*)" receiver="FlowMgrInbound" slot="process(SnoopPacket*)" sender="ProcessFilterInbound"/>
      <connect signal="processed(SnoopPacket*)" receiver="FlowChange1" slot="processToFrom(SnoopPacket*)" sender="FlowMgrInbound"/>
      <connect signal="captured(SnoopPacket*)" receiver="FlowMgrOutbound" slot="process(SnoopPacket*)" sender="ArpSpoof1"/>
      <connect signal="changedFromTo(SnoopPacket*)" receiver="WriteWinDivert1" slot="move(SnoopPacket*)" sender="FlowChange1"/>
    </connectList>
  </graph>
  <coord>
    <object x="-171" y="-143" name="WinDivertOutbound"/>
    <object x="-165" y="-101" name="BpFilterOutbound"/>
    <object x="-202" y="-64" name="FlowMgrOutboundProcessFilter"/>
    <object x="-181" y="-25" name="ProcessFilterOutbound"/>
    <object x="-161" y="14" name="FlowMgrOutbound"/>
    <object x="33" y="-142" name="WinDivertInbound"/>
    <object x="37" y="-105" name="BpFilterInbound"/>
    <object x="6" y="-66" name="FlowMgrInboundProcessFilter"/>
    <object x="23" y="-27" name="ProcessFilterInbound"/>
    <object x="23" y="11" name="FlowMgrInbound"/>
    <object x="-51" y="53" name="FlowChange1"/>
    <object x="-116" y="98" name="HttpProxy8080"/>
    <object x="-104" y="128" name="HttpProxy80"/>
    <object x="-51" y="166" name="WinDivertProxy"/>
    <object x="-98" y="203" name="DumpProxy"/>
    <object x="-3" y="200" name="WriteAdapterProxy"/>
    <object x="13" y="97" name="HttpProxy4433"/>
    <object x="17" y="127" name="HttpProxy443"/>
    <object x="-273" y="17" name="ArpSpoof1"/>
    <object x="-168" y="70" name="WriteWinDivert1"/>
  </coord>
</httpsniff>
