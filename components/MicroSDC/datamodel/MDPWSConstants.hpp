#pragma once

#include "frozen/string.h"

#define NameSpaceConstant static constexpr frozen::string
#define MDPWSConstant static constexpr const char*

namespace MDPWS
{

  MDPWSConstant DPWS11 = "DPWS_11";

  MDPWSConstant XML_ENCODING = "UTF-8";

  static constexpr int APP_MAX_DELAY = 500; // milliseconds [WS-discovery]
  static constexpr int MATCH_TIMEOUT = 10;  // seconds [WS-discovery]
  static constexpr int DP_MAX_TIMEOUT = 5;  // seconds for discovery proxy timeout [WS-discovery]
  static constexpr int MAX_ENVELOPE_SIZE = 32767;    // octets
  static constexpr int MAX_UDP_ENVELOPE_SIZE = 4096; // octets
  static constexpr int MAX_FIELD_SIZE = 256;         // unicode characters
  static constexpr int MAX_URI_SIZE = 2048;          // octets
  static constexpr int MULTICAST_UDP_REPEAT = 1;     // [SOAP-over-UDP]
  static constexpr int UDP_MAX_DELAY = 250;          // milliseconds [SOAP-over-UDP]
  static constexpr int UDP_MIN_DELAY = 50;           // milliseconds [SOAP-over-UDP]
  static constexpr int UDP_UPPER_DELAY = 450;        // milliseconds [SOAP-over-UDP]
  static constexpr int UDP_ENVELOPE_SIZE = 4096;
  static constexpr int UDP_UNICAST_UDP_REPEAT = 1;
  static constexpr int UDP_UNICAST_MIN_DELAY = 50;
  static constexpr int UDP_UNICAST_MAX_DELAY = 250;
  static constexpr int UDP_UNICAST_UPPER_DELAY = 500;
  static constexpr unsigned short UDP_MULTICAST_DISCOVERY_PORT = 3702;
  static constexpr unsigned short UDP_MULTICAST_STREAMING_PORT = 5555;
  static constexpr int UDP_MULTICAST_TIMETOLIVE = 8;

  static constexpr int UDP_MULTICAST_UDP_REPEAT = 2;
  static constexpr int UDP_MULTICAST_MIN_DELAY = 50;
  static constexpr int UDP_MULTICAST_MAX_DELAY = 250;
  static constexpr int UDP_MULTICAST_UPPER_DELAY = 500;
  MDPWSConstant UDP_MULTICAST_DISCOVERY_IP_V4 = "239.255.255.250";
  MDPWSConstant UDP_MULTICAST_DISCOVERY_IP_V6 = "FF02::C";
  MDPWSConstant UDP_MULTICAST_STREAMING_IP_V4 = "239.239.239.235";
  MDPWSConstant UDP_MULTICAST_STREAMING_IP_V6 = "FF01::C";


  NameSpaceConstant WS_NS_DPWS = "http://docs.oasis-open.org/ws-dd/ns/dpws/2009/01";
  NameSpaceConstant NS_MDPWS = "http://standards.ieee.org/downloads/11073/11073-20702-2016";
  NameSpaceConstant WS_NS_SOAP_ENVELOPE = "http://www.w3.org/2003/05/soap-envelope";
  NameSpaceConstant WS_NS_WSDL_SOAP = "http://schemas.xmlsoap.org/wsdl/";
  NameSpaceConstant WS_NS_WSDL_W3_SOAP = "http://www.w3.org/ns/wsdl/soap";
  NameSpaceConstant WS_NS_WSDL_W3_SOAP_HTTP = "http://www.w3.org/2003/05/soap/bindings/HTTP/";
  NameSpaceConstant WS_NS_WSDL_SOAP12_SCHEMA = "http://schemas.xmlsoap.org/wsdl/soap12/";
  NameSpaceConstant WS_NS_WSDL_XML_SCHEMA = "http://www.w3.org/2001/XMLSchema";
  NameSpaceConstant WS_NS_WSDL_XML_SCHEMA_INSTANCE = "http://www.w3.org/2001/XMLSchema-instance";
  NameSpaceConstant WS_NS_WSDL_SOAP_HTTP = "http://schemas.xmlsoap.org/soap/http";
  NameSpaceConstant WS_NS_ADDRESSING = "http://www.w3.org/2005/08/addressing";
  NameSpaceConstant WS_NS_ADDRESSING_METADATA = "http://www.w3.org/2007/05/addressing/metadata";
  NameSpaceConstant WS_NS_METADATA_EXCHANGE = "http://schemas.xmlsoap.org/ws/2004/09/mex";
  NameSpaceConstant WS_NS_DISCOVERY = "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01";
  NameSpaceConstant WS_NS_EVENTING = "http://schemas.xmlsoap.org/ws/2004/08/eventing";

