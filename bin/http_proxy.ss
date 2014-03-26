<http_proxy>
  <graph>
    <objectList>
      <object tcpEnabled="true" sslEnabled="true" _class="VHttpProxy" name="HttpProxy1">
        <outPolicy method="0" port="0" host=""/>
        <tcpServer localHost="" port="8080" onceWriteSize="0" _class="VTcpServer"/>
        <sslServer localHost="" certificatePath="certificate/" methodType="4" port="4433" onceWriteSize="0" processConnectMessage="false" defaultKeyCrtFileName="default.pem" _class="VSslServer"/>
        <inboundDataChange>
          <item syntax="2" pattern="hello world" minimal="false" enabled="true" log="true" replace="HELLO WORLD" cs="1"/>
        </inboundDataChange>
        <outboundDataChange>
          <item syntax="2" pattern="Accept-Encoding: gzip,deflate,sdch" minimal="false" enabled="true" log="true" replace="Accept-Encoding:              sdch" cs="1"/>
          <item syntax="2" pattern="Accept-Encoding: gzip, deflate" minimal="false" enabled="true" log="true" replace="Accept-Encoding:              " cs="1"/>
        </outboundDataChange>
      </object>
    </objectList>
    <connectList/>
  </graph>
  <coord>
    <object x="-44" y="-170" name="HttpProxy1"/>
  </coord>
</http_proxy>
