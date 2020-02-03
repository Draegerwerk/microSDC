#pragma once

#include "frozen/string.h"

#define NameSpaceConstant static constexpr frozen::string
#define SDCConstant static constexpr const char*

namespace SDC
{
  SDCConstant UUID_SDC_PREFIX = "urn:uuid:";
  NameSpaceConstant NS_GLUE = "http://standards.ieee.org/downloads/11073/11073-20701-2018";
  NameSpaceConstant NS_BICEPS_MESSAGE_MODEL =
      "http://standards.ieee.org/downloads/11073/11073-10207-2017/message";
  NameSpaceConstant NS_BICEPS_PARTICIPANT_MODEL =
      "http://standards.ieee.org/downloads/11073/11073-10207-2017/participant";
  NameSpaceConstant NS_BICEPS_EXTENSION =
      "http://standards.ieee.org/downloads/11073/11073-10207-2017/extension";

  SDCConstant QNAME_GETSERVICE = "GetService";
  SDCConstant ACTION_GET_MDIB_REQUEST =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/GetService/GetMdib";
  SDCConstant ACTION_GET_MDIB_RESPONSE =
      "http://standards.ieee.org/downloads/11073/11073-20701-2018/GetService/GetMdibResponse";

} // namespace SDC
