//Test module for Temp-Module while LibIIO is being ported
//
//acts as random number generator to generate fake temp vals

#include "Placeholder_TempInterface.h"
#include "string.h"

void getTempVals(char* Buff)
{
  float RTDRandom = 0,TCRandom = 0;
  
  RTDRandom = rand()/(float)0x3fffffff*(float)2;
  TCRandom = rand()/(float)0x3fffffff*(float)2;
  
  sprintf(Buff, "sCJT:%f;TCT:%f;\n\r", (float)20 + RTDRandom, (float)21 + TCRandom);
}

void getMockXml(char* Buff)
{
  char *Buff1 = 
    "<?xml version=\"1.0\" encoding=\"utf-8\"?><!DOCTYPE context [<!ELEMENT context "
"(device)*><!ELEMENT device (channel | attribute | debug-attribute)*><!ELEMENT "
"channel (scan-element?, attribute*)><!ELEMENT attribute EMPTY><!ELEMENT "
"scan-element EMPTY><!ELEMENT debug-attribute EMPTY><!ATTLIST context name "
"CDATA #REQUIRED description CDATA #IMPLIED><!ATTLIST device id CDATA "
"#REQUIRED name CDATA #IMPLIED><!ATTLIST channel id CDATA #REQUIRED type "
"(input|output) #REQUIRED name CDATA #IMPLIED><!ATTLIST scan-element index "
"CDATA #REQUIRED format CDATA #REQUIRED scale CDATA #IMPLIED><!ATTLIST "
"attribute name CDATA #REQUIRED filename CDATA #IMPLIED><!ATTLIST "
"debug-attribute name CDATA #REQUIRED>]><context name=\"tiny\" "
"description=\"Tiny IIOD\" >"

"<device id=\"iio:0\" name=\"TEMP_MODULE\" >"

"<channel id=\"temp0\" name=\"cold_junction\" type=\"input\" >"
"<scan-element index=\"0\" format=\"le:s32/32&gt;&gt;0\" />"
"<attribute name=\"Sensor\" />"
"<attribute name=\"SensorType\" />"
"<attribute name=\"Gain\" />"
"<attribute name=\"ExcitationCurrent\" />"
"<attribute name=\"ReferenceResistor\" />"
"<attribute name=\"TemperatureMax\" />"
"<attribute name=\"TemperatureMin\" />"
"</channel>"

"<channel id=\"temp1\" name=\"thermocouple\" type=\"input\" >"
"<scan-element index=\"1\" format=\"le:s32/32&gt;&gt;1\" />"
"<attribute name=\"Sensor\" />"
"<attribute name=\"SensorType\" />"
"<attribute name=\"Gain\" />"
"<attribute name=\"VBiasEnable\" />"
"<attribute name=\"TemperatureMin\" />"
"<attribute name=\"TemperatureMax\" />"
"</channel>"

"<attribute name=\"PowerMode\" />"
"<attribute name=\"OperationalMode\" />"
"<attribute name=\"FilterType\" />"
"<attribute name=\"FirFrequency\" />"
"<attribute name=\"FS\" />"
"<attribute name=\"TemperatureUnit\" />"

"<debug-attribute name=\"direct_reg_access\" />"
"</device></context>\r\n";

  strcpy(Buff, Buff1);
}
  