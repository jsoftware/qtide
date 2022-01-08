// for debug only
// #define DEBUG_WEBSOCKET

#ifdef DEBUG_WEBSOCKET
#include <QDebug>
#endif

#include <stdint.h>
#include "wssvr.h"
#include "jsvr.h"
#include "svr.h"
#include "util.h"

// using namespace std;

#define   ONOPEN        0
#define   ONCLOSE       1
#define   ONMESSAGE     2
#define   ONERROR       3
#define   ONSSLERROR    4
#define   ONSTATECHANGE 5
#define   ONPONG        6

extern WsSvr *wssvr;

void wssvr_handler(void *, QWebSocket*);

// ---------------------------------------------------------------------
WsSvr::WsSvr(int port, int secureMode) : clients()
{
  errstring = "";
#ifdef QT50
  server = new QWebSocketServer("JWebserver", (QWebSocketServer::SslMode)secureMode, this);
#else
  Q_UNUSED(secureMode);
  server = new QWebSocketServer(this, (QtWebsocket::Protocol)1);
#endif
  if (! server->listen(QHostAddress::Any, port)) {
    errstring = q2s(server->errorString());
#ifdef DEBUG_WEBSOCKET
    qDebug() << "Error: Can't launch websocket server";
    qDebug() << QString("QWsServer error : %1").arg(server->errorString());
#endif
    return;
  } else {
#ifdef DEBUG_WEBSOCKET
    qDebug() << QString("Websocket server is listening on port %1").arg(port);
#endif
    QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
#ifdef QT50
    QObject::connect(server, SIGNAL(closed()), this, SLOT(onClosed()));
#endif
  }
}

// ---------------------------------------------------------------------
#ifdef QT50
WsSvr::~WsSvr() {}

// ---------------------------------------------------------------------
void WsSvr::onClosed()
#else
WsSvr::~WsSvr()
#endif
{
  QWebSocket* socket;
  foreach (socket, clients) {
#ifdef QT50
    socket->close();
#else
    socket->disconnectFromHost();
#endif
  }
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Websocket server terminated");
#endif
}

// ---------------------------------------------------------------------
void WsSvr::onNewConnection()
{
  QWebSocket *socket = server->nextPendingConnection();

  QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
  QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
  QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
  QObject::connect(socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onSslErrors(const QList<QSslError>&)));
#ifdef QT50
  QObject::connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
  QObject::connect(socket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(onBinaryMessageReceived(QByteArray)));
#else
  QObject::connect(socket, SIGNAL(frameReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
  QObject::connect(socket, SIGNAL(frameReceived(QByteArray)), this, SLOT(onBinaryMessageReceived(QByteArray)));
#endif

  clients << socket;
  wssvr_handler((void *)ONOPEN, socket);

#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Client 0x%1 connected: ").arg((quintptr)socket, QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif
}

// ---------------------------------------------------------------------
void WsSvr::onDisconnected()
{
  QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
  if (socket == 0) {
    return;
  }
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Client 0x%1 disconnected: ").arg((quintptr)socket, QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif

  wssvr_handler((void *)ONCLOSE, socket);

  if (socket) {
    clients.removeAll(socket);
    socket->deleteLater();
  }
}

// ---------------------------------------------------------------------
void WsSvr::messageReceived(QWebSocket* socket, QByteArray ba, bool binary)
{
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Client 0x%1 message received: ").arg((quintptr)socket, QT_POINTER_SIZE * 2, 16, QChar('0'));
  qDebug() << "number of bytes received: " + QString::number(ba.size());
  qDebug() << QString(ba.toHex());
#endif
  jsetc((char *)"wss0_jrx_",(C*)ba.data(), ba.size());
  if (binary)
    jsetc((char *)"wss1_jrx_",(C*)"binary", 6);
  else
    jsetc((char *)"wss1_jrx_",(C*)"text", 4);
  wssvr_handler((void *)ONMESSAGE, socket);
}

// ---------------------------------------------------------------------
void WsSvr::onTextMessageReceived(QString message)
{
  QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
  if (socket == 0) {
    return;
  }
  messageReceived(socket, message.toUtf8(), false);
}

// ---------------------------------------------------------------------
void WsSvr::onBinaryMessageReceived(QByteArray message)
{
  QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
  if (socket == 0) {
    return;
  }
  messageReceived(socket, message, true);
}

// ---------------------------------------------------------------------
void WsSvr::onError(QAbstractSocket::SocketError error)
{
  Q_UNUSED(error);
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket == 0) {
    return;
  }
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Client 0x%1 error: ").arg((quintptr)socket, QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif

  std::string er = q2s(socket->errorString()) + '\012';
  jsetc((char *)"wss0_jrx_",(C*)er.c_str(), er.size());
  jsetc((char *)"wss1_jrx_",(C*)"text", 4);
  wssvr_handler((void *)ONERROR, socket);

}

