#pragma once

#include <uFire_PAR.h>

class uFire_PAR_MP
{
public:
  float value;
  uFire_PAR_MP(){}
  void begin(uFire_PAR *par);
  String processMsgPack(String json);
private:
  uFire_PAR *par;
  String measurePar();
  String par_connected();
};
