#include <QApplication>

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

using namespace std;

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
  int index=getfileindex(path);
  Nedit *e=(Nedit *)widget(index);
  if (e->text==cfread(e->file)) return;
  QString m="File contents changed on disk: " + e->sname;
  m=m+"\n\n" + "Reload from disk?";
  if (queryNY("File Changed",m)) {
    e->text = cfread(e->file);
    e->setPlainText(e->text);
    setmodified(index,false);
  }
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

  if (project.Id == Id) return;

  noevents(1);
  tabcloseall();
  Id = project.Id;
  if (Id.isEmpty()) {
    tabsetindex(-1);
    pnote->siderefresh();
    recent.ProjectOpen=false;
    return;
  }
  noevents(0);

  s=recent.projectget(Id);
  index=s.at(1).toInt();
  for (i=2; i<s.size(); i++) {
    t = project.fullname(s.at(i));
    if (cfexist(t))
      open.append(t);
    else if (i <= index+2) index--;
  }

  noevents(1);
  len=0;
  if(openfiles) {
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
  for (int i=0; i<count(); i++)
    ((Nedit *)widget(i))->setFont(font);
}

// ---------------------------------------------------------------------
void Ntabs::setlinenos(bool b)
{
  Q_UNUSED(b);
  for(int i=0; i<count(); i++)
    ((Nedit *)widget(i))->resizer();
  repaint();
}

// ---------------------------------------------------------------------
void Ntabs::setlinewrap(bool b)
{
  setUpdatesEnabled(false);
  PlainTextEdit::LineWrapMode
  m=b?PlainTextEdit::WidgetWidth:PlainTextEdit::NoWrap;
  for(int i=0; i<count(); i++)
    ((Nedit *)widget(i))->setLineWrapMode(m);
  setUpdatesEnabled(true);
  repaint();
}

// ---------------------------------------------------------------------
void Ntabs::setmodified(int index,bool b)
{
  if (index<0) return;
  if(index==currentIndex())
    pnote->setWindowModified(b);
  ((Nedit *)widget(index))->document()->setModified(b);
  tabsetcolor(index,b);
}

// ---------------------------------------------------------------------
void Ntabs::tabclose(int index)
{
  noevents(1);
  if (tabsave(index)) {
    watcher->removePath(((Nedit *)widget(index))->fname);
    removeTab(index);
  }
  pnote->scriptenable();
  noevents(0);
}

// ---------------------------------------------------------------------
void Ntabs::tabcloseall()
{
  noevents(1);
  int len=count();
  for (int i=0; i<len; i++)
    tabclose(0);
  noevents(0);
}

// ---------------------------------------------------------------------
// close any tab with file
void Ntabs::tabclosefile(QString f)
{
  noevents(1);
  int i;
  for (i=0; i<count(); i++)
    if (((Nedit *)widget(i))->fname==f) {
      tabclose(i);
      break;
    }
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
  for (i=0; i<count(); i++) {
    e=(Nedit *)widget(i);
    if (e->fname==s) {
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
  } else
    e->settop(config.filepos_get(s));
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
  if(note2)
    note2->fileclose(s);
  watcher->addPath(s);
  return n;
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
bool Ntabs::tabprint(int index)
{
  if (index<0) return true;
  Nedit *e=(Nedit *)widget(index);
#ifdef QT50
  e->print((QPagedPaintDevice *)config.Printer);
#else
  e->print(config.Printer);
#endif
  return true;
}

// ---------------------------------------------------------------------
bool Ntabs::tabprintall()
{
  bool r=true;
  noevents(1);
  for(int i=0; i<count(); i++) {
    r=tabprint(i);
    if (!r) break;
  }
  noevents(0);
  pnote->siderefresh();
  return r;
}
#endif

// ---------------------------------------------------------------------
void Ntabs::tabrestore(int index)
{
  Nedit *e=(Nedit *)widget(index);
  QString m="OK to restore to saved: " + toprojectname(e->fname) + "?";
  if(!queryNY("File Restore",m)) return;
  e->setPlainText(e->text);
  setmodified(index,false);
  pnote->siderefresh();
}

// ---------------------------------------------------------------------
bool Ntabs::tabsave(int index)
{
  if (index<0) return true;
  Nedit *e=(Nedit *)widget(index);
  config.filepos_set(e->fname,e->readtop());
  QString t = e->toPlainText();
  if (config.TrimTrailingWS)
    t=dtbs(t);
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
  for(int i=0; i<count(); i++) {
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
  QString p=cfpath(e->fname);
  QString s = cfcase(dialogsaveas(this,"Save As", p));
  if (s.isEmpty() || s==e->fname)  return;
  if (!s.contains('.'))
    s+=config.DefExt;
  QFile *f=new QFile(s);
  cfwrite(f,e->text);
  e->file = f; // NB. e takes ownership
  e->fname = s;
  e->sname = cfsname(s);
  e->saved=true;
  setmodified(index,false);
  setTabText(index,e->sname);
  tabsetindex(index);
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
