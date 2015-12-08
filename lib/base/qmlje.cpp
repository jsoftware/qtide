
#ifdef QT50
#include <QtQml>
#endif

#include "qmlje.h"
#include "jsvr.h"

QmlJE::QmlJE(QObject* parent) : QObject(parent)
{
}

QmlJE::~QmlJE()
{
}

// ---------------------------------------------------------------------
QString QmlJE::verb(QString v,QString y,bool ignoreResult)
{
  QString res="";
  int rc;
  setvar("qmly_jrx_",y);
  if (!ignoreResult)
    rc=jedo(("qmldors_jrx_=:"+v+" qmly_jrx_").toUtf8().data());
  else rc=jedo((v+" qmly_jrx_").toUtf8().data());
  jedo((char *)"4!:55<'qmly_jrx_'");
  if (!ignoreResult) {
    if (!rc) res=getvar("qmldors_jrx_");
    jedo((char *)"4!:55<'qmldors_jrx_'");
  }
  return res;
}

// ---------------------------------------------------------------------
int QmlJE::dor(QString s)
{
  return jedo(s.toUtf8().data());
}

// ---------------------------------------------------------------------
QString QmlJE::dors(QString s)
{
  QString res="";
  int rc=jedo(("qmldors_jrx_=:"+s).toUtf8().data());
  if (!rc) res=getvar("qmldors_jrx_");
  jedo((char *)"4!:55<'qmldors_jrx_'");
  return res;
}

// ---------------------------------------------------------------------
QString QmlJE::getvar(QString n)
{
  char *r;
  I len;
  r=jgetc(n.toUtf8().data(), &len);
  if (r&&len)
    return QString::fromUtf8(r,len);
  else return "";
}

// ---------------------------------------------------------------------
void QmlJE::setvar(QString n,QString s)
{
  jsetc(n.toUtf8().data(), s.toUtf8().data(), s.toUtf8().size());
}

#ifdef QT50
// define the singleton type provider function (callback).
static QObject *qmlje_qobject_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  QmlJE *qmlje = new QmlJE();
  return qmlje;
}

void regQmlJE()
{
  qmlRegisterSingletonType<QmlJE>("com.jsoftware.qtide.qmljengine", 1, 0, "QmlJE", qmlje_qobject_singletontype_provider);
}

#endif

