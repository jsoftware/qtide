#ifndef WSSVR_H
#define WSSVR_H

// #include <QtCore>
// #include <QtNetwork>

#ifdef QT50
#include <QtWebSockets/QtWebSockets>
#else
#include "../QtWebsocket/QWsServer.h"
#include "../QtWebsocket/QWsSocket.h"
#define QWebSocket QtWebsocket::QWsSocket
#define QWebSocketServer QtWebsocket::QWsServer
#define sendTextMessage write
#define sendBinaryMessage write
#endif

#include "jsvr.h"

class WsSvr : public QObject
{
  Q_OBJECT

public:
  WsSvr(int port = 80, int secureMode = 1);  // 0=SecureMode 1=NonSecureMode
  ~WsSvr();
  void disconnect(void * client);
  I write(void * client, const char * msg, I len, bool binary);
  std::string querySocket();
  std::string errstring;
  bool hasSocket(void * client);
#ifdef QT50
  std::string state(void * client);

Q_SIGNALS:
  void closed();
#endif

private Q_SLOTS:
  void onNewConnection();
  void onDisconnected();
  void onTextMessageReceived(QString message);
  void onBinaryMessageReceived(QByteArray message);
  void onError(QAbstractSocket::SocketError error);
  void onSslErrors(const QList<QSslError>& errors);
  void onStateChanged(QAbstractSocket::SocketState socketState);
  void onPong(quint64 elapsedTime, const QByteArray & payload);
#ifdef QT50
  void onClosed();
#endif

private:
  QWebSocketServer *server;
  QList<QWebSocket *> clients;
  void messageReceived(QWebSocket* socket, QByteArray ba, bool binary);
};

#endif
