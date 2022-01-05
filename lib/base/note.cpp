#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QDesktopServices>
#if defined(QT57)
#include <QScreen>
#else
#include <QDesktopWidget>
#endif
#include <QUrl>

#include "base.h"
#include "eview.h"
#include "jsvr.h"
#include "menu.h"
#include "nedit.h"
#include "nmain.h"
#include "note.h"
#include "nside.h"
#include "ntabs.h"
#include "proj.h"
#include "psel.h"
#include "recent.h"
#include "state.h"
#include "svr.h"
#include "tedit.h"
#include "term.h"

// using namespace std;

Note *note=0;
Note *note2=0;

QStringList DS = QStringList() << "ico" << "gif" << "jpeg" << "jpg" << "pdf" << "png" << "svg";

// ---------------------------------------------------------------------
Note::Note()
{
  EditText="Edit";
  if (!config.ProjInit)
    project.init();
  setAttribute(Qt::WA_DeleteOnClose);
  setFocusPolicy(Qt::StrongFocus);
  sideBarShow=true;
  QVBoxLayout *layout=new QVBoxLayout;
  layout->setContentsMargins(layout->contentsMargins());
  layout->setSpacing(0);
  menuBar = new Menu();
  split = new QSplitter(0);
  sideBar = new Nside();
#ifdef SMALL_SCREEN
  sideBar->hide();
  sideBarShow=false;
#endif
  mainBar = new Nmain(this);
  split->addWidget(sideBar);
  split->addWidget(mainBar);
  split->setStretchFactor(1,1);
  QList<int> w;
  w << sideBar->width << sideBar->width;
  split->setSizes(w);
  layout->addWidget(menuBar);
  layout->addWidget(split);
  layout->setStretchFactor(split,1);
  setLayout(layout);
  setWindowTitle("[*]" + EditText);
  setpos();
  menuBar->createActions();
  menuBar->createMenus("note");
  menuBar->createMenus_fini("note");
  scriptenable();
  setWindowIcon(QIcon(":/images/jgreen.png"));
  if (!ShowIde) setVisible(ShowIde);
  QMetaObject::connectSlotsByName(this);
}

// ---------------------------------------------------------------------
Note::~Note()
{
  delactivewindow(this);
  if (this==note2)
    note2=0;
  else {
    note=0;
    if (note2)
      setnote(note2);
  }
}

// ---------------------------------------------------------------------
void Note::activate()
{
  setid();
  activateWindow();
  raise();
  int n=editIndex();
  if (n>=0)
    tabs->currentWidget()->setFocus();
}

// ---------------------------------------------------------------------
void Note::changeEvent(QEvent *event)
{
  if (NoEvents) return;
  if (event->type()==QEvent::ActivationChange && isActiveWindow())  {
    setactivewindow(this);
    setnote(this);
    projectenable();
    QWidget::changeEvent(event);
  }
}

// ---------------------------------------------------------------------
void Note::closeEvent(QCloseEvent *event)
{
  if (!saveall()) {
    event->ignore();
    return;
  }
  projectsave();
  if (note2) {
    setnote(note2);
    note->setFocus();
  }
  QWidget::closeEvent(event);
}

// ---------------------------------------------------------------------
int Note::count()
{
  return tabs->count();
}

// ---------------------------------------------------------------------
int Note::editIndex()
{
  return tabs->currentIndex();
}

// ---------------------------------------------------------------------
QString Note::editFile()
{
  if (tabs->count()==0) return "";
  return ((Nedit *)tabs->currentWidget())->fname;
}

// ---------------------------------------------------------------------
Nedit *Note::editPage()
{
  return (Nedit *)tabs->currentWidget();
}

// ---------------------------------------------------------------------
QString Note::editText()
{
  return ((Nedit *)tabs->currentWidget())->toPlainText();
}

// ---------------------------------------------------------------------
// close tab with file
void Note::fileclose(QString f)
{
  tabs->tabclosefile(f);
}

// ---------------------------------------------------------------------
bool Note::fileopen(QString s,int line)
{
  setid();
  QString ext=cfext(s).mid(1).toLower();
  if (DS.contains(ext))
    return QDesktopServices::openUrl(QUrl("file:///" + s, QUrl::TolerantMode));
  else
    return tabs->tabopen(s,line);
}

