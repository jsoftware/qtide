#include <QDebug>

#include "wscln.h"
#include "jsvr.h"
#include "util.h"

using namespace std;

WsCln *wscln;

#define   ONOPEN        0
#define   ONCLOSE       1
#define   ONMESSAGE     2
#define   ONERROR       3
#define   ONSTATECHANGE 4
#define   ONPONG        5

WsCln::WsCln()
{
  qDebug() << QString("Websocket client terminated");
}

WsCln::~WsCln()
{
  QtWebsocket::QWsSocket* socket;
  foreach (socket, servers) {
    servers.removeOne(socket);
    socket->deleteLater();
  }
  qDebug() << QString("Websocket client terminated");
}

void * WsCln::connect(QString ipaddr)
{
  ipaddr.trimmed();
  if (ipaddr.isEmpty()) {
    qDebug() << QString("empty ip address");
    return (void *) 0;
  }

  qDebug() << QString("try connect to: ") + ipaddr;

  QString ip;
  quint16 port;
  if (ipaddr.contains(QRegExp(QLatin1String(":([0-9]|[1-9][0-9]{1,3}|[1-5][0-9]{1,4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$")))) {
    QStringList splitted = ipaddr.split(':');
    port = splitted.takeLast().toUInt();
    ip = splitted.join(':');
  } else {
    ip = ipaddr;
    port = 80;
  }

  QtWebsocket::QWsSocket* socket = new QtWebsocket::QWsSocket(this, NULL, QtWebsocket::WS_V13);
  servers << socket;
  socket->connectToHost(ip.toUtf8(), port);


  onStateChange(socket->state());

  QObject::connect(socket, SIGNAL(connected()), this, SLOT(onOpen()));
  QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onClose()));
  QObject::connect(socket, SIGNAL(frameReceived(QString)), this, SLOT(onMessage(QString)));
  QObject::connect(socket, SIGNAL(frameReceived(QByteArray)), this, SLOT(onMessage(QByteArray)));
  QObject::connect(socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onError(const QList<QSslError>&)));
  QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChange(QAbstractSocket::SocketState)));

  return (void *) socket;
}

void WsCln::disconnect(void * server)
{
  QtWebsocket::QWsSocket* socket;
  if (server) {
    socket = (QtWebsocket::QWsSocket*)server;
    if (servers.contains(socket))
      socket->disconnectFromHost();
  }
}

void WsCln::onOpen()
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  string s = "wscln_handler_z_ " + p2s((void *)ONOPEN) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());
  qDebug() << QString("Server 0x%1 connected: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));
}

void WsCln::onClose()
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  qDebug() << QString("Server 0x%1 disconnected: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));

  string s = "wscln_handler_z_ " + p2s((void *)ONCLOSE) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());

  servers.removeOne(socket);
  socket->deleteLater();
}

void WsCln::frameReceived(QtWebsocket::QWsSocket* socket, QByteArray ba, bool binary)
{
  qDebug() << QString("Server 0x%1 frame received: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));

  jsetc((char *)"wscln0_jrx_",(C*)ba.data(), ba.size());
  if (binary)
    jsetc((char *)"wscln1_jrx_",(C*)"binary", 6);
  else
    jsetc((char *)"wscln1_jrx_",(C*)"utf8", 4);
  string s = "wscln_handler_z_ " + p2s((void *)ONMESSAGE) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());
  I len0,len1;
  C *data0, *data1;
  data0 = jgetc((char *)"wscln0_jrx_", &len0);
  data1 = jgetc((char *)"wscln1_jrx_", &len1);
  if (string("binary")==string(data1))
    socket->write(QByteArray(data0,(int)len0));
  else
    socket->write(QString::fromUtf8(data0,(int)len0));
}

void WsCln::onMessage(QString frame)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  frameReceived(socket, frame.toUtf8(), false);
}

void WsCln::onMessage(QByteArray frame)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  frameReceived(socket, frame, true);
}

void WsCln::onError(const QList<QSslError>& errors)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  qDebug() << QString("Server 0x%1 ssl error: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));

  string s = "wscln_handler_z_ " + p2s((void *)ONERROR) + " " + p2s((void *)socket);
  string er = "";
  for (int i=0, sz=errors.size(); i<sz; i++) {
    er = er + q2s(errors.at(i).errorString()) + '\012';
  }
  jsetc((char *)"wscln0_jrx_",(C*)er.c_str(), er.size());
  jsetc((char *)"wscln1_jrx_",(C*)"utf8", 4);
  jedo((char *)s.c_str());
}

void WsCln::onStateChange(QAbstractSocket::SocketState socketState)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  qDebug() << QString("Server 0x%1 statechange: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));
  string st;
  switch (socketState) {
  case QAbstractSocket::UnconnectedState:
    st = "Unconnected";
    break;
  case QAbstractSocket::HostLookupState:
    st = "HostLookup";
    break;
  case QAbstractSocket::ConnectingState:
    st = "Connecting";
    break;
  case QAbstractSocket::ConnectedState:
    st = "Connected";
    break;
  case QAbstractSocket::BoundState:
    st = "Bound";
    break;
  case QAbstractSocket::ClosingState:
    st = "Closing";
    break;
  case QAbstractSocket::ListeningState:
    st = "Listening";
    break;
  default:
    st = "Unknown";
    break;
  }

  string s = "wscln_handler_z_ " + p2s((void *)ONSTATECHANGE) + " " + p2s((void *)socket);
  jsetc((char *)"wscln0_jrx_",(C*)st.c_str(), st.size());
  jsetc((char *)"wscln1_jrx_",(C*)"utf8", 4);
  jedo((char *)s.c_str());
}

string WsCln::queryServer()
{
  QtWebsocket::QWsSocket* socket;
  string s = "";
  foreach (socket, servers) {
    s = s + p2s((void *)socket) + '\012';
  }
  return s;
}

void WsCln::write(void * server, const char * msg, I len, bool binary)
{
  QByteArray ba;
  QString s;
  QtWebsocket::QWsSocket* socket;
  if (binary)
    ba = QByteArray(msg,len);
  else
    s = QString::fromUtf8(msg,len);
  if (server) {
    socket = (QtWebsocket::QWsSocket*)server;
    if (servers.contains(socket)) {
      if (binary)
        socket->write(ba);
      else
        socket->write(s);
    }
  } else {
    foreach (socket, servers) {
      if (binary)
        socket->write(ba);
      else
        socket->write(s);
    }
  }
}
