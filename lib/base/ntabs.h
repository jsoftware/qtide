#ifndef NTABS_H
#define NTABS_H

#include <QFileSystemWatcher>
#include <QTabWidget>

class Nedit;
class Note;

class Ntabs : public QTabWidget

{
  Q_OBJECT

public:
  Ntabs(Note *);
  void projectopen(bool);
  QStringList gettablist();
  std::string gettabstate();
  void setfont(QFont font);
  void setlinenos(bool b);
  void setlinewrap(bool b);
  void setmodified(int index,bool b);

  void tabclose(int index);
  void tabclose1(int index);
  void tabcloseall();
  void tabclosefile(QString f);

  bool tabopen(QString s,int line);
  bool tabreplace(QString s,int line);
  void tabrestore(int index);
  bool tabsave(int index);
  bool tabsaveall();
#ifndef QT_NO_PRINTER
  bool tabprint(int index);
  bool tabprintall();
#endif
  void tabsaveas(int index);
  int tabsaveOK(int index);
  void tabsetindex(int index);
  void tabsettext(QString s);

private slots:
  void currentChanged(int index);
  void fileChanged(const QString &);
  void modificationChanged(bool b);
  void tabCloseRequested(int index);

private:
  int getfileindex(QString f);
  void tabsetcolor(int index,bool ifmod);
  int tabopen1(QString s,int line);

  Note *pnote;
  QFileSystemWatcher *watcher;
};

#endif
