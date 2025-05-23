
#include <QApplication>
#include <QStyle>
#ifdef QT65
#include <QStyleHints>
#endif
#include <QString>
#include "base.h"
#include "proj.h"
#include "recent.h"
#include "state.h"
#include "../wd/wd.h"
#include "qtstate.h"

// ---------------------------------------------------------------------
static std::string qpair(QString s, QString t)
{
  return spair(q2s(s),q2s(t));
}

// ---------------------------------------------------------------------
static std::string qpair(QString s, std::string t)
{
  return spair(q2s(s),t);
}

// ---------------------------------------------------------------------
std::string qtstate(std::string p)
{
  QStringList s=s2q(p).split(" ",_SkipEmptyParts);
  QString c;
  std::string r;

  if (s.size()==0) return "";
  bool all="all"==s.at(0);

  c="debugdissect";
  if (all || s.contains(c))
    r+=qpair(c,QString(config.DebugDissect ? "true" : "false"));
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
  c="style";
  if (all || s.contains(c))
    r+=qpair(c,QApplication::style()->objectName());
#ifdef QT65
  c="colorscheme";
  if (all || s.contains(c)) {
    if (Qt::ColorScheme::Dark == QGuiApplication::styleHints()->colorScheme()) {
      r+=qpair(c,QString("dark"));
    } else if (Qt::ColorScheme::Light == QGuiApplication::styleHints()->colorScheme()) {
      r+=qpair(c,QString("light"));
    } else {
      r+=qpair(c,QString("unknown"));
    }
  }
#endif
  c="version";
  if (all || s.contains(c))
    r+=qpair(c,getversion());
  return r;
}
