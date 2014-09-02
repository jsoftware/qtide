
#include <QGridLayout>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QGroupBox>
#include <QSignalMapper>
#include <QSplitter>

#include "wd.h"
#include "font.h"
#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "child.h"
#include "layout.h"

#include "button.h"
#include "checkbox.h"
#include "combobox.h"
#include "dateedit.h"
#include "isigrid.h"
#include "dspinbox.h"
#include "dummy.h"
#include "edit.h"
#include "editm.h"
#include "edith.h"
#include "image.h"
#include "isidraw.h"
#include "isigraph.h"
#include "listbox.h"
#ifndef QT_NO_OPENGL
#include "opengl.h"
#endif
#include "progressbar.h"
#ifndef QT_NO_QUICKWIDGET
#include "quickwidget.h"
#endif
#include "qwidget.h"
#include "radiobutton.h"
#include "scrollarea.h"
#include "slider.h"
#include "spinbox.h"
#include "static.h"
#include "statusbar.h"
#include "table.h"
#include "tabs.h"
#include "timeedit.h"
#include "toolbar.h"
#ifndef QT_NO_WEBKIT
#include "webview.h"
#endif

extern Font *fontdef;

// ---------------------------------------------------------------------
Pane::Pane(int n,Form *f) : QWidget(f)
{
  pform=f;
  buttongroup=0;
  child=0;
  groupboxw=0;
  layout=0;
  sizew=sizeh=0;
  if (n==1) {
    bin("v");
    layout->bin->setContentsMargins(0,0,0,0);
    layout->bin->setSpacing(0);
  }
}

// ---------------------------------------------------------------------
// return if child valid
bool Pane::addchild(string n,string c,string p)
{
  if (!layout)
    bin("v");
  if (c=="button")
    child=(Child *) new Button(n,p,pform,this);
  else if (c=="checkbox")
    child=(Child *) new CheckBox(n,p,pform,this);
  else if (c=="combobox")
    child=(Child *) new ComboBox(n,"edit " + p,pform,this);
  else if (c=="combolist")
    child=(Child *) new ComboBox(n,p,pform,this);
  else if (c=="dateedit")
    child=(Child *) new DateEdit(n,p,pform,this);
  else if (c=="dspinbox")
    child=(Child *) new DSpinBox(n,p,pform,this);
  else if (c=="edit")
    child=(Child *) new Edit(n,p,pform,this);
  else if (c=="editm")
    child=(Child *) new Editm(n,p,pform,this);
  else if (c=="edith")
    child=(Child *) new Edith(n,p,pform,this);
  else if (c=="image")
    child=(Child *) new Image(n,p,pform,this);
  else if (c=="isidraw")
    child=(Child *) new Isidraw(n,p,pform,this);
  else if (c=="isigraph")
    child=(Child *) new Isigraph(n,p,pform,this);
  else if (c=="isigrid")
    child=(Child *) new IsiGrid(n,p,pform,this);
  else if (c=="listbox")
    child=(Child *) new ListBox(n,p,pform,this);
#ifndef QT_NO_OPENGL
  else if (c=="opengl")
    child=(Child *) new Opengl(n,p,pform,this);
#endif
  else if (c=="progressbar")
    child=(Child *) new ProgressBar(n,p,pform,this);
#ifndef QT_NO_QUICKWIDGET
  else if (c=="quickwidget")
    child=(Child *) new QuickWidget(n,p,pform,this);
#endif
  else if (c=="qwidget")
    child=(Child *) new QWidgex(n,p,pform,this);
  else if (c=="radiobutton")
    child=(Child *) new RadioButton(n,p,pform,this);
  else if (c=="scrollarea")
    child=(Child *) new ScrollArea(n,p,pform,this);
  else if (c=="slider")
    child=(Child *) new Slider(n,p,pform,this);
  else if (c=="spinbox")
    child=(Child *) new SpinBox(n,p,pform,this);
  else if (c=="static")
    child=(Child *) new Static(n,p,pform,this);
  else if (c=="statusbar")
    child=(Child *) new StatusBar(n,p,pform,this);
  else if (c=="table")
    child=(Child *) new Table(n,p,pform,this);
  else if (c=="tab")
    child=(Child *) new Tabs(n,p,pform,this);
  else if (c=="timeedit")
    child=(Child *) new TimeEdit(n,p,pform,this);
  else if (c=="toolbar")
    child=(Child *) new ToolBar(n,p,pform,this);
#ifndef QT_NO_WEBKIT
  else if (c=="webview")
    child=(Child *) new Webview(n,p,pform,this);
#endif
  else {
    sizew=sizeh=0;
    error("child not supported " + c);
    return false;
  }
  if (fontdef) child->setfont(fontdef->font);
  if (child->widget) {
    layout->addWidget(child->widget);
    child->setminwh(sizew,sizeh);
    lasttype=child->type;
    QStringList opt=qsplit(p);
    if (opt.contains("flush")) {
      layout->bin->setContentsMargins(0,0,0,0);
      layout->bin->setSpacing(0);
    }
  }
  sizew=sizeh=0;
  pform->addchild(child);
  return true;
}

