/* utils - app specific utils */

#include <QApplication>
#include <QCompleter>
#include <QCryptographicHash>
#include <QDirIterator>
#include <QEventLoop>
#include <QFont>
#include <QPoint>
#include <QProcess>
#include <QUrl>
#include <QWidget>
#ifdef TABCOMPLETION
#include <QStringListModel>
#include <QTextCursor>
#endif

#include "base.h"
#include "bedit.h"
#include "dialog.h"
#include "note.h"
#include "jsvr.h"
#include "proj.h"
#include "recent.h"
#include "state.h"
#include "svr.h"
#include "tedit.h"
#include "term.h"

// using namespace std;

extern "C" {
  Dllexport int gethash(const char *, const char *, const int, char *&, int &);
  Dllexport void openj(const char *s);
}

QWidget *getactiveedit();
void writewinstate(QWidget *);

bool ShowIde=true;
static std::string hashbuf;
static QList<int> Modifiers =
  QList<int>() << Qt::Key_Alt << Qt::Key_AltGr
  << Qt::Key_Control << Qt::Key_Meta << Qt::Key_Shift;

// ---------------------------------------------------------------------
// convert name to full path name
QString cpath(QString s)
{
  int t;
  QString f,p;

  if ((s.size() == 0) || isroot(s))
    return cfcase(s);
  t=(int)(s.at(0)=='~');
  int n = s.indexOf('/');
  if (n < 0) {
    f=s.mid(t);
    p="";
  } else {
    f=s.mid(t,n-t);
    p=s.mid(n);
  }

  if (f.size() == 0) f = "home";
  bool par = f.at(0) == '.';
  if (par) f.remove(0,1);

  n = config.AllFolderKeys.indexOf(f);
  if (n<0) return cfcase(s);
  f = config.AllFolderValues.at(n);

  if (par) f = cfpath(f);
  return cfcase(f + p);
}

// ---------------------------------------------------------------------
// url from current directory
QUrl currenturl()
{
  return QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html"));
}

// ---------------------------------------------------------------------
QString defext(QString s)
{
  if (s.isEmpty() || s.contains('.')) return s;
  return s + config.DefExt;
}

// ---------------------------------------------------------------------
int fkeynum(int key,bool c,bool s)
{
  return key + (c*100) + (s*100000);
}

// ---------------------------------------------------------------------
// b is base directory
QStringList folder_tree(QString b,QString filters,bool subdir)
{
  if (!subdir)
    return cflistfull(b,filters);
  return folder_tree1(b,"",getfilters(filters));
}

// ---------------------------------------------------------------------
// b is base directory, s is current subdirectory
QStringList folder_tree1(QString b,QString s,QStringList f)
{
  QString n;
  QString t=b + "/" + s;

  QDir d(t);
  d.setNameFilters(f);
  QStringList r=d.entryList(QDir::Files|QDir::Readable);
  for (int i=0; i<r.size(); i++)
    r.replace(i,t+r.at(i));

  QDirIterator p(t,QDir::Dirs|QDir::NoDotAndDotDot);
  while (p.hasNext()) {
    p.next();
    if (!config.DirTreeX.contains(p.fileName()))
      r=r+folder_tree1(b,s+p.fileName()+"/",f);
  }

  return r;
}

// ---------------------------------------------------------------------
void fontdiff(int n)
{
  config.Font.setPointSize(n+config.Font.pointSize());
  fontset(config.Font);
}

// ---------------------------------------------------------------------
void fontset(QFont font)
{
  config.Font=font;
  tedit->setFont(font);
  tedit->settabwidth();
  if (note) {
    note->setfont(font);
    if (note2)
      note2->setfont(font);
  }
  tedit->ifResized=false;
}

// ---------------------------------------------------------------------
void fontsetsize(int n)
{
  config.Font.setPointSize(n);
  fontset(config.Font);
}

// ---------------------------------------------------------------------
QString fontspec(QFont font)
{
  QString r;
  r="\"" + font.family() + "\" " + QString::number(font.pointSizeF());
  if (font.bold()) r+=" bold";
  if (font.italic()) r+=" italic";
  if (font.strikeOut()) r+=" strikeout";
  if (font.underline()) r+=" underline";
  return r;
}

