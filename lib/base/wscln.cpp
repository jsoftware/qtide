// for debug only
// #define DEBUG_WEBSOCKET

#ifdef DEBUG_WEBSOCKET
#include <QDebug>
#endif

#include "wscln.h"
#include "jsvr.h"
#include "util.h"

using namespace std;

#define   ONOPEN        0
#define   ONCLOSE       1
#define   ONMESSAGE     2
#define   ONERROR       3
#define   ONSSLERROR    4
#define   ONSTATECHANGE 5
#define   ONPONG        6

extern WsCln *wscln;

WsCln::WsCln() : servers()
{
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Websocket client started");
#endif
}

WsCln::~WsCln()
{
  QWebSocket* socket;
  foreach (socket, servers) {
#ifdef QT53
    socket->close();
#else
    socket->disconnectFromHost();
#endif
  }
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Websocket client terminated");
#endif
}

void * WsCln::openurl(QString url)
{
  int port=80;
  url.trimmed();
  if (url.isEmpty()) {
#ifdef DEBUG_WEBSOCKET
    qDebug() << QString("empty url");
#endif
    return (void *) 0;
  } else if (!(url.startsWith("ws://", Qt::CaseInsensitive)||url.startsWith("wss://", Qt::CaseInsensitive))) {
#ifdef DEBUG_WEBSOCKET
    qDebug() << QString("url not ws:// ro wss://");
#endif
    return (void *) 0;
  }
  if (url.startsWith("wss://", Qt::CaseInsensitive)) port=443;

#ifdef QT53
  QWebSocket* socket = new QWebSocket();
#else
  QWebSocket* socket = new QWebSocket(this, NULL, QtWebsocket::WS_V13);
#endif

  QObject::connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
  QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
  QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
  QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
  QObject::connect(socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onSslErrors(const QList<QSslError>&)));
#ifdef QT53
  QObject::connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
  QObject::connect(socket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(onBinaryMessageReceived(QByteArray)));
#else
  QObject::connect(socket, SIGNAL(frameReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
  QObject::connect(socket, SIGNAL(frameReceived(QByteArray)), this, SLOT(onBinaryMessageReceived(QByteArray)));
#endif

  servers << socket;

#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("try connect to: ") + url;
#endif

#ifdef QT53
  socket->open(QUrl(url));
#else
  if (2>url.count(QChar(':'))) {
    socket->connectToHost(url.toUtf8(), port);
  } else {
    QStringList n = url.split(":",QString::SkipEmptyParts);
    QString p;
    int i;
    if (0<=(i=n.at(2).lastIndexOf(QChar('/')))) {
      port=n.at(2).left(i).toInt();
      p = n.at(0)+":"+n.at(1)+n.at(2).mid(i);
    } else {
      port=n.at(2).toInt();
      p = n.at(0)+":"+n.at(1);
    }
#ifdef DEBUG_WEBSOCKET
    qDebug() << QString("parsed url: ") + p + " port: " + QString::number(port);
#endif
    socket->connectToHost(p.toUtf8(), port);
  }
#endif

  return (void *) socket;
}

void WsCln::onConnected()
{
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket == 0) {
    return;
  }
  string s = "wscln_handler_z_ " + p2s((void *)ONOPEN) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Server 0x%1 connected: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif
}

void WsCln::onDisconnected()
{
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket == 0) {
    return;
  }
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Server 0x%1 disconnected: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif

  string s = "wscln_handler_z_ " + p2s((void *)ONCLOSE) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());

  servers.removeAll(socket);
  socket->deleteLater();
}

void WsCln::messageReceived(QWebSocket* socket, QByteArray ba, bool binary)
{
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Server 0x%1 message received: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));
  qDebug() << "number of bytes received: " + QString::number(ba.size());
  qDebug() << QString(ba.toHex());
#endif

  jsetc((char *)"wsc0_jrx_",(C*)ba.data(), ba.size());
  if (binary)
    jsetc((char *)"wsc1_jrx_",(C*)"binary", 6);
  else
    jsetc((char *)"wsc1_jrx_",(C*)"text", 4);
  string s = "wscln_handler_z_ " + p2s((void *)ONMESSAGE) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());
}

