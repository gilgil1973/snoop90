<httpsniff>
  <graph>
    <objectList>
      <object correctChecksum="true" autoParse="true" tos="0" autoRead="true" queueTime="1024" queueLen="8192" _class="SnoopWinDivert" name="wdOutbound" filter="true" enabled="true" flags="0" priority="0" layer="0"/>
      <object autoParse="true" readTimeout="1" virtualMac="000000-000000" selfRelay="true" infectInterval="1000" autoRead="true" snapLen="1600" adapterIndex="0" _class="SnoopArpSpoof" name="asOutbound" filter="" enabled="false" flags="1" disableAutoRouting="true">
        <sessionList/>
      </object>
      <object checkInterval="1" macFlowTimeout="3600" ipFlowTimeout="300" tcpFlowTimeout="300" udpFlowTimeout="300" _class="SnoopFlowMgr" name="fmOutboundProc"/>
      <object showStatus="false" flowMgr="fmOutboundProc" _class="SnoopProcessFilter" name="pfOutbound">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="false" processName="snoopspy.exe"/>
        </policies>
      </object>
      <object checkInterval="1" macFlowTimeout="3600" ipFlowTimeout="300" tcpFlowTimeout="300" udpFlowTimeout="300" _class="SnoopFlowMgr" name="fmOutbound"/>
      <object correctChecksum="true" autoParse="true" tos="0" autoRead="true" queueTime="1024" queueLen="8192" _class="SnoopWinDivert" name="wdInbound" filter="true" enabled="true" flags="0" priority="0" layer="0"/>
      <object checkInterval="1" macFlowTimeout="3600" ipFlowTimeout="300" tcpFlowTimeout="300" udpFlowTimeout="300" _class="SnoopFlowMgr" name="fmInboundProc"/>
      <object showStatus="false" flowMgr="fmInboundProc" _class="SnoopProcessFilter" name="pfInbound">
        <policies>
          <policy ack="false" processName=""/>
          <policy ack="false" processName="snoopspy.exe"/>
        </policies>
      </object>
      <object checkInterval="1" macFlowTimeout="3600" ipFlowTimeout="300" tcpFlowTimeout="300" udpFlowTimeout="300" _class="SnoopFlowMgr" name="fmInbound"/>
      <object toFlowMgr="fmInbound" tcpChange="true" udpChange="true" _class="SnoopFlowChange" name="fcMain" fromFlowMgr="fmOutbound">
        <changeItems/>
      </object>
      <object Direction="0" correctChecksum="true" SubIfIdx="0" autoParse="false" tos="0" IfIdx="0" autoRead="false" queueTime="1024" queueLen="8192" changeDivertAddr="true" _class="SnoopWriteWinDivert" name="wwdOutbound" filter="true" enabled="true" flags="1" priority="0" layer="0"/>
      <object disableLoopbackConnection="true" tcpEnabled="true" outInThreadTimeout="30000" sslEnabled="true" maxContentCacheSize="10485756" _class="VHttpProxy" name="hpTcpIn" keepAliveTimeout="60000">
        <outPolicy method="0" port="0" host=""/>
        <tcpServer port="8080" onceWriteSize="0" _class="VTcpServer" localHost=""/>
        <sslServer port="4433" onceWriteSize="0" methodType="4" certificatePath="certificate\" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item replace="Accept-Encoding:      " syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true"/>
        </outboundDataChange>
      </object>
      <object disableLoopbackConnection="true" tcpEnabled="true" outInThreadTimeout="30000" sslEnabled="true" maxContentCacheSize="10485756" _class="VHttpProxy" name="hpTcpOut" keepAliveTimeout="60000">
        <outPolicy method="0" port="0" host=""/>
        <tcpServer port="8080" onceWriteSize="0" _class="VTcpServer" localHost=""/>
        <sslServer port="4433" onceWriteSize="0" methodType="4" certificatePath="certificate\" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item replace="Accept-Encoding:      " syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true"/>
        </outboundDataChange>
      </object>
      <object disableLoopbackConnection="true" tcpEnabled="true" outInThreadTimeout="30000" sslEnabled="true" maxContentCacheSize="10485756" _class="VHttpProxy" name="hpSslIn" keepAliveTimeout="60000">
        <outPolicy method="0" port="0" host=""/>
        <tcpServer port="8080" onceWriteSize="0" _class="VTcpServer" localHost=""/>
        <sslServer port="4433" onceWriteSize="0" methodType="4" certificatePath="certificate\" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item replace="Accept-Encoding:      " syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true"/>
        </outboundDataChange>
      </object>
      <object disableLoopbackConnection="true" tcpEnabled="true" outInThreadTimeout="30000" sslEnabled="true" maxContentCacheSize="10485756" _class="VHttpProxy" name="hpSslOut" keepAliveTimeout="60000">
        <outPolicy method="0" port="0" host=""/>
        <tcpServer port="8080" onceWriteSize="0" _class="VTcpServer" localHost=""/>
        <sslServer port="4433" onceWriteSize="0" methodType="4" certificatePath="certificate\" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item replace="Accept-Encoding:      " syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true"/>
        </outboundDataChange>
      </object>
      <object correctChecksum="true" autoParse="true" tos="0" autoRead="true" queueTime="1024" queueLen="8192" _class="SnoopWinDivert" name="wdProxy" filter="true" enabled="true" flags="0" priority="0" layer="0"/>
      <object linkType="1" filePath="pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap" _class="SnoopDump" name="dProxy"/>
      <object autoParse="false" readTimeout="1" srcMac="000000-000000" autoRead="false" snapLen="1600" dstMac="000000-000000" adapterIndex="-1" _class="SnoopWriteAdapter" name="waProxy" filter="" enabled="true" flags="1"/>
      <object writer="wdOutbound" _class="SnoopDnsChange" name="dcStrip">
        <changeItems>
          <item syntax="1" cs="1" log="true" pattern="ss.*" minimal="false" enabled="true" ip="80.43.80.43"/>
        </changeItems>
      </object>
      <object disableLoopbackConnection="true" tcpEnabled="true" outInThreadTimeout="30000" sslEnabled="true" maxContentCacheSize="10485756" _class="VHttpProxy" name="hpStripIn" keepAliveTimeout="60000">
        <outPolicy method="0" port="0" host=""/>
        <tcpServer port="8080" onceWriteSize="0" _class="VTcpServer" localHost=""/>
        <sslServer port="4433" onceWriteSize="0" methodType="4" certificatePath="certificate\" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item replace="Accept-Encoding:      " syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true"/>
        </outboundDataChange>
      </object>
      <object disableLoopbackConnection="true" tcpEnabled="true" outInThreadTimeout="30000" sslEnabled="true" maxContentCacheSize="10485756" _class="VHttpProxy" name="hpStripOut" keepAliveTimeout="60000">
        <outPolicy method="0" port="0" host=""/>
        <tcpServer port="8080" onceWriteSize="0" _class="VTcpServer" localHost=""/>
        <sslServer port="4433" onceWriteSize="0" methodType="4" certificatePath="certificate\" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer" localHost=""/>
        <inboundDataChange/>
        <outboundDataChange>
          <item replace="Accept-Encoding:      " syntax="2" cs="1" log="false" pattern="Accept-Encoding: gzip," minimal="false" enabled="true"/>
        </outboundDataChange>
      </object>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" receiver="fmOutboundProc" slot="process(SnoopPacket*)" sender="wdOutbound"/>
      <connect signal="captured(SnoopPacket*)" receiver="fmOutbound" slot="process(SnoopPacket*)" sender="asOutbound"/>
      <connect signal="processed(SnoopPacket*)" receiver="pfOutbound" slot="check(SnoopPacket*)" sender="fmOutboundProc"/>
      <connect signal="nak(SnoopPacket*)" receiver="fmOutbound" slot="process(SnoopPacket*)" sender="pfOutbound"/>
      <connect signal="captured(SnoopPacket*)" receiver="fmInboundProc" slot="process(SnoopPacket*)" sender="wdInbound"/>
      <connect signal="processed(SnoopPacket*)" receiver="pfInbound" slot="check(SnoopPacket*)" sender="fmInboundProc"/>
      <connect signal="ack(SnoopPacket*)" receiver="fmInbound" slot="process(SnoopPacket*)" sender="pfInbound"/>
      <connect signal="captured(SnoopPacket*)" receiver="dProxy" slot="dump(SnoopPacket*)" sender="wdProxy"/>
      <connect signal="captured(SnoopPacket*)" receiver="waProxy" slot="copy(SnoopPacket*)" sender="wdProxy"/>
      <connect signal="captured(SnoopPacket*)" receiver="dcStrip" slot="check(SnoopPacket*)" sender="asOutbound"/>
      <connect signal="captured(SnoopPacket*)" receiver="dcStrip" slot="check(SnoopPacket*)" sender="wdOutbound"/>
      <connect signal="processed(SnoopPacket*)" receiver="fcMain" slot="processFromTo(SnoopPacket*)" sender="fmOutbound"/>
      <connect signal="processed(SnoopPacket*)" receiver="fcMain" slot="processToFrom(SnoopPacket*)" sender="fmInbound"/>
      <connect signal="changedFromTo(SnoopPacket*)" receiver="wwdOutbound" slot="move(SnoopPacket*)" sender="fcMain"/>
    </connectList>
  </graph>
  <coord>
    <object x="-141" name="wdOutbound" y="-114"/>
    <object x="-236" name="asOutbound" y="-114"/>
    <object x="-153" name="fmOutboundProc" y="-75"/>
    <object x="-138" name="pfOutbound" y="-38"/>
    <object x="-139" name="fmOutbound" y="1"/>
    <object x="-32" name="wdInbound" y="-114"/>
    <object x="-43" name="fmInboundProc" y="-75"/>
    <object x="-28" name="pfInbound" y="-37"/>
    <object x="-30" name="fmInbound" y="0"/>
    <object x="-68" name="fcMain" y="39"/>
    <object x="-145" name="wwdOutbound" y="74"/>
    <object x="-137" name="hpTcpIn" y="119"/>
    <object x="-142" name="hpTcpOut" y="149"/>
    <object x="-67" name="hpSslIn" y="119"/>
    <object x="-71" name="hpSslOut" y="148"/>
    <object x="-72" name="wdProxy" y="181"/>
    <object x="-35" name="dProxy" y="217"/>
    <object x="-102" name="waProxy" y="218"/>
    <object x="-222" name="dcStrip" y="-73"/>
    <object x="1" name="hpStripIn" y="119"/>
    <object x="-3" name="hpStripOut" y="148"/>
  </coord>
</httpsniff>
