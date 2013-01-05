
#include <QBoxLayout>
#include <QButtonGroup>
#include <QMenuBar>
#include <QSignalMapper>

#include "wd.h"
#include "font.h"
#include "cmd.h"
#include "form.h"
#include "child.h"
#include "button.h"
#include "checkbox.h"
#include "combobox.h"
#include "dummy.h"
#include "edit.h"
#include "editm.h"
#include "isigraph.h"
#include "isigraph2.h"
#include "listbox.h"
#include "radiobutton.h"
#include "static.h"
#include "table.h"
#include "menus.h"

extern int jedo(char*);

// ---------------------------------------------------------------------
Form::Form(string s, string p, string loc, QWidget *parent)
{
  Q_UNUSED(parent);
  id=s;
  buttongroup=0;
  child=0;
  evtchild=0;
  layout=0;
  locale=loc;
  fontdef=0;
  menubar=0;
  closed=false;
  shown=false;
  sizew=sizeh=0;

  setAttribute(Qt::WA_DeleteOnClose);
  QStringList m=s2q(p).split(' ',QString::SkipEmptyParts);
  closeok=m.contains("closeok");
  escclose=m.contains("escclose");
  setpn(s);

  signalMapper=new QSignalMapper(this);
  connect(signalMapper,SIGNAL(mapped(QWidget *)),
          this,SLOT(buttonClicked(QWidget *)));
}

// ---------------------------------------------------------------------
Form::~Form()
{
  Child *n;
  for (int i=0; i<children.size(); i++)
    if ("isigraph"==(n=children.at(i))->type) {
      if (((Isigraph2*)n->widget)->painter) {
        ((Isigraph2*)n->widget)->painter->end();
        delete ((Isigraph2*)n->widget)->painter;
        ((Isigraph2*)n->widget)->painter=0;
      }
    }
  if (this==form) form = 0;
  if (this==evtform) evtform = 0;
  Forms.removeOne(this);
}

// ---------------------------------------------------------------------
// return if child valid
bool Form::addchild(string n,string c,string p)
{
  if (!layout)
    bin("v");
  if (c=="button")
    child=(Child *) new Button(n,p,this);
  else if (c=="checkbox")
    child=(Child *) new CheckBox(n,p,this);
  else if (c=="edit")
    child=(Child *) new Edit(n,p,this);
  else if (c=="editm")
    child=(Child *) new Editm(n,p,this);
  else if (c=="combobox")
    child=(Child *) new ComboBox(n,"edit " + p,this);
  else if (c=="combolist")
    child=(Child *) new ComboBox(n,p,this);
  else if (c=="groupbox")
    child=(Child *) new Static(n,p,this);
  else if (c=="listbox")
    child=(Child *) new ListBox(n,p,this);
  else if (c=="isigraph")
    child=(Child *) new Isigraph(n,p,this);
  else if (c=="radiobutton")
    child=(Child *) new RadioButton(n,p,this);
  else if (c=="static")
    child=(Child *) new Static(n,p,this);
  else if (c=="staticbox")
    child=(Child *) new Static(n,p,this);
  else if (c=="table")
    child=(Child *) new Table(n,p,this);
  else
// TODO
  {
    qDebug () << s2q("child not supported " + c);
    sizew=sizeh=0;
    return true;
  }
//    return false;
  if (fontdef) child->setfont(fontdef->font);
  if (child->widget) {
    layout->addWidget(child->widget);
    child->setminwh(sizew,sizeh);
    lasttype=child->type;
  }
  sizew=sizeh=0;
  children.append(child);
  return true;
}

// ---------------------------------------------------------------------
void Form::addlayout(QBoxLayout *b, int n)
{
  layout=b;
  layouts.append(b);
  layoutx.append(n);
}

// ---------------------------------------------------------------------
void Form::addmenu()
{
  if (!layout)
    bin("v");
  menubar= new Menus("menu","",this);
  child=(Child *) menubar;
  form->layout->insertWidget(0,child->widget);
  children.append(child);
}

