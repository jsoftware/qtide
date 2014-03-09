#include <QApplication>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#ifdef QT_OS_ANDROID
#include <QProcess>
#endif

#include <sstream>
#include "base.h"
#include "term.h"
#include "note.h"

using namespace std;

int NoEvents=0;

// ---------------------------------------------------------------------
void about(QString t,QString s)
{
  QMessageBox::about(QApplication::focusWidget(), t, s);
}

// ---------------------------------------------------------------------
QString c2q(const char *c)
{
  return QString::fromUtf8(c);
}

// ---------------------------------------------------------------------
string c2s(const char *c)
{
  string s(c);
  return s;
}

// ---------------------------------------------------------------------
bool cderase(QString name)
{
  bool r;
  QDir dir(name);
  if (!dir.exists(name)) return true;
  Q_FOREACH(QFileInfo info,dir.entryInfoList(
              QDir::NoDotAndDotDot | QDir::System | QDir::Hidden
              | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
    if (info.isDir())
      r = cderase(info.absoluteFilePath());
    else
      r = QFile::remove(info.absoluteFilePath());
    if (!r) return r;
  }
  return dir.rmdir(name);
}

// ---------------------------------------------------------------------
int cfappend (QFile *file, QString s)
{
  if (!file->open(QIODevice::Append | QIODevice::Text))
    return 0;
  QTextStream out(file);
  out << s;
  file->close();
  return s.size();
}

// ---------------------------------------------------------------------
int cfappend (QFile *file, QByteArray b)
{
  if (!file->open(QIODevice::Append))
    return 0;
  int r=file->write(b);
  file->close();
  return r;
}

// ---------------------------------------------------------------------
QString cfcase(QString s)
{
#if defined Q_WS_WIN || defined Q_WS_MAC
  s=s.toLower();
#endif
  return s;
}

// ---------------------------------------------------------------------
bool cfcopy(QString from, QString to)
{
  QDir d;
  if (!d.mkpath(cfpath(to))) return false;
  QFile f(from);
  QFile t(to);
  t.remove();
  return f.copy(to);
}

// ---------------------------------------------------------------------
bool cfcreate(QString s)
{
  QFile *f=new QFile(s);
  QString p;
  cfwrite(f,p);
  return f->exists();
}

// ---------------------------------------------------------------------
bool cfdelete(QString s)
{
  QFile f(s);
  return f.remove();
}

// ---------------------------------------------------------------------
bool cfexist(QString s)
{
  QFile f(s);
  return f.exists();
}

// ---------------------------------------------------------------------
QString cfext(QString s)
{
  int n=s.lastIndexOf('.');
  if (n<0) return "";
  return s.mid(n);
}

// ---------------------------------------------------------------------
// file list
QStringList cflist(QString path,QString filters)
{
  QDir d(path);
  QStringList f=getfilters(filters);
  return d.entryList(f,QDir::Files|QDir::Readable,QDir::Name);
}

// ---------------------------------------------------------------------
// prepend path
QStringList cflistfull(QString b,QString filters)
{
  QStringList r=cflist(b,filters);
  QString t=b+"/";
  for(int i=0; i<r.size(); i++)
    r.replace(i,t+r.at(i));
  return r;
}

// ---------------------------------------------------------------------
// list text files
// is utf8 and size < 1e6
QStringList cflisttext(QString path)
{
  QByteArray b;
  QStringList p=cflistfull(path,"");
  QFile *f=new QFile;
  QStringList r;
  foreach (QString s,p) {
    f->setFileName(s);
    if (f->size() < 1e6 && f->open(QIODevice::ReadOnly)) {
      if(isutf8(f->readAll()))
        r.append(s);
      f->close();
    }
  }
  return r;
}

// ---------------------------------------------------------------------
// get path from filename
// same as parent for a directory
QString cfpath(QString s)
{
  int n = s.lastIndexOf('/');
  if (n < 1) return "";
  return s.left(n);
}

// ---------------------------------------------------------------------
QString cfread(QFile *file)
{
  if (!file->open(QIODevice::ReadOnly|QIODevice::Text))
    return s2q("");
  QTextStream in(file);
  in.setCodec("UTF-8");
  QString r = in.readAll();
  file->close();
  return r;
}

// ---------------------------------------------------------------------
QString cfread(QString s)
{
  QFile *f = new QFile(s);
  return cfread(f);
}

// ---------------------------------------------------------------------
QByteArray cfreadbin(QString s)
{
  QByteArray r;
  QFile *f = new QFile(s);
  if (f->open(QIODevice::ReadOnly)) {
    r = f->readAll();
    f->close();
  }
  return r;
}

// ---------------------------------------------------------------------
QStringList cfreads(QFile *file)
{
  int i;
  QStringList r;
  QString s;
  s=cfread(file);
  if (s.isEmpty()) return r;
  s = s.remove('\r').replace('\t',' ');
  for (i=s.size(); i>0; i--)
    if (s.at(i-1) != '\n') break;
  return s.left(i).split('\n');
}

// ---------------------------------------------------------------------
QStringList cfreads(QString s)
{
  QFile *f = new QFile(s);
  return cfreads(f);
}

// ---------------------------------------------------------------------
QStringList cfreadx(QString s)
{
  QStringList r=cfreads(s);
  QString t;
  r.removeDuplicates();
  for(int i=r.size()-1; i>=0; i--) {
    t=r.at(i);
    if (t.isEmpty() || t.at(0)=='#' || t.mid(0,3)=="NB.")
      r.removeAt(i);
  }
  return r;
}

// ---------------------------------------------------------------------
// get short name from filename
QString cfsname(QString s)
{
  int n = s.lastIndexOf('/');
  if (n < 1) return s;
  return s.mid(n+1);
}

// ---------------------------------------------------------------------
QDateTime cftime(QString s)
{
  QFileInfo f(s);
  return f.lastModified();
}

// ---------------------------------------------------------------------
int cfwrite(QFile *file, QString t)
{
  if (!file->open(QIODevice::WriteOnly|QIODevice::Text))
    return 0;
  QTextStream out(file);
  out.setCodec("UTF-8");
  out << t;
  file->close();
  return t.size();
}

// ---------------------------------------------------------------------
int cfwrite(QString s, QString t)
{
  QFile *f=new QFile(s);
  return cfwrite(f,t);
}

// ---------------------------------------------------------------------
int cfwrite(QString s, QByteArray b)
{
  QFile *f=new QFile(s);
  if (!f->open(QIODevice::WriteOnly))
    return 0;
  int r=f->write(b);
  f->close();
  return r;
}

// ---------------------------------------------------------------------
bool createdir(QDir d)
{
  if (d.exists()) return true;
  return  d.mkpath(".");
}

// ---------------------------------------------------------------------
QString detab(QString s)
{
  return s.replace('\t',' ');
}

// ---------------------------------------------------------------------
QString dlb(QString s)
{
  for (int n=0; n<s.size(); n++) {
    if (!s.at(n).isSpace()) {
      return s.mid(n);
    }
  }
  return "";
}

// ---------------------------------------------------------------------
QString dtb(QString s)
{
  for (int n = s.size()-1; n>=0; n--) {
    if (!s.at(n).isSpace()) {
      return s.left(n + 1);
    }
  }
  return "";
}

// ---------------------------------------------------------------------
// delete a trailing LF
QString dtLF(QString s)
{
  if (s.endsWith('\n')) s.chop(1);
  return s;
}

// ---------------------------------------------------------------------
QStringList getfilters(QString s)
{
  int i;
  QString p;
  QRegExp sep("(\\s|,)");
  QStringList f=s.split(sep,QString::SkipEmptyParts);
  for(i=0; i<f.size(); i++) {
    p=f.at(i);
    if(!p.contains("*"))
      f.replace(i,"*."+p);
  }
  return f;
}

// ---------------------------------------------------------------------
// integer to string
string i2s(int i)
{
  stringstream s;
  s << i;
  string s1=s.str();
  if (s1[0]=='-') s1[0]='_';
  return s1;
}

// ---------------------------------------------------------------------
// double to string
string d2s(double d)
{
  stringstream s;
  s.precision(16);
  s << d;
  string s1=s.str();
  if (s1[0]=='-') s1[0]='_';
  return s1;
}

// ---------------------------------------------------------------------
bool ifshift()
{
  return QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);
}