// ---------------------------------------------------------------------
// if editor is active, return the note edit control,
// otherwise return the term edit control
QWidget *getactiveedit()
{
  if (note && ActiveWindows.indexOf(note)<ActiveWindows.indexOf(term))
    return (Bedit *)note->editPage();
  return tedit;
}

// ---------------------------------------------------------------------
// get command string in form: mode)text
QString getcmd(QString mode,QString t)
{
  std::string v=q2s(t.trimmed());
  const char *c=v.c_str();
  int i=0,p=0,s=(int)v.size();
  for (; i<s; i++) {
    if (c[i]==')') p=i;
    if (!(isalnum(c[i]) || c[i]==')' || c[i]=='.')) break;
  }
  if (p==0) return mode + ")" + t;
  size_t b = v.find_last_of(')',p-1);
  if (b==std::string::npos) return t;
  v.erase(0,b+1);
  return s2q(v);
}

#ifdef TABCOMPLETION
// ---------------------------------------------------------------------
QAbstractItemModel *getcompletermodel(QCompleter *completer,const QString& fileName)
{
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly))
    return new QStringListModel(completer);

  QStringList words;

  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    if (!line.isEmpty())
      words << line.trimmed();
  }

  return new QStringListModel(words, completer);
}
#endif

// ---------------------------------------------------------------------
QTextCursor getcursor(QWidget *w)
{
  if (w==tedit)
    return tedit->textCursor();
  return ((Bedit *)w)->textCursor();
}


// ---------------------------------------------------------------------
QTextDocument *getdocument(QWidget *w)
{
  if (w==tedit)
    return tedit->document();
  return ((Bedit *)w)->document();
}

// ---------------------------------------------------------------------
int gethash(const char *s, const char *t, const int wid, char *&msg, int &len)
{
  int rc=0;
  QCryptographicHash::Algorithm a;
  std::string m=c2s(s);
  if (m=="md4")
    a=QCryptographicHash::Md4;
  else if (m=="md5")
    a=QCryptographicHash::Md5;
  else if (m=="sha1")
    a=QCryptographicHash::Sha1;
#ifdef QT50
  else if (m=="sha224")
    a=QCryptographicHash::Sha224;
  else if (m=="sha256")
    a=QCryptographicHash::Sha256;
  else if (m=="sha384")
    a=QCryptographicHash::Sha384;
  else if (m=="sha512")
    a=QCryptographicHash::Sha512;
  else if (m=="sha3_224")
    a=QCryptographicHash::Sha3_224;
  else if (m=="sha3_256")
    a=QCryptographicHash::Sha3_256;
  else if (m=="sha_384")
    a=QCryptographicHash::Sha3_384;
  else if (m=="sha3_512")
    a=QCryptographicHash::Sha3_512;
#endif
  else {
    rc=1;
    hashbuf="Hash type unknown: " + m;
  }
  if (rc==0)
    hashbuf=q2s(QCryptographicHash::hash(QByteArray(t,wid),a).toHex());
  msg=(char *)hashbuf.c_str();
  len=(int)hashbuf.size();
  return rc;
}

// ---------------------------------------------------------------------
// get parent for message box
QWidget *getmbparent()
{
  QWidget *w;
  w=app->focusWidget();
  if (!w)
    w=app->activeWindow();
  if (!w)
    w=getactivewindow();
  return w;
}

// ---------------------------------------------------------------------
QString getplaintext(QWidget *w)
{
  if (w==tedit)
    return tedit->toPlainText();
  return ((Bedit *)w)->toPlainText();
}

// ---------------------------------------------------------------------
int getpositioninblock(QTextCursor c)
{
  return c.positionInBlock();
}

// ---------------------------------------------------------------------
QString getsha1(QString s)
{
  return QCryptographicHash::hash(s.toUtf8(),QCryptographicHash::Sha1).toHex();
}

// ---------------------------------------------------------------------
std::string getversion()
{
  QString s=QString::fromUtf8(JQTVERSION);
  QString rt=QString(qVersion());
  s=s.split(":")[1];
  QStringList t=s.split("/");
  std::string r=q2s(t[1]);
  if ("slim"==t[0])
    r+="s";
  if ("fat"==t[0])
    r+="f";
  r+="/" + q2s(t[2]) + "(" + q2s(rt) + ")";
  return r;
}

