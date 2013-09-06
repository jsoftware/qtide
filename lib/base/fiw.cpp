#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegExp>

#include "base.h"
#include "widget.h"

#include "fiw.h"
#include "recent.h"
#include "bedit.h"
#include "note.h"
#include "tedit.h"
#include "term.h"
#include "state.h"

using namespace std;

QStringList Fiw::ReplaceList;
QStringList Fiw::SearchList;

// ---------------------------------------------------------------------
Fiw::Fiw(int p, QString s)
{
  Assign=false;
  ifReplace=false;
  Inc=0;

  Matchcase=true;
  Max=15;
  Name=false;
  Parent=p;
  readwin();
  setsearchlist(s);

  QVBoxLayout *v=new QVBoxLayout();
  v->setSpacing(12);

  QFormLayout *f = new QFormLayout;
  lsearchfor = new QLabel("&Search for:");
  searchfor = makecombobox("searchfor");
  lsearchfor->setBuddy(searchfor);
  lreplaceby = new QLabel("&Replace by:");
  replaceby = makecombobox("replaceby");
  lreplaceby->setBuddy(replaceby);
  f->setVerticalSpacing(2);
  f->addRow(lsearchfor,searchfor);
  f->addRow(lreplaceby,replaceby);
  v->addLayout(f,0);

  QHBoxLayout *h=new QHBoxLayout();
  QVBoxLayout *c=new QVBoxLayout();
  c->setContentsMargins(0,0,0,0);
  c->setSpacing(0);
  matchcase=makecheckbox("&Match case");
  assigned=makecheckbox("&Assigned");
  nameonly=makecheckbox("&Name only");
  c->addWidget(matchcase,0);
  c->addWidget(assigned,0);
  c->addWidget(nameonly,0);
  c->addStretch(1);
  h->addLayout(c,0);
  h->addStretch(1);

  QVBoxLayout *d=new QVBoxLayout();
  QGridLayout *g=new QGridLayout();
  g->setContentsMargins(0,0,0,0);
  g->setHorizontalSpacing(4);
  g->setVerticalSpacing(2);
  d->addLayout(g,0);
  d->addStretch(1);
  h->addLayout(d,0);

  findback=makebutton("Find &Back");
  findnext=makebutton("Find &Next");
  findtop=makebutton("&Find Top");
  replace=makebutton("Re&place");
  replaceforward=makebutton("Replace Forward");
  replaceforward->setText("&Replace");
  undolast=makebutton("&Undo Last");

  g->addWidget(findback,0,0);
  g->addWidget(findtop,0,1);
  g->addWidget(findnext,0,2);
  g->addWidget(undolast,1,0);
  g->addWidget(replace,1,1);
  g->addWidget(replaceforward,1,2);

  write();
  findtop->setFocus();
  searchfor->setFocus();
  if (s.size())
    searchfor->lineEdit()->selectAll();

  lreplaceby->hide();
  replaceby->hide();
  undolast->hide();
  replace->hide();
  v->addLayout(h,0);
  v->addStretch(1);
  setLayout(v);
  setWindowTitle("Find");
  resize(450,0);
  QMetaObject::connectSlotsByName(this);
  show();
}

// ---------------------------------------------------------------------
void Fiw::finfo(QString s)
{
#ifdef Q_OS_ANDROID
  int t=idewin;
  idewin=-1;
#endif
  info("Find",s);
#ifdef Q_OS_ANDROID
  activateWindow();
  raise();
  repaint();
  idewin=t;
#endif
}

// ---------------------------------------------------------------------
void Fiw::keyPressEvent(QKeyEvent *e)
{
  Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();
  bool ctrl = mod.testFlag(Qt::ControlModifier);
  if (ctrl && e->key()==Qt::Key_R && !ifReplace)
    on_replaceforward_clicked();
  else
    QDialog::keyPressEvent(e);
}

// ---------------------------------------------------------------------
void Fiw::on_assigned_clicked()
{
  Assign=assigned->isChecked();
  if (Assign) {
    Name=false;
    writebuttons();
  }
}

// ---------------------------------------------------------------------
void Fiw::on_findback_clicked()
{
  read();
  search(-1);
}

// ---------------------------------------------------------------------
void Fiw::on_findnext_clicked()
{
  read();
  search(1);
}

// ---------------------------------------------------------------------
void Fiw::on_findtop_clicked()
{
  read();
  search(0);
}

// ---------------------------------------------------------------------
void Fiw::on_nameonly_clicked()
{
  Name=nameonly->isChecked();
  if (Name) {
    Assign=false;
    writebuttons();
  }
}

// ---------------------------------------------------------------------
void Fiw::on_replace_clicked()
{
  read();
  search_replace(0);
}

// ---------------------------------------------------------------------
void Fiw::on_replaceforward_clicked()
{
  read();
  if (ifReplace)
    search_replace(1);
  else
    open_replace();
}

// ---------------------------------------------------------------------
void Fiw::on_searchfor_activated()
{
  on_findnext_clicked();
}

// ---------------------------------------------------------------------
void Fiw::on_undolast_clicked()
{
  read();
  Text=LastText;
  Win->setPlainText(Text);
  TextPos=LastPos;
  LastPos=0;
  LastText="";
  showit();
}

