#include <QBoxLayout>
#include <QListWidget>
#include <QPushButton>

#include "base.h"
#include "eview.h"
#include "recent.h"
#include "rsel.h"
#include "state.h"
#include "term.h"

// using namespace std;

// ---------------------------------------------------------------------
Rsel::Rsel()
{
  setAttribute(Qt::WA_DeleteOnClose);
  int i;
  QStringList s;

  recent.Files=qslexists(recent.Files);

  if (recent.Files.isEmpty()) {
    info("Recent Files","There are no recent files");
    return;
  }

  for (i=0; i<recent.Files.size(); i++)
    s.append(tofoldername(recent.Files.at(i)));

  QHBoxLayout *h=new QHBoxLayout();
  h->setContentsMargins(0,0,0,0);

  flist=new QListWidget();
  flist->setAlternatingRowColors(true);
  flist->addItems(s);
  flist->setCurrentRow(0);
  flist->setObjectName("flist");
  h->addWidget(flist);

  QVBoxLayout *v=new QVBoxLayout();
  v->setContentsMargins(0,4,2,0);
  v->setSpacing(0);
  open=makebutton(v,"Open");
  view=makebutton(v,"View");
  run=makebutton(v,"Run");
  rundisplay=makebutton(v,"Run Display");
  v->addStretch(1);
  h->addLayout(v);

  setLayout(h);
  setWindowTitle("Recent Files");
#ifdef SMALL_SCREEN
  move(0,0);
  resize(term->width(),term->height());
#else
  resize(450,400);
#endif
  QMetaObject::connectSlotsByName(this);
  show();
}

// ---------------------------------------------------------------------
QPushButton *Rsel::makebutton(QVBoxLayout *v,QString id)
{
  QPushButton *p=new QPushButton(id);
  p->setObjectName(id.remove(' ').toLower());
  v->addWidget(p);
  return p;
}

// ---------------------------------------------------------------------
void Rsel::on_flist_itemActivated()
{
  on_open_clicked();
}

// ---------------------------------------------------------------------
void Rsel::on_open_clicked()
{
  QString s=selected();
  if (s.isEmpty()) return;
  if (!ifshift())
    close();
  openfile1(s);
}

// ---------------------------------------------------------------------
void Rsel::on_run_clicked()
{
  QString s=selected();
  if (s.isEmpty()) return;
  if (!ifshift())
    close();
  term->load(s,0);
}

// ---------------------------------------------------------------------
void Rsel::on_rundisplay_clicked()
{
  QString s=selected();
  if (s.isEmpty()) return;
  if (!ifshift())
    close();
  term->load(s,1);
}

// ---------------------------------------------------------------------
void Rsel::on_view_clicked()
{
  QString s=selected();
  if (s.isEmpty()) return;
  if (!ifshift())
    close();
  textview(cfread(s));
}

// ---------------------------------------------------------------------
QString Rsel::selected()
{
  QList<QListWidgetItem *> s=flist->selectedItems();
  if (s.size()) return cpath(s.at(0)->text());
  else return "";
}
