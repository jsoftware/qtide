
#include <QMenu>
#include <QMenuBar>

#include "wd.h"
#include "menus.h"
#include "cmd.h"
#include "form.h"
#include "pane.h"

// ---------------------------------------------------------------------
Menus::Menus(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="menu";
  curMenu=0;
  id="";
  widget=(QWidget*) new QMenuBar(f);
  widget->setSizePolicy (QSizePolicy::Ignored, QSizePolicy::Maximum);
}

// ---------------------------------------------------------------------
QAction *Menus::makeact(string id, string p)
{
  QStringList s=qsplit(p);
  QString text=s.value(0);
  QString shortcut=s.value(1);
  QAction *r = new QAction(text,widget);
  QString name=s2q(id);
  r->setObjectName(name);
  r->setMenuRole(QAction::NoRole);
  if (shortcut.size())
    r->setShortcut(shortcut);
  items[name]=r;
  return r;
}

// ---------------------------------------------------------------------
int Menus::menu(string c, string p)
{
  if (curMenu==0)
    return 1;
  curMenu->addAction(makeact(c,p));
  return 0;
}

// ---------------------------------------------------------------------
int Menus::menupop(string c)
{
  QString s=s2q(c);
  if (curMenu==0) {
    curMenu=((QMenuBar*) widget)->addMenu(s);
    connect(curMenu,SIGNAL(triggered(QAction *)),
            this,SLOT(menu_triggered(QAction *)));
  } else
    curMenu=curMenu->addMenu(s);
  curMenu->menuAction()->setMenuRole(QAction::NoRole);
  menus.append(curMenu);
  return 0;
}

// ---------------------------------------------------------------------
int Menus::menupopz()
{
  if (menus.isEmpty()) return 0;
  menus.removeLast();
  if (menus.isEmpty())
    curMenu=0;
  else
    curMenu=menus.last();
  return 0;
}

// ---------------------------------------------------------------------
int Menus::menusep()
{
  if (curMenu==0)
    return 1;
  curMenu->addSeparator();
  return 0;
}

// ---------------------------------------------------------------------
void Menus::menu_triggered(QAction *a)
{
  event="button";
  eid=q2s(a->objectName());
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Menus::set(string p, string v)
{
  QString id,m,parm,t;
  QStringList sel,opt;

  sel=qsplit(p);
  if (sel.size()!=2) {
    error("invalid menu command: " + p + " " + v);
    return;
  }

  id=sel.at(0);
  m=sel.at(1);
  t=s2q(v);

  if (t.size()==0) {
    t=m;
    m="checked";
  }

  if (m=="checked" || m=="value") {
    QAction *a=items.value(id);
    a->setCheckable(true);
    a->setChecked(t=="1");
  } else if (m=="enable") {
    items.value(id)->setEnabled(t=="1");
  } else
    Child::set(p,v);
}
