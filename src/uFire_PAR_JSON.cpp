#if __has_include("ArduinoJson.h")
#include "uFire_PAR_JSON.h"
#include <ArduinoJson.h>

void uFire_PAR_JSON::begin(uFire_PAR *p_par)
{
  par = p_par;
}

String uFire_PAR_JSON::processJSON(String json)
{
  String cmd = json.substring(0, json.indexOf(" ", 0));
  cmd.trim();
  json.remove(0, json.indexOf(" ", 0));
  json.trim();
  String parameter = json.substring(0, json.indexOf(" ", 0));
  parameter.trim();

  String value = "";
  if (cmd == "par")          value = measurePar();
  if (cmd == "parc")         value = par_connected();

  if (value != "")
  {
    this->value = value.toFloat();
    return value;
  } else
  {
    this->value = -1;
    return "";
  }
}

String uFire_PAR_JSON::measurePar()
{
  String output;
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 20;
  DynamicJsonDocument doc(bufferSize);
  doc["par"] = par->measurePAR();
  serializeJson(doc, output);
  return output;
}

String uFire_PAR_JSON::par_connected()
{
  String output;
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 20;
  DynamicJsonDocument doc(bufferSize);
  doc["parc"] = par->connected();
  serializeJson(doc, output);
  return output;
}
#endif