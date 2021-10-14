#ifndef UTIL_H
#define UTIL_H

class QDateTime;
class QDir;
class QFile;
class QString;

bool cderase(QString name);
int cfappend(QFile *, QString);
int cfappend(QFile *, QByteArray);
QString cfcase(QString s);
bool cfcopy(QString from, QString to);
bool cfcreate(QString s);
bool cftouch(QString s);
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
QByteArray cfreadbin(QString s);
QStringList cfreads(QFile *file);
QStringList cfreads(QString s);
QStringList cfreadx(QString s);
bool cfrmdir(const QString &d);
QDateTime cftime(QString s);
int cfwrite(QFile *file,QString);
int cfwrite(QString s, QString t);
int cfwrite(QString s, QByteArray b);

void about(QString t,QString s);
QString boxj2utf8(std::string s);
QString c2q(const char *);
std::string c2s(const char *);
bool createdir(QDir d);
QStringList cut2(QString s);
QString detab(QString s);
QString dlb(QString s);
QString dtb(QString s);
QString dtLF(QString s);
QStringList getfilters(QString s);
void helpabout();
std::string i2s(int i);
std::string d2s(double d);
bool ifshift();
void info(QString t,QString s);
int initialblanks(QString t);
QString intlist2qs(QList<int>);
bool isint(std::string s);
bool isroot(QString name);
bool isutf8(QByteArray b);
bool matchhead(QString, QString);
bool matchfolder(QString s, QString t);
int matchparens(QChar, QString);
int modpy(int p, int y);
void noevents(int n);
void notyet(QString);
QString p2q(QList<int>);
std::string p2s(const void *p);
QList<int> q2p(QString);
bool qshasonly(const QString s, const QString t);
std::string q2s(QString);
int queryCNY(QString t,QString s);
bool queryNY(QString t,QString s);
bool queryOK(QString t,QString s);
bool queryRETRY(QString t,QString s);
QString quote(QString s);
QStringList qsless(QStringList,QStringList);
bool qsnumeric(QStringList);
std::string strless(std::string a,std::string w);
std::string remquotes(std::string s);
QString remsep(QString);
QString remtilde(QString);
QByteArray s2b(std::string);
QString s2q(std::string);
std::string spair(std::string s,std::string t);
std::string spair(std::string s,QString t);
QString termLF(QString s);
QString termsep(QString);
QString trimtws(QString s);
QString toqlist(QStringList);
QString uptoLF(QString);

QList<int> qs2intlist(QString c);
QList<int> qsl2intlist(QStringList s);
QString qstaketo(QString s,QString c);
QVector<int> qs2intvector(QString c);

QStringList qsldtbeach(QStringList s);
QStringList qsldropeach(int n,QStringList s);
QStringList qslexists(QStringList s);
QStringList qslfcase(QStringList s);
QStringList qslprependeach(QString p,QStringList s);
QStringList qslreverse(QStringList s);
QStringList qsltrim(QStringList p);
QStringList qsltrimeach(QStringList p);

extern int NoEvents;

#if defined(_WIN64)||defined(__LP64__)
typedef long long SI;
#else
typedef long SI;
#endif

int c_strtoi(std::string s);
SI c_strtol(std::string s);
double c_strtod(std::string s);

#endif