// ---------------------------------------------------------------------
bool Note::filereplace(QString s,int line)
{
  setid();
  return tabs->tabreplace(s,line);
}

// ---------------------------------------------------------------------
std::string Note::gettabstate()
{
  return tabs->gettabstate();
}

// ---------------------------------------------------------------------
bool Note::isNoteline(QString s)
{
  if (s.size()<5 || "Note" != s.left(4)) return false;
  QString t=" ('\t";
  return t.contains(s[4]);
}

// ---------------------------------------------------------------------
void Note::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {
  case Qt::Key_Escape:
    if (config.EscClose)
      close(); /* fall-thru */
  default:
    QWidget::keyPressEvent(event);
  }
}

// ---------------------------------------------------------------------
void Note::loadscript(QString s,bool show)
{
  if (note->saveall())
    tedit->loadscript(s,show);
}

// ---------------------------------------------------------------------
void Note::newtemp()
{
  QString f=newtempscript();
  cfcreate(f);
  openfile1(f);
}

// ---------------------------------------------------------------------
void Note::on_lastprojectAct_triggered()
{
  projectsave();
  project.open(project.LastId);
  projectopen(true);
}

// ---------------------------------------------------------------------
void Note::on_openprojectAct_triggered()
{
  new Psel();
}

// ---------------------------------------------------------------------
void Note::on_runallAct_triggered()
{
  runlines(true);
}

// ---------------------------------------------------------------------
void Note::prettyprint()
{
  int n,pos,top;
  QString r;
  savecurrent();
  Nedit *e=editPage();
  jcon->cmddo("require PPScript_jp_");
  jcon->set("arg_jpp_",q2s(editText()));
  r=jcon->cmdr("pplintqt_jpp_ arg_jpp_");
  if (r.isEmpty()) return;
  if (r.at(0)=='0') {
    pos=e->readcurpos();
    top=e->readtop();
    r.remove(0,1);
    settext(r);
    e->settop(top);
    e->setcurpos(pos);
  } else {
    r.remove(0,1);
    n=r.indexOf(' ');
    selectline(r.mid(0,n).toInt());
    info("Format Script",r.mid(n+1));
  }
}

// ---------------------------------------------------------------------
void Note::projectenable()
{
  bool b=project.Id.size()>0;
  foreach (QAction *s, menuBar->ProjectEnable)
    s->setEnabled(b);
  if (config.ifGit) {
    b=project.Git;
    foreach (QAction *s, menuBar->GitEnable)
      s->setEnabled(b);
  }
}

// ---------------------------------------------------------------------
void Note::projectopen(bool b)
{
  tabs->projectopen(b);
  scriptenable();
  projectenable();
}

// ---------------------------------------------------------------------
void Note::projectsave()
{
  setid();
  if (Id.size())
    project.save(tabs->gettablist());
}

// ---------------------------------------------------------------------
void Note::replacetext(Nedit *e, QString txt)
{
  e->selectAll();
  e->insertPlainText(txt);
}

// ---------------------------------------------------------------------
bool Note::saveall()
{
  return tabs->tabsaveall();
}

// ---------------------------------------------------------------------
void Note::savecurrent()
{
  tabs->tabsave(editIndex());
}

// ---------------------------------------------------------------------
void Note::scriptenable()
{
  bool b=tabs->count();
  menuBar->selMenu->setEnabled(b);
  foreach (QAction *s, menuBar->ScriptEnable)
    s->setEnabled(b);
  mainBar->runallAct->setEnabled(b);
}

// ---------------------------------------------------------------------
void Note::scriptglobals()
{
  savecurrent();
  Nedit *e=editPage();
  QString t=editText();
  QString dext=cfext(e->fname);
  QStringList n=globalassigns(t,dext);
  textview("Script Globals","in script: " + e->sname,n.join("\n"));
}


// ---------------------------------------------------------------------
void Note::selectline(int linenum)
{
  editPage()->selectline(linenum);
}

