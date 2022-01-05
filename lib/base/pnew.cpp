#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "base.h"
#include "pnew.h"
#include "dialog.h"
#include "note.h"
#include "proj.h"
#include "state.h"
#include "term.h"
#include "widget.h"

// using namespace std;

// ---------------------------------------------------------------------
Pnew::Pnew()
{
  setAttribute(Qt::WA_DeleteOnClose);
  Title="New Project";
  Path=cfpath(getprojectpath());
  QVBoxLayout *v=new QVBoxLayout;
  v->setContentsMargins(v->contentsMargins());
  v->setSpacing(0);
  v->addWidget(createfolderpanel());
  v->addWidget(createscriptspanel());
  v->addWidget(createotherpanel());
  v->addWidget(makehline());
  v->addWidget(createbuttonpanel());
  setLayout(v);
  setWindowTitle(Title);
#ifdef SMALL_SCREEN
  move(0,0);
  resize(term->width(),term->height());
#else
  resize(500,0);
#endif
  QMetaObject::connectSlotsByName(this);
}

// ---------------------------------------------------------------------
QWidget *Pnew::createbuttonpanel()
{
  QWidget *w=new QWidget();
  QHBoxLayout *h=new QHBoxLayout;
  QPushButton *create=makebutton("Create");
  create->setAutoDefault(true);
  h->addStretch(1);
  h->addWidget(create,0);
  w->setLayout(h);
  return w;
}

// ---------------------------------------------------------------------
QWidget *Pnew::createfolderpanel()
{
  QString s;
  QWidget *w=new QWidget();
  QHBoxLayout *h=new QHBoxLayout;
  h->addWidget(new QLabel("Folder:"),0);
  folder=new QLineEdit();
  s=termsep(tofoldername(Path));
  folder->setText(s);
  h->addWidget(folder,1);
  QPushButton *browse=makebutton("Browse");
  browse->setAutoDefault(false);
  browse->setText("");
  browse->setIcon(QIcon(":/images/dir.png"));
  h->addWidget(browse,0);
  w->setLayout(h);
  return w;
}

// ---------------------------------------------------------------------
QWidget *Pnew::createotherpanel()
{
  QWidget *w=new QWidget();
  QHBoxLayout *b=new QHBoxLayout;
  b->addWidget(new QLabel("Other Scripts:"));
  other=new QLineEdit();
  b->addWidget(other,1);
  w->setLayout(b);
  return w;
}

// ---------------------------------------------------------------------
QWidget *Pnew::createscriptspanel()
{
  QWidget *w=new QWidget();
  QHBoxLayout *b=new QHBoxLayout;
  b->addWidget(new QLabel("Create Scripts:"));
  cbuild=makecheckbox("build");
  crun=makecheckbox("run");
  cinit=makecheckbox("init");
  cbuild->setChecked(true);
  crun->setChecked(true);
  cinit->setChecked(true);
  b->addWidget(cbuild);
  b->addWidget(crun);
  b->addWidget(cinit);
  b->addStretch(1);
  w->setLayout(b);
  return w;
}

// ---------------------------------------------------------------------
void Pnew::on_browse_clicked()
{
  QString s=dialogdirectory(this,Title,Path);
  if (s.size())
    folder->setText(tofoldername(s)+"/");
}

// ---------------------------------------------------------------------
void Pnew::on_create_clicked()
{
  int i;
  QString m,pf,s,t;

  s=remsep(cpath(folder->text()));
  if (s.isEmpty()) return;

  Dir=new QDir();
  Dir->setPath(s);
  if (!Dir->exists()) {
    if (!Dir->mkpath(Dir->path())) {
      info(Title,"directory could not be created:\n\n"+s);
      return;
    }
  }

  t=cfsname(s) + config.ProjExt;
  pf=Dir->filePath(t);
  if (cfexist(pf)) {
    info(Title,"Project already exists:\n\n"+ pf);
    return;
  }

  QStringList p;
  p.append(t);

  if (cbuild->isChecked())
    p.append("build");

  if (crun->isChecked())
    p.append("run");

  if (cinit->isChecked())
    p.append("init");

  p+=other->text().split(" ",_SkipEmptyParts);
  for (i=0; i<p.size(); i++)
    p.replace(i,defext(p.at(i)));
  p.removeDuplicates();
  foreach (QString m,p)
    cfcreate(Dir->filePath(m));

  t=config.DefCCmt;
  m=t+" project:\n"+t+"\n"+t+" defines list of source files.\n";
  m+=t+" path defaults to project directory.\n\n";

  p.removeFirst();
  p.removeAll(defext("build"));
  p.removeAll(defext("run"));
  cfwrite(pf,m+p.join("\n"));

  QString id=tofoldername(s);
  if ('~'==id.at(0))
    id=id.mid(1);

  if (note == 0)
    term->vieweditor();
  else
    note->projectsave();
  project.open(id);
  note->projectopen(true);

  accept();
}

// ---------------------------------------------------------------------
bool Pnew::run()
{
  return QDialog::Accepted==exec();
}
