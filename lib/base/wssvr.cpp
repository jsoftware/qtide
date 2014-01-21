#include <QDebug>

#include "wssvr.h"
#include "jsvr.h"
#include "util.h"

using namespace std;

WsSvr *wssvr;

#define   ONOPEN        0
#define   ONCLOSE       1
#define   ONMESSAGE     2
#define   ONERROR       3
#define   ONSTATECHANGE 4
#define   ONPONG        5

WsSvr::WsSvr(int port, int protocol)
{
  server = new QtWebsocket::QWsServer(this, (QtWebsocket::Protocol)protocol);
  if (! server->listen(QHostAddress::Any, port)) {
    qDebug() << "Error: Can't launch websocket server";
    qDebug() << QString("QWsServer error : %1").arg(server->errorString());
  } else {
    qDebug() << QString("Websocket server is listening on port %1").arg(port);
  }
  QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onOpen()));
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

void WsSvr::onOpen()
{
  QtWebsocket::QWsSocket* clientSocket = server->nextPendingConnection();

  QObject::connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onClose()));
  QObject::connect(clientSocket, SIGNAL(frameReceived(QString)), this, SLOT(onMessage(QString)));
  QObject::connect(clientSocket, SIGNAL(frameReceived(QByteArray)), this, SLOT(onMessage(QByteArray)));
  QObject::connect(clientSocket, SIGNAL(pong(quint64)), this, SLOT(onPong(quint64)));
  QObject::connect(clientSocket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onError(const QList<QSslError>&)));

  clients << clientSocket;
  string s = "wssvr_handler_z_ " + p2s((void *)ONOPEN) + " " + p2s((void *)clientSocket);
  jedo((char *)s.c_str());
  qDebug() << QString("Client 0x%1 connected: ").arg((quintptr)clientSocket , QT_POINTER_SIZE * 2, 16, QChar('0'));
}

void WsSvr::onClose()
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  qDebug() << QString("Client 0x%1 disconnected: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));

  string s = "wssvr_handler_z_ " + p2s((void *)ONCLOSE) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());

  clients.removeOne(socket);
  socket->deleteLater();
}

void WsSvr::frameReceived(QtWebsocket::QWsSocket* socket, QByteArray ba, bool binary)
{
  qDebug() << QString("Client 0x%1 frame received: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));

  jsetc((char *)"wssvr0_jrx_",(C*)ba.data(), ba.size());
  if (binary)
    jsetc((char *)"wssvr1_jrx_",(C*)"binary", 6);
  else
    jsetc((char *)"wssvr1_jrx_",(C*)"utf8", 4);
  string s = "wssvr_handler_z_ " + p2s((void *)ONMESSAGE) + " " + p2s((void *)socket);
  jedo((char *)s.c_str());
  I len0,len1;
  C *data0, *data1;
  data0 = jgetc((char *)"wssvr0_jrx_", &len0);
  data1 = jgetc((char *)"wssvr1_jrx_", &len1);
  if (string("binary")==string(data1))
    socket->write(QByteArray(data0,(int)len0));
  else
    socket->write(QString::fromUtf8(data0,(int)len0));
}

void WsSvr::onMessage(QString frame)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  frameReceived(socket, frame.toUtf8(), false);
}

void WsSvr::onMessage(QByteArray frame)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  frameReceived(socket, frame, true);
}

void WsSvr::onError(const QList<QSslError>& errors)
{
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  qDebug() << QString("Client 0x%1 ssl error: ").arg((quintptr)socket , QT_POINTER_SIZE * 2, 16, QChar('0'));

  string s = "wssvr_handler_z_ " + p2s((void *)ONERROR) + " " + p2s((void *)socket);
  string er = "";
  for (int i=0, sz=errors.size(); i<sz; i++) {
    er = er + q2s(errors.at(i).errorString()) + '\012';
  }
  jsetc((char *)"wssrv0_jrx_",(C*)er.c_str(), er.size());
  jsetc((char *)"wssvr1_jrx_",(C*)"utf8", 4);
  jedo((char *)s.c_str());
}

void WsSvr::onPong(quint64 elapsedTime)
{
  Q_UNUSED(elapsedTime);
  QtWebsocket::QWsSocket* socket = qobject_cast<QtWebsocket::QWsSocket*>(sender());
  if (socket == 0) {
    return;
  }
  qDebug() << QString("ping: %1 ms").arg(elapsedTime);

// no need to call J until there is a reason
//  string s = "wssvr_handler_z_ " + p2s((void *)ONPONG) + " " + p2s((void *)socket);
//  jedo((char *)s.c_str());
}

string WsSvr::queryClient()
{
  QtWebsocket::QWsSocket* client;
  string s = "";
  foreach (client, clients) {
    s = s + p2s((void *)client) + '\012';
  }
  return s;
}

void WsSvr::write(void * client, const char * msg, I len, bool binary)
{
  QByteArray ba;
  QString s;
  QtWebsocket::QWsSocket* socket;
  if (binary)
    ba = QByteArray(msg,len);
  else
    s = QString::fromUtf8(msg,len);
  if (client) {
    socket = (QtWebsocket::QWsSocket*)client;
    if (clients.contains(socket)) {
      if (binary)
        socket->write(ba);
      else
        socket->write(s);
    }
  } else {
    foreach (socket, clients) {
      if (binary)
        socket->write(ba);
      else
        socket->write(s);
    }
  }
}