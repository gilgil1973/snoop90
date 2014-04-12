<httpsniff>
  <graph>
    <objectList>
      <object priority="0" correctChecksum="true" queueTime="1024" enabled="true" _class="SnoopWinDivert" autoRead="true" filter="(ifIdx!=1) and outbound and (tcp.DstPort==80 or tcp.DstPort==8080 or tcp.DstPort==8888 or tcp.DstPort==443 or tcp.DstPort==4433)" flags="0" layer="0" queueLen="8192" autoParse="true" name="wdOutbound" tos="0"/>
      <object infectInterval="1000" readTimeout="1" enabled="false" adapterIndex="0" virtualMac="000000-000000" _class="SnoopArpSpoof" autoRead="true" filter="tcp dst port 80 or tcp dst port 8080 or tcp dst port 8888 or tcp dst port 443 or tcp dst port 4433" flags="1" disableAutoRouting="true" autoParse="true" name="asOutbound" selfRelay="true" snapLen="1600">
        <sessionList/>
      </object>
      <object checkInterval="1" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" _class="SnoopFlowMgr" name="fmOutboundProc"/>
      <object flowMgr="fmOutboundProc" _class="SnoopProcessFilter" showStatus="false" name="pfOutbound">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="GoogleUpdate.exe" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="firefox.exe" ack="false"/>
          <policy processName="mstsc.exe" ack="false"/>
          <policy processName="snoopspy.exe" ack="true"/>
        </policies>
      </object>
      <object checkInterval="1" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" _class="SnoopFlowMgr" name="fmOutbound"/>
      <object priority="0" correctChecksum="true" queueTime="1024" enabled="true" _class="SnoopWinDivert" autoRead="true" filter="(ifIdx==1) and (tcp.SrcPort=8079 or tcp.SrcPort=4432)" flags="0" layer="0" queueLen="8192" autoParse="true" name="wdInbound" tos="0"/>
      <object checkInterval="1" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" _class="SnoopFlowMgr" name="fmInboundProc"/>
      <object flowMgr="fmInboundProc" _class="SnoopProcessFilter" showStatus="false" name="pfInbound">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="firefox.exe" ack="false"/>
          <policy processName="mstsc.exe" ack="false"/>
          <policy processName="snoopspy.exe" ack="true"/>
          <policy processName="svchost.exe" ack="false"/>
        </policies>
      </object>
      <object checkInterval="1" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" _class="SnoopFlowMgr" name="fmInbound"/>
      <object udpChange="true" _class="SnoopFlowChange" toFlowMgr="fmInbound" name="fcMain" fromFlowMgr="fmOutbound" tcpChange="true">
        <changeItems>
          <item log="true" dstIpFixValue="127.0.0.1" dstIp="0.0.0.0" srcIp="0.0.0.0" srcPortChangeType="1" enabled="true" dstIpMask="0.0.0.0" dstPort="80" srcIpFixValue="127.0.0.1" srcIpChangeType="1" srcIpMask="0.0.0.0" dstIpChangeType="1" dstPortChangeType="2" srcPort="0" protocol="1" srcPortFixValue="0" dstPortFixValue="8079"/>
          <item log="true" dstIpFixValue="127.0.0.1" dstIp="0.0.0.0" srcIp="0.0.0.0" srcPortChangeType="1" enabled="true" dstIpMask="0.0.0.0" dstPort="8080" srcIpFixValue="127.0.0.1" srcIpChangeType="1" srcIpMask="0.0.0.0" dstIpChangeType="1" dstPortChangeType="2" srcPort="0" protocol="1" srcPortFixValue="0" dstPortFixValue="8079"/>
          <item log="true" dstIpFixValue="127.0.0.1" dstIp="0.0.0.0" srcIp="0.0.0.0" srcPortChangeType="1" enabled="true" dstIpMask="0.0.0.0" dstPort="888" srcIpFixValue="127.0.0.1" srcIpChangeType="1" srcIpMask="0.0.0.0" dstIpChangeType="1" dstPortChangeType="2" srcPort="0" protocol="1" srcPortFixValue="0" dstPortFixValue="8079"/>
          <item log="true" dstIpFixValue="127.0.0.1" dstIp="0.0.0.0" srcIp="0.0.0.0" srcPortChangeType="1" enabled="true" dstIpMask="0.0.0.0" dstPort="443" srcIpFixValue="127.0.0.1" srcIpChangeType="1" srcIpMask="0.0.0.0" dstIpChangeType="1" dstPortChangeType="2" srcPort="0" protocol="1" srcPortFixValue="0" dstPortFixValue="4432"/>
          <item log="true" dstIpFixValue="127.0.0.1" dstIp="0.0.0.0" srcIp="0.0.0.0" srcPortChangeType="1" enabled="true" dstIpMask="0.0.0.0" dstPort="4433" srcIpFixValue="127.0.0.1" srcIpChangeType="1" srcIpMask="0.0.0.0" dstIpChangeType="1" dstPortChangeType="2" srcPort="0" protocol="1" srcPortFixValue="0" dstPortFixValue="4432"/>
        </changeItems>
      </object>
      <object priority="0" correctChecksum="true" IfIdx="0" queueTime="1024" changeDivertAddr="true" SubIfIdx="0" enabled="true" Direction="0" _class="SnoopWriteWinDivert" autoRead="false" filter="true" flags="1" layer="0" queueLen="8192" autoParse="false" name="wwdOutbound" tos="0"/>
      <object httpEnabled="true" keepAliveTimeout="60000" disableLoopbackConnection="false" outInThreadTimeout="30000" enabled="true" _class="VWebProxy" httpsEnabled="false" maxContentCacheSize="10485756" name="hpTcpIn">
        <outPolicy port="8080" host="127.0.0.1" method="1"/>
        <tcpServer port="8079" onceWriteSize="0" localHost="127.0.0.1" _class="VTcpServer"/>
        <sslServer port="4433" certificatePath="certificate\" onceWriteSize="0" methodType="4" localHost="" _class="VSslServer" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object httpEnabled="true" keepAliveTimeout="60000" disableLoopbackConnection="true" outInThreadTimeout="30000" enabled="true" _class="VWebProxy" httpsEnabled="false" maxContentCacheSize="10485756" name="hpTcpOut">
        <outPolicy port="0" host="" method="0"/>
        <tcpServer port="8080" onceWriteSize="0" localHost="127.0.0.1" _class="VTcpServer"/>
        <sslServer port="4433" certificatePath="certificate\" onceWriteSize="0" methodType="4" localHost="" _class="VSslServer" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object httpEnabled="false" keepAliveTimeout="60000" disableLoopbackConnection="false" outInThreadTimeout="30000" enabled="true" _class="VWebProxy" httpsEnabled="true" maxContentCacheSize="10485756" name="hpSslIn">
        <outPolicy port="4433" host="127.0.0.1" method="1"/>
        <tcpServer port="8080" onceWriteSize="0" localHost="" _class="VTcpServer"/>
        <sslServer port="4432" certificatePath="certificate\" onceWriteSize="0" methodType="4" localHost="127.0.0.1" _class="VSslServer" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object httpEnabled="true" keepAliveTimeout="60000" disableLoopbackConnection="true" outInThreadTimeout="30000" enabled="true" _class="VWebProxy" httpsEnabled="false" maxContentCacheSize="10485756" name="hpSslOut">
        <outPolicy port="0" host="" method="2"/>
        <tcpServer port="4433" onceWriteSize="0" localHost="127.0.0.1" _class="VTcpServer"/>
        <sslServer port="4433" certificatePath="certificate\" onceWriteSize="0" methodType="4" localHost="" _class="VSslServer" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange>
          <item log="true" pattern="Money" minimal="false" enabled="true" replace="GOLD" cs="1" syntax="2"/>
        </inboundDataChange>
        <outboundDataChange>
          <item log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object priority="0" correctChecksum="true" queueTime="1024" enabled="true" _class="SnoopWinDivert" autoRead="true" filter="ifIdx==1 and outbound and (tcp.SrcPort==8080 or tcp.DstPort==8080 or tcp.SrcPort==4433 or tcp.DstPort==4433)" flags="0" layer="0" queueLen="8192" autoParse="true" name="wdProxy" tos="0"/>
      <object linkType="1" _class="SnoopDump" filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap" name="dProxy"/>
      <object srcMac="000000-000000" readTimeout="1" dstMac="000000-000000" enabled="true" adapterIndex="1" _class="SnoopWriteAdapter" autoRead="false" filter="" flags="1" autoParse="false" name="waProxy" snapLen="1600"/>
      <object writer="wdOutbound" _class="SnoopDnsChange" name="dcStrip">
        <changeItems>
          <item log="true" pattern="ss.*" minimal="false" enabled="true" ip="80.43.80.43" cs="1" syntax="1"/>
        </changeItems>
      </object>
      <object httpEnabled="true" keepAliveTimeout="60000" disableLoopbackConnection="false" outInThreadTimeout="30000" enabled="false" _class="VWebProxy" httpsEnabled="false" maxContentCacheSize="10485756" name="hpStripIn">
        <outPolicy port="8043" host="127.0.0.1" method="1"/>
        <tcpServer port="8042" onceWriteSize="0" localHost="127.0.0.1" _class="VTcpServer"/>
        <sslServer port="4433" certificatePath="certificate\" onceWriteSize="0" methodType="4" localHost="" _class="VSslServer" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object httpEnabled="true" keepAliveTimeout="60000" disableLoopbackConnection="true" outInThreadTimeout="30000" enabled="false" _class="VWebProxy" httpsEnabled="false" maxContentCacheSize="10485756" name="hpStripOut">
        <outPolicy port="0" host="" method="0"/>
        <tcpServer port="8043" onceWriteSize="0" localHost="127.0.0.1" _class="VTcpServer"/>
        <sslServer port="4433" certificatePath="certificate\" onceWriteSize="0" methodType="4" localHost="" _class="VSslServer" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
    </objectList>
    <connectList>
      <connect sender="wdOutbound" receiver="fmOutboundProc" slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect sender="asOutbound" receiver="fmOutbound" slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect sender="fmOutboundProc" receiver="pfOutbound" slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)"/>
      <connect sender="pfOutbound" receiver="fmOutbound" slot="process(SnoopPacket*)" signal="nak(SnoopPacket*)"/>
      <connect sender="wdInbound" receiver="fmInboundProc" slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect sender="fmInboundProc" receiver="pfInbound" slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)"/>
      <connect sender="pfInbound" receiver="fmInbound" slot="process(SnoopPacket*)" signal="ack(SnoopPacket*)"/>
      <connect sender="wdProxy" receiver="dProxy" slot="dump(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect sender="wdProxy" receiver="waProxy" slot="copy(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect sender="asOutbound" receiver="dcStrip" slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect sender="wdOutbound" receiver="dcStrip" slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect sender="fmOutbound" receiver="fcMain" slot="processFromTo(SnoopPacket*)" signal="processed(SnoopPacket*)"/>
      <connect sender="fmInbound" receiver="fcMain" slot="processToFrom(SnoopPacket*)" signal="processed(SnoopPacket*)"/>
      <connect sender="fcMain" receiver="wwdOutbound" slot="move(SnoopPacket*)" signal="changedFromTo(SnoopPacket*)"/>
    </connectList>
  </graph>
  <coord>
    <object x="-141" y="-114" name="wdOutbound"/>
    <object x="-236" y="-114" name="asOutbound"/>
    <object x="-153" y="-75" name="fmOutboundProc"/>
    <object x="-138" y="-38" name="pfOutbound"/>
    <object x="-139" y="1" name="fmOutbound"/>
    <object x="-32" y="-114" name="wdInbound"/>
    <object x="-43" y="-75" name="fmInboundProc"/>
    <object x="-28" y="-37" name="pfInbound"/>
    <object x="-30" y="0" name="fmInbound"/>
    <object x="-68" y="39" name="fcMain"/>
    <object x="-145" y="74" name="wwdOutbound"/>
    <object x="-137" y="119" name="hpTcpIn"/>
    <object x="-142" y="148" name="hpTcpOut"/>
    <object x="-67" y="119" name="hpSslIn"/>
    <object x="-71" y="148" name="hpSslOut"/>
    <object x="-72" y="181" name="wdProxy"/>
    <object x="-35" y="217" name="dProxy"/>
    <object x="-102" y="218" name="waProxy"/>
    <object x="-222" y="-73" name="dcStrip"/>
    <object x="1" y="119" name="hpStripIn"/>
    <object x="-3" y="148" name="hpStripOut"/>
  </coord>
</httpsniff>
