/* common state */

#include <QSettings>

#include "base.h"
#include "state.h"
#include "menu.h"
#include "note.h"
#include "term.h"

QList<int> DefPos;
// using namespace std;

// ---------------------------------------------------------------------
void Config::dirmatch_init()
{
  if (DMTypes.size()) return;

  int i,n;
  QString t,v,w;
  QStringList s;
  DMType="All";
  DMTypeIndex=3;
  DMTypes=DefTypes;
  s=cfreadx(ConfigPath.filePath("dirmatch.cfg"));

  while (s.size()) {
    v=s.at(0);
    v.remove(' ');
    s.removeFirst();
    if (v.isEmpty())
      continue;

    if (matchhead("DMType=",v)) {
      DMType=v.mid(7).trimmed();
      continue;
    }

    if (matchhead("DMTypes=",v)) {
      DMTypes.clear();
      while (s.size()) {
        if (s.at(0).at(0)!=' ') break;
        DMTypes.append(s.at(0).trimmed());
        s.removeFirst();
      }
      continue;
    }

    if (matchhead("DMFavorites=",v)) {
      DMFavorites.clear();
      while (s.size()) {
        if (s.at(0).at(0)!=' ') break;
        DMFavorites.append(s.at(0).trimmed());
        s.removeFirst();
      }
      DMFavorites.removeDuplicates();
      DMFavorites.removeAll("");
      n=DMFavorites.size();
      if (n > 2 * n / 2)
        DMFavorites.removeLast();
    }
  }

  for (i=0; i<DMTypes.size(); i++) {
    v=DMTypes.at(i);
    n=v.indexOf(' ');
    s.clear();
    t.clear();
    if (n>0) {
      s=v.mid(n).remove(' ').split(',',_SkipEmptyParts);
      s=qslprependeach("*.",s);
      t=s.join(",");
      v=v.mid(0,n);
    }
    DMTypex.append(t);
    if (v==DMType) DMTypeIndex=i;
    if (t.size())
      v+=" (" + t + ")";
    DMTypes.replace(i,v);
  }

}

// ---------------------------------------------------------------------
int Config::filecur_get(QString f)
{
  return FileCur.value(f,0);
}

// ---------------------------------------------------------------------
void Config::filecur_set(QString f, int p)
{
  FileCur.insert(f,p);
}

// ---------------------------------------------------------------------
int Config::filetop_get(QString f)
{
  return FileTop.value(f,0);
}

// ---------------------------------------------------------------------
void Config::filetop_set(QString f, int p)
{
  FileTop.insert(f,p);
}

// ---------------------------------------------------------------------
std::string Config::formpos_read(QString id)
{
  QSettings s(ConfigPath.filePath("winpos.dat"),QSettings::IniFormat);
  s.beginGroup("Form");
  std::string p=q2s(s.value(id, "").toString());
  s.endGroup();
  return p;
}

// ---------------------------------------------------------------------
void Config::formpos_save(QWidget *w,QString id)
{
  winpos_save1(winpos_get(w),id,"Form");
}

// ---------------------------------------------------------------------
void Config::launch_init()
{
  int i;
  QString s;
  QStringList t;
  t=cfreadx(ConfigPath.filePath("launch.cfg"));
  foreach (QString s,t) {
    i=s.indexOf(";");
    if (i<0) {
      LaunchPadKeys.append("=");
      LaunchPadValues.append("");
    } else {
      LaunchPadKeys.append(s.left(i).trimmed().replace("_","&"));
      LaunchPadValues.append(s.mid(i+1).trimmed());
    }
  }
}

// ---------------------------------------------------------------------
void Config::togglelinenos()
{
  LineNos=!LineNos;
  if (note) {
    note->setlinenos(LineNos);
    if (note2)
      note2->setlinenos(LineNos);
  }
}

// ---------------------------------------------------------------------
void Config::togglelinewrap()
{
  LineWrap=!LineWrap;
  note->setlinewrap(LineWrap);
  if (note2)
    note2->setlinewrap(LineWrap);
}

// ---------------------------------------------------------------------
void Config::userkeys_init()
{
  QStringList err,t;
  QStringList v=getuserkeys();
  t=cfreadx(ConfigPath.filePath("userkeys.cfg"));
  foreach (QString f,t)
    UserKeys.append(userkeys_split(f));
  foreach (QStringList f,UserKeys) {
    if (!v.contains(f.first())) {
      err+="User key not available: "+f.first();
      UserKeys.removeOne(f);
    }
  }
  if (err.size()>0)
    info("User Keys",err.join("\n"));
  term->menuBar->createuserkeyMenu();
}

// ---------------------------------------------------------------------
QStringList Config::userkeys_split(QString s)
{
  QChar c;
  for (int i=0; i<s.length(); i++) {
    c=s.at(i);
    if (!(c.isLetter() || c.isDigit() || c=='+')) break;
  }
  return s.split(c);
}

// ---------------------------------------------------------------------
void Config::winpos_init()
{
  DefPos << 100 << 100 << 300 << 300;

  WinPos["Ctag"]=DefPos;
  WinPos["Dirm"]=DefPos;
  WinPos["Dlog"]=DefPos;
  WinPos["Fif"]=DefPos;
  WinPos["Fiw"]=DefPos;
  WinPos["Picm"]=DefPos;
  WinPos["View"]=DefPos;

  QSettings s(ConfigPath.filePath("winpos.dat"),QSettings::IniFormat);
  QStringList keys = s.allKeys();
  QStringList d;
  QList<int> v;
  foreach (const QString &k, keys) {
    d = s.value(k,"").toString().split(' ');
    if (d.size()==4) {
      v.clear();
      for (int i=0; i<4; i++)
        v.append(d.at(i).toInt());
      WinPos[k]=v;
    }
  }
}

// ---------------------------------------------------------------------
QList<int> Config::winpos_read(QString id)
{
  return (WinPos.contains(id)) ? WinPos.value(id) : DefPos;
}

// ---------------------------------------------------------------------
void Config::winpos_save(QWidget *w,QString id)
{
  winpos_save(w,id,"");
}

// ---------------------------------------------------------------------
void Config::winpos_save(QWidget *w,QString id,QString group)
{
  winpos_save1(winpos_get(w),id,group);
}

// ---------------------------------------------------------------------
void Config::winpos_save1(QList<int>d,QString id,QString group)
{
  QString r;
  r.append(
    QString::number(d[0]) + " " +
    QString::number(d[1]) + " " +
    QString::number(d[2]) + " " +
    QString::number(d[3]));
  QSettings s(ConfigPath.filePath("winpos.dat"),QSettings::IniFormat);
  if (group.length() == 0) {
    WinPos[id]=d;
    s.setValue(id, r);
  } else {
    s.beginGroup(group);
    s.setValue(id, r);
    s.endGroup();
  }
}
