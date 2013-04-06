
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

#include "button.h"
#include "checkbox.h"
#include "combobox.h"
#include "dummy.h"
#include "edit.h"
#include "editm.h"
#include "image.h"
#include "isigraph.h"
#include "listbox.h"
#ifdef QT_OPENGL
#include "opengl.h"
#endif
#include "progressbar.h"
#include "radiobutton.h"
#include "slider.h"
#include "spinbox.h"
#include "static.h"
#include "statusbar.h"
#include "table.h"
#include "tabs.h"
#include "toolbar.h"
#ifdef QT_WEBKIT
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
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
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
  else if (c=="edit")
    child=(Child *) new Edit(n,p,pform,this);
  else if (c=="editm")
    child=(Child *) new Editm(n,p,pform,this);
  else if (c=="combobox")
    child=(Child *) new ComboBox(n,"edit " + p,pform,this);
  else if (c=="combolist")
    child=(Child *) new ComboBox(n,p,pform,this);
  else if (c=="image")
    child=(Child *) new Image(n,p,pform,this);
  else if (c=="isigraph")
    child=(Child *) new Isigraph(n,p,pform,this);
  else if (c=="listbox")
    child=(Child *) new ListBox(n,p,pform,this);
#ifdef QT_OPENGL
  else if (c=="opengl")
    child=(Child *) new Opengl(n,p,pform,this);
#endif
  else if (c=="progressbar")
    child=(Child *) new ProgressBar(n,p,pform,this);
  else if (c=="radiobutton")
    child=(Child *) new RadioButton(n,p,pform,this);
  else if (c=="slider")
    child=(Child *) new Slider(n,p,pform,this);
  else if (c=="spinbox")
    child=(Child *) new SpinBox(n,p,pform,this);
  else if (c=="static")
    child=(Child *) new Static(n,p,pform,this);
//  else if (c=="staticbox")
//    child=(Child *) new Static(n,"staticbox " + p,pform,this);
  else if (c=="statusbar")
    child=(Child *) new StatusBar(n,p,pform,this);
  else if (c=="table")
    child=(Child *) new Table(n,p,pform,this);
  else if (c=="tab")
    child=(Child *) new Tabs(n,p,pform,this);
  else if (c=="toolbar")
    child=(Child *) new ToolBar(n,p,pform,this);
#ifdef QT_WEBKIT
  else if (c=="webview")
    child=(Child *) new Webview(n,p,pform,this);
#endif
  else {
//    qDebug () << s2q("child not supported " + c);
    sizew=sizeh=0;
    error("child not supported " + c);
    return false;
  }
  if (fontdef) child->setfont(fontdef->font);
  if (child->widget) {
    layout->addWidget(child->widget);
    child->setminwh(sizew,sizeh);
    lasttype=child->type;
    if ((c=="isigraph")||(c=="opengl")) {
      layout->setContentsMargins(0,0,0,0);
      layout->setSpacing(0);
    }
  }
  sizew=sizeh=0;
  pform->addchild(child);
  return true;
}

// ---------------------------------------------------------------------
void Pane::addlayout(QBoxLayout *b, int n)
{
  layout=b;
//  layout->setContentsMargins(0,0,0,0);
//  layout->setSpacing(0);
  layouts.append(b);
  layoutx.append(n);
}

// ---------------------------------------------------------------------
void Pane::bin(string s)
{
  QChar c;
  int i,n;
  QBoxLayout *b;
  QString m;
  QStringList p=bsplit(s);

  for (i=0; i<p.size(); i++) {
    m=p.at(i);
    c=m[0];
    n=c_strtoi(q2s(m.mid(1)));
    if (c=='h')
      addlayout(new QHBoxLayout,n);
    else if (c=='v')
      addlayout(new QVBoxLayout,n);
    else if (c=='p')
      layout->addSpacing(n);
    else if (c=='s')
      layout->addStretch(n);
    else if (c=='z' && layouts.size()>1) {
      b=layout;
      n=layoutx.last();
      layouts.removeLast();
      layoutx.removeLast();
      layout=layouts.last();
      layout->addLayout(b,n);
    }
  }
}

// ---------------------------------------------------------------------
void Pane::fini()
{
  if (layouts.size()) {
    while (layouts.size()>1)
      bin("z");
    setLayout(layout);
  }
  pform->closepane();
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
    return true;
  }

  if (cmd=="groupboxend") {
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
  layout->setStretchFactor(cc->widget,atoi(factor.c_str()));
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
