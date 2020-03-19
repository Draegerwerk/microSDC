#pragma once

#include <string>

#define WSDLData static constexpr const char*

namespace WSDL
{
  WSDLData SET_SERVICE_WSDL =
      R"(<wsdl:definitions xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/" xmlns:wsp="http://www.w3.org/ns/ws-policy" xmlns:wse="http://schemas.xmlsoap.org/ws/2004/08/eventing" targetNamespace="http://standards.ieee.org/downloads/11073/11073-20701-2018" xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/" xmlns:tns="http://standards.ieee.org/downloads/11073/11073-20701-2018" xmlns:wsa="http://www.w3.org/2005/08/addressing" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:mm="http://standards.ieee.org/downloads/11073/11073-10207-2017/message" xmlns:dpws="http://docs.oasis-open.org/ws-dd/ns/dpws/2009/01">
<wsdl:types>
	<xs:schema targetNamespace="http://standards.ieee.org/downloads/11073/11073-10207-2017/message" xmlns:xs="http://www.w3.org/2001/XMLSchema">
		<xs:include schemaLocation="BICEPS_MessageModel.xsd"/>
	</xs:schema>
</wsdl:types>
<wsdl:message name="SetValue">
	<wsdl:part element="msg:SetValue" name="parameters"/>
</wsdl:message>
<wsdl:message name="SetValueResponse">
	<wsdl:part element="msg:SetValueResponse" name="parameters"/>
</wsdl:message>
<wsdl:message name="SetString">
	<wsdl:part element="msg:SetString" name="parameters"/>
</wsdl:message>
<wsdl:message name="SetStringResponse">
	<wsdl:part element="msg:SetStringResponse" name="parameters"/>
</wsdl:message>
<wsdl:message name="Activate">
	<wsdl:part element="msg:Activate" name="parameters"/>
</wsdl:message>
<wsdl:message name="ActivateResponse">
	<wsdl:part element="msg:ActivateResponse" name="parameters"/>
</wsdl:message>
<wsdl:message name="SetAlertState">
	<wsdl:part element="msg:SetAlertState" name="parameters"/>
</wsdl:message>
<wsdl:message name="SetAlertStateResponse">
	<wsdl:part element="msg:SetAlertStateResponse" name="parameters"/>
</wsdl:message>
<wsdl:message name="SetComponentState">
	<wsdl:part element="msg:SetComponentState" name="parameters"/>
</wsdl:message>
<wsdl:message name="SetComponentStateResponse">
	<wsdl:part element="msg:SetComponentStateResponse" name="parameters"/>
</wsdl:message>
<wsdl:message name="SetMetricState">
	<wsdl:part element="msg:SetMetricState" name="parameters"/>
</wsdl:message>
<wsdl:message name="SetMetricStateResponse">
	<wsdl:part element="msg:SetMetricStateResponse" name="parameters"/>
</wsdl:message>
<wsdl:message name="OperationInvokedReport">
	<wsdl:part element="msg:OperationInvokedReport" name="parameters"/>
</wsdl:message>
<wsdl:portType name="SetService" wse:EventSource="true" dpws:DiscoveryType="pm:ServiceProvider">
	<wsdl:operation name="SetValue">
		<wsdl:input message="tns:SetValue" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetValue"/>
		<wsdl:output message="tns:SetValueResponse" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetValueResponse"/>
	</wsdl:operation>
	<wsdl:operation name="SetString">
		<wsdl:input message="tns:SetString" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetString"/>
		<wsdl:output message="tns:SetStringResponse" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetStringResponse"/>
	</wsdl:operation>
	<wsdl:operation name="Activate">
		<wsdl:input message="tns:Activate" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/Activate"/>
		<wsdl:output message="tns:ActivateResponse" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/ActivateResponse"/>
	</wsdl:operation>
	<wsdl:operation name="SetAlertState">
		<wsdl:input message="tns:SetAlertState" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetAlertState"/>
		<wsdl:output message="tns:SetAlertStateResponse" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetAlertStateResponse"/>
	</wsdl:operation>
	<wsdl:operation name="SetComponentState">
		<wsdl:input message="tns:SetComponentState" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetComponentState"/>
		<wsdl:output message="tns:SetComponentStateResponse" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetComponentStateResponse"/>
	</wsdl:operation>
	<wsdl:operation name="SetMetricState">
		<wsdl:input message="tns:SetMetricState" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetMetricState"/>
		<wsdl:output message="tns:SetMetricStateResponse" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetMetricStateResponse"/>
	</wsdl:operation>
	<wsdl:operation name="OperationInvokedReport">
		<wsdl:output message="tns:OperationInvokedReport" wsa:Action="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/OperationInvokedReport"/>
	</wsdl:operation>
</wsdl:portType>
<wsdl:binding name="SetServiceBinding" type="tns:SetService">
	<soap:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/>
	<wsdl:operation name="SetValue">
		<soap:operation soapAction="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetValue" style="document"/>
		<wsdl:input><soap:body use="literal"/></wsdl:input>
		<wsdl:output><soap:body use="literal"/></wsdl:output>
	</wsdl:operation>
	<wsdl:operation name="SetString">
		<soap:operation soapAction="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetString" style="document"/>
		<wsdl:input><soap:body use="literal"/></wsdl:input>
		<wsdl:output><soap:body use="literal"/></wsdl:output>
	</wsdl:operation>
	<wsdl:operation name="Activate">
		<soap:operation soapAction="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/Activate" style="document"/>
		<wsdl:input><soap:body use="literal"/></wsdl:input>
		<wsdl:output><soap:body use="literal"/></wsdl:output>
	</wsdl:operation>
	<wsdl:operation name="SetAlertState">
		<soap:operation soapAction="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetAlertState" style="document"/>
		<wsdl:input><soap:body use="literal"/></wsdl:input>
		<wsdl:output><soap:body use="literal"/></wsdl:output>
	</wsdl:operation>
	<wsdl:operation name="SetComponentState">
		<soap:operation soapAction="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetComponentState" style="document"/>
		<wsdl:input><soap:body use="literal"/></wsdl:input>
		<wsdl:output><soap:body use="literal"/></wsdl:output>
	</wsdl:operation>
	<wsdl:operation name="SetMetricState">
		<soap:operation soapAction="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetMetricState" style="document"/>
		<wsdl:input><soap:body use="literal"/></wsdl:input>
		<wsdl:output><soap:body use="literal"/></wsdl:output>
	</wsdl:operation>
	<wsdl:operation name="OperationInvokedReport">
		<soap:operation soapAction="http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/OperationInvokedReport" style="document"/>
		<wsdl:output><soap:body use="literal"/></wsdl:output>
	</wsdl:operation>
	<wsp:Policy>
		<dpws:Profile wsp:Optional="true" />
	</wsp:Policy>
</wsdl:binding>
</wsdl:definitions>)";

}
