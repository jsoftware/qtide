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

void * WsCln::connect(QString ipaddr, int port)
{
  ipaddr.trimmed();
  if (ipaddr.isEmpty()) {
    qDebug() << QString("empty ip address");
    return (void *) 0;
  }

  QtWebsocket::QWsSocket* socket = new QtWebsocket::QWsSocket(this, NULL, QtWebsocket::WS_V13);

  QObject::connect(socket, SIGNAL(connected()), this, SLOT(onOpen()));
  QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onClose()));
  QObject::connect(socket, SIGNAL(frameReceived(QString)), this, SLOT(onMessage(QString)));
  QObject::connect(socket, SIGNAL(frameReceived(QByteArray)), this, SLOT(onMessage(QByteArray)));
  QObject::connect(socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onError(const QList<QSslError>&)));
  QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChange(QAbstractSocket::SocketState)));

  servers << socket;

  qDebug() << QString("try connect to: ") + ipaddr + " port " + QString::number(port);

  socket->connectToHost(ipaddr.toUtf8(), port);

  onStateChange(socket->state());

  return (void *) socket;
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

  jsetc((char *)"wsc0_jrx_",(C*)ba.data(), ba.size());
  if (binary)
    jsetc((char *)"wsc1_jrx_",(C*)"binary", 6);
  else
    jsetc((char *)"wsc1_jrx_",(C*)"utf8", 4);
  string s = "wscln_handler_z_ " + p2s((void *)ONMESSAGE) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());
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
  jsetc((char *)"wsc0_jrx_",(C*)er.c_str(), er.size());
  jsetc((char *)"wsc1_jrx_",(C*)"utf8", 4);
  jedo((char *)s.c_str());
}

void WsCln::onStateChange(QAbstractSocket::SocketState socketState)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
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

  qDebug() << QString("Server 0x%1 statechange: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0')) << s2q(st);;
  string s = "wscln_handler_z_ " + p2s((void *)ONSTATECHANGE) + " " + p2s((void *)socket);
  jsetc((char *)"wsc0_jrx_",(C*)st.c_str(), st.size());
  jsetc((char *)"wsc1_jrx_",(C*)"utf8", 4);
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

void WsCln::disconnect(void * server)
{
  QtWebsocket::QWsSocket* socket;
  if (server) {
    socket = (QtWebsocket::QWsSocket*)server;
    if (servers.contains(socket))
      socket->disconnectFromHost();
  }
}

I WsCln::write(void * server, const char * msg, I len, bool binary)
{
  I r = -1;
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
        r = socket->write(ba);
      else
        r = socket->write(s);
    }
  } else {
    foreach (socket, servers) {
      if (binary)
        r = socket->write(ba);
      else
        r = socket->write(s);
    }
  }
  return r;
}
