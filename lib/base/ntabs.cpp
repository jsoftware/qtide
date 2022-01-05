#include <QApplication>
#ifdef TABCOMPLETION
#include <QCompleter>
extern QCompleter *completer;
#endif

#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#else
#include <QPrinter>
#include <QPrinterInfo>
#endif
#endif

#include "base.h"
#include "ntabs.h"
#include "dialog.h"
#include "nedit.h"
#include "nside.h"
#include "note.h"
#include "proj.h"
#include "spic.h"
#include "state.h"
#include "recent.h"

// using namespace std;

// ---------------------------------------------------------------------
Ntabs::Ntabs(Note *p)
{
  pnote=p;
  watcher=new QFileSystemWatcher(this);
  setDocumentMode(true);
  setObjectName("ntabs");
  setMovable(true);
  setTabsClosable(true);
  setUsesScrollButtons(true);
  connect(this, SIGNAL(tabCloseRequested(int)),
          this, SLOT(tabCloseRequested(int)));
  connect(this, SIGNAL(currentChanged(int)),
          this, SLOT(currentChanged(int)));
  connect(watcher, SIGNAL(fileChanged(const QString &)),
          this, SLOT(fileChanged(const QString &)));
}

// ---------------------------------------------------------------------
// when current tab index changed, not tabs rearranged for which
// there is no signal

void Ntabs::currentChanged(int index)
{
  if (NoEvents) return;
  if (index==-1) return;
  if (index >= count()) return;
  Nedit *e=(Nedit *)widget(index);
  pnote->settitle(e->fname,e->document()->isModified());
  pnote->siderefresh();
  pnote->projectsave();
}

// ---------------------------------------------------------------------
void Ntabs::fileChanged(const QString &path)
{
  if (NoEvents) return;
  int index=getfileindex(path);
  Nedit *e=(Nedit *)widget(index);
  if (e->pendingReload) return;
  if (e->text==cfread(e->file)) return;
  QString m="File contents changed on disk: " + e->sname;
  m=m+"\n\n" + "Reload from disk?";
  e->pendingReload=true;
  if (queryNY("File Changed",m)) {
    noevents(1);
    e->text = cfread(e->file);
    e->setPlainText(e->text);
    setmodified(index,false);
    noevents(0);
  }
  e->pendingReload=false;
}

// ---------------------------------------------------------------------
int Ntabs::getfileindex(QString f)
{
  int r=-1;
  int n=count();
  for (int i=0; i<n; i++)
    if (f==((Nedit *)widget(i))->fname)
      r=i;
  return r;
}

// ---------------------------------------------------------------------
QStringList Ntabs::gettablist()
{
  int i;
  QStringList s;
  QString f;
  Nedit *e;
  if (count() == 0)
    s.append("-1");
  else {
    s.append(QString::number(currentIndex()));
    for (i=0; i<count(); i++) {
      e=(Nedit *)widget(i);
      f=e->sname;
      if (f.contains("/"))
        f=tofoldername(e->fname);
      s.append(f);
    }
  }
  return s;
}

// ---------------------------------------------------------------------
std::string Ntabs::gettabstate()
{
  std::string r;
  Nedit *e;
  int ndx= currentIndex();
  int len=count();
  for (int i=0; i<len; i++) {
    e=(Nedit *)widget(i);
    r+=spair((ndx==i) ? "1" : "0",e->fname);
  }
  return r;
}

// ---------------------------------------------------------------------
void Ntabs::modificationChanged(bool b)
{
  setmodified(currentIndex(),b);
}

// ---------------------------------------------------------------------
void Ntabs::projectopen(bool openfiles)
{
  int i,index,len;
  QString t;
  QStringList s,open;

  if (project.Id == pnote->Id) return;

  noevents(1);
  tabcloseall();
  pnote->Id = project.Id;
  if (pnote->Id.isEmpty()) {
    tabsetindex(-1);
    pnote->siderefresh();
    recent.ProjectOpen=false;
  }
  noevents(0);

  if (pnote->Id.isEmpty()) return;

  s=recent.projectget(pnote->Id);
  index=s.at(1).toInt();
  for (i=2; i<s.size(); i++) {
    t = project.fullname(s.at(i));
    if (cfexist(t))
      open.append(t);
    else if (i <= index+2) index--;
  }

  noevents(1);
  len=0;
  if (openfiles) {
    for (i=open.size()-1; i>=0; i--) {
      if (0<=tabopen1(project.fullname(open.at(i)),-1))
        len++;
      else
        index=index-(i<index);
    }
  }
  if (len) {
    index=qMax(0,qMin(index,len-1));
  } else
    index=-1;
  tabsetindex(index);
  setUpdatesEnabled(true);
  noevents(0);

  pnote->sideBar->Path=project.Path;
  pnote->siderefresh();
}

