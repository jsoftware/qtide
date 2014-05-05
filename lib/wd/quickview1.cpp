
#include <QDir>
#include <QDeclarativeContext>
#include <QDeclarativeError>

#include "wd.h"
#include "quickview1.h"
#include "form.h"
#include "cmd.h"
#include "../base/utils.h"
#include "../base/qmlje.h"
extern QmlJE qmlje;

extern QuickView1 * quickview1;

// ---------------------------------------------------------------------
QuickView1::QuickView1(string n, string s, int resizemode) : QDeclarativeView()
{
  QString qn=s2q(n);
  setObjectName(qn);
// enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView }
  rootContext()->setContextProperty("QmlJE", &qmlje);
  setResizeMode((QDeclarativeView::ResizeMode)(this->resizeMode=resizemode));
  QObject::connect((QObject*)this->engine(), SIGNAL(quit()), this, SLOT(closeview()));
  QString t = s2q(s);
  if (t.contains("://"))
    sourceUrl = QUrl(t);
  else sourceUrl = QUrl::fromLocalFile(t);
  setSource(sourceUrl);
  connect(this, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(statusChanged( QDeclarativeView::Status)));
  connect(this, SIGNAL(sceneResized(QSize)), this, SLOT(sceneResized(QSize)));
}

// ---------------------------------------------------------------------
void QuickView1::statusChanged(QDeclarativeView::Status status)
{
  if (status == QDeclarativeView::Error) {
    QStringList errors;
    foreach (const QDeclarativeError &error, this->errors()) errors.append(error.toString());
    info("QtQuick",errors.join(QString(", ")));
//    qDebug() << errors.join(QString(", "));
  }
}

// ---------------------------------------------------------------------
void QuickView1::sceneResized (QSize size)
{
  Q_UNUSED(size);
//  qDebug() << size;
}

// ---------------------------------------------------------------------
void QuickView1::closeview ()
{
#ifdef QT_OS_ANDROID
  showide(true);
  if (Forms.size()>0)
    (Forms.at(Forms.size()-1))->setVisible(true);
#endif
  close();
}

