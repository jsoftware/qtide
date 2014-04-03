
#include <QDir>
#ifdef QT50
#include <QQmlError>
#else
#include <QDeclarativeContext>
#include <QDeclarativeError>
#endif

#include "wd.h"
#include "quickview.h"
#include "form.h"
#include "cmd.h"
#include "../base/utils.h"
#ifndef QT50
#include "../base/qmlje.h"
extern QmlJE qmlje;
#endif

extern QuickView * quickview;

// ---------------------------------------------------------------------
#ifdef QT50
QuickView::QuickView(string n, string s, int resizemode) : QQuickView()
#else
QuickView::QuickView(string n, string s, int resizemode) : QDeclarativeView()
#endif
{
  QString qn=s2q(n);
  setObjectName(qn);
// enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView }
#ifdef QT50
  setTitle(qn);
  setResizeMode((QQuickView::ResizeMode)(this->resizeMode=resizemode));
#else
  rootContext()->setContextProperty("QmlJE", &qmlje);
  setResizeMode((QDeclarativeView::ResizeMode)(this->resizeMode=resizemode));
#endif
  QObject::connect((QObject*)this->engine(), SIGNAL(quit()), this, SLOT(close()));
  QString t = s2q(s);
  if (t.contains("://"))
    sourceUrl = QUrl(t);
  else sourceUrl = QUrl::fromLocalFile(t);
  setSource(sourceUrl);
#ifdef QT50
  connect(this, SIGNAL(statusChanged(QQuickView::Status)), this, SLOT(statusChanged(QQuickView::Status)));
  connect(this, SIGNAL(sceneGraphError(QQuickWindow::SceneGraphError,QString)), this, SLOT(sceneGraphError(QQuickWindow::SceneGraphError,QString)));
#else
  connect(this, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(statusChanged( QDeclarativeView::Status)));
  connect(this, SIGNAL(sceneResized(QSize)), this, SLOT(sceneResized(QSize)));
#endif
}

#ifdef QT50
// ---------------------------------------------------------------------
void QuickView::statusChanged(QQuickView::Status status)
{
  if (status == QQuickView::Error) {
    QStringList errors;
    foreach (const QQmlError &error, this->errors()) errors.append(error.toString());
    qDebug() << errors.join(QStringLiteral(", "));
  }
}

// ---------------------------------------------------------------------
void QuickView::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
  qDebug() << message;
}

// ---------------------------------------------------------------------
void QuickView::keyPressEvent(QKeyEvent *event)
{
#ifdef QT_OS_ANDROID
  int key=event->key();
  if (key==Qt::Key_Back) {
    QQuickView::keyPressEvent(event);
    return;
  }
#endif
  QQuickView::keyPressEvent(event);
}

// ---------------------------------------------------------------------
void QuickView::keyReleaseEvent(QKeyEvent *e)
{
#ifdef QT_OS_ANDROID
  if (e->key()==Qt::Key_Back) {
    showide(true);
    if (Forms.size()>0)
      (Forms.at(Forms.size()-1))->setVisible(true);
    close();
  } else QQuickView::keyReleaseEvent(e);
#else
  QQuickView::keyReleaseEvent(e);
#endif
}

#else

// ---------------------------------------------------------------------
void QuickView::statusChanged(QDeclarativeView::Status status)
{
  if (status == QDeclarativeView::Error) {
    QStringList errors;
    foreach (const QDeclarativeError &error, this->errors()) errors.append(error.toString());
    qDebug() << errors.join(QString(", "));
  }
}

// ---------------------------------------------------------------------
void QuickView::sceneResized (QSize size)
{
  Q_UNUSED(size);
//  qDebug() << size;
}

#endif
