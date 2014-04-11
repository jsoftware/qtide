
#include <QDir>
#include <QQmlError>
#include <QSurfaceFormat>

#include "wd.h"
#include "quickwidget.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
QuickWidget::QuickWidget(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="quickwidget";
  QQuickWidget *w=new QQuickWidget;
// enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView }
  w->setResizeMode((QQuickWidget::ResizeMode)(resizeMode=1));
  w->setContentsMargins(0,0,0,0);
#if 0
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGLES);
  setFormat(format);
#endif
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  connect(w, SIGNAL(statusChanged( QQuickWidget::Status )), this, SLOT(statusChanged( QQuickWidget::Status)));
  connect(w, SIGNAL(sceneGraphError(QQuickWindow::SceneGraphError,QString)), this, SLOT(sceneGraphError(QQuickWindow::SceneGraphError,QString)));
}

// ---------------------------------------------------------------------
void QuickWidget::set(string p,string v)
{
  QQuickWidget *w = (QQuickWidget *)widget;
  if (p=="source") {
    QString t = s2q(remquotes(v));
    if (t.contains("://"))
      sourceUrl = QUrl(t);
    else sourceUrl = QUrl::fromLocalFile(t);
    w->setSource(sourceUrl);
    w->show();
  } else if (p=="resizemode") {
    w->setResizeMode((QQuickWidget::ResizeMode)(resizeMode=!!c_strtoi(v)));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
void QuickWidget::statusChanged(QQuickWidget::Status status)
{
  QQuickWidget *w = (QQuickWidget *)widget;
  if (status == QQuickWidget::Error) {
    QStringList errors;
    foreach (const QQmlError &error, w->errors()) errors.append(error.toString());
    qDebug() << errors.join(QStringLiteral(", "));
  }
}


// ---------------------------------------------------------------------
void QuickWidget::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
  qDebug() << message;
}

