
class QTextDocument;

QString dialogdirectory(QWidget *w,QString t,QString p);
void dialogfont(QWidget *w);
QString dialogfileopen(QWidget *w,QString t);
#ifndef ANDROID
void initprinter();
void dialogprint(QWidget *w,QTextDocument *d);
#endif
QString dialogsaveas(QWidget *w,QString t,QString p);
QString getfilepath();
QString getprojectpath();

#ifndef ANDROID
extern QPrinter *Printer;
#endif
