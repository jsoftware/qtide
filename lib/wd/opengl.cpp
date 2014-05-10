
#include "opengl.h"
#include "opengl2.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

Opengl *opengl=0;

// ---------------------------------------------------------------------
Opengl::Opengl(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type = "opengl";

  QStringList m=s2q(s).split(' ',QString::SkipEmptyParts);
  QGLFormat qglFormat;
  qglFormat.setSampleBuffers(true);
#ifdef QT47
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
    qglFormat.setVersion(ver1,ver2);
  }
  if (m.contains("compatibility")) qglFormat.setProfile(QGLFormat::CompatibilityProfile);
  else qglFormat.setProfile(QGLFormat::CoreProfile);
#endif
  Opengl2 *w= new Opengl2(this, qglFormat, p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  opengl = this;
}

// ---------------------------------------------------------------------
void Opengl::set(string p,string v)
{
  if (!widget) return;
  Opengl2 *w=(Opengl2*) widget;
  QStringList opt=qsplit(v);

  if (p=="timer") {
    if (opt.isEmpty()) {
      error("set timer requires 1 number: " + p);
      return;
    }
    w->setTimer(c_strtoi(q2s(opt.at(0))));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
void Opengl::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="paintz" || event=="resize" || event=="initialize" || event=="print" || event=="timer")) form=pform;
  opengl=this;
}
