
#include <QApplication>

#include "base.h"
#include "proj.h"
#include "recent.h"
#include "state.h"
#include "../wd/wd.h"
#include "qtstate.h"

// ---------------------------------------------------------------------
static string qpair(QString s, QString t)
{
  return spair(q2s(s),q2s(t));
}

// ---------------------------------------------------------------------
static string qpair(QString s, string t)
{
  return spair(q2s(s),t);
}

// ---------------------------------------------------------------------
string qtstate(string p)
{
  QStringList s=s2q(p).split(" ",QString::SkipEmptyParts);
  QString c;
  string r;

  if (s.size()==0) return "";
  bool all="all"==s.at(0);

  c="debugpos";
  if (all || s.contains(c))
    r+=qpair(c,p2q(config.DebugPosX));
  c="fixfont";
  if (all || s.contains(c))
    r+=qpair(c,fontspec(config.Font));
  c="profont";
  if (all || s.contains(c))
    r+=qpair(c,fontspec(app->font()));
  c="project";
  if (all || s.contains(c))
    r+=qpair(c,recent.ProjectOpen ? project.Path : "");
  c="version";
  if (all || s.contains(c))
    r+=qpair(c,getversion());
  return r;
}