// ---------------------------------------------------------------------
void info(QString t,QString s)
{
#ifdef QT_OS_ANDROID
  QMessageBox::about(QApplication::focusWidget(), t, s);
#else
  QMessageBox::information(QApplication::focusWidget(), t, s);
#endif
}

// ---------------------------------------------------------------------
int initialblanks(QString t)
{
  int r=0;
  for (; r<t.length(); r++)
    if (t[r] != ' ') break;
  return r;
}

// ---------------------------------------------------------------------
// is non-empty and all digit
bool isint(const string s)
{
  int n=s.size();
  if (n==0) return false;
  for(int i=0; i<n; i++)
    if(!isdigit(s[i])) return false;
  return true;
}

// ---------------------------------------------------------------------
bool isroot(QString s)
{
#ifdef Q_WS_WIN
  return s.size()>1 && s.at(1) == ':';
#else
  return s.size()>0 && s.at(0) == '/';
#endif
}

// ---------------------------------------------------------------------
bool isutf8(QByteArray b)
{
  return b==QString::fromUtf8(b).toUtf8();
}

// ---------------------------------------------------------------------
// match smaller vs larger
bool matchhead(QString s, QString t)
{
  if (s.size() > t.size()) return false;
  return s == t.left(s.size());
}

// ---------------------------------------------------------------------
// match foldername vs path
bool matchfolder(QString s, QString t)
{
  if (s.size()>t.size()) return false;
  if (s.size()==t.size()) return s==t;
  return (s+"/")==t.left(s.size() + 1);
}