// ---------------------------------------------------------------------
// return if git available
bool gitavailable()
{
#if defined(__MACH__) || defined(Q_OS_LINUX)
  QStringList r=shell("which git","");
  if(!r.length()) return false;
  return !r.at(0).isEmpty();
#else
  return false;
#endif
}

// ---------------------------------------------------------------------
// git gui
void gitgui(QString path)
{
  if (config.ifGit) {
    QProcess p;
    p.startDetached("git",QStringList() << "gui",path);
  }
}

// ---------------------------------------------------------------------
// return status or empty if not git
QString gitstatus(QString path)
{
  if (config.ifGit)
    return shell("git status",path).at(0);
  return "";
}

// ---------------------------------------------------------------------
QStringList globalassigns(QString s,QString ext)
{
  QStringList p,r;
  QString t;
  QRegExp rx;
  int c,i,pos = 0;

  t=rxassign(ext,true);
  if (t.isEmpty()) return r;
  rx.setPattern("(([a-z]|[A-Z])\\w*)"+t);

  while ((pos = rx.indexIn(s, pos)) != -1) {
    p << rx.cap(1);
    pos += rx.matchedLength();
  }

#ifdef QT57
  std::sort(p.begin(), p.end());
#else
  qSort(p);
#endif
  i=0;
  while (i<p.size()) {
    t=p.at(i);
    c=i++;
    while (i<p.size()&&t==p.at(i)) i++;
    if (1<i-c)
      t=t + " (" + QString::number(i-c) + ")";
    r.append(t);
  }
  return r;
}

// ---------------------------------------------------------------------
void helpabout()
{
  QStringList s=state_about();
  about(s.at(0),s.at(1));
}

// ---------------------------------------------------------------------
bool ismodifier(int key)
{
  return Modifiers.contains(key);
}

// ---------------------------------------------------------------------
void newfile(QWidget *w)
{
  QString s = dialogsaveas(w,"New File", getfilepath());
  if (s.isEmpty()) return;
  if (!s.contains('.'))
    s+=config.DefExt;
  cfcreate(s);
  note->fileopen(s);
}

// ---------------------------------------------------------------------
QString newtempscript()
{
  int i,m,len,t;
  QString f;
  QStringList s=cflist(config.TempPath.absolutePath(),"*" + config.DefExt);

  QList<int> n;
  len = config.DefExt.size();
  foreach (QString p, s) {
    p.chop(len);
    m=p.toInt();
    if (m)
      n.append(m);
  }
  t=1;
  if (n.size()) {
#ifdef QT57
    std::sort(n.begin(), n.end());
#else
    qSort(n);
#endif
    for (i=1; i<n.size()+2; i++)
      if (!n.contains(i)) {
        t=i;
        break;
      }
  }
  return config.TempPath.filePath(QString::number(t) + config.DefExt);
}

// ---------------------------------------------------------------------
void openfile(QWidget *w,QString type)
{
  QString f = dialogfileopen(w,type);
  if (f.isEmpty()) return;
  openfile1(f);
}

// ---------------------------------------------------------------------
void openfile1(QString f)
{
  term->vieweditor();
  note->fileopen(f);
  recent.filesadd(f);
}

// ---------------------------------------------------------------------
void openj(const char *s)
{
  if (!term) return;
  if (!ShowIde) return;
  QString f(s);
  f=f.trimmed();
  if (f.isEmpty()) return;
  if (!cfexist(f)) {
    info("Open","Not found: "+f);
    return;
  }
  openfile1(f);
}

// ---------------------------------------------------------------------
void projectclose()
{
  project.close();
  term->projectenable();
  if (note) {
    note->Id="";
    note->setindex(note->editIndex());
    note->projectenable();
  }
}

// ---------------------------------------------------------------------
void projectenable()
{
  term->projectenable();
  if (note) {
    note->projectenable();
  }
}

// ---------------------------------------------------------------------
void projectfilemanager()
{
  if (config.FileManager.isEmpty()) {
    info("FileManager","The FileManager command should be defined in qtide.cfg.");
    return;
  }
  QString d;
  if (project.Id.isEmpty()) {
    if (note->editIndex()<0)
      d=config.TempPath.absolutePath();
    else
      d=cfpath(note->editFile());
  } else
    d=project.Path;
  QProcess p;
  QStringList a;
#ifdef _WIN32
  d=QDir::toNativeSeparators(d);
#endif
  a << d;
  p.startDetached(config.FileManager,a,d);
}

