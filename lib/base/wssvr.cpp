#include <QDebug>

#include "wssvr.h"
#include "jsvr.h"
#include "util.h"

using namespace std;

WsSvr *wssvr;

#define   NEWCONNECTION    0
#define   DISCONNECTED     1
#define   FRAMERECEIVED    2
#define   PONG             3
#define   SSLERRORS        4

WsSvr::WsSvr(int port, int protocol)
{
  server = new QtWebsocket::QWsServer(this, (QtWebsocket::Protocol)protocol);
  if (! server->listen(QHostAddress::Any, port)) {
    qDebug() << "Error: Can't launch websocket server";
    qDebug() << QString("QWsServer error : %1").arg(server->errorString());
  } else {
    qDebug() << QString("Websocket server is listening on port %1").arg(port);
  }
  QObject::connect(server, SIGNAL(newConnection()), this, SLOT(socketNewConnection()));
}

WsSvr::~WsSvr()
{
  QtWebsocket::QWsSocket* client;
  foreach (client, clients) {
    clients.removeOne(client);
    client->deleteLater();
  }
  qDebug() << QString("Websocket server terminated");
}

void WsSvr::socketNewConnection()
{
  QtWebsocket::QWsSocket* clientSocket = server->nextPendingConnection();

  QObject::connect(clientSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
  QObject::connect(clientSocket, SIGNAL(frameReceived(QString)), this, SLOT(socketFrameReceived(QString)));
  QObject::connect(clientSocket, SIGNAL(frameReceived(QByteArray)), this, SLOT(socketFrameReceived(QByteArray)));
  QObject::connect(clientSocket, SIGNAL(pong(quint64)), this, SLOT(socketPong(quint64)));
  QObject::connect(clientSocket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(socketSslErrors(const QList<QSslError>&)));

  clients << clientSocket;
  string s = "websocket_handler_z_ " + p2s((void *)NEWCONNECTION) + " " + p2s((void *)clientSocket);
  jedo((char *)s.c_str());
  qDebug() << QString("Client 0x%1 connected: ").arg((quintptr)clientSocket , QT_POINTER_SIZE * 2, 16, QChar('0'));
}

void WsSvr::socketDisconnected()
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  qDebug() << QString("Client 0x%1 disconnected: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));

  string s = "websocket_handler_z_ " + p2s((void *)DISCONNECTED) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());

  clients.removeOne(socket);
  socket->deleteLater();
}

void WsSvr::frameReceived(QtWebsocket::QWsSocket* socket, QByteArray ba, bool binary)
{
  qDebug() << QString("Client 0x%1 frame received: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));

  jsetc((char *)"ws0_jrx_",(C*)ba.data(), ba.size());
  if (binary)
    jsetc((char *)"ws1_jrx_",(C*)"binary", 6);
  else
    jsetc((char *)"ws1_jrx_",(C*)"utf8", 4);
  string s = "websocket_handler_z_ " + p2s((void *)FRAMERECEIVED) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());
  I len0,len1;
  C *data0, *data1;
  data0 = jgetc((char *)"ws0_jrx_", &len0);
  data1 = jgetc((char *)"ws1_jrx_", &len1);
  if (string("binary")==string(data1))
    socket->write(QByteArray(data0,(int)len0));
  else
    socket->write(QString::fromUtf8(data0,(int)len0));
}

void WsSvr::socketFrameReceived(QString frame)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  frameReceived(socket, frame.toUtf8(), false);
}

void WsSvr::socketFrameReceived(QByteArray frame)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  frameReceived(socket, frame, true);
}

void WsSvr::socketPong(quint64 elapsedTime)
{
  Q_UNUSED(elapsedTime);
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  qDebug() << QString("ping: %1 ms").arg(elapsedTime);

  string s = "websocket_handler_z_ " + p2s((void *)PONG) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());
}

void WsSvr::socketSslErrors(const QList<QSslError>& errors)
{
  Q_UNUSED(errors);
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  qDebug() << QString("Client 0x%1 ssl error: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));

  string s = "websocket_handler_z_ " + p2s((void *)SSLERRORS) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());
}

void WsSvr::write(void * socket, const char * msg, I len, bool binary)
{
  QByteArray ba;
  QString s;
  QtWebsocket::QWsSocket* client;
  if (binary)
    ba = QByteArray(msg,len);
  else
    s = QString::fromUtf8(msg,len);
  if (socket) {
    client = (QtWebsocket::QWsSocket*)socket;
    if (clients.contains(client)) {
      if (binary)
        client->write(ba);
      else
        client->write(s);
    }
  } else {
    foreach (client, clients) {
      if (binary)
        client->write(ba);
      else
        client->write(s);
    }
  }
}
