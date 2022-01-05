
#include <QVBoxLayout>

#include "base.h"
#include "dlog.h"
#include "recent.h"
#include "state.h"
#include "tedit.h"
#include "term.h"

// using namespace std;

QFile *InputLogFile;

int InputLogMax, InputLogPos;
QStringList InputLog;

void dlog_max();
QStringList makeitems();
QStringList qsremdups(const QStringList list);
QStringList qsreverse(const QStringList list);

bool ischaracter(int);

// ---------------------------------------------------------------------
Slog::Slog()
{
  setAttribute(Qt::WA_DeleteOnClose);
  Filter="";
  LogList.append(makeitems());
  setWindowTitle("Input Log");
  QList<int>d=config.winpos_read("Dlog");
#ifdef SMALL_SCREEN
  move(0,0);
  resize(term->width(),term->height());
#else
  resize(qMax(300,d[2]),qMax(300,d[3]));
#endif
  s = new SlogList(this);
  connect(s, SIGNAL(itemActivated(QListWidgetItem*)),
          this,SLOT(itemActivated()));
  connect(s, SIGNAL(itemActivated(QListWidgetItem*)),
          tedit,SLOT(itemActivated(QListWidgetItem*)));
  t=new QLabel();
  t->hide();
  QVBoxLayout *b = new QVBoxLayout;
  b->setContentsMargins(0,0,0,0);
  b->setSpacing(0);
  b->addWidget(s);
  b->addWidget(t);
  setLayout(b);
  showlist();
  exec();
}

// ---------------------------------------------------------------------
void Slog::addfilter(QString s)
{
  Filter+=s;
  bool b=hasuppercase(Filter);
  LogList.append(LogList.last().filter(Filter,b ? Qt::CaseSensitive : Qt::CaseInsensitive));
  t->show();
  showlist();
}

// ---------------------------------------------------------------------
void Slog::delfilter(bool all)
{
  if (Filter.isEmpty()) return;
  if (all) {
    Filter="";
    LogList=LogList.mid(0,1);
  } else {
    Filter.remove(Filter.size()-1,1);
    LogList.removeLast();
  }
  showlist();
}

// ---------------------------------------------------------------------
bool Slog::hasuppercase(QString s)
{
  QString t=s.toUpper();
  for (int i=0; i<s.size(); i++)
    if (s[i]==t[i]) return true;
  return false;
}

// ---------------------------------------------------------------------
void Slog::itemActivated()
{
  savepos();
  close();
}

// ---------------------------------------------------------------------
void Slog::keyPressEvent(QKeyEvent *event)
{
  int k=event->key();
  if (ischaracter(k))
    addfilter(event->text());
  else
    QDialog::keyPressEvent(event);
}

// ---------------------------------------------------------------------
void Slog::reject()
{
  savepos();
  QDialog::reject();
}

// ---------------------------------------------------------------------
void Slog::savepos()
{
  config.winpos_save(this,"Dlog");
}

// ---------------------------------------------------------------------
void Slog::showlist()
{
  QStringList p=LogList.last();
  t->setText(" " + Filter);
  s->clear();
  s->addItems(p);
  s->setCurrentRow(p.size()-1);
}

// ---------------------------------------------------------------------
SlogList::SlogList(Slog *p)
{
  pform=p;
}

// ---------------------------------------------------------------------
void SlogList::keyPressEvent(QKeyEvent *event)
{
  if (event->key()==Qt::Key_Backspace) {
    pform->delfilter(event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier));
    event->accept();
    return;
  }
  if (event->key()==Qt::Key_Escape || ischaracter(event->key()))
    event->ignore();
  else {
#ifdef __MACH__
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
      emit itemActivated(currentItem());
      return event->accept();
    }
#endif
    QListWidget::keyPressEvent(event);
  }
}

// ---------------------------------------------------------------------
QStringList makeitems()
{
  QStringList items;
  QString s,t;
  QString p=tedit->getprompt();
  int len=p.size();
  int maxwid=250;
  for (int i=0; i<InputLog.size(); i++) {
    t = s = InputLog.value(i);
    t.truncate(len);
    if (p == t)
      s.remove(0,len);
    s=s.left(maxwid);
    items.append(s);
  }
  return qsremdups(items);
}

// ---------------------------------------------------------------------
void dlog_add(QString s)
{
  if (s.size() == 0) return;
  int i = InputLog.indexOf(s);
  if (i >= 0)
    InputLog.removeAt(i);
  InputLog.append(s);
  if (config.MaxInputLog>0) cfappend(InputLogFile,s + "\n");
  dlog_max();
}

// ---------------------------------------------------------------------
QString dlog_get(void)
{
  return InputLog.join("\n");
}

// ---------------------------------------------------------------------
void dlog_init()
{
  InputLogMax = qMax(config.MaxInputLog,100);
  InputLogPos = 0;
  if (config.MaxInputLog==0) return;
  InputLogFile = new QFile(config.ConfigPath.filePath("inputlog.dat"));
  QString s = cfread(InputLogFile);
  s.remove('\r');
  QStringList t = s.split("\n",_SkipEmptyParts);
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
  pos = std::max(0, std::min(len,InputLogPos + m));
  if (pos == InputLogPos) return "";
  InputLogPos = pos;
  if (InputLogPos == len)
    s = InputLog.at(len - 1);
  else
    s = InputLog.at(InputLogPos);
  return config.DefIndent + s;
}

// ---------------------------------------------------------------------
void dlog_set(QString s)
{
  InputLog=s.split('\n',_SkipEmptyParts);
  dlog_write();
}

// ---------------------------------------------------------------------
// write and clean up when session closes
void dlog_write()
{
  if (config.MaxInputLog>0)
    cfwrite(InputLogFile,qsremdups(InputLog).join("\n")+"\n");
}

// ---------------------------------------------------------------------
bool ischaracter(int k)
{
  return ((30<=k) && k<=255);
}

// ---------------------------------------------------------------------
QStringList qsremdups(const QStringList list)
{
  QStringList s=qsreverse(list);
  s.removeDuplicates();
  return qsreverse(s);
}

// ---------------------------------------------------------------------
QStringList qsreverse(const QStringList list)
{
  QStringList r;
  for (int i=list.size()-1; i>=0; i--)
    r.append(list.at(i));
  return r;
}
