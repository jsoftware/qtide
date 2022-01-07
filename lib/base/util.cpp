#include <QApplication>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QDirIterator>

#include <sstream>
#include <math.h>
#include "base.h"
#include "term.h"
#include "note.h"
#include "state.h"

// using namespace std;

int NoEvents=0;

// ---------------------------------------------------------------------
void about(QString t,QString s)
{
  QMessageBox::about(getmbparent(), t, s);
}

// ---------------------------------------------------------------------
// converts J 16-26 box chars to utf8
QString boxj2utf8(std::string s)
{
  QByteArray b(s.c_str());
  b.replace('\20',"\342\224\214");
  b.replace('\21',"\342\224\254");
  b.replace('\22',"\342\224\220");
  b.replace('\23',"\342\224\234");
  b.replace('\24',"\342\224\274");
  b.replace('\25',"\342\224\244");
  b.replace('\26',"\342\224\224");
  b.replace('\27',"\342\224\264");
  b.replace('\30',"\342\224\230");
  b.replace('\31',"\342\224\202");
  b.replace('\32',"\342\224\200");
  return QString::fromUtf8(b);
}

// ---------------------------------------------------------------------
QString c2q(const char *c)
{
  return QString::fromUtf8(c);
}

// ---------------------------------------------------------------------
std::string c2s(const char *c)
{
  std::string s(c);
  return s;
}