// ---------------------------------------------------------------------
void Ntabs::setfont(QFont font)
{
  for (int i=0; i<count(); i++) {
    ((Nedit *)widget(i))->setFont(font);
    ((Nedit *)widget(i))->settabwidth();
  }
}

// ---------------------------------------------------------------------
void Ntabs::setlinenos(bool b)
{
  Q_UNUSED(b);
  for (int i=0; i<count(); i++)
    ((Nedit *)widget(i))->resizer();
  repaint();
}

// ---------------------------------------------------------------------
void Ntabs::setlinewrap(bool b)
{
  setUpdatesEnabled(false);
  PlainTextEdit::LineWrapMode
  m=b?PlainTextEdit::WidgetWidth:PlainTextEdit::NoWrap;
  for (int i=0; i<count(); i++)
    ((Nedit *)widget(i))->setLineWrapMode(m);
  setUpdatesEnabled(true);
  repaint();
}

// ---------------------------------------------------------------------
void Ntabs::setmodified(int index,bool b)
{
  if (index<0) return;
  if (index==currentIndex())
    pnote->setWindowModified(b);
  ((Nedit *)widget(index))->document()->setModified(b);
  tabsetcolor(index,b);
}

// ---------------------------------------------------------------------
void Ntabs::tabclose(int index)
{
  noevents(1);
  tabclose1(index);
  tabsetindex(currentIndex());
  pnote->scriptenable();
  noevents(0);
}

// ---------------------------------------------------------------------
void Ntabs::tabclose1(int index)
{
  if (tabsave(index)) {
    watcher->removePath(((Nedit *)widget(index))->fname);
    removeTab(index);
  }
}

// ---------------------------------------------------------------------
void Ntabs::tabcloseall()
{
  noevents(1);
  int len=count();
  for (int i=0; i<len; i++)
    tabclose1(0);
  tabsetindex(currentIndex());
  pnote->scriptenable();
  noevents(0);
}

// ---------------------------------------------------------------------
// close any tab with file
void Ntabs::tabclosefile(QString f)
{
  QString s=cfcase(f);
  noevents(1);
  int i;
  for (i=0; i<count(); i++)
    if (((Nedit *)widget(i))->fname==s) {
      tabclose(i);
      break;
    }
  tabsetindex(currentIndex());
  noevents(0);
}

// ---------------------------------------------------------------------
void Ntabs::tabCloseRequested(int index)
{
  if (NoEvents) return;
  noevents(1);
  tabclose(index);
  noevents(0);
}

// ---------------------------------------------------------------------
bool Ntabs::tabopen(QString s,int line)
{
  int i,n;
  Nedit *e;
  QString t=cfcase(s);
  for (i=0; i<count(); i++) {
    e=(Nedit *)widget(i);
    if (e->fname==t) {
      setCurrentIndex(i);
      e->selectline(line);
      return true;
    }
  }
  n=tabopen1(s,line);
  if (0>n) return false;
  tabsetindex(n);
  note->activate();
  pnote->scriptenable();
  return true;
}

