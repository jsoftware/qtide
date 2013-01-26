
#include <QPainter>
#include <QPaintEngine>
#include <math.h>

#include "opengl.h"
#include "opengl2.h"
#include "form.h"

extern Opengl *opengl;

extern "C" {
  int gl_sel(void *g);
  int gl_sel2(char *g);
  int gl_qwh(int *wh);
  int gl_paint();
  int gl_paintx();
  int gl_qhandles(void **p);
  int gl_setlocale (char *c);
  void *glsl(char *vsrc, char *fsrc);
  int glsl_bind(void *p);
  void glsl_release(void *p);
  void glsl_setUniformValue_f(void *p, int matrixUniform, GLfloat data);
  void glsl_setUniformValue_f22(void *p, int matrixUniform, GLfloat *data);
  void glsl_setUniformValue_f33(void *p, int matrixUniform, GLfloat *data);
  void glsl_setUniformValue_f44(void *p, int matrixUniform, GLfloat *data);
  void glsl_enableAttributeArray(void *p, int attr);
  void glsl_disableAttributeArray(void *p, int attr);
  void glsl_setAttributeArray(void *p, int attr, GLfloat *data, int tuple, int strike);
  int glsl_attributeLocation(void *p, char *name);
  int glsl_uniformLocation(void *p, char *name);
}

