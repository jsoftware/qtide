#include <QAction>
#include <QBoxLayout>
#include <QFileSystemModel>
#include <QListView>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QModelIndex>
#include <QToolBar>
#include <QWidget>

#include "base.h"
#include "nside.h"
#include "nedit.h"
#include "note.h"
#include "proj.h"
#include "state.h"
#include "recent.h"

// using namespace std;

Nicon *nicon=0;

// ---------------------------------------------------------------------
QIcon Nicon::icon(const QFileInfo &info) const
{
  QString s;
  if (info.isDir())
    s=":/images/folder.png";
  else
    s=":/images/regular.png";
  QIcon f(config.SystemPath.filePath(s));
  return f;
}

// ---------------------------------------------------------------------
Nside::Nside()
{
  if (nicon==0)
    nicon=new Nicon();

  Path=project.Path;
  if (Path.isEmpty())
    Path=config.TempPath.absolutePath();

  QWidget *f = createfiles();
  addTab(f,"files");

  QWidget *s=createsource();
  addTab(s,"source");

  QWidget *d=createdefs();
  addTab(d,"defs");

  adjustSize();

  QTabBar *tb=tabBar();
  width= 1 + tb->tabRect(0).width() + tb->tabRect(1).width() + tb->tabRect(2).width();

  connect(this, SIGNAL(currentChanged(int)),
          this, SLOT(currentChanged(int)));

  QMetaObject::connectSlotsByName(this);

}

// ---------------------------------------------------------------------
QWidget *Nside::createdefs()
{
  QWidget *r = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(layout->contentsMargins());
  layout->setSpacing(0);
  QToolBar *t= Nside::createdefsTB();
  dlw = new QListWidget();
  connect(dlw,SIGNAL(itemActivated(QListWidgetItem*)),
          this,SLOT(defs_activated(QListWidgetItem*)));
  layout->addWidget(t,0,Qt::Alignment());
  layout->addWidget(dlw,1,Qt::Alignment());
  r->setLayout(layout);
  return r;
}

// ---------------------------------------------------------------------
QToolBar *Nside::createdefsTB()
{
  QToolBar *t=  new QToolBar(0);
  t->setObjectName("defsTB");
  t->setIconSize(QSize(18,18));
  refreshdefsAct = makeact("refresh.png","Refresh");
  t->addAction(refreshdefsAct);
  return t;
}

// ---------------------------------------------------------------------
QWidget *Nside::createfiles()
{
  QWidget *r = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(layout->contentsMargins());
  layout->setSpacing(0);
  QToolBar *t= Nside::createfileTB();
  path = new QLineEdit();
  flv= new QListView();

  fm = new QFileSystemModel;
  fm->setIconProvider(nicon);
  fm->setFilter(QDir::AllEntries|QDir::NoDot);
  flv->setModel(fm);
  file_refresh();

  connect(path,SIGNAL(returnPressed()),
          this,SLOT(path_returnPressed()));
  connect(flv,SIGNAL(activated(const QModelIndex &)),
          this,SLOT(file_activated(const QModelIndex &)));
  layout->addWidget(t,0,Qt::Alignment());
  layout->addWidget(path,0,Qt::Alignment());
  layout->addWidget(flv,1,Qt::Alignment());
  r->setLayout(layout);
  return r;
}

// ---------------------------------------------------------------------
QToolBar *Nside::createfileTB()
{
  QToolBar *t=  new QToolBar(0);
  t->setObjectName("fileTB");
  t->setIconSize(QSize(16,16));

  refreshAct = makeact("view-refresh.png", "Refresh");
  homeAct = makeact("home.png", "Home");
  setpathAct = makeact("rotate-right.png", "Set path from document");

  t->addAction(refreshAct);
  t->addAction(homeAct);
  t->addAction(setpathAct);
  return t;
}

// ---------------------------------------------------------------------
QWidget *Nside::createsource()
{
  QWidget *r = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(layout->contentsMargins());
  layout->setSpacing(0);
  QToolBar *t= Nside::createsourceTB();
  slw = new QListWidget();
  connect(slw,SIGNAL(itemActivated(QListWidgetItem*)),
          this,SLOT(source_activated(QListWidgetItem*)));
  layout->addWidget(t,0,Qt::Alignment());
  layout->addWidget(slw,1,Qt::Alignment());
  r->setLayout(layout);
  return r;
}

