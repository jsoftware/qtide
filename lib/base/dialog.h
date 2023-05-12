
#include <QObject>
#include <QString>
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#ifndef QT_NO_PRINTDIALOG
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#endif
#else
#include <QPrinter>
#ifndef QT_NO_PRINTDIALOG
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#endif
#endif
#endif
class QTextDocument;
class PlainTextEdit;

QString dialogdirectory(QWidget *w,QString t,QString p);
void dialogfont(QWidget *w);
QString dialogfileopen(QWidget *w,QString t);
#ifndef QT_NO_PRINTDIALOG
void dialogprint(QWidget *w,QTextDocument *d);
void dialogprint(QWidget *w,QWidget *d);
void dialogprintpreview(QWidget *w, QWidget *d);
#endif
QString dialogsaveas(QWidget *w,QString t,QString p);
QString getfilepath();
QString getprojectpath();

#ifndef QT_NO_PRINTER
extern QPrinter *Printer;
void printpreview(QPrinter * printer, QTextDocument *d);
#endif
