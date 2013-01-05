#ifndef ISIGRAPH_H
#define ISIGRAPH_H

#include <string>

#include "child.h"

class Isigraph : public Child
{
  Q_OBJECT

public:
  Isigraph(string n, string p, Form *f);
  void setp(string p, string v);
  void setform();

protected:

private:

};

extern Isigraph *isigraph;

#endif

