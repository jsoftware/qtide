#ifndef UTILS_H
#define UTILS_H

class QFont;
class Note;
class QTextCursor;
class QTextDocument;
class QUrl;
#ifdef TABCOMPLETION
class QAbstractItemModel;
class QCompleter;
#endif

QUrl currenturl();
QString cpath(QString);
QString defext(QString s);
int fkeynum(int key,bool c,bool s);
QStringList folder_tree(QString b,QString f,bool subdir);
QStringList folder_tree1(QString b,QString s,QStringList f);
void fontdiff(int n);
void fontset(QFont font);
extern "C" Dllexport void fontsetsize(int n);
QString fontspec(QFont font);
QString getcmd(QString,QString);
#ifdef TABCOMPLETION
QAbstractItemModel *getcompletermodel(QCompleter *,const QString&);
#endif
QTextCursor getcursor(QWidget *);
QTextDocument *getdocument(QWidget *w);
QWidget* getmbparent();
QString getplaintext(QWidget *);
int getpositioninblock(QTextCursor c);
QString getsha1(QString);
std::string getversion();
bool gitavailable();
void gitgui(QString path);
QString gitstatus(QString path);
QStringList globalassigns(QString s,QString ext);
int globalassign1(QString s,QString t,int p);
void helpabout();
bool ismodifier(int key);
void newfile(QWidget*);
QString newtempscript();
void openfile(QWidget*,QString s);
void openfile1(QString f);
void projectclose();
void projectenable();
void projectfilemanager();
QStringList project_tree(QString b);
QStringList project_tree1(QString b,QString s);
void projectterminal();
QString readhelptext(QWidget *w, int t);
QString rxassign(QString ext, bool ifglobal);
void setwh(QWidget *w, QString s);
void setxywh(QWidget *w, QString s);
QStringList shell(QString cmd, QString dir);
void setnote(Note *);
void showide(bool);
QString tofoldername(QString s);
QString toprojectname(QString s);
void userkey(int mode, QString cmd);
void utils_init();
QString win2lower(QString s);
QList<int> winpos_get(QWidget *w);
void winpos_set(QWidget *w,QList<int>p);
void xdiff(QString s,QString t);

extern bool ShowIde;

#endif
