
#include <QDir>
#include <QQmlError>
#include <QSurfaceFormat>

#include "cmd.h"
#include "form.h"
#include "quickview2.h"
#include "wd.h"
#include "../base/utils.h"

extern QuickView2 * quickview2;

// ---------------------------------------------------------------------
QuickView2::QuickView2(string n, string s, int resizemode, string glver) : QQuickView()
{
  QString qn=s2q(n);
  setObjectName(qn);
// enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView }
  setTitle(qn);
  setResizeMode((QQuickView::ResizeMode)(this->resizeMode=resizemode));

  QSurfaceFormat format;
  if (!glver.empty()) {
    int ver1,ver2;
    string::size_type d=glver.find(".",0);
    if (d == string::npos) {
      ver1=atoi(glver.c_str());
      ver2=0;
    } else {
      ver1=atoi(glver.substr(0,d).c_str());
      ver2=atoi(glver.substr(d+1).c_str());
    }
//    qDebug() << QString::number(ver1) << QString::number(ver2);
    format.setVersion(ver1,ver2);
  }
  format.setProfile(QSurfaceFormat::CoreProfile);
  setFormat(format);

  QObject::connect((QObject*)this->engine(), SIGNAL(quit()), this, SLOT(closeview()));
  QString t = s2q(s);
  if (t.contains("://"))
    sourceUrl = QUrl(t);
  else sourceUrl = QUrl::fromLocalFile(t);
  setSource(sourceUrl);
  connect(this, SIGNAL(statusChanged(QQuickView::Status)), this, SLOT(statusChanged(QQuickView::Status)));
#ifdef QT53
  connect(this, SIGNAL(sceneGraphError(QQuickWindow::SceneGraphError,QString)), this, SLOT(sceneGraphError(QQuickWindow::SceneGraphError,QString)));
#endif
}

// ---------------------------------------------------------------------
void QuickView2::statusChanged(QQuickView::Status status)
{
  if (status == QQuickView::Error) {
    QStringList errors;
    foreach (const QQmlError &error, this->errors()) errors.append(error.toString());
    info("QtQuick",errors.join(QStringLiteral(", ")));
//    qDebug() << errors.join(QStringLiteral(", "));
  }
}

#ifdef QT53
// ---------------------------------------------------------------------
void QuickView2::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
  info("QtQuick",message);
//  qDebug() << message;
}
#endif

// ---------------------------------------------------------------------
void QuickView2::keyPressEvent(QKeyEvent *event)
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
void QuickView2::keyReleaseEvent(QKeyEvent *e)
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

// ---------------------------------------------------------------------
void QuickView2::closeview ()
{
#ifdef QT_OS_ANDROID
  showide(true);
  if (Forms.size()>0)
    (Forms.at(Forms.size()-1))->setVisible(true);
#endif
  close();
}

