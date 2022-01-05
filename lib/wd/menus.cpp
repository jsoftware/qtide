
#include <QMenu>
#include <QMenuBar>

#include "cmd.h"
#include "form.h"
#include "menus.h"
#include "pane.h"
#include "wd.h"

// ---------------------------------------------------------------------
Menus::Menus(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="menu";
  curMenu=0;
  id="";
  widget=(QWidget*) new QMenuBar(f);
  widget->setSizePolicy (QSizePolicy::Ignored, QSizePolicy::Maximum);
}

// ---------------------------------------------------------------------
QAction *Menus::makeact(std::string id, std::string p)
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
int Menus::menu(std::string c, std::string p)
{
  if (curMenu==0)
    return 1;
  QAction* r=makeact(c,p);
  curMenu->addAction(r);
  connect(r,SIGNAL(triggered()),
          this,SLOT(menu_triggered()));
  return 0;
}

// ---------------------------------------------------------------------
int Menus::menupop(std::string c)
{
  QString s=s2q(c);
  if (curMenu==0) {
    curMenu=((QMenuBar*) widget)->addMenu(s);
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
void Menus::menu_triggered()
{
  event="button";
  eid=q2s(QObject::sender()->objectName());
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
std::string Menus::get(std::string p,std::string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void Menus::set(std::string p,std::string v)
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
  } else if (m=="caption" || m=="text") {
    items.value(id)->setText(t);
  } else if (m=="show"||m=="visible") {
    items.value(id)->setVisible(t=="1");
  } else
    Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Menus::state()
{
  return "";
}

