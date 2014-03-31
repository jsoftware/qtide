/* common state */

#include <QSettings>

#include "base.h"
#include "state.h"
#include "note.h"
#include "term.h"

QList<int> DefPos;

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
      while(s.size()) {
        if(s.at(0).at(0)!=' ') break;
        DMTypes.append(s.at(0).trimmed());
        s.removeFirst();
      }
      continue;
    }

    if (matchhead("DMFavorites=",v)) {
      DMFavorites.clear();
      while(s.size()) {
        if(s.at(0).at(0)!=' ') break;
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

  for(i=0; i<DMTypes.size(); i++) {
    v=DMTypes.at(i);
    n=v.indexOf(' ');
    s.clear();
    t.clear();
    if(n>0) {
      s=v.mid(n).remove(' ').split(',',QString::SkipEmptyParts);
      s=qslprependeach("*.",s);
      t=s.join(",");
      v=v.mid(0,n);
    }
    DMTypex.append(t);
    if (v==DMType) DMTypeIndex=i;
    if(t.size())
      v+=" (" + t + ")";
    DMTypes.replace(i,v);
  }

}

// ---------------------------------------------------------------------
int Config::filepos_get(QString f)
{
  return FilePos.value(f,0);
}

// ---------------------------------------------------------------------
void Config::filepos_set(QString f, int p)
{
  FilePos.insert(f,p);
}

// ---------------------------------------------------------------------
void Config::fkeys_init()
{
  int n;
  QString p,s;
  QStringList t,v;
  t=cfreadx(ConfigPath.filePath("fkeys.cfg"));
  foreach (QString f,t) {
    v=f.split("|",QString::SkipEmptyParts);
    p=s=v.at(0);
    p=p.remove("c").remove("s");
    n=fkeynum(p.toInt(),s.contains("c"),s.contains("s"));
    config.FKeyKeys.append(n);
    config.FKeyValues.append(v.mid(1));
  }
}

// ---------------------------------------------------------------------
void Config::launch_init()
{
  int i;
  QString s;
  QStringList t;
  t=cfreadx(ConfigPath.filePath("launch.cfg"));
  foreach(QString s,t) {
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
  term->refresh();
  if (note) {
    note->setlinenos(LineNos);
    if(note2)
      note2->setlinenos(LineNos);
  }
}

// ---------------------------------------------------------------------
void Config::togglelinewrap()
{
  LineWrap=!LineWrap;
  note->setlinewrap(LineWrap);
  if(note2)
    note2->setlinewrap(LineWrap);
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
      for(int i=0; i<4; i++)
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
  QSettings s(ConfigPath.filePath("winpos.dat"),QSettings::IniFormat);
  winpos_save1(winpos_get(w),id);
}

// ---------------------------------------------------------------------
void Config::winpos_save1(QList<int>d,QString id)
{
  QSettings s(ConfigPath.filePath("winpos.dat"),QSettings::IniFormat);
  WinPos[id]=d;
  QString r;
  r.append(
    QString::number(d[0]) + " " +
    QString::number(d[1]) + " " +
    QString::number(d[2]) + " " +
    QString::number(d[3]));
  s.setValue(id, r);
}
