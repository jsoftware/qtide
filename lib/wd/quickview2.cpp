
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
QuickView2::QuickView2(std::string n, std::string s, int resizemode, std::string glver) : QQuickView()
{
  QString qn=s2q(n);
  setObjectName(qn);
// enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView }
  setTitle(qn);
  setResizeMode((QQuickView::ResizeMode)(this->resizeMode=resizemode));

  QSurfaceFormat format;
  if (!glver.empty()) {
    int ver1,ver2;
    std::string::size_type d=glver.find(".",0);
    if (d == std::string::npos) {
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
  connect(this, SIGNAL(sceneGraphError(QQuickWindow::SceneGraphError,QString)), this, SLOT(sceneGraphError(QQuickWindow::SceneGraphError,QString)));
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

// ---------------------------------------------------------------------
void QuickView2::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
  info("QtQuick",message);
//  qDebug() << message;
}

// ---------------------------------------------------------------------
void QuickView2::keyPressEvent(QKeyEvent *event)
{
  QQuickView::keyPressEvent(event);
}

// ---------------------------------------------------------------------
void QuickView2::closeview ()
{
  close();
}