// ---------------------------------------------------------------------
Opengl2::Opengl2(Child *c)
{
  pchild = c;
  nopaint=false;
  jpaint=false;
  epaint=false;
  initialized = false;
  setAttribute(Qt::WA_DeleteOnClose);
//  this->setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
//  this->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
  updateGeometry();
  setMouseTracking (true);         // for mmove event
  setFocusPolicy(Qt::StrongFocus);  // for char event

  setAttribute(Qt::WA_PaintOnScreen);
  setAttribute(Qt::WA_NoSystemBackground);
  setAutoBufferSwap (false);

  timer = new QTimer();
  connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

// ---------------------------------------------------------------------
void Opengl2::timeout()
{

}

// ---------------------------------------------------------------------
void Opengl2::initializeGL()
{
//  qDebug() << "opengl2 initializeGL";

  pchild->event="initialize";
  pchild->pform->signalevent(pchild);

//  qDebug() << "opengl2 initializeGL exit";
  initialized = true;

}


// ---------------------------------------------------------------------
void Opengl2::paintgl()
{
  this->paintGL();
}

// ---------------------------------------------------------------------
void Opengl2::paintGL()
{
  if (!initialized) return;

//  qDebug() << "opengl2 paintGL";

  QPainter painter;
  painter.begin(this);
  painter.beginNativePainting();

  epaint=true;
  if (!nopaint) {
  jpaint=true;
  pchild->event="paint";
  pchild->pform->signalevent(pchild);
  jpaint=false;
  }

  painter.endNativePainting();
  painter.end();
  swapBuffers();
  epaint=false;

//  qDebug() << "opengl2 paintGL exit";
}

// ---------------------------------------------------------------------
void Opengl2::resizeGL ( int width, int height )
{
//  qDebug() << "opengl2 resizeGL";
//  qDebug() << "opengl2 resizeGL new size " + QString::number(width) + " " + QString::number(height);

  pchild->event="resize";
  pchild->pform->signalevent(pchild);

//  qDebug() << "opengl2 resizeGL exit";
}

// ---------------------------------------------------------------------
void Opengl2::buttonEvent (QEvent::Type type, QMouseEvent *event)
{
  opengl=(Opengl *)pchild;

  string lmr = "";
  switch (event->button()) {
  case Qt::LeftButton:
    lmr = "l";
    break;
  case Qt::MidButton:
    lmr = "m";
    break;
  case Qt::RightButton:
    lmr = "r";
    break;
  default:
    break;
  }

  string evtname = "mmove";
  switch (type) {
  case QEvent::MouseButtonPress:
    evtname = "mb" + lmr + "down";
    break;
  case QEvent::MouseButtonRelease:
    evtname = "mb" + lmr + "up";
    break;
  case QEvent::MouseButtonDblClick:
    evtname = "mb" + lmr + "dbl";
    break;
  case QEvent::MouseMove:
    evtname = "mmove";
    break;
  default:
    break;
  }

  // sysmodifiers = shift+2*control
  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysmodifiers[20];
  sprintf (sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[200];
  sprintf (sysdata , "%d %d %d %d %d %d %d %d %d %d %d %d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);

  pchild->event=evtname;
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::wheelEvent (QWheelEvent *event)
{
  opengl=(Opengl *)pchild;

  char deltasign = ' ';
  int delta = event->delta() / 8;  // degree
  if (delta<0) {
    delta = -delta;
    deltasign = '_';
  }

  // sysmodifiers = shift+2*control
  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysmodifiers[20];
  sprintf (sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[200];
  sprintf (sysdata , "%d %d %d %d %d %d %d %d %d %d %d %c%d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, deltasign, delta);

  pchild->event=string("mwheel");
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::mousePressEvent(QMouseEvent *event)
{
  buttonEvent (QEvent::MouseButtonPress, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseMoveEvent(QMouseEvent *event)
{
  buttonEvent (QEvent::MouseMove, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseDoubleClickEvent(QMouseEvent *event)
{
  buttonEvent (QEvent::MouseButtonDblClick, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseReleaseEvent(QMouseEvent *event)
{
  buttonEvent (QEvent::MouseButtonRelease, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseWheelEvent(QWheelEvent *event)
{
  wheelEvent (event);
}

// ---------------------------------------------------------------------
void Opengl2::focusInEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focus";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::focusOutEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focuslost";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::keyPressEvent(QKeyEvent *event)
{
  // sysmodifiers = shift+2*control
  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,0
  char sysmodifiers[20];
  sprintf (sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[20];
  QString keyt = event->text();
  if (keyt.size())
    sprintf (sysdata , "%s", keyt.toUtf8().constData());
  else sprintf (sysdata , "%s", QString(QChar(event->key())).toUtf8().constData());

  pchild->event=string("char");
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
  QWidget::keyPressEvent(event);
}

// ---------------------------------------------------------------------
Opengl2::~Opengl2()
{
//  qDebug() << "opengl2 deleted";

  delete timer;
  for (int i=0; i<programs.size(); i++)
    delete programs.at(i);
  if (pchild==(Child *)opengl) {
    opengl=0;
//    qDebug() << "opengl=0";
  }
}

// ---------------------------------------------------------------------
int gl_sel(void *g)
{
  if (!g) return 1;
  Form *f;
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (f->ischild((Child *)g)) {
//      qDebug() << "gl_sel ok "+ s2q(f->child->id);
      opengl = (Opengl *) g;
      ((Opengl2 *)opengl->widget)->makeCurrent();
      f->child = (Child *) g;
      form = f;
      return 0;
    }
  }
  qDebug() << "gl_sel failed " + QString::number((SI)g);
  return 0;
}

// ---------------------------------------------------------------------
int gl_sel2(char *g)
{
  Child *cc;
  if (!g) return 1;
  string p=string(g);
  if (p.size()==0) {
//    qDebug() << "gl_sel2 empty ok";
    return 0;
  }
  Form *f;
  string q=p;
  if (q[0]=='_') q[0]='-';
  Child *n=(Child *) strtol(q.c_str(),NULL,0);
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (f->ischild(n)) {
//      qDebug() << "gl_sel2 ok "+ s2q(f->child->id);
      opengl = (Opengl *) n;
      ((Opengl2 *)opengl->widget)->makeCurrent();
      f->child = (Child *) n;
      form=f;
      return 0;
    } else if ((cc=f->id2child(g))) {
//      qDebug() << "gl_sel2 ok "+ s2q(f->child->id);
      opengl = (Opengl *) cc;
      ((Opengl2 *)opengl->widget)->makeCurrent();
      f->child = (Child *) cc;
      form=f;
      return 0;
    }
  }
  qDebug() << "gl_sel2 failed "+ s2q(string(g));
  return 0;
}

// ---------------------------------------------------------------------
int gl_qwh(int *wh)
{
  if (!(wh && opengl && opengl->widget)) return 1;
    wh[0] = ((Opengl2 *)opengl->widget)->width();
    wh[1] = ((Opengl2 *)opengl->widget)->height();
  return 0;
}

// ---------------------------------------------------------------------
int gl_paint()
{
  return 0;
//  qDebug() << "gl_paint";
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)) return 1;
  if (((Opengl2 *)opengl->widget)->epaint) return 1;
//  qDebug() << "gl_paint begin";
  ((Opengl2 *)opengl->widget)->nopaint=true;
  ((Opengl2 *)opengl->widget)->paintgl();
  ((Opengl2 *)opengl->widget)->nopaint=false;
//  qDebug() << "gl_paint end";
  return 0;
}

// ---------------------------------------------------------------------
int gl_paintx()
{
//  qDebug() << "gl_paintx";
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)) return 1;
  if (((Opengl2 *)opengl->widget)->jpaint) return 1;
  ((Opengl2 *)opengl->widget)->updateGL();
  return 0;
}

// ---------------------------------------------------------------------
int gl_qhandles(void **p)
{
  if (!opengl) return 1;
  *p = (void *)opengl;
  return 0;
}

// ---------------------------------------------------------------------
int gl_setlocale (char *c)
{
  if (!opengl) return 1;
  opengl->locale = string(c);
  return 0;
}

// ---------------------------------------------------------------------
void *glsl(char *vsrc, char *fsrc)
{
  QGLShaderProgram *program = new QGLShaderProgram;
  if (vsrc && strlen(vsrc)) {
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, ((Opengl2 *)opengl->widget));
    vshader->compileSourceCode(vsrc);
    program->addShader(vshader);
  }
  if (fsrc && strlen(fsrc)) {
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, ((Opengl2 *)opengl->widget));
    fshader->compileSourceCode(fsrc);
    program->addShader(fshader);
  }
  program->link();
  ((Opengl2 *)opengl->widget)->programs.append(program);
  return (void *)program;
}

// ---------------------------------------------------------------------
int glsl_bind(void *p)
{
  return ((QGLShaderProgram *)p)->bind();
}

// ---------------------------------------------------------------------
void glsl_release(void *p)
{
  ((QGLShaderProgram *)p)->release();
}

// ---------------------------------------------------------------------
void glsl_setUniformValue_f(void *p, int matrixUniform, GLfloat data)
{
  ((QGLShaderProgram *)p)->setUniformValue(matrixUniform, data);
}

// ---------------------------------------------------------------------
void glsl_setUniformValue_f22(void *p, int matrixUniform, GLfloat *data)
{
  GLfloat t[2][2];
  for (int i=0; i<2; i++)
    for (int j=0; j<2; j++)
      t[i][j] = data[2*j+i];
  ((QGLShaderProgram *)p)->setUniformValue(matrixUniform, t);
}

// ---------------------------------------------------------------------
void glsl_setUniformValue_f33(void *p, int matrixUniform, GLfloat *data)
{
  GLfloat t[3][3];
  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++)
      t[i][j] = data[3*j+i];
  ((QGLShaderProgram *)p)->setUniformValue(matrixUniform, t);
}

// ---------------------------------------------------------------------
void glsl_setUniformValue_f44(void *p, int matrixUniform, GLfloat *data)
{
  GLfloat t[4][4];
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      t[i][j] = data[4*j+i];
  ((QGLShaderProgram *)p)->setUniformValue(matrixUniform, t);
}

// ---------------------------------------------------------------------
void glsl_enableAttributeArray(void *p, int attr)
{
  ((QGLShaderProgram *)p)->enableAttributeArray(attr);
}

// ---------------------------------------------------------------------
void glsl_disableAttributeArray(void *p, int attr)
{
  ((QGLShaderProgram *)p)->disableAttributeArray(attr);
}

// ---------------------------------------------------------------------
void glsl_setAttributeArray(void *p, int attr, GLfloat *data, int tuple, int strike)
{
  ((QGLShaderProgram *)p)->setAttributeArray(attr, data, tuple, strike);
}

// ---------------------------------------------------------------------
int glsl_attributeLocation(void *p, char *name)
{
  return ((QGLShaderProgram *)p)->attributeLocation(name);
}

// ---------------------------------------------------------------------
int glsl_uniformLocation(void *p, char *name)
{
  return ((QGLShaderProgram *)p)->uniformLocation(name);
}