// ---------------------------------------------------------------------
QToolBar *Nside::createsourceTB()
{
  QToolBar *t=  new QToolBar(0);
  t->setObjectName("sourceTB");
  t->setIconSize(QSize(18,18));
  refreshsourceAct = makeact("refresh.png", "Refresh");
  t->addAction(refreshsourceAct);
  return t;
}

// ---------------------------------------------------------------------
void Nside::currentChanged(int index)
{
  refresh1(index);
}

// ---------------------------------------------------------------------
void Nside::defs_activated(QListWidgetItem *item)
{
  int pos,row;
  QString name,t,txt;

  t=item->text();
  name=qstaketo(t," ");
  row=dlw->currentRow();

  if (name==t || row!=drow)
    dpos=0;

  drow=row;
  txt=note->editPage()->toPlainText();

  pos=defs_doselect(name,dpos,txt);
  if (pos==-1) {
    dpos=0;
    info("Defs","Not found: " + name);
    return;
  }
  dpos=pos+name.size();
  note->editPage()->setselect(pos,name.size());
}

// ---------------------------------------------------------------------
int Nside::defs_doselect(QString name,int pos,QString txt)
{
  QRegExp rx;
  rx.setPattern(config.Rxnna+name+rxassign(dext,true));
  return rx.indexIn(txt,pos);
}

// ---------------------------------------------------------------------
void Nside::defs_refresh()
{
  if (!note->editPage()) return;
  QString s=note->editPage()->toPlainText();
  dext=cfext(note->editPage()->fname);
  QStringList n=globalassigns(s,dext);
  if (n==dsl) return;
  dlw->clear();
  dlw->addItems(n);
  dsl=n;
  drow=-1;
  dpos=0;
}

// ---------------------------------------------------------------------
void Nside::file_activated(const QModelIndex &n)
{
  QString f, p;
  p=fm->filePath(n);

  if (!fm->isDir(n)) {
    note->fileopen(p);
    return;
  }
  f=fm->fileName(n);
  if (f=="..")
    p=cfpath(fm->rootPath());
  Path=p;
  file_refresh();
}

// ---------------------------------------------------------------------
void Nside::file_refresh()
{
  fm->setRootPath(Path);
  flv->setRootIndex(fm->index(Path));
  path->setText(remtilde(tofoldername(Path)));
}


// ---------------------------------------------------------------------
void Nside::path_returnPressed()
{
  QString p=path->text();
  Path=cpath(p);
  file_refresh();
}

// ---------------------------------------------------------------------
void Nside::on_defsTB_actionTriggered()
{
  defs_refresh();
}

// ---------------------------------------------------------------------
void Nside::on_fileTB_actionTriggered(QAction *action)
{
  if (action==homeAct)
    Path=cpath("~home");
  else if (action==setpathAct)
    if (note->editIndex()>=0)
      Path=cfpath(note->editPage()->fname);
  file_refresh();
}

// ---------------------------------------------------------------------
void Nside::on_sourceTB_actionTriggered()
{
  source_refresh();
}

// ---------------------------------------------------------------------
QAction *Nside::makeact(QString icon, QString text)
{
  QString s=":/images/" + icon;
  QAction *r = new QAction(QIcon(s),text,this);
  return r;
}

// ---------------------------------------------------------------------
void Nside::refresh()
{
  refresh1(currentIndex());
}

// ---------------------------------------------------------------------
void Nside::refresh1(int index)
{
  if (NoEvents) return;

  switch (index) {
  case 0 :
    file_refresh(); /* fall-thru */
  case 1 :
    source_refresh();
    break;
  case 2 :
    defs_refresh();
    break;
  }
}

// ---------------------------------------------------------------------
void Nside::source_activated(QListWidgetItem* item)
{
  note->fileopen(project.fullname(item->text()));
}

// ---------------------------------------------------------------------
void Nside::source_refresh()
{
  QStringList n=project.source();
  if (n==ssl) return;
  slw->clear();
  slw->addItems(n);
  ssl=n;
}