  MDPWSConstant WS_ADDRESSING_ANONYMOUS = "http://www.w3.org/2005/08/addressing/anonymous";
  MDPWSConstant WS_ADDRESSING_REPLY = "http://www.w3.org/2005/08/addressing/reply";

  MDPWSConstant WS_ACTION_HELLO = "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Hello";
  MDPWSConstant WS_ACTION_BYE = "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Bye";
  MDPWSConstant WS_ACTION_GET = "http://schemas.xmlsoap.org/ws/2004/09/transfer/Get";
  MDPWSConstant WS_ACTION_GET_RESPONSE =
      "http://schemas.xmlsoap.org/ws/2004/09/transfer/GetResponse";
  MDPWSConstant WS_ACTION_GET_METADATA_REQUEST =
      "http://schemas.xmlsoap.org/ws/2004/09/mex/GetMetadata/Request";
  MDPWSConstant WS_ACTION_GET_METADATA_RESPONSE =
      "http://schemas.xmlsoap.org/ws/2004/09/mex/GetMetadata/Response";

  MDPWSConstant WS_ACTION_RESOLVE = "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Resolve";
  MDPWSConstant WS_ACTION_RESOLVE_MATCHES =
      "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/ResolveMatches";
  MDPWSConstant WS_ACTION_PROBE = "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Probe";
  MDPWSConstant WS_ACTION_PROBE_MATCHES =
      "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/ProbeMatches";

  MDPWSConstant WS_ACTION_SUBSCRIBE = "http://schemas.xmlsoap.org/ws/2004/08/eventing/Subscribe";
  MDPWSConstant WS_ACTION_SUBSCRIBE_RESPONSE =
      "http://schemas.xmlsoap.org/ws/2004/08/eventing/SubscribeResponse";
  MDPWSConstant WS_ACTION_RENEW = "http://schemas.xmlsoap.org/ws/2004/08/eventing/Renew";
  MDPWSConstant WS_ACTION_RENEW_RESPONSE =
      "http://schemas.xmlsoap.org/ws/2004/08/eventing/RenewResponse";
  MDPWSConstant WS_ACTION_UNSUBSCRIBE =
      "http://schemas.xmlsoap.org/ws/2004/08/eventing/Unsubscribe";
  MDPWSConstant WS_ACTION_UNSUBSCRIBE_RESPONSE =
      "http://schemas.xmlsoap.org/ws/2004/08/eventing/UnsubscribeResponse";
  MDPWSConstant WS_ACTION_GETSTATUS = "http://schemas.xmlsoap.org/ws/2004/08/eventing/GetStatus";
  MDPWSConstant WS_ACTION_GETSTATUS_RESPONSE =
      "http://schemas.xmlsoap.org/ws/2004/08/eventing/GetStatusResponse";

  MDPWSConstant WS_DISCOVERY_URN = "urn:docs-oasis-open-org:ws-dd:ns:discovery:2009:01";

  MDPWSConstant WS_MEX_DIALECT_MODEL = "http://docs.oasis-open.org/ws-dd/ns/dpws/2009/01/ThisModel";
  MDPWSConstant WS_MEX_DIALECT_DEVICE =
      "http://docs.oasis-open.org/ws-dd/ns/dpws/2009/01/ThisDevice";
  MDPWSConstant WS_MEX_DIALECT_WSDL = "http://schemas.xmlsoap.org/wsdl/";

  MDPWSConstant WS_MEX_DIALECT_REL =
      "http://docs.oasis-open.org/ws-dd/ns/dpws/2009/01/Relationship";
  MDPWSConstant WS_MEX_REL_HOST = "http://docs.oasis-open.org/ws-dd/ns/dpws/2009/01/host";

  MDPWSConstant WS_EVENTING_DELIVERYMODE_PUSH =
      "http://schemas.xmlsoap.org/ws/2004/08/eventing/DeliveryModes/Push";
  MDPWSConstant WS_EVENTING_FILTER_ACTION =
      "http://docs.oasis-open.org/ws-dd/ns/dpws/2009/01/Action";

} // namespace MDPWS