// ---------------------------------------------------------------------
//void f(string s)
//{
//cout << s << " " << matchparens('j',s) << endl;
//}
//f ("[{(OK)}]");
//f ("[(missing opening brace)}]");
//f ("[{(missing trailing bracket)}");
//f ("[{(order mixed up)]}");
/*
 *  matchparens
 *  modes: jkqrs
 *   0  matches
 *   1  too few closing - more to come
 *   2  mismatched parens
 *   3  mismatched quotes
*/

// !!! need remove quoted and comments first...
int matchparens(QChar mode, QString p)
{
  Q_UNUSED(mode);
  string s=q2s(p);
  char c;
  int n, len=s.size();
  string t="";
  for(int i=0; i<len; i++) {
    if(s[i] == '(' || s[i] == '[' || s[i] == '{')
      t.push_back(s[i]);
    else if(s[i] == ')' || s[i] == ']' || s[i] == '}') {
      n=t.size()-1;
      if(n<0)
        return 2;
      else {
        c=t.at(n);
        t.resize(n);
        if (!((s[i] == ')' && c == '(') || (s[i] == '}' && c == '{')
              || (s[i] == ']' && c == '[')))
          return 1;
      }
    }
  }
  return (t.size()==0) ? 0 : 2;
}

// ---------------------------------------------------------------------
void noevents(int n)
{
  NoEvents=qMax(0,NoEvents + (n==0) ? -1 : 1);
}

// ---------------------------------------------------------------------
void notyet(QString s)
{
  info("Not Yet",s);
}

// ---------------------------------------------------------------------
QString p2q(int *n)
{
  QString s("");
  s.append(QString("%1").arg(n[0]));
  for(int i=1; i<4; i++)
    s.append(" " + QString("%1").arg(n[i]));
  return s;
}

// ---------------------------------------------------------------------
// pointer to string
string p2s(const void *p)
{
  stringstream s;
  s << (SI) p;
  string s1=s.str();
  if (s1[0]=='-') s1[0]='_';
  return s1;
}

// ---------------------------------------------------------------------
// 0=cancel, 1=no, 2=yes
int queryCNY(QString t,QString s)
{
  int r=QMessageBox::question(QApplication::focusWidget(),t,s,QMessageBox::Cancel|QMessageBox::No|QMessageBox::Yes,QMessageBox::Yes);

  return (r==QMessageBox::Cancel) ? 0 : ((r==QMessageBox::No) ? 1 : 2);
}

// ---------------------------------------------------------------------
bool queryNY(QString t,QString s)
{
  int r=QMessageBox::question(QApplication::focusWidget(),t,s,QMessageBox::No|QMessageBox::Yes,QMessageBox::Yes);
  return r==QMessageBox::Yes;
}

// ---------------------------------------------------------------------
bool queryOK(QString t,QString s)
{
  int r=QMessageBox::question(QApplication::focusWidget(),t,s,QMessageBox::Cancel|QMessageBox::Ok,QMessageBox::Ok);
  return r==QMessageBox::Ok;
}

// ---------------------------------------------------------------------
bool queryRETRY(QString t,QString s)
{
  int r=QMessageBox::question(QApplication::focusWidget(),t,s,QMessageBox::Cancel|QMessageBox::Retry,QMessageBox::Retry);
  return r==QMessageBox::Retry;
}

// ---------------------------------------------------------------------
QList<int> qs2intlist(QString c)
{
  QList<int> r;
  QStringList s=c.split(' ',QString::SkipEmptyParts);
  for (int i=0; i<s.size(); i++)
    r.append(s.at(i).toInt());
  return r;
}

// ---------------------------------------------------------------------
// if s has only characters in t
bool qshasonly(const QString s, const QString t)
{
  for (int i=0; i<s.size(); i++)
    if (!t.contains(s.at(i))) return false;
  return true;
}

// ---------------------------------------------------------------------
QVector<int> qs2intvector(QString c)
{
  QStringList s=c.split(' ',QString::SkipEmptyParts);
  QVector<int> r(s.size());
  for (int i=0; i<s.size(); i++)
    r[i]=s.at(i).toInt();
  return r;
}

// ---------------------------------------------------------------------
QString qstaketo(QString s,QString c)
{
  int n=s.indexOf(c);
  if (n<0) return s;
  return s.left(n);
}

