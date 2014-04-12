<httpsniff>
  <graph>
    <objectList>
      <object enabled="true" priority="0" name="wdOutbound" autoParse="true" tos="0" correctChecksum="true" queueTime="1024" _class="SnoopWinDivert" filter="(ifIdx!=1) and outbound and (tcp.DstPort==80 or tcp.DstPort==8080 or tcp.DstPort==8888 or tcp.DstPort==443 or tcp.DstPort==4433)" autoRead="true" flags="0" queueLen="8192" layer="0"/>
      <object enabled="false" name="asOutbound" autoParse="true" infectInterval="1000" selfRelay="true" disableAutoRouting="true" _class="SnoopArpSpoof" filter="tcp dst port 80 or tcp dst port 8080 or tcp dst port 8888 or tcp dst port 443 or tcp dst port 4433" readTimeout="1" autoRead="true" adapterIndex="0" snapLen="1600" flags="1" virtualMac="000000-000000">
        <sessionList/>
      </object>
      <object ipFlowTimeout="300" name="fmOutboundProc" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" checkInterval="1" _class="SnoopFlowMgr"/>
      <object flowMgr="fmOutboundProc" name="pfOutbound" showStatus="false" _class="SnoopProcessFilter">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="false" processName="GoogleUpdate.exe"/>
          <policy ack="false" processName="chrome.exe"/>
          <policy ack="false" processName="firefox.exe"/>
          <policy ack="false" processName="iexplore.exe"/>
          <policy ack="false" processName="mstsc.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
          <policy ack="false" processName="svchost.exe"/>
        </policies>
      </object>
      <object ipFlowTimeout="300" name="fmOutbound" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" checkInterval="1" _class="SnoopFlowMgr"/>
      <object enabled="true" priority="0" name="wdInbound" autoParse="true" tos="0" correctChecksum="true" queueTime="1024" _class="SnoopWinDivert" filter="(ifIdx==1) and (tcp.SrcPort=8079 or tcp.SrcPort=4432)" autoRead="true" flags="0" queueLen="8192" layer="0"/>
      <object ipFlowTimeout="300" name="fmInboundProc" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" checkInterval="1" _class="SnoopFlowMgr"/>
      <object flowMgr="fmInboundProc" name="pfInbound" showStatus="false" _class="SnoopProcessFilter">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="false" processName="chrome.exe"/>
          <policy ack="false" processName="firefox.exe"/>
          <policy ack="false" processName="mstsc.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
          <policy ack="false" processName="svchost.exe"/>
        </policies>
      </object>
      <object ipFlowTimeout="300" name="fmInbound" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" checkInterval="1" _class="SnoopFlowMgr"/>
      <object name="fcMain" fromFlowMgr="fmOutbound" toFlowMgr="fmInbound" tcpChange="true" _class="SnoopFlowChange" udpChange="true">
        <changeItems>
          <item srcIpMask="0.0.0.0" dstIpChangeType="1" enabled="true" srcPortFixValue="0" dstPort="80" srcPortChangeType="1" dstPortFixValue="8079" dstIp="0.0.0.0" srcIp="0.0.0.0" log="true" srcPort="0" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpMask="0.0.0.0" srcIpChangeType="1" protocol="1" srcIpFixValue="127.0.0.1"/>
          <item srcIpMask="0.0.0.0" dstIpChangeType="1" enabled="true" srcPortFixValue="0" dstPort="8080" srcPortChangeType="1" dstPortFixValue="8079" dstIp="0.0.0.0" srcIp="0.0.0.0" log="true" srcPort="0" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpMask="0.0.0.0" srcIpChangeType="1" protocol="1" srcIpFixValue="127.0.0.1"/>
          <item srcIpMask="0.0.0.0" dstIpChangeType="1" enabled="true" srcPortFixValue="0" dstPort="888" srcPortChangeType="1" dstPortFixValue="8079" dstIp="0.0.0.0" srcIp="0.0.0.0" log="true" srcPort="0" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpMask="0.0.0.0" srcIpChangeType="1" protocol="1" srcIpFixValue="127.0.0.1"/>
          <item srcIpMask="0.0.0.0" dstIpChangeType="1" enabled="true" srcPortFixValue="0" dstPort="443" srcPortChangeType="1" dstPortFixValue="4432" dstIp="0.0.0.0" srcIp="0.0.0.0" log="true" srcPort="0" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpMask="0.0.0.0" srcIpChangeType="1" protocol="1" srcIpFixValue="127.0.0.1"/>
          <item srcIpMask="0.0.0.0" dstIpChangeType="1" enabled="true" srcPortFixValue="0" dstPort="4433" srcPortChangeType="1" dstPortFixValue="4432" dstIp="0.0.0.0" srcIp="0.0.0.0" log="true" srcPort="0" dstIpFixValue="127.0.0.1" dstPortChangeType="2" dstIpMask="0.0.0.0" srcIpChangeType="1" protocol="1" srcIpFixValue="127.0.0.1"/>
        </changeItems>
      </object>
      <object Direction="0" enabled="true" priority="0" name="wwdOutbound" autoParse="false" IfIdx="0" SubIfIdx="0" tos="0" correctChecksum="true" queueTime="1024" _class="SnoopWriteWinDivert" filter="true" autoRead="false" changeDivertAddr="true" flags="1" queueLen="8192" layer="0"/>
      <object httpsEnabled="false" outInThreadTimeout="30000" enabled="true" name="wpHttpIn" disableLoopbackConnection="false" httpEnabled="true" maxContentCacheSize="10485756" _class="VWebProxy" keepAliveTimeout="60000">
        <outPolicy host="127.0.0.1" port="8080" method="1"/>
        <tcpServer localHost="127.0.0.1" port="8079" _class="VTcpServer" onceWriteSize="0"/>
        <sslServer localHost="" processConnectMessage="false" defaultKeyCrtFileName="default.pem" port="4433" certificatePath="certificate\" _class="VSslServer" onceWriteSize="0" methodType="4"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " log="false" cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object httpsEnabled="false" outInThreadTimeout="30000" enabled="true" name="wpHttpOut" disableLoopbackConnection="true" httpEnabled="true" maxContentCacheSize="10485756" _class="VWebProxy" keepAliveTimeout="60000">
        <outPolicy host="" port="0" method="0"/>
        <tcpServer localHost="127.0.0.1" port="8080" _class="VTcpServer" onceWriteSize="0"/>
        <sslServer localHost="" processConnectMessage="false" defaultKeyCrtFileName="default.pem" port="4433" certificatePath="certificate\" _class="VSslServer" onceWriteSize="0" methodType="4"/>
        <inboundDataChange>
          <item pattern="Michael" minimal="false" enabled="true" replace="GILGIL" log="true" cs="0" syntax="2"/>
        </inboundDataChange>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " log="false" cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object httpsEnabled="true" outInThreadTimeout="30000" enabled="true" name="wpHttpsIn" disableLoopbackConnection="false" httpEnabled="false" maxContentCacheSize="10485756" _class="VWebProxy" keepAliveTimeout="60000">
        <outPolicy host="127.0.0.1" port="4433" method="1"/>
        <tcpServer localHost="" port="8080" _class="VTcpServer" onceWriteSize="0"/>
        <sslServer localHost="127.0.0.1" processConnectMessage="false" defaultKeyCrtFileName="default.pem" port="4432" certificatePath="certificate\" _class="VSslServer" onceWriteSize="0" methodType="4"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " log="false" cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object httpsEnabled="false" outInThreadTimeout="30000" enabled="true" name="wpHttpsOut" disableLoopbackConnection="true" httpEnabled="true" maxContentCacheSize="10485756" _class="VWebProxy" keepAliveTimeout="60000">
        <outPolicy host="" port="0" method="2"/>
        <tcpServer localHost="127.0.0.1" port="4433" _class="VTcpServer" onceWriteSize="0"/>
        <sslServer localHost="" processConnectMessage="false" defaultKeyCrtFileName="default.pem" port="4433" certificatePath="certificate\" _class="VSslServer" onceWriteSize="0" methodType="4"/>
        <inboundDataChange>
          <item pattern="Money" minimal="false" enabled="true" replace="GOLDY" log="true" cs="1" syntax="2"/>
          <item pattern="Michael" minimal="false" enabled="true" replace="GILGIL" log="true" cs="0" syntax="2"/>
        </inboundDataChange>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " log="false" cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object enabled="true" priority="0" name="wdProxy" autoParse="true" tos="0" correctChecksum="true" queueTime="1024" _class="SnoopWinDivert" filter="ifIdx==1 and outbound and (tcp.SrcPort==8080 or tcp.DstPort==8080 or tcp.SrcPort==4433 or tcp.DstPort==4433)" autoRead="true" flags="0" queueLen="8192" layer="0"/>
      <object name="dProxy" linkType="1" _class="SnoopDump" filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap"/>
      <object enabled="true" srcMac="000000-000000" name="waProxy" autoParse="false" dstMac="000000-000000" _class="SnoopWriteAdapter" filter="" readTimeout="1" autoRead="false" adapterIndex="1" snapLen="1600" flags="1"/>
      <object name="dnscStrip" writer="wdOutbound" _class="SnoopDnsChange">
        <changeItems>
          <item pattern="ss.*" minimal="false" enabled="true" log="true" ip="80.43.80.43" cs="1" syntax="1"/>
        </changeItems>
      </object>
      <object httpsEnabled="false" outInThreadTimeout="30000" enabled="false" name="wpStripIn" disableLoopbackConnection="false" httpEnabled="true" maxContentCacheSize="10485756" _class="VWebProxy" keepAliveTimeout="60000">
        <outPolicy host="127.0.0.1" port="8043" method="1"/>
        <tcpServer localHost="127.0.0.1" port="8042" _class="VTcpServer" onceWriteSize="0"/>
        <sslServer localHost="" processConnectMessage="false" defaultKeyCrtFileName="default.pem" port="4433" certificatePath="certificate\" _class="VSslServer" onceWriteSize="0" methodType="4"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " log="false" cs="1" syntax="2"/>
        </outboundDataChange>
      </object>
      <object httpsEnabled="false" outInThreadTimeout="30000" enabled="false" name="wpStripOut" disableLoopbackConnection="true" httpEnabled="true" maxContentCacheSize="10485756" _class="VWebProxy" keepAliveTimeout="60000">
        <outPolicy host="" port="0" method="0"/>
        <tcpServer localHost="127.0.0.1" port="8043" _class="VTcpServer" onceWriteSize="0"/>
        <sslServer localHost="" processConnectMessage="false" defaultKeyCrtFileName="default.pem" port="4433" certificatePath="certificate\" _class="VSslServer" onceWriteSize="0" methodType="4"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      " log="false" cs="1" syntax="2"/>
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
      <connect sender="asOutbound" receiver="dnscStrip" slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect sender="wdOutbound" receiver="dnscStrip" slot="check(SnoopPacket*)" signal="captured(SnoopPacket*)"/>
      <connect sender="fmOutbound" receiver="fcMain" slot="processFromTo(SnoopPacket*)" signal="processed(SnoopPacket*)"/>
      <connect sender="fmInbound" receiver="fcMain" slot="processToFrom(SnoopPacket*)" signal="processed(SnoopPacket*)"/>
      <connect sender="fcMain" receiver="wwdOutbound" slot="move(SnoopPacket*)" signal="changedFromTo(SnoopPacket*)"/>
    </connectList>
  </graph>
  <coord>
    <object name="wdOutbound" x="-141" y="-114"/>
    <object name="asOutbound" x="-236" y="-114"/>
    <object name="fmOutboundProc" x="-153" y="-75"/>
    <object name="pfOutbound" x="-138" y="-38"/>
    <object name="fmOutbound" x="-139" y="1"/>
    <object name="wdInbound" x="-32" y="-114"/>
    <object name="fmInboundProc" x="-43" y="-75"/>
    <object name="pfInbound" x="-28" y="-37"/>
    <object name="fmInbound" x="-30" y="0"/>
    <object name="fcMain" x="-68" y="39"/>
    <object name="wwdOutbound" x="-145" y="74"/>
    <object name="wpHttpIn" x="-151" y="119"/>
    <object name="wpHttpOut" x="-156" y="148"/>
    <object name="wpHttpsIn" x="-75" y="119"/>
    <object name="wpHttpsOut" x="-79" y="148"/>
    <object name="wdProxy" x="-72" y="181"/>
    <object name="dProxy" x="-35" y="217"/>
    <object name="waProxy" x="-102" y="218"/>
    <object name="dnscStrip" x="-229" y="-75"/>
    <object name="wpStripIn" x="8" y="119"/>
    <object name="wpStripOut" x="5" y="148"/>
  </coord>
</httpsniff>
