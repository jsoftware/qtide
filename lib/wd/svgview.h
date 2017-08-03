#ifndef SVGVIEW_H
#define SVGVIEW_H

#include <string>

#include "child.h"

class Form;
class Pane;

class SvgView : public Child
{
  Q_OBJECT

public:
  SvgView(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  void cmd(string p,string v);
  string state();
  string imageFile;
  string imageXml;
};

#endif