// ---------------------------------------------------------------------
// does file open only, s is full name
// returns new tab index or -1
int Ntabs::tabopen1(QString s,int line)
{
  int n;
  s=cfcase(s);
  if (note2)
    note2->fileclose(s);
  QFile *f=new QFile(s);
  if (!f->exists()) {
    delete f;
    return -1;
  }
  Nedit *e = new Nedit;
  e->file = f; // NB. e takes ownership
  e->fname = s;
  e->saved=false;
  e->sname = toprojectname(s);
  e->text = cfread(e->file);
  e->setPlainText(e->text);
  if (line>=0) {
    e->moveCursor(QTextCursor::Start);
    e->selectline(line);
  } else {
    e->settop(config.filetop_get(s));
    e->setselect(config.filecur_get(s),0);
  }
  switch (config.OpenTabAt) {
  case 1 :
    n=insertTab(1+currentIndex(),e,e->sname);
    break;
  case 2 :
    n=addTab(e,e->sname);
    break;
  default :
    n=insertTab(0,e,e->sname);
  }
  setmodified(n,false);
  connect(e, SIGNAL(modificationChanged(bool)),
          this, SLOT(modificationChanged(bool)));
  watcher->addPath(s);
#ifdef TABCOMPLETION
  Q_ASSERT(completer);
  if (config.Completion)
    e->setCompleter(completer);
  else
    e->setCompleter(0);
#endif
  config.LastFileOpen=e->fname;
  return n;
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
bool Ntabs::tabprint(int index)
{
  if (index<0) return true;
  Nedit *e=(Nedit *)widget(index);
  dialogprintpreview(this, e);
  return true;
}

// ---------------------------------------------------------------------
bool Ntabs::tabprintall()
{
  bool r=true;
  noevents(1);
  for (int i=0; i<count(); i++) {
    r=tabprint(i);
    if (!r) break;
  }
  noevents(0);
  pnote->siderefresh();
  return r;
}
#endif

// ---------------------------------------------------------------------
bool Ntabs::tabreplace(QString s,int line)
{
  int index=currentIndex();
  tabsave(index);
  Nedit *e=(Nedit *)widget(index);
  s=cfcase(s);
  if (e->fname==s) return true;
  if (!cfexist(s)) return false;
  watcher->removePath(e->fname);
  tabclosefile(s);
  e->file = new QFile(s);
  e->fname = s;
  e->saved=false;
  e->sname = toprojectname(s);
  e->text = cfread(e->file);
  e->setPlainText(e->text);
  if (line>=0) {
    e->moveCursor(QTextCursor::Start);
    e->selectline(line);
  } else {
    e->settop(config.filetop_get(s));
    e->setselect(config.filecur_get(s),0);
  }
  setmodified(index,false);
  tabsetindex(index);
  setTabText(index,e->sname);
  watcher->addPath(s);
  return true;
}

// ---------------------------------------------------------------------
void Ntabs::tabrestore(int index)
{
  Nedit *e=(Nedit *)widget(index);
  QString m="OK to restore to saved: " + toprojectname(e->fname) + "?";
  if (!queryNY("File Restore",m)) return;
  e->setPlainText(e->text);
  setmodified(index,false);
  pnote->siderefresh();
}

// ---------------------------------------------------------------------
bool Ntabs::tabsave(int index)
{
  if (index<0) return true;
  Nedit *e=(Nedit *)widget(index);
  config.filecur_set(e->fname,e->readcurpos());
  config.filetop_set(e->fname,e->readtop());
  QString t = e->toPlainText();
  if (config.TrimTrailingWS)
    t=trimtws(t);
  if (t==e->text) {
    setmodified(index,false);
    return true;
  }
  int r=tabsaveOK(index);
  if (r==0) return false;
  if (r==1)
    e->setPlainText(e->text);
  else {
    pic(e->fname,t);
    cfwrite(e->file,t);
    e->text=t;
    e->saved=true;
  }
  setmodified(index,false);
  pnote->siderefresh();
  return true;
}

// ---------------------------------------------------------------------
bool Ntabs::tabsaveall()
{
  bool r=true;
  noevents(1);
  for (int i=0; i<count(); i++) {
    r=tabsave(i);
    if (!r) break;
  }
  noevents(0);
  pnote->siderefresh();
  return r;
}

// ---------------------------------------------------------------------
void Ntabs::tabsaveas(int index)
{
  if (index<0) return;
  Nedit *e=(Nedit *)widget(index);
  QString old=e->fname;
  QString p=cfpath(old);
  QString s = cfcase(dialogsaveas(this,"Save As", p));
  if (s.isEmpty() || s==old)  return;
  if (!s.contains('.'))
    s+=config.DefExt;
  QFile *f=new QFile(s);
  cfwrite(f,e->text);
  e->file = f; // NB. e takes ownership
  e->fname = s;
  e->sname = toprojectname(s);
  e->saved=true;
  setmodified(index,false);
  setTabText(index,e->sname);
  tabsetindex(index);
  if (recent.Files.contains(old))
    recent.filesadd(s);
  watcher->removePath(old);
  watcher->addPath(s);
}

// ---------------------------------------------------------------------
// returns 0=cancel, 1=no save (restore), 2=OK to save
//
int Ntabs::tabsaveOK(int index)
{
  if (!config.ConfirmSave) return 2;
  Nedit *e=(Nedit *)widget(index);
  if (e->saved) return 2;
  return queryCNY("Edit","OK to save: " + e->sname + "?");
}

// ---------------------------------------------------------------------
void Ntabs::tabsetcolor(int index,bool ifmod)
{
  tabBar()->setTabTextColor(index,ifmod ? Qt::red : Qt::black);
}

// ---------------------------------------------------------------------
void Ntabs::tabsetindex(int index)
{
  if (index<0)
    pnote->settitle("",false);
  else {
    Nedit *e=(Nedit *)widget(index);
    pnote->settitle(e->fname,false);
    setCurrentIndex(index);
    pnote->siderefresh();
    e->setFocus();
  }
}

// ---------------------------------------------------------------------
void Ntabs::tabsettext(QString s)
{
  int n=currentIndex();
  if (n<0) return;
  Nedit *e=(Nedit *)widget(n);
  e->setPlainText(s);
  setmodified(n,s!=e->text);
  pnote->siderefresh();
}