// ---------------------------------------------------------------------
void Form::bin(string s)
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
void Form::buttonClicked(QWidget *w)
{
  Child *child;
  child=(Child *) w;
  child->event="button";
  signalevent(child);
}

// ---------------------------------------------------------------------
void Form::closeEvent(QCloseEvent *e)
{
  qDebug() << "form closeEvent " + s2q(id);
  if (closeok || closed) {
    qDebug() << "form close by closeok or wd'pclose' " + s2q(id);
    e->accept();
    return;
  }
  e->ignore();
  qDebug() << "call wd form_close " + s2q(id);
  event="close";
  form=this;
  signalevent(0);
  if (closed) {
    e->accept();
  } else e->ignore();
}

// ---------------------------------------------------------------------
string Form::hschild()
{
  return p2s((void *)child);
}

// ---------------------------------------------------------------------
string Form::hsform()
{
  return p2s((void *)this);
}

// ---------------------------------------------------------------------
Child *Form::id2child(string n)
{
  for (int i=0; i<children.size(); i++)
    if (n==children.at(i)->id)
      return children.at(i);
  return (Child *) 0;
}

// ---------------------------------------------------------------------
bool Form::ischild(Child *n)
{
  for (int i=0; i<children.size(); i++)
    if (n==children.at(i))
      return true;
  return false;
}


// ---------------------------------------------------------------------
void Form::keyPressEvent(QKeyEvent *e)
{
  if (escclose && e->key()==Qt::Key_Escape) {
    e->ignore();
    delete this;
  }
}

// ---------------------------------------------------------------------
void Form::setstretch(Child *cc, string factor)
{
  layout->setStretchFactor(cc->widget,atoi(factor.c_str()));
}

// ---------------------------------------------------------------------
Child *Form::setmenuid(string id)
{
  if (menubar && menubar->items.contains(s2q(id)))
    return (Child *) menubar;
  return (Child *) 0;
}

// ---------------------------------------------------------------------
void Form::setpn(string p)
{
  setWindowTitle(s2q(p));
}

// ---------------------------------------------------------------------
void Form::showit()
{
  while (layouts.size()>1)
    bin("z");
  setLayout(layout);
  show();
  shown=true;
}

// ---------------------------------------------------------------------
void Form::signalevent(Child *c)
{
  string loc = locale;
  if (closed || !shown) return;
  evtform=this;
  evtchild=c;
  if (c) {
    c->setform();
    sysmodifiers=c->sysmodifiers;
    sysdata=c->sysdata;
    loc = (""!=c->locale)?c->locale:locale;
//  qDebug() << "signalevent child " + s2q(c->id) + " " + s2q(c->event);
  } else {
  }
  string s="(i.0 0)\"_ wdhandler_" + loc + "_$0";
  jedo((char *)s.c_str());
}

// ---------------------------------------------------------------------
string Form::state(int evt)
{
  string c,c1,e,r,s,ec;

  if (evt) {
    if (evtchild) {
      c=evtchild->id;
      c1=c+"_";
      e=evtchild->event;
      ec=evtchild->locale;
    } else
      e=event;
    r+=spair("syshandler",id+"_handler");
    r+=spair("sysevent",id+"_"+c1+e);
    r+=spair("sysdefault",id+"_default");
    r+=spair("sysparent",id);
    r+=spair("syschild",c);
    r+=spair("systype",e);
    r+=spair("syslocalec",ec);
  }

  // need only syslocale (not syslocalep, syslocalec)?...  in isigraph
  r+=spair("syslocalep",locale);
  r+=spair("syshwndp",hsform());
  r+=spair("syshwndc",hschild());
  r+=spair("syslastfocus","");
  r+=spair("sysmodifiers",sysmodifiers);
  r+=spair("sysdata",sysdata);

  for (int i=0; i<children.size(); i++)
    s+=children.at(i)->state();

  return r+s;
}
