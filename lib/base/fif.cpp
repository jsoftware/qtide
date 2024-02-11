#include <QBoxLayout>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

#include "base.h"
#include "eview.h"
#include "fif.h"
#include "note.h"
#include "pcombobox.h"
#include "proj.h"
#include "recent.h"
#include "rsel.h"
#include "state.h"
#include "term.h"
#include "widget.h"

static int MaxFif=100; // max search length

Fif *fif;

// using namespace std;

// ---------------------------------------------------------------------
Fif::Fif(QString s, bool ifname)
{
  setAttribute(Qt::WA_DeleteOnClose);
  Title="Find in Files";

  int layoutcmv,checkboxcmv,checkboxspacing;
#ifdef __APPLE__
  layoutcmv=5;
  checkboxcmv=2;
  checkboxspacing=4;
#else
  layoutcmv=2;
  checkboxcmv=0;
  checkboxspacing=0;
#endif

  QVBoxLayout *a=new QVBoxLayout();
  a->setContentsMargins(0,0,0,0);

  QVBoxLayout *v=new QVBoxLayout();
  v->setContentsMargins(11,11,11,layoutcmv);
  v->setSpacing(12);

  QFormLayout *f = new QFormLayout;
  f->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
  QLabel *lsearchfor = new QLabel("&Search for:");
  searchfor = makecombobox("searchfor");
  lsearchfor->setBuddy(searchfor);
  QLabel *linfolder = new QLabel("In &folder:");
  infolder = makecombobox("infolder");
  linfolder->setBuddy(infolder);
  QLabel *lfiletypes = new QLabel("File &types:");
  filetypes = makecombobox("filetypes");
  lfiletypes->setBuddy(filetypes);

  f->setContentsMargins(0,0,0,0);
  f->setVerticalSpacing(2);
  f->addRow(lsearchfor,searchfor);
  f->addRow(linfolder,infolder);
  f->addRow(lfiletypes,filetypes);
  v->addLayout(f,0);

  QHBoxLayout *h=new QHBoxLayout();

  QVBoxLayout *c=new QVBoxLayout();
  c->setContentsMargins(0,0,0,checkboxcmv);
  c->setSpacing(checkboxspacing);
  matchcase=makecheckbox("&Match case");
  subdir=makecheckbox("Search su&bfolders","subdir");
  fileonly=makecheckbox("Fil&enames only","fileonly");
  c->addWidget(matchcase,0);
  c->addWidget(subdir,0);
  c->addWidget(fileonly,0);
  c->addStretch(1);
  h->addLayout(c,0);
  h->addStretch(1);

  QVBoxLayout *d=new QVBoxLayout();
  d->setContentsMargins(0,0,0,checkboxcmv);
  d->setSpacing(checkboxspacing);
  assigned=makecheckbox("&Assigned");
  nameonly=makecheckbox("&Name only");
  regex=makecheckbox("&Regex");
  d->addWidget(assigned,0);
  d->addWidget(nameonly,0);
  d->addWidget(regex,0);
  d->addStretch(1);
  h->addLayout(d,0);
  h->addStretch(1);

  QVBoxLayout *e=new QVBoxLayout();
  e->setContentsMargins(0,0,0,0);
  e->setSpacing(0);
  find=makebutton("&Find");
  e->addWidget(find,0);
#ifndef Q_OS_ANDROID
  e->addStretch(1);
#else
  view=makebutton("View");
  e->addWidget(view,0);
  e->addStretch(1);
  cancel=makebutton("Cancel");
  e->addWidget(cancel,0);
#endif
  h->addLayout(e,0);
  searchfor->setFocus();
  v->addLayout(h,0);

  QVBoxLayout *g=new QVBoxLayout();
  found=new QListWidget();
  found->setObjectName("found");
  found->setAlternatingRowColors(true);
  found->hide();
  g->addWidget(found);

  a->addLayout(v,0);
  a->addLayout(g,1);
  setLayout(a);
  setWindowTitle(Title);

  Pos=config.winpos_read("Fif");
#ifdef SMALL_SCREEN
  move(0,0);
  resize(term->width(),term->height());
#else
  move(Pos[0],Pos[1]);
  resize(Pos[2],0);
#endif

  QMetaObject::connectSlotsByName(this);

  initshow(s,ifname);
}

// ---------------------------------------------------------------------
void Fif::initshow(QString s, bool ifname)
{
  init(s,ifname);
  write();
  if (s.size())
    searchfor->lineEdit()->selectAll();
  else
    searchfor->setCurrentIndex(-1);

  show();
  activateWindow();
  raise();
}

// ---------------------------------------------------------------------
void Fif::finfo(QString txt)
{
  info(Title,txt);
}

// ---------------------------------------------------------------------
void Fif::init(QString s, bool ifname)
{
  Assign=false;
  Fileonly=false;
  Matchcase=true;
  Name=ifname;
  Max=15;
  Regex=false;
  SearchList=recent.recentFif;
  Subdir=true;
  Types=config.FifExt;
  Type=Types.at(0);

  ifResults=false;

  setpathlist(project.Id);
  setsearchlist(s);
}

// ---------------------------------------------------------------------
void Fif::on_assigned_clicked()
{
  Assign=assigned->isChecked();
  if (Assign) {
    Name=false;
    writebuttons();
  }
  research();
}

