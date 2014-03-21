<ssl_strip>
  <graph>
    <objectList>
      <object priority="0" flags="0" correctChecksum="true" layer="0" tos="0" autoParse="true" autoRead="true" queueLen="8192" name="WinDivert1" queueTime="1024" _class="SnoopWinDivert" filter="tcp.DstPort == 80 or tcp.SrcPort == 80 or udp.DstPort == 53"/>
      <object checkInterval="1" macFlowTimeout="3600" udpFlowTimeout="300" tcpFlowTimeout="300" ipFlowTimeout="300" name="FlowMgr1" _class="SnoopFlowMgr"/>
      <object linkType="1" name="BpFilter1" _class="SnoopBpFilter" filter="tcp dst port 80"/>
      <object linkType="1" name="BpFilter2" _class="SnoopBpFilter" filter="tcp src port 80"/>
      <object linkType="1" name="BpFilter3" _class="SnoopBpFilter" filter="udp dst port 53"/>
      <object flowMgr="FlowMgr1" tcpChange="true" name="DataChange1" _class="SnoopDataChange" udpChange="false">
        <dataChange>
          <item cs="1" syntax="2" log="false" pattern="Accept-Encoding: gzip,deflate" minimal="false" enabled="true" replace="Accept-Encoding:             "/>
          <item cs="1" syntax="2" log="false" pattern="Accept-Encoding: gzip, deflate" minimal="false" enabled="true" replace="Accept-Encoding:              "/>
        </dataChange>
      </object>
      <object flowMgr="FlowMgr1" tcpChange="true" name="DataChange2" _class="SnoopDataChange" udpChange="false">
        <dataChange>
          <item cs="1" syntax="2" log="true" pattern="https://a" minimal="false" enabled="true" replace="http://a."/>
          <item cs="1" syntax="2" log="true" pattern="https://b" minimal="false" enabled="true" replace="http://b."/>
          <item cs="1" syntax="2" log="true" pattern="https://c" minimal="false" enabled="true" replace="http://c."/>
          <item cs="1" syntax="2" log="true" pattern="https://d" minimal="false" enabled="true" replace="http://d."/>
          <item cs="1" syntax="2" log="true" pattern="https://e" minimal="false" enabled="true" replace="http://e."/>
          <item cs="1" syntax="2" log="true" pattern="https://f" minimal="false" enabled="true" replace="http://f."/>
          <item cs="1" syntax="2" log="true" pattern="https://g" minimal="false" enabled="true" replace="http://g."/>
          <item cs="1" syntax="2" log="true" pattern="https://h" minimal="false" enabled="true" replace="http://h."/>
          <item cs="1" syntax="2" log="true" pattern="https://i" minimal="false" enabled="true" replace="http://i."/>
          <item cs="1" syntax="2" log="true" pattern="https://j" minimal="false" enabled="true" replace="http://j."/>
          <item cs="1" syntax="2" log="true" pattern="https://k" minimal="false" enabled="true" replace="http://k."/>
          <item cs="1" syntax="2" log="true" pattern="https://l" minimal="false" enabled="true" replace="http://l."/>
          <item cs="1" syntax="3" log="true" pattern="https://m" minimal="false" enabled="true" replace="http://m."/>
          <item cs="1" syntax="5" log="true" pattern="https://n" minimal="false" enabled="true" replace="http://n."/>
          <item cs="1" syntax="2" log="true" pattern="https://o" minimal="false" enabled="true" replace="http://o."/>
          <item cs="1" syntax="2" log="true" pattern="https://p" minimal="false" enabled="true" replace="http://p."/>
          <item cs="1" syntax="2" log="true" pattern="https://q" minimal="false" enabled="true" replace="http://q."/>
          <item cs="1" syntax="2" log="true" pattern="https://r" minimal="false" enabled="true" replace="http://r."/>
          <item cs="1" syntax="2" log="true" pattern="https://s" minimal="false" enabled="true" replace="http://s."/>
          <item cs="1" syntax="2" log="true" pattern="https://t" minimal="false" enabled="true" replace="http://t."/>
          <item cs="1" syntax="2" log="true" pattern="https://u" minimal="false" enabled="true" replace="http://u."/>
          <item cs="1" syntax="2" log="true" pattern="https://v" minimal="false" enabled="true" replace="http://v."/>
          <item cs="1" syntax="2" log="true" pattern="https://w" minimal="false" enabled="true" replace="http://w."/>
          <item cs="1" syntax="2" log="true" pattern="https://x" minimal="false" enabled="true" replace="http://x."/>
          <item cs="1" syntax="2" log="true" pattern="https://y" minimal="false" enabled="true" replace="http://y."/>
          <item cs="1" syntax="2" log="true" pattern="https://z" minimal="false" enabled="true" replace="http://z."/>
          <item cs="1" syntax="2" log="true" pattern="window.location.replace(&quot;https:\/\/www.facebook.com\/&quot;)" minimal="false" enabled="true" replace="window.location.replace(&quot;http:\/\/w.ww.facebook.com\/&quot;)"/>
        </dataChange>
      </object>
      <object capture="WinDivert1" name="DnsChange1" _class="SnoopDnsChange">
        <changeItems>
          <item cs="1" syntax="0" log="true" pattern="[a-z]\..*" minimal="false" enabled="true" ip="127.0.0.1"/>
        </changeItems>
      </object>
      <object name="Command1" _class="SnoopCommand">
        <open>
          <command0 sync="true" enabled="true" command="ipconfig /flushdns"/>
        </open>
        <close/>
      </object>
      <object name="DataFind1" _class="SnoopDataFind">
        <dataFind>
          <item cs="1" syntax="0" pattern="https://[^\r\n]*" minimal="false" enabled="true"/>
        </dataFind>
      </object>
      <object name="DataFind2" _class="SnoopDataFind">
        <dataFind>
          <item cs="1" syntax="0" pattern="POST.*" minimal="false" enabled="true"/>
        </dataFind>
      </object>
      <object readTimeout="1" flags="1" snapLen="1600" autoParse="true" autoRead="false" name="WriteAdapter1" adapterIndex="1" _class="SnoopWriteAdapter" filter=""/>
    </objectList>
    <connectList>
      <connect slot="process(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="FlowMgr1" sender="WinDivert1"/>
      <connect slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="BpFilter1" sender="FlowMgr1"/>
      <connect slot="check(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="DnsChange1" sender="BpFilter3"/>
      <connect slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="BpFilter2" sender="FlowMgr1"/>
      <connect slot="check(SnoopPacket*)" signal="processed(SnoopPacket*)" receiver="BpFilter3" sender="FlowMgr1"/>
      <connect slot="find(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="DataFind1" sender="BpFilter2"/>
      <connect slot="change(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="DataChange2" sender="BpFilter2"/>
      <connect slot="find(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="DataFind2" sender="BpFilter1"/>
      <connect slot="change(SnoopPacket*)" signal="ack(SnoopPacket*)" receiver="DataChange1" sender="BpFilter1"/>
      <connect slot="output(SnoopPacket*)" signal="captured(SnoopPacket*)" receiver="WriteAdapter1" sender="WinDivert1"/>
    </connectList>
  </graph>
  <coord>
    <object x="25" y="-213" name="WinDivert1"/>
    <object x="27" y="-175" name="FlowMgr1"/>
    <object x="-139" y="-132" name="BpFilter1"/>
    <object x="24" y="-134" name="BpFilter2"/>
    <object x="144" y="-136" name="BpFilter3"/>
    <object x="-154" y="-79" name="DataChange1"/>
    <object x="33" y="-81" name="DataChange2"/>
    <object x="131" y="-81" name="DnsChange1"/>
    <object x="125" y="-217" name="Command1"/>
    <object x="-40" y="-81" name="DataFind1"/>
    <object x="-228" y="-79" name="DataFind2"/>
    <object x="118" y="-174" name="WriteAdapter1"/>
  </coord>
</ssl_strip>
