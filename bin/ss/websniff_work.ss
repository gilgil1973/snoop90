<httpsniff>
  <graph>
    <objectList>
      <object autoParse="true" tos="0" correctChecksum="true" flags="0" layer="0" queueTime="1024" autoRead="true" name="wdOutbound" queueLen="8192" enabled="true" _class="SnoopWinDivert" filter="true" priority="0"/>
      <object infectInterval="1000" autoParse="true" selfRelay="true" snapLen="1600" flags="1" autoRead="true" name="asOutbound" enabled="false" adapterIndex="0" virtualMac="000000-000000" disableAutoRouting="true" _class="SnoopArpSpoof" filter="" readTimeout="1">
        <sessionList/>
      </object>
      <object macFlowTimeout="3600" udpFlowTimeout="300" checkInterval="1" tcpFlowTimeout="300" name="fmOutboundProc" ipFlowTimeout="300" _class="SnoopFlowMgr"/>
      <object showStatus="false" name="pfOutbound" flowMgr="fmOutboundProc" _class="SnoopProcessFilter">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="firefox.exe" ack="false"/>
          <policy processName="mstsc.exe" ack="false"/>
          <policy processName="snoopspy.exe" ack="false"/>
        </policies>
      </object>
      <object macFlowTimeout="3600" udpFlowTimeout="300" checkInterval="1" tcpFlowTimeout="300" name="fmOutbound" ipFlowTimeout="300" _class="SnoopFlowMgr"/>
      <object autoParse="true" tos="0" correctChecksum="true" flags="0" layer="0" queueTime="1024" autoRead="true" name="wdInbound" queueLen="8192" enabled="true" _class="SnoopWinDivert" filter="true" priority="0"/>
      <object macFlowTimeout="3600" udpFlowTimeout="300" checkInterval="1" tcpFlowTimeout="300" name="fmInboundProc" ipFlowTimeout="300" _class="SnoopFlowMgr"/>
      <object showStatus="false" name="pfInbound" flowMgr="fmInboundProc" _class="SnoopProcessFilter">
        <policies>
          <policy processName="" ack="false"/>
          <policy processName="chrome.exe" ack="false"/>
          <policy processName="firefox.exe" ack="false"/>
          <policy processName="mstsc.exe" ack="false"/>
          <policy processName="snoopspy.exe" ack="false"/>
        </policies>
      </object>
      <object macFlowTimeout="3600" udpFlowTimeout="300" checkInterval="1" tcpFlowTimeout="300" name="fmInbound" ipFlowTimeout="300" _class="SnoopFlowMgr"/>
      <object toFlowMgr="fmInbound" tcpChange="true" fromFlowMgr="fmOutbound" name="fcMain" udpChange="true" _class="SnoopFlowChange">
        <changeItems/>
      </object>
      <object SubIfIdx="0" autoParse="false" tos="0" correctChecksum="true" flags="1" layer="0" queueTime="1024" autoRead="false" name="wwdOutbound" queueLen="8192" changeDivertAddr="true" enabled="true" _class="SnoopWriteWinDivert" filter="true" IfIdx="0" priority="0" Direction="0"/>
      <object httpsEnabled="false" disableLoopbackConnection="true" outInThreadTimeout="30000" name="hpTcpIn" maxContentCacheSize="10485756" enabled="true" httpEnabled="true" keepAliveTimeout="60000" _class="VWebProxy">
        <outPolicy method="0" host="" port="0"/>
        <tcpServer onceWriteSize="0" port="8079" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" onceWriteSize="0" methodType="4" port="4433" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object httpsEnabled="false" disableLoopbackConnection="true" outInThreadTimeout="30000" name="hpTcpOut" maxContentCacheSize="10485756" enabled="true" httpEnabled="true" keepAliveTimeout="60000" _class="VWebProxy">
        <outPolicy method="0" host="" port="0"/>
        <tcpServer onceWriteSize="0" port="8080" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" onceWriteSize="0" methodType="4" port="4433" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object httpsEnabled="true" disableLoopbackConnection="true" outInThreadTimeout="30000" name="hpSslIn" maxContentCacheSize="10485756" enabled="true" httpEnabled="false" keepAliveTimeout="60000" _class="VWebProxy">
        <outPolicy method="0" host="" port="0"/>
        <tcpServer onceWriteSize="0" port="8080" _class="VTcpServer" localHost=""/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" onceWriteSize="0" methodType="4" port="4432" _class="VSslServer" localHost="127.0.0.1"/>
        <inboundDataChange/>
        <outboundDataChange>
          <item syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object httpsEnabled="false" disableLoopbackConnection="true" outInThreadTimeout="30000" name="hpSslOut" maxContentCacheSize="10485756" enabled="true" httpEnabled="true" keepAliveTimeout="60000" _class="VWebProxy">
        <outPolicy method="0" host="" port="0"/>
        <tcpServer onceWriteSize="0" port="4433" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" onceWriteSize="0" methodType="4" port="4433" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object autoParse="true" tos="0" correctChecksum="true" flags="0" layer="0" queueTime="1024" autoRead="true" name="wdProxy" queueLen="8192" enabled="true" _class="SnoopWinDivert" filter="true" priority="0"/>
      <object linkType="1" name="dProxy" filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap" _class="SnoopDump"/>
      <object autoParse="false" snapLen="1600" flags="1" srcMac="000000-000000" autoRead="false" name="waProxy" dstMac="000000-000000" enabled="true" adapterIndex="1" _class="SnoopWriteAdapter" filter="" readTimeout="1"/>
      <object writer="wdOutbound" name="dcStrip" _class="SnoopDnsChange">
        <changeItems>
          <item syntax="1" cs="1" log="true" pattern="ss.*" minimal="false" enabled="true" ip="80.43.80.43"/>
        </changeItems>
      </object>
      <object httpsEnabled="false" disableLoopbackConnection="true" outInThreadTimeout="30000" name="hpStripIn" maxContentCacheSize="10485756" enabled="false" httpEnabled="true" keepAliveTimeout="60000" _class="VWebProxy">
        <outPolicy method="0" host="" port="0"/>
        <tcpServer onceWriteSize="0" port="8042" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" onceWriteSize="0" methodType="4" port="4433" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
      <object httpsEnabled="false" disableLoopbackConnection="true" outInThreadTimeout="30000" name="hpStripOut" maxContentCacheSize="10485756" enabled="false" httpEnabled="true" keepAliveTimeout="60000" _class="VWebProxy">
        <outPolicy method="0" host="" port="0"/>
        <tcpServer onceWriteSize="0" port="8043" _class="VTcpServer" localHost="127.0.0.1"/>
        <sslServer processConnectMessage="false" defaultKeyCrtFileName="default.pem" certificatePath="certificate\" onceWriteSize="0" methodType="4" port="4433" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true" replace="Accept-Encoding:      "/>
        </outboundDataChange>
      </object>
    </objectList>
    <connectList>
      <connect receiver="fmOutboundProc" signal="captured(SnoopPacket*)" sender="wdOutbound" slot="process(SnoopPacket*)"/>
      <connect receiver="fmOutbound" signal="captured(SnoopPacket*)" sender="asOutbound" slot="process(SnoopPacket*)"/>
      <connect receiver="pfOutbound" signal="processed(SnoopPacket*)" sender="fmOutboundProc" slot="check(SnoopPacket*)"/>
      <connect receiver="fmOutbound" signal="nak(SnoopPacket*)" sender="pfOutbound" slot="process(SnoopPacket*)"/>
      <connect receiver="fmInboundProc" signal="captured(SnoopPacket*)" sender="wdInbound" slot="process(SnoopPacket*)"/>
      <connect receiver="pfInbound" signal="processed(SnoopPacket*)" sender="fmInboundProc" slot="check(SnoopPacket*)"/>
      <connect receiver="fmInbound" signal="ack(SnoopPacket*)" sender="pfInbound" slot="process(SnoopPacket*)"/>
      <connect receiver="dProxy" signal="captured(SnoopPacket*)" sender="wdProxy" slot="dump(SnoopPacket*)"/>
      <connect receiver="waProxy" signal="captured(SnoopPacket*)" sender="wdProxy" slot="copy(SnoopPacket*)"/>
      <connect receiver="dcStrip" signal="captured(SnoopPacket*)" sender="asOutbound" slot="check(SnoopPacket*)"/>
      <connect receiver="dcStrip" signal="captured(SnoopPacket*)" sender="wdOutbound" slot="check(SnoopPacket*)"/>
      <connect receiver="fcMain" signal="processed(SnoopPacket*)" sender="fmOutbound" slot="processFromTo(SnoopPacket*)"/>
      <connect receiver="fcMain" signal="processed(SnoopPacket*)" sender="fmInbound" slot="processToFrom(SnoopPacket*)"/>
      <connect receiver="wwdOutbound" signal="changedFromTo(SnoopPacket*)" sender="fcMain" slot="move(SnoopPacket*)"/>
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
    <object x="-66" y="119" name="hpSslIn"/>
    <object x="-71" y="148" name="hpSslOut"/>
    <object x="-72" y="181" name="wdProxy"/>
    <object x="-35" y="217" name="dProxy"/>
    <object x="-102" y="218" name="waProxy"/>
    <object x="-222" y="-73" name="dcStrip"/>
    <object x="1" y="119" name="hpStripIn"/>
    <object x="-3" y="148" name="hpStripOut"/>
  </coord>
</httpsniff>
