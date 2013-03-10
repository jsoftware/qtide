#ifndef UTIL_H
#define UTIL_H

class QDateTime;
class QDir;
class QFile;
class QString;

bool cderase(QString name);
int cfappend (QFile *, QString);
int cfappend (QFile *, QByteArray);
QString cfcase(QString s);
bool cfcopy(QString from, QString to);
bool cfcreate(QString s);
bool cfdelete(QString s);
bool cfexist(QString s);
QString cfext(QString s);
QStringList cflist(QString d,QString f);
QStringList cflist(QString path,QStringList f);
QStringList cflistfull(QString b,QString f);
QStringList cflisttext(QString path);
QString cfpath(QString s);
QString cfsname(QString);
QString cfread(QFile *);
QString cfread(QString s);
QStringList cfreads(QFile *file);
QStringList cfreads(QString s);
QStringList cfreadx(QString s);
QDateTime cftime(QString s);
int cfwrite(QFile *file,QString);
int cfwrite(QString s, QString t);
int cfwrite(QString s, QByteArray b);

void about(QString t,QString s);
QString c2q(char *);
std::string c2s(char *);
bool createdir(QDir d);
QString detab(QString s);
QString dlb(QString s);
QString dtb(QString s);
QString dtLF(QString s);
QStringList folder_tree(QString b,QString f,bool subdir);
QStringList folder_tree1(QString b,QString s,QStringList f);
QStringList getfilters(QString s);
void helpabout();
std::string i2s(int i);
bool ifshift();
void info(QString t,QString s);
bool isint(std::string s);
bool isroot(QString name);
bool isutf8(QByteArray b);
bool matchhead(QString, QString);
bool matchfolder(QString s, QString t);
int matchparens(QChar, QString);
void noevents(int n);
void notyet(QString);
QString p2q(int *);
std::string p2s(const void *p);
void q2p(QString,int *);
bool qshasonly(const QString s, const QString t);
std::string q2s(QString);
bool queryNY(QString t,QString s);
bool queryOK(QString t,QString s);
bool queryRETRY(QString t,QString s);
QString remsep(QString);
QString remtilde(QString);
QString s2q(std::string);
std::string spair(std::string s,std::string t);
QString termLF(QString s);
QString termsep(QString);
QString toqlist(QStringList);

QList<int> qs2intlist(QString c);
QString qstaketo(QString s,QString c);
QVector<int> qs2intvector(QString c);

QStringList qsldtbeach(QStringList s);
QStringList qsldropeach(int n,QStringList s);
QStringList qslexists(QStringList s);
QStringList qslfcase(QStringList s);
QStringList qslprependeach(QString p,QStringList s);
QStringList qslreverse(QStringList s);
QStringList qsltrim(QStringList p);

extern int NoEvents;

#ifdef ANDROID
extern "C" void android_exec_host(char *,char *,char *);
#endif

#if defined(_WIN64)||defined(__LP64__)
typedef long long SI;
#else
typedef long SI;
#endif

int c_strtoi(std::string s);
SI c_strtol(std::string s);

#endif
