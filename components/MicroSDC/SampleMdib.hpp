static constexpr const char* SampleMdib_l = R"(
    <mm:GetMdibResponse MdibVersion="82" SequenceId="0">
      <mm:Mdib MdibVersion="82" SequenceId="0" xmlns:pm="http://standards.ieee.org/downloads/11073/11073-10207-2017/participant" xmlns:ext="http://standards.ieee.org/downloads/11073/11073-10207-2017/extension">
        <pm:MdDescription>
          <pm:Mds Handle="MedicalDevices">
            <pm:MetaData>
              <pm:Manufacturer Ref="Draeger"/>
              <pm:ModelName Ref="MicroSDC"/>
              <pm:ModelNumber>1</pm:ModelNumber>
              <pm:SerialNumber>2019</pm:SerialNumber>
            </pm:MetaData>
            <pm:SystemContext Handle="_contextHandlemd0">
              <pm:PatientContext Handle="_patientHandlemd0" SafetyClassification="MedA">
              </pm:PatientContext>
            </pm:SystemContext>
            <pm:Vmd Handle="device_vmd">
              <pm:Channel Handle="device_channel" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
                <pm:Metric Handle="handle_get" MetricAvailability="Cont" MetricCategory="Msrmt" Resolution="1" xsi:type="pm:NumericMetricDescriptor">
                  <pm:Unit Code="262656" />
                </pm:Metric>
              </pm:Channel>
            </pm:Vmd>
          </pm:Mds>
        </pm:MdDescription>
        <pm:MdState xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
          <pm:State ActivationState="On" DescriptorHandle="handle_get" xsi:type="pm:NumericMetricState">
            <pm:MetricValue Value=")";
static constexpr const char* SampleMdib_r = R"(">
              <pm:MetricQuality Validity="Vld"/>
            </pm:MetricValue>
          </pm:State>
          <pm:State xsi:type="pm:ChannelState" DescriptorHandle="device_channel"></pm:State>
          <pm:State xsi:type="pm:VmdState" DescriptorHandle="device_vmd"></pm:State>
          <pm:State xsi:type="pm:MdsState" DescriptorHandle="MedicalDevices"></pm:State>
        </pm:MdState>
      </mm:Mdib>
    </mm:GetMdibResponse>
    )";