<httpsniff>
  <graph>
    <objectList>
      <object correctChecksum="true" autoRead="true" queueLen="8192" queueTime="1024" enabled="true" priority="0" flags="0" layer="0" _class="SnoopWinDivert" filter="(ifIdx!=1) and (tcp.DstPort==80 or tcp.DstPort==443)" name="wdOutbound" autoParse="true" tos="0"/>
      <object ipFlowTimeout="300" macFlowTimeout="3600" tcpFlowTimeout="300" udpFlowTimeout="300" _class="SnoopFlowMgr" name="fmOutboundProcessFilter" checkInterval="1"/>
      <object flowMgr="fmOutboundProcessFilter" _class="SnoopProcessFilter" showStatus="true" name="pfOutbound">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="FlashPlayerUpdateService.exe" ack="false"/>
          <policy processName="KakaoTalk.exe" ack="false"/>
          <policy processName="Line.exe" ack="false"/>
          <policy processName="SMemo.exe" ack="false"/>
          <policy processName="Wireshark.exe" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="clover.exe" ack="false"/>
          <policy processName="firefox.exe" ack="false"/>
          <policy processName="git-remote-https.exe" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="iexplore.exe" ack="false"/>
          <policy processName="netclient.exe" ack="false"/>
          <policy processName="netserver.exe" ack="true"/>
          <policy processName="netsession_win.exe" ack="false"/>
          <policy processName="snoopspy.exe" ack="true"/>
          <policy processName="svchost.exe" ack="false"/>
        </policies>
      </object>
      <object ipFlowTimeout="300" macFlowTimeout="3600" tcpFlowTimeout="300" udpFlowTimeout="300" _class="SnoopFlowMgr" name="fmOutbound" checkInterval="1"/>
      <object correctChecksum="true" autoRead="true" queueLen="8192" queueTime="1024" enabled="true" priority="0" flags="0" layer="0" _class="SnoopWinDivert" filter="(ifIdx==1) and (tcp.SrcPort==8080 or tcp.SrcPort==4433)" name="wdInbound" autoParse="true" tos="0"/>
      <object ipFlowTimeout="300" macFlowTimeout="3600" tcpFlowTimeout="300" udpFlowTimeout="300" _class="SnoopFlowMgr" name="fmInboundProcessFilter" checkInterval="1"/>
      <object flowMgr="fmInboundProcessFilter" _class="SnoopProcessFilter" showStatus="true" name="pfInbound">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="httpproxy.exe" ack="true"/>
          <policy processName="netserver.exe" ack="true"/>
          <policy processName="snoopspy.exe" ack="true"/>
        </policies>
      </object>
      <object ipFlowTimeout="300" macFlowTimeout="3600" tcpFlowTimeout="300" udpFlowTimeout="300" _class="SnoopFlowMgr" name="fmInbound" checkInterval="1"/>
      <object tcpChange="true" fromFlowMgr="fmOutbound" udpChange="false" _class="SnoopFlowChange" name="fc" toFlowMgr="fmInbound">
        <changeItems>
          <item log="false" dstIp="0.0.0.0" srcIp="0.0.0.0" dstPortChangeType="2" srcIpFixValue="127.0.0.1" protocol="1" srcIpChangeType="1" dstIpChangeType="1" enabled="true" dstPort="80" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPortFixValue="0" dstPortFixValue="8080" srcPortChangeType="1" srcPort="0" dstIpFixValue="127.0.0.1"/>
          <item log="false" dstIp="0.0.0.0" srcIp="0.0.0.0" dstPortChangeType="2" srcIpFixValue="127.0.0.1" protocol="1" srcIpChangeType="1" dstIpChangeType="1" enabled="true" dstPort="443" dstIpMask="0.0.0.0" srcIpMask="0.0.0.0" srcPortFixValue="0" dstPortFixValue="4433" srcPortChangeType="1" srcPort="0" dstIpFixValue="127.0.0.1"/>
        </changeItems>
      </object>
      <object disableLoopbackConnection="false" outInThreadTimeout="30000" maxContentCacheSize="10485756" _class="VHttpProxy" tcpEnabled="true" name="hpTcpIn" sslEnabled="false" keepAliveTimeout="60000">
        <outPolicy port="8081" host="127.0.0.1" method="1"/>
        <tcpServer onceWriteSize="0" port="8080" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer onceWriteSize="0" certificatePath="D:\project\net\snoop90\bin\certificate\" port="4433" methodType="4" _class="VSslServer" localHost="" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange>
          <item syntax="2" log="true" pattern="michael" minimal="false" enabled="true" cs="0" replace="GILBERT"/>
          <item syntax="2" log="true" pattern="hacking" minimal="false" enabled="true" cs="1" replace="FUCKING"/>
        </inboundDataChange>
        <outboundDataChange>
          <item syntax="2" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" cs="1" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object disableLoopbackConnection="true" outInThreadTimeout="30000" maxContentCacheSize="10485756" _class="VHttpProxy" tcpEnabled="true" name="hpTcpOut" sslEnabled="false" keepAliveTimeout="60000">
        <outPolicy port="0" host="" method="1"/>
        <tcpServer onceWriteSize="0" port="8081" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer onceWriteSize="0" certificatePath="certificate/" port="4433" methodType="4" _class="VSslServer" localHost="" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
      <object correctChecksum="true" autoRead="true" queueLen="8192" queueTime="1024" enabled="true" priority="0" flags="1" layer="0" _class="SnoopWinDivert" filter="ifIdx==1 and outbound and (tcp.SrcPort==8081 or tcp.DstPort==8081 or tcp.SrcPort==4434 or tcp.DstPort==4434)" name="wdProxy" autoParse="false" tos="0"/>
      <object filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap" _class="SnoopDump" name="dProxy" linkType="1"/>
      <object autoRead="false" snapLen="1600" adapterIndex="1" srcMac="000000-000000" dstMac="000000-000000" enabled="true" flags="1" readTimeout="1" _class="SnoopWriteAdapter" filter="" name="waProxy" autoParse="false"/>
      <object disableLoopbackConnection="false" outInThreadTimeout="30000" maxContentCacheSize="10485756" _class="VHttpProxy" tcpEnabled="false" name="hpSslIn" sslEnabled="true" keepAliveTimeout="60000">
        <outPolicy port="4434" host="127.0.0.1" method="1"/>
        <tcpServer onceWriteSize="0" port="8080" _class="VTcpServer" localHost=""/>
        <sslServer onceWriteSize="0" certificatePath="D:\project\net\snoop90\bin\certificate\" port="4433" methodType="4" _class="VSslServer" localHost="127.0.0.1" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange>
          <item syntax="2" log="true" pattern="hacking" minimal="false" enabled="true" cs="1" replace="FUCKING"/>
          <item syntax="2" log="true" pattern="Michael" minimal="false" enabled="true" cs="1" replace="GILBERT"/>
        </inboundDataChange>
        <outboundDataChange>
          <item syntax="2" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" cs="1" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object disableLoopbackConnection="true" outInThreadTimeout="30000" maxContentCacheSize="10485756" _class="VHttpProxy" tcpEnabled="true" name="hpSslOut" sslEnabled="false" keepAliveTimeout="60000">
        <outPolicy port="0" host="" method="2"/>
        <tcpServer onceWriteSize="0" port="4434" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer onceWriteSize="0" certificatePath="certificate/" port="4433" methodType="4" _class="VSslServer" localHost="" processConnectMessage="false" defaultKeyCrtFileName="default.pem"/>
        <inboundDataChange>
          <item syntax="2" log="true" pattern="You may know" minimal="false" enabled="true" cs="1" replace="You may AAAA"/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
      <object autoRead="true" selfRelay="true" snapLen="1600" adapterIndex="0" infectInterval="1000" disableAutoRouting="true" enabled="false" flags="1" virtualMac="000000-000000" readTimeout="1" _class="SnoopArpSpoof" filter="tcp dst port 80 or tcp dst port 443" name="as" autoParse="true">
        <sessionList>
          <session targetIp="192.168.10.1" targetMac="002E00-1040F8" senderMac="002666-4F9E3F" senderIp="192.168.10.2"/>
        </sessionList>
      </object>
      <object correctChecksum="true" autoRead="false" IfIdx="1" queueLen="8192" queueTime="1024" enabled="true" priority="0" flags="1" layer="0" _class="SnoopWriteWinDivert" filter="true" SubIfIdx="0" Direction="0" changeDivertAddr="true" name="wwdOutbound" autoParse="true" tos="0"/>
    </objectList>
    <connectList>
      <connect signal="processed(SnoopPacket*)" sender="fmOutboundProcessFilter" slot="check(SnoopPacket*)" receiver="pfOutbound"/>
      <connect signal="nak(SnoopPacket*)" sender="pfOutbound" slot="process(SnoopPacket*)" receiver="fmOutbound"/>
      <connect signal="captured(SnoopPacket*)" sender="wdInbound" slot="process(SnoopPacket*)" receiver="fmInboundProcessFilter"/>
      <connect signal="processed(SnoopPacket*)" sender="fmInboundProcessFilter" slot="check(SnoopPacket*)" receiver="pfInbound"/>
      <connect signal="ack(SnoopPacket*)" sender="pfInbound" slot="process(SnoopPacket*)" receiver="fmInbound"/>
      <connect signal="captured(SnoopPacket*)" sender="wdProxy" slot="dump(SnoopPacket*)" receiver="dProxy"/>
      <connect signal="captured(SnoopPacket*)" sender="wdProxy" slot="copy(SnoopPacket*)" receiver="waProxy"/>
      <connect signal="captured(SnoopPacket*)" sender="as" slot="process(SnoopPacket*)" receiver="fmOutbound"/>
      <connect signal="processed(SnoopPacket*)" sender="fmOutbound" slot="processFromTo(SnoopPacket*)" receiver="fc"/>
      <connect signal="processed(SnoopPacket*)" sender="fmInbound" slot="processToFrom(SnoopPacket*)" receiver="fc"/>
      <connect signal="changedFromTo(SnoopPacket*)" sender="fc" slot="move(SnoopPacket*)" receiver="wwdOutbound"/>
      <connect signal="captured(SnoopPacket*)" sender="wdOutbound" slot="process(SnoopPacket*)" receiver="fmOutboundProcessFilter"/>
    </connectList>
  </graph>
  <coord>
    <object name="wdOutbound" x="-165" y="-115"/>
    <object name="fmOutboundProcessFilter" x="-214" y="-70"/>
    <object name="pfOutbound" x="-183" y="-33"/>
    <object name="fmOutbound" x="-223" y="15"/>
    <object name="wdInbound" x="19" y="-111"/>
    <object name="fmInboundProcessFilter" x="6" y="-66"/>
    <object name="pfInbound" x="23" y="-27"/>
    <object name="fmInbound" x="23" y="11"/>
    <object name="fc" x="-28" y="60"/>
    <object name="hpTcpIn" x="-81" y="151"/>
    <object name="hpTcpOut" x="-87" y="179"/>
    <object name="wdProxy" x="-51" y="242"/>
    <object name="dProxy" x="-97" y="287"/>
    <object name="waProxy" x="3" y="286"/>
    <object name="hpSslIn" x="12" y="151"/>
    <object name="hpSslOut" x="6" y="180"/>
    <object name="as" x="-278" y="-95"/>
    <object name="wwdOutbound" x="-153" y="94"/>
  </coord>
</httpsniff>