// ---------------------------------------------------------------------
void WsSvr::onSslErrors(const QList<QSslError>& errors)
{
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket == 0) {
    return;
  }
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Client 0x%1 ssl error: ").arg((quintptr)socket, QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif

  std::string er = "";
  for (int i=0, sz=errors.size(); i<sz; i++) {
    er = er + q2s(errors.at(i).errorString()) + '\012';
  }
  jsetc((char *)"wss0_jrx_",(C*)er.c_str(), er.size());
  jsetc((char *)"wss1_jrx_",(C*)"text", 4);
  wssvr_handler((void *)ONSSLERROR, socket);

}

// ---------------------------------------------------------------------
void WsSvr::onStateChanged(QAbstractSocket::SocketState socketState)
{
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket == 0) {
    return;
  }
  std::string st;
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
  qDebug() << QString("Client 0x%1 statechange: ").arg((quintptr)socket, QT_POINTER_SIZE * 2, 16, QChar('0')) << s2q(st);;
#endif
  jsetc((char *)"wss0_jrx_",(C*)st.c_str(), st.size());
  jsetc((char *)"wss1_jrx_",(C*)"text", 4);
  wssvr_handler((void *)ONSTATECHANGE, socket);
}

// ---------------------------------------------------------------------
void WsSvr::onPong(quint64 elapsedTime, const QByteArray & payload)
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
}

// ---------------------------------------------------------------------
std::string WsSvr::querySocket()
{
  QWebSocket* client;
  std::string s = "";
  foreach (client, clients) {
    s = s + p2s((void *)client) + '\012';
  }
  return s;
}

// ---------------------------------------------------------------------
#ifdef QT50
std::string WsSvr::state(void * client)
{
  std::string s = "";
  QWebSocket* socket = (QWebSocket*)client;
  if (!hasSocket(socket)) return s;
  s += spair("errorString", socket->errorString());
  s += spair("isValid", socket->isValid()?(std::string)"1":(std::string)"0");
  s += spair("localAddress", socket->localAddress().toString());
  s += spair("localPort", QString::number(socket->localPort()));
  s += spair("origin", socket->origin());
  s += spair("peerAddress", socket->peerAddress().toString());
  s += spair("peerName", socket->peerName());
  s += spair("peerPort", QString::number(socket->peerPort()));
  s += spair("requestUrl", socket->requestUrl().toDisplayString());
  s += spair("resourceName", socket->resourceName());
  return s;
}
#endif

// ---------------------------------------------------------------------
bool WsSvr::hasSocket(void * client)
{
  QWebSocket* socket = (QWebSocket*)client;
  return clients.contains(socket);
}

// ---------------------------------------------------------------------
void WsSvr::disconnect(void * client)
{
  QWebSocket* socket;
  if (client) {
    socket = (QWebSocket *)client;
    if (clients.contains(socket))
#ifdef QT50
      socket->close();
#else
      socket->disconnectFromHost();
#endif
  }
}

// ---------------------------------------------------------------------
I WsSvr::write(void * client, const char * msg, I len, bool binary)
{
  I r=-1;
  QByteArray ba;
  QString s;
  QWebSocket* socket;
#ifdef DEBUG_WEBSOCKET
  qDebug() << QString("Client 0x%1 write: ").arg((quintptr)client, QT_POINTER_SIZE * 2, 16, QChar('0'));
#endif
  if (binary)
    ba = QByteArray(msg,len);
  else
    s = QString::fromUtf8(msg,len);
  if (client) {
    socket = (QWebSocket *)client;
    if (clients.contains(socket)) {
      if (binary)
        r = socket->sendBinaryMessage(ba);
      else
        r = socket->sendTextMessage(s);
    }
  } else {
    foreach (socket, clients) {
      if (binary)
        r = socket->sendBinaryMessage(ba);
      else
        r = socket->sendTextMessage(s);
    }
  }
  return r;
}

// ---------------------------------------------------------------------
void wssvr_handler(void *n, QWebSocket* socket)
{
  std::string s="(i.0 0)\"_ wssvr_handler_z_ " + p2s(n) + " " + p2s((void *)socket);
  jcon->cmddo(s);
}