// ---------------------------------------------------------------------
// folder tree from folder name
QStringList project_tree(QString s)
{
  QStringList r;
  r=project_tree1(cpath(s),"");
  r.sort();
  return r;
}

// ---------------------------------------------------------------------
// b is base directory, s is current subdirectory
QStringList project_tree1(QString b,QString s)
{
  QString n,p;
  QString t=b + "/" + s;
  QDirIterator d(t,QDir::Dirs|QDir::NoDotAndDotDot);
  QStringList r;
  while (d.hasNext()) {
    d.next();
    n=d.fileName();
    if (QFileInfo(t + n + "/" + n + config.ProjExt).exists())
      r.append(s + n);
    r = r + project_tree1(b,s + n + "/");
  }
  return r;
}

// ---------------------------------------------------------------------
void projectterminal()
{
  if (config.Terminal.isEmpty()) {
    info("Terminal","The Terminal command should be defined in qtide.cfg.");
    return;
  }
  QString d;
  if (project.Id.isEmpty()) {
    if (note->editIndex()<0)
      d=config.TempPath.absolutePath();
    else
      d=cfpath(note->editFile());
  } else
    d=project.Path;
  QProcess p;
  QStringList a;
#ifdef _WIN32
  if (config.Terminal=="cmd" || config.Terminal=="cmd.exe")
    a << "/K" << "start";
#endif
#ifdef __MACH__
  a << d;
#endif
  p.startDetached(config.Terminal,a,d);
}

// ---------------------------------------------------------------------
// get class, position and text
QString readhelptext(QWidget *w, int t)
{
  int bgn, end;
  QString txt=getplaintext(w);
  QTextCursor c = getcursor(w);
  bgn=c.selectionStart();
  end=c.selectionEnd();
  QString hdr=QString::number(t)+" "
              +QString::number(bgn)+" "
              +QString::number(end)+" ";
  return hdr+txt;
}

// ---------------------------------------------------------------------
QString rxassign(QString ext, bool ifglobal)
{
  QString r;
  if (ext==".ijs"||ext==".ijt")
    r=ifglobal ? "\\s*=:" : "\\s*=[.:]" ;
  else if (ext==".k"||ext==".q")
    r="\\s*:";
  return r;
}

// ---------------------------------------------------------------------
void setwh(QWidget *w, QString s)
{
  QList<int>p=config.winpos_read(s);
  w->resize(qMax(p[2],300),qMax(p[3],300));
}

// ---------------------------------------------------------------------
void setxywh(QWidget *w, QString s)
{
  QList<int>p=config.winpos_read(s);
  w->move(p[0],p[1]);
  w->resize(qMax(p[2],300),qMax(p[3],300));
}

// ---------------------------------------------------------------------
// return standard output, standard error
QStringList shell(QString cmd, QString dir)
{
  QStringList r;
  QProcess p;
  if (!dir.isEmpty())
    p.setWorkingDirectory(dir);
#if defined(QT60)
  p.startCommand(cmd);
#else
  p.start(cmd,QStringList());
#endif
  try {
    if (!p.waitForStarted())
      return r;
  } catch (...) {
    return r;
  }
  if (!p.waitForFinished())
    return r;
  r.append((QString)p.readAllStandardOutput());
  r.append((QString)p.readAllStandardError());
  return r;
}

// ---------------------------------------------------------------------
void setnote(Note *n)
{
  if (note!=n) {
    note2=note;
    if (note2)
      note2->settitle2(true);
    note=n;
    note->settitle2(false);
    note->setid();
  }
}

// ---------------------------------------------------------------------
void showide(bool b)
{
  if (!term) return;
  if (note2)
    note2->setVisible(b);
  if (note)
    note->setVisible(b);
  term->setVisible(b);
  ShowIde=b;
}

