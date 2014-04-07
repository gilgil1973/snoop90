<httpsniff>
  <graph>
    <objectList>
      <object flags="0" enabled="true" layer="0" _class="SnoopWinDivert" autoRead="true" autoParse="true" filter="(ifIdx!=1) and (tcp.DstPort==80 or tcp.DstPort==443)" queueLen="8192" name="WinDivertOutbound" correctChecksum="true" queueTime="1024" priority="0" tos="0"/>
      <object linkType="1" _class="SnoopBpFilter" filter="dst port 80 or dst port 443" name="BpFilterOutbound"/>
      <object _class="SnoopFlowMgr" macFlowTimeout="3600" checkInterval="1" tcpFlowTimeout="300" udpFlowTimeout="300" name="FlowMgrOutboundProcessFilter" ipFlowTimeout="300"/>
      <object showStatus="false" _class="SnoopProcessFilter" name="ProcessFilterOutbound" flowMgr="FlowMgrOutboundProcessFilter">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="false" processName="KakaoTalk.exe"/>
          <policy ack="false" processName="Line.exe"/>
          <policy ack="false" processName="Wireshark.exe"/>
          <policy ack="false" processName="chrome.exe"/>
          <policy ack="false" processName="clover.exe"/>
          <policy ack="false" processName="firefox.exe"/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="false" processName="iexplore.exe"/>
          <policy ack="false" processName="netclient.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="false" processName="netsession_win.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
          <policy ack="false" processName="svchost.exe"/>
        </policies>
      </object>
      <object _class="SnoopFlowMgr" macFlowTimeout="3600" checkInterval="1" tcpFlowTimeout="300" udpFlowTimeout="300" name="FlowMgrOutbound" ipFlowTimeout="300"/>
      <object flags="0" enabled="true" layer="0" _class="SnoopWinDivert" autoRead="true" autoParse="true" filter="(ifIdx==1) and (tcp.SrcPort==8080 or tcp.SrcPort==4433)" queueLen="8192" name="WinDivertInbound" correctChecksum="true" queueTime="1024" priority="0" tos="0"/>
      <object linkType="1" _class="SnoopBpFilter" filter="src port 8080 or src port 4433" name="BpFilterInbound"/>
      <object _class="SnoopFlowMgr" macFlowTimeout="3600" checkInterval="1" tcpFlowTimeout="300" udpFlowTimeout="300" name="FlowMgrInboundProcessFilter" ipFlowTimeout="300"/>
      <object showStatus="false" _class="SnoopProcessFilter" name="ProcessFilterInbound" flowMgr="FlowMgrInboundProcessFilter">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
        </policies>
      </object>
      <object _class="SnoopFlowMgr" macFlowTimeout="3600" checkInterval="1" tcpFlowTimeout="300" udpFlowTimeout="300" name="FlowMgrInbound" ipFlowTimeout="300"/>
      <object _class="SnoopFlowChange" toFlowMgr="FlowMgrInbound" fromFlowMgr="FlowMgrOutbound" tcpChange="true" name="FlowChange1" udpChange="false">
        <changeItems>
          <item dstIpChangeType="1" enabled="true" dstPort="80" srcPortFixValue="0" srcPortChangeType="1" dstPortFixValue="8080" srcPort="0" protocol="1" dstIpFixValue="127.0.0.1" dstIp="0.0.0.0" srcIp="0.0.0.0" dstPortChangeType="2" srcIpFixValue="127.0.0.1" dstIpMask="0.0.0.0" log="false" srcIpMask="0.0.0.0" srcIpChangeType="1"/>
          <item dstIpChangeType="1" enabled="true" dstPort="443" srcPortFixValue="0" srcPortChangeType="1" dstPortFixValue="4433" srcPort="0" protocol="1" dstIpFixValue="127.0.0.1" dstIp="0.0.0.0" srcIp="0.0.0.0" dstPortChangeType="2" srcIpFixValue="127.0.0.1" dstIpMask="0.0.0.0" log="false" srcIpMask="0.0.0.0" srcIpChangeType="1"/>
        </changeItems>
      </object>
      <object tcpEnabled="true" _class="VHttpProxy" sslEnabled="false" name="HttpProxy8080" maxContentCacheSize="10485756" disableLoopbackConnection="false" keepAliveTimeout="60000">
        <outPolicy host="127.0.0.1" method="1" port="80"/>
        <tcpServer localHost="127.0.0.1" _class="VTcpServer" onceWriteSize="0" port="8080"/>
        <sslServer localHost="" processConnectMessage="false" _class="VSslServer" defaultKeyCrtFileName="default.pem" onceWriteSize="0" certificatePath="D:\project\net\snoop90\bin\certificate\" methodType="4" port="4433"/>
        <inboundDataChange>
          <item pattern="michael" minimal="false" enabled="true" replace="GILBERT" syntax="2" log="true" cs="0"/>
          <item pattern="hacking" minimal="false" enabled="true" replace="FUCKING" syntax="2" log="true" cs="1"/>
        </inboundDataChange>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " syntax="2" log="true" cs="1"/>
        </outboundDataChange>
      </object>
      <object tcpEnabled="true" _class="VHttpProxy" sslEnabled="false" name="HttpProxy80" maxContentCacheSize="10485756" disableLoopbackConnection="true" keepAliveTimeout="60000">
        <outPolicy host="" method="1" port="0"/>
        <tcpServer localHost="127.0.0.1" _class="VTcpServer" onceWriteSize="0" port="80"/>
        <sslServer localHost="" processConnectMessage="false" _class="VSslServer" defaultKeyCrtFileName="default.pem" onceWriteSize="0" certificatePath="certificate/" methodType="4" port="4433"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
      <object flags="1" enabled="true" layer="0" _class="SnoopWinDivert" autoRead="true" autoParse="true" filter="ifIdx==1 and outbound and (tcp.SrcPort==80 or tcp.DstPort==80 or tcp.SrcPort==443 or tcp.DstPort==443)" queueLen="8192" name="WinDivertProxy" correctChecksum="true" queueTime="1024" priority="0" tos="0"/>
      <object linkType="1" _class="SnoopDump" filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap" name="DumpProxy"/>
      <object flags="1" enabled="true" _class="SnoopWriteAdapter" autoRead="false" autoParse="true" filter="" name="WriteAdapterProxy" snapLen="1600" adapterIndex="1" srcMac="000000-000000" readTimeout="1" dstMac="000000-000000"/>
      <object tcpEnabled="false" _class="VHttpProxy" sslEnabled="true" name="HttpProxy4433" maxContentCacheSize="10485756" disableLoopbackConnection="false" keepAliveTimeout="60000">
        <outPolicy host="127.0.0.1" method="1" port="443"/>
        <tcpServer localHost="" _class="VTcpServer" onceWriteSize="0" port="8080"/>
        <sslServer localHost="127.0.0.1" processConnectMessage="false" _class="VSslServer" defaultKeyCrtFileName="default.pem" onceWriteSize="0" certificatePath="D:\project\net\snoop90\bin\certificate\" methodType="4" port="4433"/>
        <inboundDataChange>
          <item pattern="hacking" minimal="false" enabled="true" replace="FUCKING" syntax="2" log="true" cs="1"/>
          <item pattern="Michael" minimal="false" enabled="true" replace="GILBERT" syntax="2" log="true" cs="1"/>
        </inboundDataChange>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " syntax="2" log="false" cs="1"/>
        </outboundDataChange>
      </object>
      <object tcpEnabled="true" _class="VHttpProxy" sslEnabled="false" name="HttpProxy443" maxContentCacheSize="10485756" disableLoopbackConnection="true" keepAliveTimeout="60000">
        <outPolicy host="" method="2" port="0"/>
        <tcpServer localHost="127.0.0.1" _class="VTcpServer" onceWriteSize="0" port="443"/>
        <sslServer localHost="" processConnectMessage="false" _class="VSslServer" defaultKeyCrtFileName="default.pem" onceWriteSize="0" certificatePath="certificate/" methodType="4" port="4433"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
      <object flags="1" enabled="false" disableAutoRouting="true" _class="SnoopArpSpoof" autoRead="true" autoParse="true" filter="tcp dst port 80 or tcp dst port 443" infectInterval="1000" selfRelay="true" name="ArpSpoof1" snapLen="1600" adapterIndex="0" readTimeout="1" virtualMac="000000-000000">
        <sessionList>
          <session targetIp="192.168.10.1" targetMac="002E00-1040F8" senderMac="002666-4F9E3F" senderIp="192.168.10.2"/>
        </sessionList>
      </object>
      <object flags="1" Direction="0" enabled="true" layer="0" _class="SnoopWriteWinDivert" autoRead="false" autoParse="true" filter="true" queueLen="8192" IfIdx="1" name="WriteWinDivert1" correctChecksum="true" queueTime="1024" priority="0" tos="0" changeDivertAddr="true" SubIfIdx="0"/>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" sender="WinDivertProxy" receiver="DumpProxy" slot="dump(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" sender="WinDivertProxy" receiver="WriteAdapterProxy" slot="copy(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" sender="WinDivertOutbound" receiver="BpFilterOutbound" slot="check(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" sender="WinDivertInbound" receiver="BpFilterInbound" slot="check(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" sender="BpFilterOutbound" receiver="FlowMgrOutboundProcessFilter" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" sender="FlowMgrOutboundProcessFilter" receiver="ProcessFilterOutbound" slot="check(SnoopPacket*)"/>
      <connect signal="nak(SnoopPacket*)" sender="ProcessFilterOutbound" receiver="FlowMgrOutbound" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" sender="FlowMgrOutbound" receiver="FlowChange1" slot="processFromTo(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" sender="BpFilterInbound" receiver="FlowMgrInboundProcessFilter" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" sender="FlowMgrInboundProcessFilter" receiver="ProcessFilterInbound" slot="check(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" sender="ProcessFilterInbound" receiver="FlowMgrInbound" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" sender="FlowMgrInbound" receiver="FlowChange1" slot="processToFrom(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" sender="ArpSpoof1" receiver="FlowMgrOutbound" slot="process(SnoopPacket*)"/>
      <connect signal="changedFromTo(SnoopPacket*)" sender="FlowChange1" receiver="WriteWinDivert1" slot="move(SnoopPacket*)"/>
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
    <object x="-99" y="149" name="HttpProxy8080"/>
    <object x="-87" y="185" name="HttpProxy80"/>
    <object x="-51" y="242" name="WinDivertProxy"/>
    <object x="-97" y="287" name="DumpProxy"/>
    <object x="3" y="286" name="WriteAdapterProxy"/>
    <object x="-4" y="147" name="HttpProxy4433"/>
    <object x="-4" y="185" name="HttpProxy443"/>
    <object x="-273" y="17" name="ArpSpoof1"/>
    <object x="-153" y="94" name="WriteWinDivert1"/>
  </coord>
</httpsniff>