// ---------------------------------------------------------------------
QStringList qsldtbeach(QStringList s)
{
  QStringList r;
  for(int i=0; i<s.size(); i++)
    r.append(dtb(s.at(i)));
  return r;
}

// ---------------------------------------------------------------------
QStringList qsldropeach(int n,QStringList s)
{
  QStringList r;
  for(int i=0; i<s.size(); i++)
    r.append(s.at(i).mid(n));
  return r;
}

// ---------------------------------------------------------------------
QStringList qslexists(QStringList s)
{
  QStringList r;
  foreach (QString f,s)
  if (cfexist(f))
    r.append(f);
  return r;
}

// ---------------------------------------------------------------------
QStringList qslfcase(QStringList s)
{
  QStringList r;
  for(int i=0; i<s.size(); i++)
    r.append(cfcase(s.at(i)));
  return r;
}

// ---------------------------------------------------------------------
QStringList qslprependeach(QString p,QStringList s)
{
  QStringList r;
  for(int i=0; i<s.size(); i++)
    r.append(p+s.at(i));
  return r;
}

// ---------------------------------------------------------------------
QStringList qslreverse(QStringList s)
{
  int i,n=s.size();
  for(i=0; i<n/2; i++) s.swap(i,n-1-i);
  return s;
}

// ---------------------------------------------------------------------
QStringList qsltrim(QStringList p)
{
  while (p.size()) {
    if (p.first().trimmed().size()) break;
    p.removeFirst();
  }
  while (p.size()) {
    if (p.last().trimmed().size()) break;
    p.removeLast();
  }
  return p;
}

// ---------------------------------------------------------------------
void q2p(QString s,int *n)
{
  QStringList t = s.split(" ",QString::SkipEmptyParts);
  for (int i=0; i<4; i++)
    n[i] = t.at(i).toInt();
}

// ---------------------------------------------------------------------
string q2s(QString s)
{
  return s.toUtf8().constData();
}

// ---------------------------------------------------------------------
QString remsep(QString s)
{
  if (s.endsWith("/"))
    s=s.left(s.size()-1);
  return s;
}

// ---------------------------------------------------------------------
QString remtilde(QString s)
{
  if (s.startsWith("~"))
    s=s.mid(1);
  return s;
}

// ---------------------------------------------------------------------
QString s2q(string s)
{
  QString r=QString::fromUtf8(s.c_str());
  return r;
}

// ---------------------------------------------------------------------
// pair strings with zero delimeter
string spair(string s,string t)
{
  string r=s;
  r.push_back('\0');
  r.append(t);
  r.push_back('\0');
  return r;
}

// ---------------------------------------------------------------------
QString termLF(QString s)
{
  if (s.isEmpty()||s.endsWith("\n")) return s;
  return s + "\n";
}

// ---------------------------------------------------------------------
QString termsep(QString s)
{
  if (s.isEmpty()||s.endsWith("/")) return s;
  return s + "/";
}

// ---------------------------------------------------------------------
QString toqlist(QStringList s)
{
  int n=s.size();
  if (n == 0) return "";
  QString r = "(\"" + s.at(0) + "\"";
  for (int i=1; i<n; i++)
    r.append(";\"" + s.at(i) + "\"");
  r.append(")");
  return r;
}

#ifdef QT_OS_ANDROID
// ---------------------------------------------------------------------
void android_exec_host(const char *intent,const char *cmd,const char *mimetype)
{
// eg. QProcess::execute( "am start -a android.intent.action.VIEW -d file:///sdcard/test.mp3 -t audio/mp3" );
  if ((!mimetype) || !strlen(mimetype))
    QProcess::execute( "am start -a " + QString::fromUtf8(intent) + " -d \"" + QString::fromUtf8(cmd) + "\"" );
  else QProcess::execute( "am start -a " + QString::fromUtf8(intent) + " -d \"" + QString::fromUtf8(cmd) + "\" -t " + QString::fromUtf8(mimetype) );
}
#endif

// ---------------------------------------------------------------------
int c_strtoi(string s)
{
  if (!s.size()) return 0;
  string p=s;
  if (p[0]=='_') p[0]='-';
  return (int)strtol(p.c_str(),NULL,0);
}

// ---------------------------------------------------------------------
SI c_strtol(string s)
{
  if (!s.size()) return 0;
  string p=s;
  if (p[0]=='_') p[0]='-';
#if defined(_WIN64)
  return _strtoi64(p.c_str(),NULL,0);
#else
  return strtol(p.c_str(),NULL,0);
#endif
}

// ---------------------------------------------------------------------
double c_strtod(string s)
{
  if (!s.size()) return 0;
  string p=s;
  if (p[0]=='_') p[0]='-';
  return strtod(p.c_str(),NULL);
}