// ---------------------------------------------------------------------
void Fif::on_fileonly_clicked()
{
  research();
}

// ---------------------------------------------------------------------
void Fif::on_find_clicked()
{
  read();
  search();
}

#ifdef Q_OS_ANDROID
// ---------------------------------------------------------------------
void Fif::on_view_clicked()
{
  on_found_itemActivated(found->currentItem());
}

// ---------------------------------------------------------------------
void Fif::on_cancel_clicked()
{
  reject();
  term->vieweditor();
  fif=0;
}
#endif

// ---------------------------------------------------------------------
void Fif::on_found_itemActivated(QListWidgetItem * item)
{
  int n,r,p=-1;
  QString f,s;
  s=item->text();
  n=s.indexOf(":");
  f=cpath(Path)+"/"+s.left(n);
  if (!Fileonly) {
    r=s.indexOf(":",n+1);
// r in origin 1, p set in origin 0
    p=s.mid(n+1,r-n-1).toInt() - 1 ;
  }
  term->vieweditor();
  note->fileopen(f,p);
}

// ---------------------------------------------------------------------
void Fif::on_matchcase_clicked()
{
  research();
}

// ---------------------------------------------------------------------
void Fif::on_nameonly_clicked()
{
  Name=nameonly->isChecked();
  if (Name) {
    Assign=false;
    writebuttons();
  }
  research();
}

// ---------------------------------------------------------------------
void Fif::on_regex_clicked()
{
  Regex=regex->isChecked();
  if (Regex) {
    Assign=false;
    Name=false;
    writebuttons();
  }
  setenable();
  research();
}

// ---------------------------------------------------------------------
void Fif::on_subdir_clicked()
{
  research();
}

// ---------------------------------------------------------------------
void Fif::read()
{
  Search=searchfor->currentText();
  Path=infolder->currentText();
  Type=filetypes->currentText();
  Assign=assigned->isChecked();
  Fileonly=fileonly->isChecked();
  Name=nameonly->isChecked();
  Matchcase=matchcase->isChecked();
  Regex=regex->isChecked();
  Subdir=subdir->isChecked();
  refresh();
}

// ---------------------------------------------------------------------
void Fif::refresh()
{
  setpathlist(Path);
  setsearchlist(Search);
  settypelist(Type);
  write();
}

// ---------------------------------------------------------------------
void Fif::reject()
{
  setsearchmaxlength();
  recent.recentFif=SearchList;
  recent.save_recent();

  Pos[0]=pos().rx();
  Pos[1]=pos().ry();
  Pos[2]=size().width();
  if (ifResults)
    Pos[3]=size().height();
  config.winpos_save1(Pos,"Fif","");
  QDialog::reject();
}

// ---------------------------------------------------------------------
void Fif::keyReleaseEvent(QKeyEvent *event)
{
#ifdef Q_OS_ANDROID
  if (event->key()==Qt::Key_Back) {
//    reject();
    hide();
  } else QDialog::keyReleaseEvent(event);
#else
  QDialog::keyReleaseEvent(event);
#endif
}

// ---------------------------------------------------------------------
void Fif::research()
{
  if (NoEvents) return;
  read();
  if (ifResults && Search.size()>0) search();
}

// ---------------------------------------------------------------------
void Fif::setenable()
{
  assigned->setEnabled(!Regex);
  nameonly->setEnabled(!Regex);
}

// ---------------------------------------------------------------------
void Fif::setpathlist(QString p)
{
  QStringList r;
  if (p.isEmpty())
    p=(Path.isEmpty()) ? project.Folder : Path;
  while (p.size()) {
    r.append(p);
    p=cfpath(p);
  }
  r=r+config.UserFolderKeys;
  r.removeDuplicates();
  PathList=r;
}

// ---------------------------------------------------------------------
void Fif::setsearchlist(QString s)
{
  if (s.isEmpty()) return;
  SearchList.prepend(s);
  SearchList.removeDuplicates();
  SearchList=SearchList.mid(0,Max);
}

// ---------------------------------------------------------------------
void Fif::setsearchmaxlength()
{
  QString s;
  for (int i=0; i<SearchList.size(); i++) {
    s=SearchList.at(i);
    if (s.length()>MaxFif)
      SearchList.replace(i,s.left(MaxFif));
  }
}

// ---------------------------------------------------------------------
void Fif::settypelist(QString t)
{
  if (t.isEmpty()) return;
  Types.prepend(t);
  Types.removeDuplicates();
}

// ---------------------------------------------------------------------
void Fif::write()
{
  writebuttons();
  setenable();
  searchfor->clear();
  searchfor->addItems(SearchList);
  if (SearchList.size())
    searchfor->setCurrentIndex(0);

  infolder->clear();
  infolder->addItems(PathList);
  if (PathList.size())
    infolder->setCurrentIndex(0);

  filetypes->clear();
  filetypes->addItems(Types);
  filetypes->setCurrentIndex(0);
}

// ---------------------------------------------------------------------
void Fif::writebuttons()
{
  assigned->setChecked(Assign);
  fileonly->setChecked(Fileonly);
  nameonly->setChecked(Name);
  matchcase->setChecked(Matchcase);
  regex->setChecked(Regex);
  subdir->setChecked(Subdir);
}

