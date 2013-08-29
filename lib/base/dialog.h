
class QPrinter;
class QTextDocument;

QString dialogdirectory(QWidget *w,QString t,QString p);
void dialogfont(QWidget *w);
QString dialogfileopen(QWidget *w,QString t);
#ifndef QT_NO_PRINTDIALOG
void dialogprint(QWidget *w,QTextDocument *d);
#endif
QString dialogsaveas(QWidget *w,QString t,QString p);
QString getfilepath();
QString getprojectpath();

#ifndef QT_NO_PRINTER
extern QPrinter *Printer;
#endif
