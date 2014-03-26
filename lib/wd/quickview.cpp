
#include <QDir>
#include <QQmlError>

#include "wd.h"
#include "quickview.h"
#include "form.h"
#include "cmd.h"
#include "../base/utils.h"

extern QuickView * quickview;

// ---------------------------------------------------------------------
QuickView::QuickView(string n, string s, int resizemode) : QQuickView()
{
  QString qn=s2q(n);
  setObjectName(qn);
  setTitle(qn);
// enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView }
  setResizeMode((QQuickView::ResizeMode)(this->resizeMode=resizemode));
  QString t = s2q(s);
  if (t.contains("://"))
    sourceUrl = QUrl(t);
  else sourceUrl = QUrl::fromLocalFile(t);
  setSource(sourceUrl);
  connect(this, SIGNAL(statusChanged( QQuickView::Status )), this, SLOT(statusChanged( QQuickView::Status)));
  connect(this, SIGNAL(sceneGraphError(QQuickWindow::SceneGraphError,QString)), this, SLOT(sceneGraphError(QQuickWindow::SceneGraphError,QString)));
}

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
  int key=event->key();
#ifdef QT_OS_ANDROID
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

