
#include <QBoxLayout>
#include <QButtonGroup>
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
#include "isigraph.h"
#include "listbox.h"
#include "opengl.h"
#include "radiobutton.h"
#include "static.h"
#include "table.h"
#include "webview.h"

extern int jedo(char*);

// ---------------------------------------------------------------------
Pane::Pane(int n,Form *f)
{
  pform=f;
  buttongroup=0;
  child=0;
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
  else if (c=="opengl")
    child=(Child *) new Opengl(n,p,pform,this);
  else if (c=="groupbox")
    child=(Child *) new Static(n,"groupbox " + p,pform,this);
  else if (c=="listbox")
    child=(Child *) new ListBox(n,p,pform,this);
  else if (c=="isigraph")
    child=(Child *) new Isigraph(n,p,pform,this);
  else if (c=="radiobutton")
    child=(Child *) new RadioButton(n,p,pform,this);
  else if (c=="static")
    child=(Child *) new Static(n,p,pform,this);
  else if (c=="staticbox")
    child=(Child *) new Static(n,"staticbox " + p,pform,this);
  else if (c=="table")
    child=(Child *) new Table(n,p,pform,this);
  else if (c=="webview")
    child=(Child *) new Webview(n,p,pform,this);
  else
// TODO
  {
    qDebug () << s2q("child not supported " + c);
    sizew=sizeh=0;
    return true;
  }
  if (pform->fontdef) child->setfont(pform->fontdef->font);
  if (child->widget) {
    layout->addWidget(child->widget);
    child->setminwh(sizew,sizeh);
    lasttype=child->type;
  }
  sizew=sizeh=0;
  pform->addchild(child);
  return true;
}

// ---------------------------------------------------------------------
void Pane::addlayout(QBoxLayout *b, int n)
{
  layout=b;
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
    n=m.mid(1).toInt();
    if (c=='h')
      addlayout(new QHBoxLayout,n);
    else if (c=='v')
      addlayout(new QVBoxLayout,n);
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
    qsplit=new QSplitter((c=="splith")?Qt::Horizontal : Qt::Vertical);
    qsplit->addWidget(pform->addpane(1));
    qsplitp=qs2intlist(s2q(s));
    return true;
  }

  if (!(c=="splitend" || c=="splitsep")) return false;

  fini();
  Pane *sp=pform->pane;

  if (c=="splitend")
    sp->splitend();
  else
    sp->qsplit->addWidget(pform->addpane(1));
  return true;
}

// ---------------------------------------------------------------------
void Pane::splitend()
{
  if (qsplitp.size()==4)
    qsplit->setSizes(qsplitp.mid(2));
  if (qsplitp.size()>=2) {
    qsplit->setStretchFactor(0,qsplitp[0]);
    qsplit->setStretchFactor(1,qsplitp[1]);
  }
  layout->addWidget(qsplit);
}
