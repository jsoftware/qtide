
#include <QAction>
#include <QToolBar>

#include "wd.h"
#include "toolbar.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
ToolBar::ToolBar(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="ToolBar";

  QToolBar *w=new QToolBar;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  w->setObjectName(qn);

  if (opt.size()) {
    QString t=opt.at(0);
    if (qshasonly(t,"0123456789x")) {
      QStringList sizes=t.split('x');
      if (sizes.size()<2) {
        info("Toolbar","invalid icon width, height: "+t);
        return;
      }
      qDebug() << "setting icon size" << sizes;
      w->setIconSize(QSize(sizes.at(0).toInt(),sizes.at(1).toInt()));
    }
  }

  connect(w,SIGNAL(actionTriggered(QAction *)),
          this,SLOT(actionTriggered(QAction *)));
}

// ---------------------------------------------------------------------
void ToolBar::actionTriggered(QAction *a)
{
  event="button";
  id=q2s(a->objectName());
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
//QAction *ToolBar::makeact(QStringList opt)
void ToolBar::makeact(QStringList opt)
{
  if (opt.size()<3) {
    info("Toolbar","toolbar add needs id, text, image");
    return;
  }
  QToolBar *w=(QToolBar *)widget;
  QString id=opt.at(0);
  QString text=opt.at(1);
  QIcon image=QIcon(opt.at(2));
  if (image.isNull()) {
    info("Toolbar","invalid icon image: " + opt.at(2));
    return;
  }
  QAction *a=w->addAction(image,text);
  a->setObjectName(id);
}

// ---------------------------------------------------------------------
void ToolBar::setp(string p,string v)
{

  QToolBar *w=(QToolBar *)widget;
  QStringList opt=qsplit(v);
  if (p=="add")
    makeact(opt);
  else if (p=="addsep")
    w->addSeparator();
  else
    Child::setp(p,v);
}

