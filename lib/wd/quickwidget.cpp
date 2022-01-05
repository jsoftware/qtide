
#include <QDir>
#include <QQmlError>
#include <QSurfaceFormat>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "quickwidget.h"
#include "wd.h"

// ---------------------------------------------------------------------
QuickWidget::QuickWidget(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="quickwidget";

  int mode=1;
  QStringList m=s2q(s).split(' ',_SkipEmptyParts);
  if (m.size() && (m.at(0)=="0"||m.at(0)=="1")) mode=!!c_strtoi(q2s(m.at(0)));
  QSurfaceFormat format;
  int l=m.indexOf("version");
  if ((l!=-1) && (l<m.size()-1) && 0!=m.at(l+1).toDouble()) {
    int ver1,ver2;
    QString s=m.at(l+1);
    int d=s.indexOf(".");
    if (d==-1) {
      ver1=s.toInt();
      ver2=0;
    } else {
      ver1=s.mid(0,d).toInt();
      ver2=s.mid(d+1).toInt();
    }
//    qDebug() << QString::number(ver1) << QString::number(ver2);
    format.setVersion(ver1,ver2);
  }
  if (m.contains("compatibility")) format.setProfile(QSurfaceFormat::CompatibilityProfile);
  else format.setProfile(QSurfaceFormat::CoreProfile);

  QQuickWidget *w=new QQuickWidget;
  w->setFormat(format);
// enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView }
  w->setResizeMode((QQuickWidget::ResizeMode)(resizeMode=mode));
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  childStyle(m);
  connect(w, SIGNAL(statusChanged( QQuickWidget::Status )), this, SLOT(statusChanged( QQuickWidget::Status)));
  connect(w, SIGNAL(sceneGraphError(QQuickWindow::SceneGraphError,QString)), this, SLOT(sceneGraphError(QQuickWindow::SceneGraphError,QString)));
}

// ---------------------------------------------------------------------
std::string QuickWidget::get(std::string p,std::string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void QuickWidget::set(std::string p,std::string v)
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
    info("QtQuick",errors.join(QStringLiteral(", ")));
//    qDebug() << errors.join(QStringLiteral(", "));
  }
}


// ---------------------------------------------------------------------
void QuickWidget::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
  info("QtQuick",message);
//  qDebug() << message;
}

// ---------------------------------------------------------------------
std::string QuickWidget::state()
{
  return "";
}
