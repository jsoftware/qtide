
#include "opengl.h"
#include "opengl2.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Opengl::Opengl(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="opengl";
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"version compatibility")) return;
#ifdef USE_QOpenGLWidget
  QSurfaceFormat qglFormat;
#else
  QGLFormat qglFormat;
  qglFormat.setSampleBuffers(true);
#endif
#ifdef QT47
  int l=opt.indexOf("version");
  if ((l!=-1) && (l<opt.size()-1) && 0!=opt.at(l+1).toDouble()) {
    int ver1,ver2;
    QString s=opt.at(l+1);
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
#ifdef USE_QOpenGLWidget
  if (opt.contains("compatibility")) qglFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
  else qglFormat.setProfile(QSurfaceFormat::CoreProfile);
#else
  if (opt.contains("compatibility")) qglFormat.setProfile(QGLFormat::CompatibilityProfile);
  else qglFormat.setProfile(QGLFormat::CoreProfile);
#endif
#endif

  Opengl2 *w= new Opengl2(this, qglFormat);
  widget=(QWidget *) w;
  w->setObjectName(qn);
  childStyle(opt);
  f->opengl = this;
}

// ---------------------------------------------------------------------
Opengl::~Opengl()
{
  if (widget) delete (Opengl2 *)widget;
  widget=0;
}

// ---------------------------------------------------------------------
void Opengl::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="paintz" || event=="resize" || event=="initialize" || event=="print")) form=pform;
  form->opengl=this;
}

// ---------------------------------------------------------------------
string Opengl::get(string p,string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void Opengl::set(string p,string v)
{
  Child::set(p,v);
}

// ---------------------------------------------------------------------
string Opengl::state()
{
  return "";
}