// ---------------------------------------------------------------------
void Note::select_line(QString s)
{
  int pos,len;
  QString hdr,ftr,txt;
  QStringList mid;
  Nedit *e=editPage();
  config.filetop_set(e->fname,e->readtop());
  txt=e->readselect_line(&pos,&len);
  if (s=="note")
    txt=select_line_note(txt,&pos,&len);
  else {
    hdr=txt.mid(0,pos);
    mid=txt.mid(pos,len).split('\n');
    ftr=txt.mid(pos+len);
    mid=select_line1(mid,s,&pos,&len);
    txt=hdr+mid.join("\n")+ftr;
  }
  replacetext(e,txt);
  e->settop(config.filetop_get(e->fname));
  e->setselect(pos,len);
  siderefresh();
}

// ---------------------------------------------------------------------
QStringList Note::select_line1(QStringList mid,QString s,int *pos, int *len)
{
  int i;
  QString com, comment, p, t;

  if (s=="sort") {
    mid.sort();
    return mid;
  }

  if (s=="wrap") {
    sets("inputx_jrx_",q2s(mid.join("\n")));
    return s2q(dors("70 foldtext inputx_jrx_")).split("\n");
  }

  comment=editPage()->getcomment();
  if (comment.isEmpty()) return mid;
  com=comment+" ";

  if (s=="comment") {
    int m=0;
    for (i=0; i<mid.size(); i++)
      if (!matchhead(comment,mid.at(i))) {
        m=1;
        break;
      }
    if (m) {
      for (i=0; i<mid.size(); i++) {
        p=mid.at(i);
        if (p.size())
          p=com+p;
        else
          p=comment;
        mid.replace(i,p);
      }
    } else {
      for (i=0; i<mid.size(); i++) {
        p=mid.at(i);
        if (matchhead(comment,p) && (!matchhead(com+"----",p))
            && (!matchhead(com+"====",p)))
          p=p.mid(comment.size());
        if (p.size() && (p.at(0)==' '))
          p=p.mid(1);
        mid.replace(i,p);
      }
    }
    *len=mid.join("a").size();
    return mid;
  }

  if (s=="plusline1")
    t.fill('-',57);
  else if (s=="plusline2")
    t.fill('=',57);
  else
    return mid;
  t=com + t;
  mid.prepend(t);
  *pos=*pos+1+t.size();
  *len=0;
  return mid;
}

// ---------------------------------------------------------------------
QString Note::select_line_note(QString txt,int *ppos, int *plen)
{
  int cnt,i,len,n,ndx,pos,row;
  bool add;
  QString r;
  QString end=")\n";
  QString end2="))\n";
  QStringList lns,res;

  pos=*ppos;
  len=*plen;
  lns=cut2(txt + '\n');

  row=0;
  for (n=0; row<lns.size(); row++) {
    n+=lns[row].size();
    if (n>pos) break;
  }
  if (row==lns.size())
    return txt;
  ndx=row;
  for (n=0; ndx<lns.size(); ndx++) {
    n+=lns[ndx].size();
    if (n>len) break;
  }
  cnt=1+qMin(ndx,lns.size()-1)-row;

  add=true;
  for (i=row; i>=0; i--) {
    if ((i<row) && lns[i]==end)
      break;
    if (isNoteline(lns[i])) {
      row=i;
      add=false;
      break;
    }
  }

  if (add) {
    n=row;
    for (; n<row+cnt; n++) {
      if (lns[n]==end)
        lns[n]=end2;
      else if (isNoteline(lns[n]))
        break;
    }
    cnt=qMin(cnt,n-row);
    res=lns.mid(0,row);
    res.append("Note''\n");
    res=res + lns.mid(row,cnt);
    res.append(end);
    res=res + lns.mid(row+cnt);
    row++;
  } else {
    lns.removeAt(row);
    n=lns.indexOf(end,row);
    if (n>-1) {
      cnt=n-row;
      lns.removeAt(n);
    } else
      cnt=lns.size()-row-1;
    for (i=row; i<row+cnt; i++)
      if (lns[i]==end2)
        lns[i]=end;
    res=lns;
  }

  pos=len=0;
  for (i=0; i<row; i++)
    pos+=res[i].size();
  for (i=0; i<cnt; i++)
    len+=res[row+i].size();

  r=res.join("");
  r=r.mid(0,r.size()-1);
  *ppos=pos;
  *plen=qMin(len,r.size()-pos);
  return r;
}