// ---------------------------------------------------------------------
void Fiw::open_replace()
{
  lreplaceby->show();
  replaceby->show();
  undolast->show();
  replace->show();
  replaceby->setFocus();
  replaceforward->setText("Replace For&ward");
  ifReplace=1;
}

// ---------------------------------------------------------------------
void Fiw::read()
{
  Assign=assigned->isChecked();
  Matchcase=matchcase->isChecked();
  Name=nameonly->isChecked();
  Search=searchfor->currentText();
  Replace=replaceby->currentText();
  refresh();
}

// ---------------------------------------------------------------------
void Fiw::readtext()
{
  readwin();
  Text=Win->toPlainText();
  TextPos=Win->textCursor().position();
}

// ---------------------------------------------------------------------
void Fiw::readwin()
{
  if (Parent==0)
    Win=tedit;
  else
    Win=(Bedit *)note->editPage();
}

// ---------------------------------------------------------------------
void Fiw::refresh()
{
  setsearchlist(Search);
  setreplacelist(Replace);
  write();
}

// ---------------------------------------------------------------------
void Fiw::reject()
{
  config.winpos_save(this,"Fiw");
  QDialog::reject();
}

// ---------------------------------------------------------------------
void Fiw::search(int d)
{
  int hit;
  readtext();
  hit=search1(d);

  if (hit==-1)
    finfo("not found: " + Search);
  else {
    TextPos=hit;
    Inc=1;
    showhit();
  }
  show();
}

// ---------------------------------------------------------------------
int Fiw::search1(int d)
{

  int p,r;
  QString f,txt,s;

  s = Search;
  txt=Text;

  if (!Matchcase) {
    s.toLower();
    txt.toLower();
  }

  if (d<0) {
    return searchback(s,txt.mid(0,TextPos));
  }

  if (d==0)
    TextPos = Inc = 0;

  p=TextPos + Inc;
  r=searchforward(s,txt.mid(p));

  return r + ((r<0) ? 0 : p);
}

// ---------------------------------------------------------------------
int Fiw::searchback(QString s, QString txt)
{

  QRegExp r;
  if (Assign) {
    r.setPattern(config.Rxnna+s+rxassign(config.DefExt,false));
    return r.lastIndexIn(txt);
  }

  if (Name) {
    r.setPattern(config.Rxnna+s+config.Rxnnz);
    return r.lastIndexIn(txt);
  }

  return txt.lastIndexOf(s);
}

// ---------------------------------------------------------------------
int Fiw::searchforward(QString s, QString txt)
{
  QRegExp r;

  if (Assign) {
    r.setPattern(config.Rxnna+s+rxassign(config.DefExt,false));
    return r.indexIn(txt);
  }

  if (Name) {
    r.setPattern(config.Rxnna+s+config.Rxnnz);
    return r.indexIn(txt);
  }

  return txt.indexOf(s);
}

// ---------------------------------------------------------------------
void Fiw::search_replace(int d)
{
  int count, hit, pad;
  readtext();
  hit=search1(1);
  Inc=0;
  LastPos=0;
  LastText="";
  QString s;

  hit=search1(1);
  if (hit==-1) {
    finfo("not found: " + Search);
    showit();
    return;
  }

  TextPos=LastPos=hit;
  LastText=Text;
  Inc=1;
  Text=Text.mid(0,TextPos) + Replace + Text.mid(TextPos+Search.size());

  if (d==0) {
    Win->setPlainText(Text);
    if (0 <= (hit=search1(1))) {
      TextPos=hit;
      showhit();
    }
  } else {
    count=1;
    pad=Replace.size()-1;
    TextPos+=pad;
    while (0 <= (hit=search1(1))) {
      Text=Text.mid(0,hit) + Replace + Text.mid(hit+Search.size());
      TextPos=hit+pad;
      count++;
    }

    int pos=Win->readcurpos();
    int top=Win->readtop();
    Win->setPlainText(Text);
    Win->settop(top);
    Win->setcurpos(pos);

    s=QString::number(count) + " replacement";
    if(count>1) s+='s';
    s+=" made";
    finfo(s);
  }
  showit();

}

// ---------------------------------------------------------------------
QStringList Fiw::setlist(QString s, QStringList t)
{
  if (s.isEmpty()) return t;
  t.prepend(s);
  t.removeDuplicates();
  return t.mid(0,Max);
}

// ---------------------------------------------------------------------
void Fiw::setsearchlist(QString s)
{
  SearchList=setlist(s,SearchList);
}

// ---------------------------------------------------------------------
void Fiw::setreplacelist(QString s)
{
  ReplaceList=setlist(s,ReplaceList);
}

// ---------------------------------------------------------------------
void Fiw::showhit()
{
  Win->setselect(TextPos,Search.size());
}

// ---------------------------------------------------------------------
void Fiw::showit()
{
}

// ---------------------------------------------------------------------
void Fiw::write()
{
  writebuttons();

  searchfor->clear();
  searchfor->addItems(SearchList);
  if (SearchList.size())
    searchfor->setCurrentIndex(0);

  replaceby->clear();
  replaceby->addItems(ReplaceList);
  if (ReplaceList.size())
    replaceby->setCurrentIndex(0);

}

// ---------------------------------------------------------------------
void Fiw::writebuttons()
{
  matchcase->setChecked(Matchcase);
  assigned->setChecked(Assign);
  nameonly->setChecked(Name);
}