// ---------------------------------------------------------------------
// convert file name to folder name
// searches for the best fit
// if none then check for parent folders
QString tofoldername(QString f)
{
  int i;
  QString g,p,s,t;
  if (f.isEmpty()) return f;

  for (i=0; i<config.AllFolderValues.size(); i++) {
    t=config.AllFolderValues.at(i);
    if (matchfolder(t,f) && t.size() > s.size())
      s=t;
    else if (matchfolder(cfpath(t),f) && t.size() > p.size())
      p=t;
  }

  if (s.size()) {
    i=config.AllFolderValues.indexOf(s);
    return '~' + config.AllFolderKeys.at(i) + f.mid(s.size());
  }

  if (p.size()) {
    i=config.AllFolderValues.indexOf(p);
    return "~." + config.AllFolderKeys.at(i) + f.mid(cfpath(p).size());
  }

  return f;
}

// ---------------------------------------------------------------------
// shortest name relative to current project if any
QString toprojectname(QString f)
{
  QString s=cpath(f);

  if (project.Id.size() && matchfolder(project.Path,s))
    s=cfsname(s);
  else {
    s=tofoldername(s);
    if ('~'==s.at(0))
      s=s.mid(1);
  }
  return s;
}

// ---------------------------------------------------------------------
void userkey(int mode, QString s)
{
  QWidget *w=getactiveedit();

#ifdef DEBUG_JDO
  qDebug() << "userkey " + s + " mode "+ mode;
#endif
  if (mode==0 || mode==1) {
    writewinstate(w);
    tedit->docmds(s,mode==1);
    return;
  }

  if (!w) return;

  if (w==tedit) {

    if (mode==2) {
      s=tedit->getprompt()+s;
      tedit->appendPlainText(s);
      return;
    }

    if (mode==4)
      tedit->moveCursor(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
    tedit->textCursor().insertText(s);
    return;
  }

  Bedit *b=(Bedit *) w;

  if (mode==2) {
    b->appendPlainText(s);
    return;
  }

  if (mode==4)
    b->moveCursor(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
  b->textCursor().insertText(s);
}

// ---------------------------------------------------------------------
// Windows tolower if not case-sensitive. s should be a full pathname.
QString win2lower(QString s)
{
  if (s.isEmpty()) return s;
  if (config.CasePaths.isEmpty()) return s.toLower();
  int n,p;
  n=1+s.indexOf(":");
  QString d=s.left(n);
  if (config.CasePaths.contains(d)) return s;
  QString b=s.mid(n);
  if (b.isEmpty()) return s.toLower();
  if (b[0] != '/')
    b=b.prepend("/");
  QString t=d+b+"/";
  n=0;
  p=1;
  while (-1 != (n=t.indexOf("/",p))) {
    if (config.CasePaths.contains(t.left(n)))
      return s;
    p=n+1;
  }
  return s.toLower();
}

// ---------------------------------------------------------------------
// get window position
// subject to minimum/maximum size and fit on screen
QList<int> winpos_get(QWidget *s)
{
  QList<int> d;
  QPoint p=s->pos();
  QSize z=s->size();
  int x=p.rx();
  int y=p.ry();
  int w=z.width();
  int h=z.height();

  w=qMax(100,qMin(w,config.ScreenWidth));
  h=qMax(50,qMin(h,config.ScreenHeight));
  x=qMax(0,qMin(x,config.ScreenWidth-w));
  y=qMax(0,qMin(y,config.ScreenHeight-h));

  d << x << y << w << h;
  return d;
}

// ---------------------------------------------------------------------
void winpos_set(QWidget *w,QList<int>p)
{
  if (p[0] >= 0)
    w->move(p[0],p[1]);
  w->resize(p[2],p[3]);
}

// ---------------------------------------------------------------------
void writewinstate(QWidget *w)
{
  if (w==0) {
    sets("WinText_jqtide_","");
    jcon->cmddo("WinSelect_jqtide_=: $0");
    return;
  }
  QTextCursor c=getcursor(w);
  QString t=getplaintext(w);
  int b=c.selectionStart();
  int e=c.selectionEnd();
  QString s=QString::number(b)+" "+QString::number(e);
  sets("WinText_jqtide_",q2s(t));
  sets("inputx_jrx_",q2s(s));
  jcon->cmddo("WinSelect_jqtide_=: 0 \". inputx_jrx_");
}

// ---------------------------------------------------------------------
void xdiff(QString s,QString t)
{
  if (config.XDiff.size()==0) {
    info("External Diff","First define XDiff in the config");
    return;
  }
  QStringList a;
  a << s << t;
  QProcess p;
  p.startDetached(config.XDiff,a);
}