// ---------------------------------------------------------------------
void Pane::addlayout(Layout *b)
{
  layout=b;
  layouts.append(b);
}

// ---------------------------------------------------------------------
void Pane::bin(string s)
{
  QChar c;
  int i,n;
  Layout *b;
  QString m;
  QStringList p=bsplit(s);

  for (i=0; i<p.size(); i++) {
    m=p.at(i);
    c=m[0];
    n=c_strtoi(q2s(m.mid(1)));
    if (c=='h'||c=='v'||c=='g')
      addlayout(new Layout(c,n,this));
    else if (c=='m' && layout->type!='g') {
      layout->bin->setContentsMargins(n,n,n,n);
      layout->bin->setSpacing(n);
    } else if (c=='p' && layout->type!='g')
      layout->addSpacing(n);
    else if (c=='s') {
      if ('g'==layout->type) {
        error("grid cannot contain bin s");
        return;
      }
      layout->addStretch(n);
    } else if (c=='z' && layouts.size()>1) {
      b=layout;
      n=layouts.last()->stretch;
      layouts.removeLast();
      layout=layouts.last();
      layout->addLayout(b);
    }
  }
}

// ---------------------------------------------------------------------
void Pane::fini()
{
  if (layouts.size()) {
    while (layouts.size()>1)
      bin("z");
    setLayout(layout->bin);
  }
  pform->closepane();
}

// ---------------------------------------------------------------------
void Pane::grid(string c, string s)
{
  if (c=="size") {
    int rmax,cmax;
    QStringList opt=qsplit(s);
    int n=opt.size();
    if (2>n) {
      error("missing grid size row_size or column_size");
      return;
    }
    if (!layout)
      bin("v");
    if ('g'!=layout->type)
      bin("g");
    rmax=c_strtoi(q2s(opt.at(0)));
    cmax=c_strtoi(q2s(opt.at(1)));
    if ((rmax<=0)||(cmax<=0)) {
      error("grid size row_size and column_size must be positive");
      return;
    }
    layout->rmax=rmax;
    layout->cmax=cmax;
    layout->razed=true;
  } else if (c=="cell") {
    int r,c,rs,cs,alignment=0;
    QStringList opt=qsplit(s);
    int n=opt.size();
    if (!(2==n || 3==n || 4==n || 5==n)) {
      error("not grid cell row, column [,row_span, column_span] [,alignment] ");
      return;
    }
    if (!layout)
      bin("v");
    if ('g'!=layout->type)
      bin("g");
    if (layout->razed) {
      error("grid is razed");
      return;
    }
    if (4>n) {
      r=c_strtoi(q2s(opt.at(0)));
      c=c_strtoi(q2s(opt.at(1)));
      rs=cs=1;
      if (3==n) alignment=c_strtoi(q2s(opt.at(2)));
    } else {
      r=c_strtoi(q2s(opt.at(0)));
      c=c_strtoi(q2s(opt.at(1)));
      rs=c_strtoi(q2s(opt.at(2)));
      cs=c_strtoi(q2s(opt.at(3)));
      if (5==n) alignment=c_strtoi(q2s(opt.at(4)));
    }
    if ((r<0)||(c<0)) {
      error("grid cell row and column must be non-negative");
      return;
    }
    if ((rs<=0)||(cs<=0)) {
      error("grid cell row_span and column_span must be positive");
      return;
    }
    if (alignment<0) {
      error("grid cell alignment must be non-negative");
      return;
    }
    layout->r=r;
    layout->c=c;
    layout->rs=rs;
    layout->cs=cs;
    layout->alignment=alignment;
  } else if (c=="colwidth") {
    QStringList opt=qsplit(s);
    int c,w=0;
    int n=opt.size();
    if (2>n) {
      error("grid colwidth must specify column and width");
      return;
    }
    c=c_strtoi(q2s(opt.at(0)));
    w=c_strtoi(q2s(opt.at(1)));
    if ((c<0)||(w<0)) {
      error("grid colwidth column and width must be non-negative");
      return;
    }
    if (layout->razed && (layout->cmax<=c)) {
      error("grid colwidth invalid column");
      return;
    }
    ((QGridLayout *)(layout->bin))->setColumnMinimumWidth(c,w);
  } else if (c=="colstretch") {
    QStringList opt=qsplit(s);
    int c,s=0;
    int n=opt.size();
    if (2>n) {
      error("grid colstretch must specify column and stretch");
      return;
    }
    c=c_strtoi(q2s(opt.at(0)));
    s=c_strtoi(q2s(opt.at(1)));
    if ((c<0)||(s<0)) {
      error("grid colstretch column and stretch must be non-negative");
      return;
    }
    if (layout->razed && (layout->cmax<=c)) {
      error("grid colstretch invalid column");
      return;
    }
    ((QGridLayout *)(layout->bin))->setColumnStretch(c,s);
  } else if (c=="rowheight") {
    QStringList opt=qsplit(s);
    int r,h=0;
    int n=opt.size();
    if (2>n) {
      error("grid row height must specify row and height");
      return;
    }
    r=c_strtoi(q2s(opt.at(0)));
    h=c_strtoi(q2s(opt.at(1)));
    if ((r<0)||(h<0)) {
      error("grid rowheight row and height must be non-negative");
      return;
    }
    if (layout->razed && (layout->rmax<=r)) {
      error("grid rowheight invalid row");
      return;
    }
    ((QGridLayout *)(layout->bin))->setRowMinimumHeight(r,h);
  } else if (c=="rowstretch") {
    QStringList opt=qsplit(s);
    int r,s=0;
    int n=opt.size();
    if (2>n) {
      error("grid row stretch must specify row and height");
      return;
    }
    r=c_strtoi(q2s(opt.at(0)));
    s=c_strtoi(q2s(opt.at(1)));
    if ((r<0)||(s<0)) {
      error("grid rowstretch row and stretch must be non-negative");
      return;
    }
    if (layout->razed && (layout->rmax<=r)) {
      error("grid rowstretch invalid row");
      return;
    }
    ((QGridLayout *)(layout->bin))->setRowStretch(r,s);
  } else
    error("bad grid command");
}

