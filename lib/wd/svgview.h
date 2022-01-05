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
  SvgView(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  void cmd(std::string p,std::string v);
  std::string state();
  std::string imageFile;
  std::string imageXml;
};

#endif

