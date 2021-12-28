#ifndef WSSVR_H
#define WSSVR_H

// #include <QtCore>
// #include <QtNetwork>

#include <QtWebSockets/QtWebSockets>

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
  std::string state(void * client);

Q_SIGNALS:
  void closed();

private Q_SLOTS:
  void onNewConnection();
  void onDisconnected();
  void onTextMessageReceived(QString message);
  void onBinaryMessageReceived(QByteArray message);
  void onError(QAbstractSocket::SocketError error);
  void onSslErrors(const QList<QSslError>& errors);
  void onStateChanged(QAbstractSocket::SocketState socketState);
  void onPong(quint64 elapsedTime, const QByteArray & payload);
  void onClosed();

private:
  QWebSocketServer *server;
  QList<QWebSocket *> clients;
  void messageReceived(QWebSocket* socket, QByteArray ba, bool binary);
};

#endif