void WsCln::onTextMessageReceived(QString message)
{
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket == 0) {
    return;
  }
  messageReceived(socket, message.toUtf8(), false);
}

void WsCln::onBinaryMessageReceived(QByteArray message)
{
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket == 0) {
    return;
  }
  messageReceived(socket, message, true);
}

void WsCln::onError(QAbstractSocket::SocketError error)
{
  Q_UNUSED(error);
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket == 0) {
    return;
  }
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Server 0x%1 error: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif

  string s = "wscln_handler_z_ " + p2s((void *)ONERROR) + " " + p2s((void *)socket);
  string er = q2s(socket->errorString()) + '\012';
  jsetc((char *)"wsc0_jrx_",(C*)er.c_str(), er.size());
  jsetc((char *)"wsc1_jrx_",(C*)"text", 4);
  jedo((char *)s.c_str());
}

void WsCln::onSslErrors(const QList<QSslError> &errors)
{
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket == 0) {
    return;
  }
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Server 0x%1 ssl error: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif

  string s = "wscln_handler_z_ " + p2s((void *)ONSSLERROR) + " " + p2s((void *)socket);
  string er = "";
  for (int i=0, sz=errors.size(); i<sz; i++) {
    er = er + q2s(errors.at(i).errorString()) + '\012';
  }
  jsetc((char *)"wsc0_jrx_",(C*)er.c_str(), er.size());
  jsetc((char *)"wsc1_jrx_",(C*)"text", 4);
  jedo((char *)s.c_str());
}

void WsCln::onStateChanged(QAbstractSocket::SocketState socketState)
{
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
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

#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Server 0x%1 statechange: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0')) << s2q(st);;
#endif
  string s = "wscln_handler_z_ " + p2s((void *)ONSTATECHANGE) + " " + p2s((void *)socket);
  jsetc((char *)"wsc0_jrx_",(C*)st.c_str(), st.size());
  jsetc((char *)"wsc1_jrx_",(C*)"text", 4);
  jedo((char *)s.c_str());
}

void WsCln::onPong(quint64 elapsedTime, const QByteArray & payload)
{
  Q_UNUSED(elapsedTime);
  Q_UNUSED(payload);
  QWebSocket* socket = qobject_cast<QWebSocket *>(sender());
  if (socket == 0) {
    return;
  }
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("ping: %1 ms").arg(elapsedTime);
#endif

// no need to call J until there is a reason
//  string s = "wscln_handler_z_ " + p2s((void *)ONPONG) + " " + p2s((void *)socket);
//  jedo((char *)s.c_str());
}

string WsCln::querySocket()
{
  QWebSocket* socket;
  string s = "";
  foreach (socket, servers) {
    s = s + p2s((void *)socket) + '\012';
  }
  return s;
}

bool WsCln::hasSocket(void * server)
{
  QWebSocket* socket = (QWebSocket*)server;
  return servers.contains(socket);
}

void WsCln::disconnect(void * server)
{
  QWebSocket* socket;
  if (server) {
    socket = (QWebSocket*)server;
    if (servers.contains(socket))
#ifdef QT53
      socket->close();
#else
      socket->disconnectFromHost();
#endif
  }
}

I WsCln::write(void * server, const char * msg, I len, bool binary)
{
  I r = -1;
  QByteArray ba;
  QString s;
  QWebSocket* socket;
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Server 0x%1 write: ").arg((quintptr)server , QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif
  if (binary)
    ba = QByteArray(msg,len);
  else
    s = QString::fromUtf8(msg,len);
  if (server) {
    socket = (QWebSocket*)server;
    if (servers.contains(socket)) {
      if (binary)
        r = socket->sendBinaryMessage(ba);
      else
        r = socket->sendTextMessage(s);
    }
  } else {
    foreach (socket, servers) {
      if (binary)
        r = socket->sendBinaryMessage(ba);
      else
        r = socket->sendTextMessage(s);
    }
  }
  return r;
}
