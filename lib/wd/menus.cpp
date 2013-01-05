
#include <QMenu>
#include <QMenuBar>

#include "wd.h"
#include "menus.h"
#include "cmd.h"
#include "form.h"

// ---------------------------------------------------------------------
Menus::Menus(string s, string p, Form *f) : Child(s,p,f)
{
  type="menu";
  curMenu=0;
  id="";
  widget=(QWidget*) new QMenuBar;
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
  id=q2s(a->objectName());
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Menus::setenable(string p)
{
  int n=p.find_first_of(' ');
  QString s=s2q(p.substr(0,n));
  p=p.substr(n+1);
  if (items.contains(s))
    items.value(s)->setEnabled(0==p.compare("1"));
}

// ---------------------------------------------------------------------
// this sets checked state
void Menus::setp(string p, string v)
{
  QString s=s2q(p);
  if (!items.contains(s)) return;
  QAction *a=items.value(s);
  a->setCheckable(true);
  a->setChecked(0==v.compare("1"));
}

