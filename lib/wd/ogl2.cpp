#define GLOPENGL
#include "gl2class.h"

// ---------------------------------------------------------------------
int gl_updategl(void *g)
{
  if (!g) return 1;
  Form *f;
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (f->ischild((Child *)g)) {
      if ((((Child *)g)->type == "opengl") && ((Child *)g)->widget) {
#ifdef QT54
        ((Opengl2 *)(((Opengl *) g)->widget))->update();
#else
        ((Opengl2 *)(((Opengl *) g)->widget))->updateGL();
#endif
        return 0;
      }
    }
  }
  qDebug() << "gl_updategl failed " + QString::number((SI)g);
  return 1;
}
