
#include <QDebug>
#include <QVBoxLayout>

#include "base.h"
#include "dlog.h"
#include "recent.h"
#include "state.h"
#include "tedit.h"

using namespace std;

QFile *InputLogFile;

int InputLogMax, InputLogPos;
QStringList InputLog;

void dlog_max();
QStringList makeitems();
QStringList qsreverse(const QStringList list);

// ---------------------------------------------------------------------
Slog::Slog()
{
  setWindowTitle("Input Log");
  QList<int>d=config.winpos_read("Dlog");
  resize(qMax(300,d[2]),qMax(300,d[3]));
  QListWidget *s = new QListWidget();
  s->addItems(makeitems());
  s->setCurrentRow(InputLog.size()-1);
  connect(s, SIGNAL(itemActivated(QListWidgetItem*)),
          this,SLOT(itemActivated()));
  connect(s, SIGNAL(itemActivated(QListWidgetItem*)),
          tedit,SLOT(itemActivated(QListWidgetItem*)));
  QVBoxLayout *b = new QVBoxLayout;
  b->setContentsMargins(0,0,0,0);
  b->addWidget(s);
  setLayout(b);
  exec();
}

// ---------------------------------------------------------------------
QStringList makeitems()
{
  QStringList items;
  QString s,t;
  QString p=tedit->getprompt();
  int len=p.size();
  for (int i=0; i<InputLog.size(); i++) {
    t = s = InputLog.value(i);
    t.truncate(len);
    if (p == t)
      s.remove(0,len);
    items.append(s);
  }
  return items;
}

// ---------------------------------------------------------------------
void dlog_add(QString s)
{
  if (s.size() == 0) return;
  int i = InputLog.indexOf(s);
  if (i >= 0)
    InputLog.removeAt(i);
  InputLog.append(s);
  cfappend(InputLogFile,s + "\n");
  dlog_max();
}

// ---------------------------------------------------------------------
void dlog_init()
{
  InputLogMax = 100;
  InputLogPos = 0;
  InputLogFile = new QFile(config.ConfigPath.filePath("inputlog.dat"));
  QString s = cfread(InputLogFile);
  s.remove('\r');
  QStringList t = s.split("\n",QString::SkipEmptyParts);
  t = qsreverse(t);
  t.removeDuplicates();
  InputLog = qsreverse(t);
  dlog_max();
  dlog_write();
}

// ---------------------------------------------------------------------
void dlog_max()
{
  int p = InputLog.size() - InputLogMax;
  if (p > 0)
    for (int i=0; i<p; i++)
      InputLog.removeFirst();
  InputLogPos = InputLog.size();
}

// ---------------------------------------------------------------------
QString dlog_scroll(int m)
{
  int len, pos;
  QString s;
  len = InputLog.size();
  if (len == 0) return "";
  pos = max (0, min(len,InputLogPos + m));
  if (pos == InputLogPos) return "";
  InputLogPos = pos;
  if (InputLogPos == len)
    s = InputLog.at (len - 1);
  else
    s = InputLog.at(InputLogPos);
  return config.DefIndent + s;
}

// ---------------------------------------------------------------------
// write and clean up when session closes
void dlog_write()
{
  cfwrite(InputLogFile,InputLog.join("\n")+"\n");
}

// ---------------------------------------------------------------------
void Slog::reject()
{
  savepos();
  QDialog::reject();
}

// ---------------------------------------------------------------------
void Slog::itemActivated()
{
  savepos();
  close();
}

// ---------------------------------------------------------------------
void Slog::savepos()
{
  config.winpos_save(this,"Dlog");
}

// ---------------------------------------------------------------------
QStringList qsreverse(const QStringList list)
{
  QStringList r;
  for(int i=list.size()-1; i>=0; i--)
    r.append(list.at(i));
  return r;
}
