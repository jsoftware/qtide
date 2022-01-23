
#include "cmd.h"
#include "form.h"
#include "opengl2.h"
#include "opengl.h"
#include "pane.h"

extern Form *form;
extern Child *opengl;

// ---------------------------------------------------------------------
Opengl::Opengl(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
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
#ifdef USE_QOpenGLWidget
  if (opt.contains("compatibility")) qglFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
  else qglFormat.setProfile(QSurfaceFormat::CoreProfile);
#else
  if (opt.contains("compatibility")) qglFormat.setProfile(QGLFormat::CompatibilityProfile);
  else qglFormat.setProfile(QGLFormat::CoreProfile);
#endif
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
  } else {
#ifdef __MACH__
    qglFormat.setVersion(4,1);
#endif
  }

  Opengl2 *w= new Opengl2(this, qglFormat);
  widget=(QWidget *) w;
  w->setObjectName(qn);
  childStyle(opt);
  opengl = this;
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
  if (!(event=="paint" || event=="paintz" || event=="resize" || event=="initialize" || event=="print")) Child::setform();
  opengl=this;
}

// ---------------------------------------------------------------------
std::string Opengl::get(std::string p,std::string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void Opengl::set(std::string p,std::string v)
{
  Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Opengl::state()
{
  return "";
}
