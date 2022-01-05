
#include <QAction>
#include <QToolBar>
#include <QStyle>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "toolbar.h"
#include "wd.h"

// ---------------------------------------------------------------------
ToolBar::ToolBar(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="toolbar";

  QToolBar *w=new QToolBar;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"vertical")) return;
  w->setObjectName(qn);
  childStyle(opt);

  if (opt.contains("vertical"))
    w->setOrientation(Qt::Vertical);
  if (opt.size()) {
    QString t=opt.at(0);
    if (qshasonly(t,"0123456789x")) {
      QStringList sizes=t.split('x');
      if (sizes.size()<2) {
        error("invalid icon width, height: " + q2s(t));
        return;
      }
      w->setIconSize(QSize(c_strtoi(q2s(sizes.at(0))),c_strtoi(q2s(sizes.at(1)))));
    }
  }

  connect(w,SIGNAL(actionTriggered(QAction *)),
          this,SLOT(actionTriggered(QAction *)));
}

// ---------------------------------------------------------------------
void ToolBar::actionTriggered(QAction *a)
{
  event="button";
  eid=q2s(a->objectName());
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void ToolBar::makeact(QStringList opt)
{
  if (opt.size()<3) {
    error("toolbar add needs id, text, image: " + id);
    return;
  }
  QToolBar *w=(QToolBar *)widget;
  QString id=opt.at(0);
  QString text=opt.at(1);
  std::string iconFile=remquotes(q2s(opt.at(2)));
  QIcon image;
  int spi;
  if (iconFile.substr(0,8)=="qstyle::" && -1!=(spi=wdstandardicon(iconFile)))
    image=w->style()->standardIcon((QStyle::StandardPixmap)spi);
  else
    image=QIcon(s2q(iconFile));
  if (image.isNull()) {
    error("invalid icon image: " + q2s(opt.at(2)));
    return;
  }
  QAction *a=w->addAction(image,text);
  a->setObjectName(id);
  acts.append(a);
}

// ---------------------------------------------------------------------
void ToolBar::makeactright(QStringList opt)
{
  if (opt.size()<3) {
    error("toolbar addright needs id, text, image: " + id);
    return;
  }
  QToolBar *w=(QToolBar *)widget;
  QWidget *spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  w->addWidget(spacer);
  makeact(opt);
}

// ---------------------------------------------------------------------
std::string ToolBar::get(std::string p,std::string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void ToolBar::set(std::string p,std::string v)
{

  QToolBar *w=(QToolBar *)widget;
  QStringList opt=qsplit(v);
  if (p=="add")
    makeact(opt);
  else if (p=="addright")
    makeactright(opt);
  else if (p=="addsep")
    w->addSeparator();
  else if (p=="checkable")
    setbutton(p,opt);
  else if (p=="checked")
    setbutton(p,opt);
  else if (p=="enable") {
    if (opt.isEmpty()) Child::set(p,v);
    else if (1==opt.size() && (!opt.at(0).isEmpty()) && opt.at(0)[0].isNumber()) Child::set(p,v);
    else setbutton(p,opt);
  } else
    Child::set(p,v);
}

// ---------------------------------------------------------------------
QAction * ToolBar::getaction(QString id)
{
  for (int i=0; i<acts.size(); i++)
    if (acts.at(i)->objectName()==id) return acts.at(i);
  return 0;
}

// ---------------------------------------------------------------------
void ToolBar::setbutton(std::string p, QStringList opt)
{
  bool n=true;
  if (opt.isEmpty()) {
    error("set toolbar requires button_id: " + p);
    return;
  } else if (1<opt.size())
    n=!!c_strtoi(q2s(opt.at(1)));
  QString btnid= opt.at(0);
  QAction * a=getaction(btnid);
  if (!a) {
    error("set toolbar cannot find button_id: " + p + " " + q2s(btnid));
    return;
  }
  if (p=="checkable")
    a->setCheckable(n);
  else if (p=="checked")
    a->setChecked(n);
  else if (p=="enable")
    a->setEnabled(n);
  else {
    error("set toolbar attribute error: " + p);
    return;
  }
}

// ---------------------------------------------------------------------
std::string ToolBar::state()
{
  return "";
}
