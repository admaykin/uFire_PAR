#pragma once

#include <uFire_PAR.h>

class uFire_PAR_JSON
{
public:
  float value;
  uFire_PAR_JSON(){}
  void begin(uFire_PAR *par);
  String processJSON(String json);
private:
  uFire_PAR *par;
  String measurePar();
  String par_connected();
};