// ---------------------------------------------------------------------
void Note::select_text(QString s)
{
  int i,pos,len;
  QString hdr,mid,ftr,txt;
  Nedit *e=editPage();
  config.filetop_set(e->fname,e->readtop());
  txt=e->readselect_text(&pos,&len);
  if (len==0) {
    info("Note","No text selected") ;
    return;
  }

  hdr=txt.mid(0,pos);
  mid=txt.mid(pos,len);
  ftr=txt.mid(pos+len);

  if (s=="lower")
    mid=mid.toLower();
  else if (s=="upper")
    mid=mid.toUpper();
  else if (s=="toggle") {
    QString old=mid;
    QString lc=mid.toLower();
    mid=mid.toUpper();
    for (i=0; i<mid.size(); i++)
      if (mid[i]==old[i]) mid[i]=lc[i];
  }
  replacetext(e,hdr+mid+ftr);
  e->settop(config.filetop_get(e->fname));
  e->setselect(pos,0);
  siderefresh();
}

// ---------------------------------------------------------------------
void Note::setfont(QFont font)
{
  tabs->setfont(font);
}

// ---------------------------------------------------------------------
// ensure current project matches Note
void Note::setid()
{
  if (Id.size() && Id != project.Id)
    project.open(Id);
}

// ---------------------------------------------------------------------
void Note::setindex(int index)
{
  tabs->tabsetindex(index);
}

// ---------------------------------------------------------------------
void Note::setlinenos(bool b)
{
  menuBar->viewlinenosAct->setChecked(b);
  tabs->setlinenos(b);
}

// ---------------------------------------------------------------------
void Note::setlinewrap(bool b)
{
  menuBar->viewlinewrapAct->setChecked(b);
  tabs->setlinewrap(b);
}

// ---------------------------------------------------------------------
void Note::setmodified(int n, bool b)
{
  tabs->setmodified(n,b);
}

// ---------------------------------------------------------------------
// for new note or second note
void Note::setpos()
{
  int x,y,w,h,wid;

  if (note==0) {
    x=config.EditPosX[0];
    y=config.EditPosX[1];
    w=config.EditPosX[2];
    h=config.EditPosX[3];
  } else {
#if defined(QT57)
    QRect s=qApp->primaryScreen()->geometry();
#else
    QDesktopWidget *d=qApp->desktop();
    QRect s=d->screenGeometry();
#endif
    wid=s.width();
    QPoint p=note->pos();
    QSize z=note->size();
    x=p.x();
    y=p.y();
    w=z.width();
    h=z.height();
    x=(wid > w + 2*x) ? wid-w : 0;
  }
  move(x,y);
  resize(w,h);
}

// ---------------------------------------------------------------------
void Note::settext(QString s)
{
  tabs->tabsettext(s);
}

// ---------------------------------------------------------------------
void Note::settitle(QString file, bool mod)
{
  QString f,n,s;

  if (file.isEmpty()) {
    s=EditText;
    if (Id.size())
      s="[" + Id + "] - " + s;
    setWindowTitle(s);
    return;
  }

  s=cfsname(file);
  if (Id.size()) n="[" + Id + "] - ";

  if (file == cpath("~" + Id + "/" + s))
    f = s;
  else
    f = project.projectname(file);
  setWindowTitle("[*]" + f + " - " + n + EditText);
  setWindowModified(mod);
}

// ---------------------------------------------------------------------
void Note::settitle2(bool edit2)
{
  QString t=windowTitle();
  if (edit2) {
    EditText="Edit2";
    if ("2"!=t.right(1))
      setWindowTitle(t+"2");
  } else {
    EditText="Edit";
    if ("2"==t.right(1))
      setWindowTitle(t.remove(t.size()-1,1));
  }
}

// ---------------------------------------------------------------------
void Note::siderefresh()
{
  sideBar->refresh();
}

// ---------------------------------------------------------------------
void Note::tabclose(int index)
{
  tabs->tabclose(index);
  scriptenable();
}
