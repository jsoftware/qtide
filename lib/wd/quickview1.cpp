
#include <QDir>
#include <QDeclarativeContext>
#include <QDeclarativeError>

#include "cmd.h"
#include "form.h"
#include "quickview1.h"
#include "wd.h"
#include "../base/qmlje.h"
#include "../base/utils.h"
extern QmlJE qmlje;

extern QuickView1 * quickview1;

// ---------------------------------------------------------------------
QuickView1::QuickView1(std::string n, std::string s, int resizemode) : QDeclarativeView()
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
  close();
}