// ---------------------------------------------------------------------
bool Pane::groupbox(string c, string s)
{
  QString cmd=s2q(c);
  QString id;

  if (cmd=="groupbox") {
    if (!layout)
      bin("v");
    QStringList opt=qsplit(s);
    if (opt.size())
      id=opt.at(0);
    groupboxw=new QGroupBox(id);
    if (fontdef) groupboxw->setFont(fontdef->font);
    layout->addWidget(groupboxw);
    QVBoxLayout *vb=new QVBoxLayout;
    vb->addWidget(pform->addpane(0));
    groupboxw->setLayout(vb);
    form->pane->bin("v");
    return true;
  }

  if (cmd=="groupboxend") {
    bin("z");
    int n=pform->panes.size();
    if (n>1) {
      Pane *p=pform->panes.at(n-2);
      if (p->groupboxw) {
        fini();
        p->groupboxw=0;
        return true;
      }
      error("No groupbox to end");
      return false;
    }
  }
  return false;
}

// ---------------------------------------------------------------------
bool Pane::line(string c, string s)
{
  Q_UNUSED(s);
  QString cmd=s2q(c);
  if (!(cmd=="line" || cmd=="lineh" || cmd=="linev"))
    return false;
  QFrame *f=new QFrame();
  f->setFrameShape((cmd=="linev") ? QFrame::VLine : QFrame::HLine);
  f->setFrameShadow(QFrame::Sunken);
  layout->addWidget(f);
  return true;
}

// ---------------------------------------------------------------------
void Pane::setstretch(Child *cc, string factor)
{
  if (layout->type!='g')((QBoxLayout *)(layout->bin))->setStretchFactor(cc->widget,atoi(factor.c_str()));
}

// ---------------------------------------------------------------------
bool Pane::split(string c, string s)
{
  if (c=="splith" || c=="splitv") {
    if (!layout)
      bin("v");
    qsplitter=new QSplitter((c=="splith")?Qt::Horizontal : Qt::Vertical);
    qsplitter->addWidget(pform->addpane(1));
    qsplitterp=qs2intlist(s2q(s));
    return true;
  }

  if (!(c=="splitend" || c=="splitsep")) return false;

  fini();
  Pane *sp=pform->pane;

  if (c=="splitend")
    sp->splitend();
  else
    sp->qsplitter->addWidget(pform->addpane(1));
  return true;
}

// ---------------------------------------------------------------------
void Pane::splitend()
{
  if (qsplitterp.size()==4)
    qsplitter->setSizes(qsplitterp.mid(2));
  if (qsplitterp.size()>=2) {
    qsplitter->setStretchFactor(0,qsplitterp[0]);
    qsplitter->setStretchFactor(1,qsplitterp[1]);
  }
  layout->addWidget(qsplitter);
}
