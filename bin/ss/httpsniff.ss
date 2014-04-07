<httpsniff>
  <graph>
    <objectList>
      <object priority="0" _class="SnoopWinDivert" enabled="true" autoParse="true" filter="(ifIdx!=1) and (tcp.DstPort==80 or tcp.DstPort==443)" queueTime="1024" autoRead="true" flags="0" correctChecksum="true" layer="0" queueLen="8192" tos="0" name="wbOutbound"/>
      <object macFlowTimeout="3600" _class="SnoopFlowMgr" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="fmOutboundProcessFilter" checkInterval="1"/>
      <object _class="SnoopProcessFilter" showStatus="false" name="pfOutbound" flowMgr="fmOutboundProcessFilter">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="false" processName="KakaoTalk.exe"/>
          <policy ack="false" processName="Line.exe"/>
          <policy ack="false" processName="SMemo.exe"/>
          <policy ack="false" processName="Wireshark.exe"/>
          <policy ack="false" processName="chrome.exe"/>
          <policy ack="false" processName="clover.exe"/>
          <policy ack="false" processName="firefox.exe"/>
          <policy ack="false" processName="git-remote-https.exe"/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="false" processName="iexplore.exe"/>
          <policy ack="false" processName="netclient.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="false" processName="netsession_win.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
          <policy ack="false" processName="svchost.exe"/>
        </policies>
      </object>
      <object macFlowTimeout="3600" _class="SnoopFlowMgr" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="fmOutbound" checkInterval="1"/>
      <object priority="0" _class="SnoopWinDivert" enabled="true" autoParse="true" filter="(ifIdx==1) and (tcp.SrcPort==8080 or tcp.SrcPort==4433)" queueTime="1024" autoRead="true" flags="0" correctChecksum="true" layer="0" queueLen="8192" tos="0" name="wdInbound"/>
      <object macFlowTimeout="3600" _class="SnoopFlowMgr" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="fmInboundProcessFilter" checkInterval="1"/>
      <object _class="SnoopProcessFilter" showStatus="false" name="pfInbound" flowMgr="fmInboundProcessFilter">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="true" processName="httpproxy.exe"/>
          <policy ack="true" processName="netserver.exe"/>
          <policy ack="true" processName="snoopspy.exe"/>
        </policies>
      </object>
      <object macFlowTimeout="3600" _class="SnoopFlowMgr" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="fmInbound" checkInterval="1"/>
      <object _class="SnoopFlowChange" toFlowMgr="fmInbound" tcpChange="true" udpChange="false" fromFlowMgr="fmOutbound" name="fc">
        <changeItems>
          <item enabled="true" dstIpChangeType="1" dstPort="80" dstIp="0.0.0.0" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="8080" srcPort="0" srcPortChangeType="1" log="false" dstIpMask="0.0.0.0" dstPortChangeType="2" protocol="1" srcIpMask="0.0.0.0" dstIpFixValue="127.0.0.1" srcIpChangeType="1"/>
          <item enabled="true" dstIpChangeType="1" dstPort="443" dstIp="0.0.0.0" srcIp="0.0.0.0" srcIpFixValue="127.0.0.1" srcPortFixValue="0" dstPortFixValue="4433" srcPort="0" srcPortChangeType="1" log="false" dstIpMask="0.0.0.0" dstPortChangeType="2" protocol="1" srcIpMask="0.0.0.0" dstIpFixValue="127.0.0.1" srcIpChangeType="1"/>
        </changeItems>
      </object>
      <object _class="VHttpProxy" disableLoopbackConnection="false" tcpEnabled="true" maxContentCacheSize="10485756" sslEnabled="false" keepAliveTimeout="60000" name="hpTcpIn" outInThreadTimeout="30000">
        <outPolicy method="1" host="127.0.0.1" port="8081"/>
        <tcpServer onceWriteSize="0" _class="VTcpServer" localHost="127.0.0.1" port="8080"/>
        <sslServer onceWriteSize="0" defaultKeyCrtFileName="default.pem" _class="VSslServer" certificatePath="D:\project\net\snoop90\bin\certificate\" localHost="" processConnectMessage="false" port="4433" methodType="4"/>
        <inboundDataChange>
          <item minimal="false" enabled="true" replace="GILBERT" syntax="2" cs="0" log="true" pattern="michael"/>
          <item minimal="false" enabled="true" replace="FUCKING" syntax="2" cs="1" log="true" pattern="hacking"/>
        </inboundDataChange>
        <outboundDataChange>
          <item minimal="false" enabled="true" replace="Accept-Encoding:      " syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip,"/>
        </outboundDataChange>
      </object>
      <object _class="VHttpProxy" disableLoopbackConnection="true" tcpEnabled="true" maxContentCacheSize="10485756" sslEnabled="false" keepAliveTimeout="60000" name="hpTcpOut" outInThreadTimeout="30000">
        <outPolicy method="1" host="" port="0"/>
        <tcpServer onceWriteSize="0" _class="VTcpServer" localHost="127.0.0.1" port="8081"/>
        <sslServer onceWriteSize="0" defaultKeyCrtFileName="default.pem" _class="VSslServer" certificatePath="certificate/" localHost="" processConnectMessage="false" port="4433" methodType="4"/>
        <inboundDataChange/>
        <outboundDataChange/>
      </object>
      <object priority="0" _class="SnoopWinDivert" enabled="true" autoParse="false" filter="ifIdx==1 and outbound and (tcp.SrcPort==8081 or tcp.DstPort==8081 or tcp.SrcPort==4434 or tcp.DstPort==4434)" queueTime="1024" autoRead="true" flags="1" correctChecksum="true" layer="0" queueLen="8192" tos="0" name="wdProxy"/>
      <object _class="SnoopDump" linkType="1" filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap" name="dProxy"/>
      <object readTimeout="1" _class="SnoopWriteAdapter" enabled="true" autoParse="false" filter="" autoRead="false" flags="1" srcMac="000000-000000" snapLen="1600" name="waProxy" dstMac="000000-000000" adapterIndex="1"/>
      <object _class="VHttpProxy" disableLoopbackConnection="false" tcpEnabled="false" maxContentCacheSize="10485756" sslEnabled="true" keepAliveTimeout="60000" name="hpSslIn" outInThreadTimeout="30000">
        <outPolicy method="1" host="127.0.0.1" port="4434"/>
        <tcpServer onceWriteSize="0" _class="VTcpServer" localHost="" port="8080"/>
        <sslServer onceWriteSize="0" defaultKeyCrtFileName="default.pem" _class="VSslServer" certificatePath="D:\project\net\snoop90\bin\certificate\" localHost="127.0.0.1" processConnectMessage="false" port="4433" methodType="4"/>
        <inboundDataChange>
          <item minimal="false" enabled="true" replace="FUCKING" syntax="2" cs="1" log="true" pattern="hacking"/>
          <item minimal="false" enabled="true" replace="GILBERT" syntax="2" cs="1" log="true" pattern="Michael"/>
        </inboundDataChange>
        <outboundDataChange>
          <item minimal="false" enabled="true" replace="Accept-Encoding:      " syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip,"/>
        </outboundDataChange>
      </object>
      <object _class="VHttpProxy" disableLoopbackConnection="true" tcpEnabled="true" maxContentCacheSize="10485756" sslEnabled="false" keepAliveTimeout="60000" name="hpSslOutn" outInThreadTimeout="30000">
        <outPolicy method="2" host="" port="0"/>
        <tcpServer onceWriteSize="0" _class="VTcpServer" localHost="127.0.0.1" port="4434"/>
        <sslServer onceWriteSize="0" defaultKeyCrtFileName="default.pem" _class="VSslServer" certificatePath="certificate/" localHost="" processConnectMessage="false" port="4433" methodType="4"/>
        <inboundDataChange>
          <item minimal="false" enabled="true" replace="You may AAAA" syntax="2" cs="1" log="true" pattern="You may know"/>
        </inboundDataChange>
        <outboundDataChange/>
      </object>
      <object readTimeout="1" _class="SnoopArpSpoof" enabled="false" autoParse="true" filter="tcp dst port 80 or tcp dst port 443" virtualMac="000000-000000" selfRelay="true" disableAutoRouting="true" autoRead="true" flags="1" snapLen="1600" name="as" adapterIndex="0" infectInterval="1000">
        <sessionList>
          <session senderIp="192.168.10.2" targetMac="002E00-1040F8" senderMac="002666-4F9E3F" targetIp="192.168.10.1"/>
        </sessionList>
      </object>
      <object priority="0" _class="SnoopWriteWinDivert" enabled="true" autoParse="true" filter="true" IfIdx="1" SubIfIdx="0" changeDivertAddr="true" queueTime="1024" autoRead="false" flags="1" correctChecksum="true" layer="0" queueLen="8192" tos="0" name="wwdOutbound" Direction="0"/>
    </objectList>
    <connectList>
      <connect slot="process(SnoopPacket*)" receiver="fmOutboundProcessFilter" signal="captured(SnoopPacket*)" sender="wbOutbound"/>
      <connect slot="check(SnoopPacket*)" receiver="pfOutbound" signal="processed(SnoopPacket*)" sender="fmOutboundProcessFilter"/>
      <connect slot="process(SnoopPacket*)" receiver="fmOutbound" signal="nak(SnoopPacket*)" sender="pfOutbound"/>
      <connect slot="process(SnoopPacket*)" receiver="fmInboundProcessFilter" signal="captured(SnoopPacket*)" sender="wdInbound"/>
      <connect slot="check(SnoopPacket*)" receiver="pfInbound" signal="processed(SnoopPacket*)" sender="fmInboundProcessFilter"/>
      <connect slot="process(SnoopPacket*)" receiver="fmInbound" signal="ack(SnoopPacket*)" sender="pfInbound"/>
      <connect slot="dump(SnoopPacket*)" receiver="dProxy" signal="captured(SnoopPacket*)" sender="wdProxy"/>
      <connect slot="copy(SnoopPacket*)" receiver="waProxy" signal="captured(SnoopPacket*)" sender="wdProxy"/>
      <connect slot="process(SnoopPacket*)" receiver="fmOutbound" signal="captured(SnoopPacket*)" sender="as"/>
      <connect slot="processFromTo(SnoopPacket*)" receiver="fc" signal="processed(SnoopPacket*)" sender="fmOutbound"/>
      <connect slot="processToFrom(SnoopPacket*)" receiver="fc" signal="processed(SnoopPacket*)" sender="fmInbound"/>
      <connect slot="move(SnoopPacket*)" receiver="wwdOutbound" signal="changedFromTo(SnoopPacket*)" sender="fc"/>
    </connectList>
  </graph>
  <coord>
    <object x="-184" y="-107" name="wbOutbound"/>
    <object x="-202" y="-64" name="fmOutboundProcessFilter"/>
    <object x="-181" y="-25" name="pfOutbound"/>
    <object x="-161" y="14" name="fmOutbound"/>
    <object x="19" y="-111" name="wdInbound"/>
    <object x="6" y="-66" name="fmInboundProcessFilter"/>
    <object x="23" y="-27" name="pfInbound"/>
    <object x="23" y="11" name="fmInbound"/>
    <object x="-28" y="60" name="fc"/>
    <object x="-81" y="151" name="hpTcpIn"/>
    <object x="-83" y="182" name="hpTcpOut"/>
    <object x="-51" y="242" name="wdProxy"/>
    <object x="-97" y="287" name="dProxy"/>
    <object x="3" y="286" name="waProxy"/>
    <object x="-1" y="151" name="hpSslIn"/>
    <object x="-7" y="182" name="hpSslOutn"/>
    <object x="-273" y="17" name="as"/>
    <object x="-153" y="94" name="wwdOutbound"/>
  </coord>
</httpsniff>
