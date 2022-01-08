#ifndef WSCLN_H
#define WSCLN_H

// #include <QtCore>
// #include <QtNetwork>

#ifdef QT50
#include <QtWebSockets/QtWebSockets>
#else
#include "../QtWebsocket/QWsSocket.h"
#define QWebSocket QtWebsocket::QWsSocket
#define QWebSocketServer QtWebsocket::QWsServer
#define sendTextMessage write
#define sendBinaryMessage write
#endif

#include "jsvr.h"

class WsCln : public QObject
{
  Q_OBJECT

public:
  WsCln();
  ~WsCln();
  void * openurl(QString url);
  void disconnect(void * server);
  I write(void * server, const char * msg, I len, bool binary);
  std::string querySocket();
  bool hasSocket(void * server);
#ifdef QT50
  std::string state(void * server);
#endif

private Q_SLOTS:
  void onConnected();
  void onDisconnected();
  void onTextMessageReceived(QString message);
  void onBinaryMessageReceived(QByteArray message);
  void onError(QAbstractSocket::SocketError error);
  void onSslErrors(const QList<QSslError> &errors);
  void onStateChanged(QAbstractSocket::SocketState socketState);
  void onPong(quint64 elapsedTime, const QByteArray & payload);

private:
  QList<QWebSocket *> servers;
  void messageReceived(QWebSocket* socket, QByteArray ba, bool binary);
};

#endif