// ---------------------------------------------------------------------
bool cderase(QString name)
{
  bool r;
  QDir dir(name);
  if (!dir.exists(name)) return true;
  Q_FOREACH (QFileInfo info,dir.entryInfoList(
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
int cfappend(QFile *file, QString s)
{
  if (!file->open(QIODevice::Append | QIODevice::Text))
    return 0;
  QTextStream out(file);
  out << s;
  file->close();
  return s.size();
}

// ---------------------------------------------------------------------
int cfappend(QFile *file, QByteArray b)
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
#if defined(_WIN32)
  return win2lower(s);
#elif defined(__MACH__)
  return s.toLower();
#else
  return s;
#endif
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
  QFile f(s);
  QString p;
  cfwrite(&f,p);
  return f.exists();
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
  for (int i=0; i<r.size(); i++)
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
  QFile f;
  QStringList r;
  foreach (QString s,p) {
    f.setFileName(s);
    if (f.size() < 1e6 && f.open(QIODevice::ReadOnly)) {
      if (isutf8(f.readAll()))
        r.append(s);
      f.close();
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
#if !defined(QT60)
  in.setCodec("UTF-8");
#endif
  QString r = in.readAll();
  file->close();
  return r;
}

// ---------------------------------------------------------------------
QString cfread(QString s)
{
  QFile f(s);
  return cfread(&f);
}

// ---------------------------------------------------------------------
QByteArray cfreadbin(QString s)
{
  QByteArray r;
  QFile f(s);
  if (f.open(QIODevice::ReadOnly)) {
    r = f.readAll();
    f.close();
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
  QFile f(s);
  return cfreads(&f);
}

// ---------------------------------------------------------------------
QStringList cfreadx(QString s)
{
  QStringList r=cfreads(s);
  QString t;
  r.removeDuplicates();
  for (int i=r.size()-1; i>=0; i--) {
    t=r.at(i);
    if (t.isEmpty() || t.at(0)=='#' || t.mid(0,3)=="NB.")
      r.removeAt(i);
  }
  return r;
}

// ---------------------------------------------------------------------
// remove directory (may be non-empty)
// when used should first check OK to remove directory
bool cfrmdir(const QString & d)
{
  bool r = true;
  QDir dir(d);

  if (dir.exists(d)) {
    Q_FOREACH (QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
      if (info.isDir())
        r = cfrmdir(info.absoluteFilePath());
      else
        r = QFile::remove(info.absoluteFilePath());
      if (!r)
        return r;
    }
    r = dir.rmdir(d);
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
bool cftouch(QString s)
{
  QFile f(s);
  f.open(QIODevice::Append);
  return f.exists();
}

// ---------------------------------------------------------------------
int cfwrite(QFile *file, QString t)
{
  if (!file->open(QIODevice::WriteOnly|QIODevice::Text))
    return 0;
  QTextStream out(file);
#if !defined(QT60)
  out.setCodec("UTF-8");
#endif
  out << t;
  file->close();
  return t.size();
}

// ---------------------------------------------------------------------
int cfwrite(QString s, QString t)
{
  QFile f(s);
  return cfwrite(&f,t);
}

// ---------------------------------------------------------------------
int cfwrite(QString s, QByteArray b)
{
  QFile f(s);
  if (!f.open(QIODevice::WriteOnly))
    return 0;
  int r=f.write(b);
  f.close();
  return r;
}

// ---------------------------------------------------------------------
bool createdir(QDir d)
{
  if (d.exists()) return true;
  return  d.mkpath(".");
}

// ---------------------------------------------------------------------
QStringList cut2(QString s)
{
  int n;
  QStringList r;
  if (s.isEmpty()) return r;
  QChar p=s[s.size()-1];
  while (s.size()>0) {
    n=s.indexOf(p);
    r.append(s.mid(0,n+1));
    s=s.mid(n+1);
  }
  return r;
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
#if defined(QT60)
  QRegularExpression sep("(\\s|,)");
#else
  QRegExp sep("(\\s|,)");
#endif
  QStringList f=s.split(sep,_SkipEmptyParts);
  for (i=0; i<f.size(); i++) {
    p=f.at(i);
    if (!p.contains("*"))
      f.replace(i,"*."+p);
  }
  return f;
}

// ---------------------------------------------------------------------
// integer to string
std::string i2s(int i)
{
  std::stringstream s;
  s << i;
  std::string s1=s.str();
  if (s1[0]=='-') s1[0]='_';
  return s1;
}

// ---------------------------------------------------------------------
// double to string
std::string d2s(double d)
{
  std::stringstream s;
  s.precision(16);
  s << d;
  std::string s1=s.str();
  if (s1[0]=='-') s1[0]='_';
  return s1;
}

// ---------------------------------------------------------------------
bool ifshift()
{
  return app->keyboardModifiers().testFlag(Qt::ShiftModifier);
}

// ---------------------------------------------------------------------
void info(QString t,QString s)
{
  QMessageBox::information(getmbparent(), t, s);
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
QString intlist2qs(QList<int> p)
{
  QString s("");
  int n=p.size();
  for (int i=0; i<n; i++) {
    if (i>0) s.append(" ");
    s.append(QString::number(p[i]));
  }
  return s;
}

// ---------------------------------------------------------------------
// is non-empty and all digit
bool isint(const std::string s)
{
  int n=(int)s.size();
  if (n==0) return false;
  for (int i=0; i<n; i++)
    if (!isdigit(s[i])) return false;
  return true;
}

// ---------------------------------------------------------------------
bool isroot(QString s)
{
#ifdef _WIN32
  return s.size()>1 && ((s.at(1) == ':') || (s.startsWith("\\\\")) || (s.startsWith("//")));
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
//void f(std::string s)
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
  std::string s=q2s(p);
  char c;
  int n, len=(int)s.size();
  std::string t="";
  for (int i=0; i<len; i++) {
    if (s[i] == '(' || s[i] == '[' || s[i] == '{')
      t.push_back(s[i]);
    else if (s[i] == ')' || s[i] == ']' || s[i] == '}') {
      n=(int)t.size()-1;
      if (n<0)
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
int modpy(int p, int y)
{
  return (p+y)%p;
}

// ---------------------------------------------------------------------
void noevents(int n)
{
  NoEvents=qMax(0,NoEvents + ((n==0) ? -1 : 1));
}

// ---------------------------------------------------------------------
void notyet(QString s)
{
  info("Not Yet",s);
}

// ---------------------------------------------------------------------
QString p2q(QList<int> n)
{
  QString s("");
  s.append(QString("%1").arg(n[0]));
  for (int i=1; i<4; i++)
    s.append(" " + QString("%1").arg(n[i]));
  return s;
}

// ---------------------------------------------------------------------
// pointer to string
std::string p2s(const void *p)
{
  std::stringstream s;
  s << (SI) p;
  std::string s1=s.str();
  if (s1[0]=='-') s1[0]='_';
  return s1;
}

// ---------------------------------------------------------------------
// 0=cancel, 1=no, 2=yes
int queryCNY(QString t,QString s)
{
  int r=QMessageBox::question(getmbparent(),t,s,QMessageBox::Cancel|QMessageBox::No|QMessageBox::Yes,QMessageBox::Yes);

  return (r==QMessageBox::Cancel) ? 0 : ((r==QMessageBox::No) ? 1 : 2);
}

// ---------------------------------------------------------------------
bool queryNY(QString t,QString s)
{
  int r=QMessageBox::question(getmbparent(),t,s,QMessageBox::No|QMessageBox::Yes,QMessageBox::Yes);
  return r==QMessageBox::Yes;
}

// ---------------------------------------------------------------------
bool queryOK(QString t,QString s)
{
  int r=QMessageBox::question(getmbparent(),t,s,QMessageBox::Cancel|QMessageBox::Ok,QMessageBox::Ok);
  return r==QMessageBox::Ok;
}

// ---------------------------------------------------------------------
bool queryRETRY(QString t,QString s)
{
  int r=QMessageBox::question(getmbparent(),t,s,QMessageBox::Cancel|QMessageBox::Retry,QMessageBox::Retry);
  return r==QMessageBox::Retry;
}

// ---------------------------------------------------------------------
// J quote - doubles internal quotes
QString quote(QString s)
{
  QString r="'";
  for (int i=0; i<s.size(); i++) {
    r+=s[i];
    if (s[i]=='\'')
      r+="'";
  }
  r+="'";
  return r;
}

// ---------------------------------------------------------------------
QList<int> qs2intlist(QString c)
{
  QStringList s=c.split(' ',_SkipEmptyParts);
  return qsl2intlist(s);
}

// ---------------------------------------------------------------------
QList<int> qsl2intlist(QStringList s)
{
  QList<int> r;
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
  QStringList s=c.split(' ',_SkipEmptyParts);
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
  for (int i=0; i<s.size(); i++)
    r.append(dtb(s.at(i)));
  return r;
}

// ---------------------------------------------------------------------
QStringList qsldropeach(int n,QStringList s)
{
  QStringList r;
  for (int i=0; i<s.size(); i++)
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
  for (int i=0; i<s.size(); i++)
    r.append(cfcase(s.at(i)));
  return r;
}

// ---------------------------------------------------------------------
QStringList qslprependeach(QString p,QStringList s)
{
  QStringList r;
  for (int i=0; i<s.size(); i++)
    r.append(p+s.at(i));
  return r;
}

// ---------------------------------------------------------------------
QStringList qslreverse(QStringList s)
{
  int i,n=s.size();
#ifdef QT515
  for (i=0; i<n/2; i++) s.swapItemsAt(i,n-1-i);
#else
  for (i=0; i<n/2; i++) s.swap(i,n-1-i);
#endif
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
QStringList qsltrimeach(QStringList s)
{
  QStringList r;
  for (int i=0; i<s.size(); i++)
    r.append(s.at(i).simplified());
  return r;
}

// ---------------------------------------------------------------------
// dyadic -. for QStringList
QStringList qsless(QStringList a,QStringList w)
{
  foreach (QString s, w)
    if (a.contains(s)) a.removeAll(s);
  return a;
}

// ---------------------------------------------------------------------
// return true if all items are numbers
bool qsnumeric(QStringList a)
{
  foreach (QString s,a)
    if (s.size() && s.at(0)!='_' && s.at(0)!='-' && s.at(0)!='0' && s.at(0)!='1' && s.at(0)!='2' && s.at(0)!='3' && s.at(0)!='4' && s.at(0)!='5' && s.at(0)!='6' && s.at(0)!='7' && s.at(0)!='8' && s.at(0)!='9') return false;
  return true;
}

// ---------------------------------------------------------------------
std::string strless(std::string a,std::string w)
{
  std::string r="";
  for (size_t i=0; i<a.size(); i++) {
    if (std::string::npos == w.find_first_of(a[i])) r+=a[i];
  }
  return r;
}

// ---------------------------------------------------------------------
QList<int> q2p(QString s)
{
  QStringList t = s.split(" ",_SkipEmptyParts);
  QList<int> r;
  for (int i=0; i<4; i++)
    r.append(t.at(i).toInt());
  return r;
}

// ---------------------------------------------------------------------
std::string q2s(QString s)
{
  return s.toUtf8().constData();
}

// ---------------------------------------------------------------------
std::string remquotes(std::string s)
{
  int len=(int)s.size();
  if (len<2) return s;
  if ((s[0]=='"' && s[len-1]=='"')||(s[0]=='\177' && s[len-1]=='\177'))
    s=s.substr(1,len-2);
  return s;
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
QByteArray s2b(std::string s)
{
  return QByteArray(s.c_str(),(int)s.length());
}

// ---------------------------------------------------------------------
QString s2q(std::string s)
{
  return QString::fromUtf8(s.c_str());
}

// ---------------------------------------------------------------------
// pair strings with zero delimeter
std::string spair(std::string s,std::string t)
{
  std::string r=s;
  r.push_back('\0');
  r.append(t);
  r.push_back('\0');
  return r;
}

// ---------------------------------------------------------------------
// pair strings with zero delimeter
std::string spair(std::string s,QString t)
{
  std::string r=s;
  r.push_back('\0');
  r.append(q2s(t));
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

// ---------------------------------------------------------------------
// trim trailing whitespace (for TrimTrailingWS)
// trims WS on each line, and trim extra trailing LFs
QString trimtws(QString s)
{
  QStringList r=s.split('\n');
  for (int i=0; i<r.size(); i++)
    r[i]=dtb(r.at(i));
  while ((r.size()>1) && (r.at(r.size()-1).isEmpty()) && (r.at(r.size()-2).isEmpty()))
    r.removeLast();
  return r.join("\n");
}

// ---------------------------------------------------------------------
// return characters up to a LF or line separator
QString uptoLF(QString s)
{
  int n;
  n= s.indexOf('\n');
  if (n > -1) s = s.left(n);
  n = s.indexOf(QChar::LineSeparator);
  if (n > -1) s = s.left(n);
  return s;
}

// ---------------------------------------------------------------------
int c_strtoi(std::string s)
{
  if (!s.size()) return 0;
  std::string p=s;
  if (p[0]=='_') p[0]='-';
  return (int)strtol(p.c_str(),NULL,0);
}

// ---------------------------------------------------------------------
SI c_strtol(std::string s)
{
  if (!s.size()) return 0;
  std::string p=s;
  if (p[0]=='_') p[0]='-';
#if defined(_WIN64)
  return _strtoi64(p.c_str(),NULL,0);
#else
  return strtol(p.c_str(),NULL,0);
#endif
}

// ---------------------------------------------------------------------
double c_strtod(std::string s)
{
  if (!s.size()) return 0;
  std::string p=s;
  if (p[0]=='_') p[0]='-';
  return strtod(p.c_str(),NULL);
}

