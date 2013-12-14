#include <QBoxLayout>
#include <QDate>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>

#include "base.h"
#include "spic.h"
#include "comp.h"
#include "note.h"
#include "snap.h"
#include "state.h"
#include "recent.h"
#include "view.h"
#include "widget.h"

using namespace std;

bool pic_inidir(QString s);
QByteArray pp_stamp();

// ---------------------------------------------------------------------
Picm::Picm()
{
  QString f,s;
  f=note->editFile();
  s=note->editText();

  Path=snapgetpath(cfpath(f)) + "/p" + ss_date();
  File=cfsname(f);
  Text=s;

  Title="File Snapshots";
  QVBoxLayout *layout=new QVBoxLayout;
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);
  layout->addWidget(createpanel());
  layout->addWidget(makehline());
  layout->addWidget(createview(),1);
  QWidget *r=new QWidget();
  layout->addWidget(r);
  setWindowTitle(Title);
  setLayout(layout);
  setxywh(this,"Picm");
  QMetaObject::connectSlotsByName(this);
  init();
  show();
}

// ---------------------------------------------------------------------
void Picm::closeit()
{
  foreach (QString s,Temps)
  cfdelete(s);
  config.winpos_save(this,"Picm");
  close();
}

// ---------------------------------------------------------------------
QWidget *Picm::createpanel()
{
  QWidget *w=new QWidget();
  QHBoxLayout *h=new QHBoxLayout();
  h->setSpacing(2);
  QFormLayout *f = new QFormLayout;
  QLabel *lfile = new QLabel("File:");
  sfile = makecombobox("sfile");
  sfile->setMinimumSize(175,0);
  f->addRow(lfile,sfile);
  h->addLayout(f,0);
  h->addStretch(1);
  bview=makebutton("View");
  bxdiff=makebutton("External Diff");
  brestore=makebutton("Restore");
  h->addWidget(bview,0);
  h->addWidget(bxdiff,0);
  h->addWidget(brestore,0);
  w->setLayout(h);
  return w;
}

// ---------------------------------------------------------------------
QWidget *Picm::createview()
{
  QWidget *w=new QWidget();
  QVBoxLayout *v=new QVBoxLayout();
  v->setContentsMargins(0,0,0,0);
  QSplitter *s=new QSplitter();
  times = new QListWidget();
  times->setFont(config.Font);
  times->setObjectName("times");
  tview = new Eview();
  tview->ensureCursorVisible();
  tview->setLineWrapMode(QPlainTextEdit::NoWrap);
  tview->setFont(config.Font);
  s->addWidget(times);
  s->addWidget(tview);
  QList<int> n;
  n << 110 << 200;
  s->setSizes(n);
  s->setStretchFactor(0,0);
  s->setStretchFactor(1,1);
  v->addWidget(s);
  w->setLayout(v);
  return w;
}

// ---------------------------------------------------------------------
void Picm::init()
{
  int i;
  QList<QByteArray> t;
  QString m,n;
  QStringList s,f;

  f=pic_files();
  sfile->addItems(f);
  sfile->setCurrentIndex(f.indexOf(File));

  t=cfreadbin(Path + "/" + File).split(char(255));
  t.removeLast();
  if (t.isEmpty())
    t.append((Text+"000000").toUtf8());
  for (i=0; i<t.size(); i++)
    s.append(QString(t.at(i)));
  Stamps.clear();
  Texts.clear();
  for (i=s.size()-1; i>=0; i--) {
    m=s.at(i);
    Texts.append(m.left(m.size()-6));
    Stamps.append(unstamp(m.right(6)));
  }
  Stamps.replace(s.size()-1,"start   ");

  times->clear();
  times->addItems(Stamps);
  times->setCurrentRow(0);
  tcompare(0);
}

// ---------------------------------------------------------------------
void Picm::on_externaldiff_clicked()
{
  note->savecurrent();
  QString s=newtempscript();
  cfwrite(s,Texts.at(times->currentRow()));
  Temps.append(s);
  xdiff(s,note->editFile());
}

// ---------------------------------------------------------------------
void Picm::on_restore_clicked()
{
  int n=times->currentRow();
  QString m="OK to restore file snapshot: ";
  m+=Stamps.at(n).trimmed() + "?";
  if(!queryNY(Title,m)) return;
  note->settext(Texts.at(n));
  closeit();
}

// ---------------------------------------------------------------------
void Picm::on_times_currentRowChanged(int n)
{
  tcompare(n);
}

// ---------------------------------------------------------------------
void Picm::on_view_clicked()
{
  textview(Texts.at(times->currentRow()));
}

// ---------------------------------------------------------------------
void Picm::tcompare(int n)
{
  QString s,t;
  t=Texts.at(n);
  s="comparing:\n";
  s+=File + "  " + Stamps.at(n) + "  " + QString::number(t.size()) + "\n";
  s+=File + "  " + "current   " + QString::number(Text.size()) + "\n";

  s+=compare(t.split('\n'),Text.split('\n'));
  tview->setPlainText(s);
  tview->setFocus();
  tview->moveCursor(QTextCursor::End);
}

// ---------------------------------------------------------------------
// get pic file list in directory
QStringList Picm::pic_files()
{
  return cflist(Path + "/p" + ss_date(),"");
}

// ---------------------------------------------------------------------
void Picm::reject()
{
  closeit();
  QDialog::reject();
}

// ---------------------------------------------------------------------
void Picm::keyReleaseEvent(QKeyEvent *event)
{
#ifdef Q_OS_ANDROID
  if (event->key()==Qt::Key_Back) {
    reject();
  } else QDialog::keyReleaseEvent(event);
#else
  QDialog::keyReleaseEvent(event);
#endif
}

// ---------------------------------------------------------------------
QString Picm::unstamp(QString s)
{
  return s.left(2) + ":" + s.mid(2,2) + ":" + s.right(2);
}

// ---------------------------------------------------------------------
void pic(QString f,QString s)
{
  QFile *t;
  QString d,n,p,v;
  QByteArray a;

  a=s.toUtf8() + pp_stamp() + (char)255;

  p=snapgetpath(cfpath(f));
  n=cfsname(f);
  d=p + "/p" + ss_date();
  t=new QFile(d + "/" + n);

  if(!t->exists()) {
    if (!pic_inidir(d)) return;
    a=(cfread(f)+"000000").toUtf8() + (char)255 + a;
  }
  cfappend(t,a);
}

// ---------------------------------------------------------------------
// check dir is initialized
bool pic_inidir(QString s)
{
  QDir d,h;
  QFile *p=new QFile;

  d.setPath(s);
  if (d.exists()) return true;
  h.setPath(cfpath(s));
  p->setFileName(h.filePath("/plast"));
  if (p->exists())
    p->remove();
  QStringList f;
  f << "p*";
  QStringList e=h.entryList(f,QDir::Dirs|QDir::Readable);
  if(e.size()) {
    qSort(e);
    p->setFileName(h.filePath(e.last()));
    p->rename(h.filePath("/plast"));
    e.removeLast();
    foreach (const QString m,e)
    cfdelete(h.filePath(m));
  }
  ss_mkdir(s);
  return true;

}

// ---------------------------------------------------------------------
QByteArray pp_stamp()
{
  return QTime::currentTime().toString("hhmmss").toUtf8();
}
