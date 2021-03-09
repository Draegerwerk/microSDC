#pragma once

#define NameSpaceConstant static constexpr const char*
#define SDCConstant static constexpr const char*

namespace SDC
{
  SDCConstant UUID_SDC_PREFIX = "urn:uuid:";
  NameSpaceConstant NS_GLUE = "http://standards.ieee.org/downloads/11073/11073-20701-2018";
  NameSpaceConstant NS_GLUE_PREFIX = "glue";
  NameSpaceConstant NS_BICEPS_MESSAGE_MODEL =
      "http://standards.ieee.org/downloads/11073/11073-10207-2017/message";
  NameSpaceConstant NS_BICEPS_PARTICIPANT_MODEL =
      "http://standards.ieee.org/downloads/11073/11073-10207-2017/participant";
  NameSpaceConstant NS_BICEPS_EXTENSION =
      "http://standards.ieee.org/downloads/11073/11073-10207-2017/extension";

  SDCConstant QNAME_GETSERVICE = "GetService";
  SDCConstant QNAME_SETSERVICE = "SetService";
  SDCConstant QNAME_STATEEVENTSERVICE = "StateEventService";
  SDCConstant ACTION_GET_MDIB_REQUEST =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/GetService/GetMdib";
  SDCConstant ACTION_GET_MDIB_RESPONSE =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/GetService/GetMdibResponse";

  SDCConstant ACTION_OPERATION_INVOKED_REPORT =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/"
      "OperationInvokedReport";

  SDCConstant ACTION_SET_STRING =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetString";
  SDCConstant ACTION_SET_STRING_RESPONSE =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetStringResponse";
  SDCConstant ACTION_SET_VALUE =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetValue";
  SDCConstant ACTION_SET_VALUE_RESPONSE =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/SetService/SetValueResponse";
  SDCConstant ACTION_EPISODIC_ALERT_REPORT =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/StateEventService/"
      "EpisodicAlertReport";
  SDCConstant ACTION_PERIODIC_ALERT_REPORT =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/StateEventService/"
      "PeriodicAlertReport";
  SDCConstant ACTION_EPISODIC_COMPONENT_REPORT =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/StateEventService/"
      "EpisodicComponentReport";
  SDCConstant ACTION_PERIODIC_COMPONENT_REPORT =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/StateEventService/"
      "PeriodicComponentReport";
  SDCConstant ACTION_EPISODIC_METRIC_REPORT =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/StateEventService/"
      "EpisodicMetricReport";
  SDCConstant ACTION_PERIODIC_METRIC_REPORT =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/StateEventService/"
      "PeriodicMetricReport";
  SDCConstant ACTION_EPISODIC_OPERATIONAL_STATE_REPORT =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/StateEventService/"
      "EpisodicOperationalStateReport";
  SDCConstant ACTION_PERIODIC_OPERATIONAL_STATE_REPORT =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/StateEventService/"
      "PeriodicOperationalStateReport";

} // namespace SDC
