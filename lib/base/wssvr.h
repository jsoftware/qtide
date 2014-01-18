#ifndef WSSVR_H
#define WSSVR_H

#include <QtCore>
#include <QtNetwork>

#include "../QtWebsocket/QWsServer.h"
#include "../QtWebsocket/QWsSocket.h"

#include "jsvr.h"

class WsSvr : public QObject
{
  Q_OBJECT

public:
  WsSvr(int port = 80, int protocol = 1);
  ~WsSvr();
  void write(void * socket, const char * msg, I len, bool binary);

public slots:
  void socketNewConnection();
  void socketDisconnected();
  void socketFrameReceived(QString message);
  void socketFrameReceived(QByteArray message);
  void socketPong(quint64 elapsedTime);
  void socketSslErrors(const QList<QSslError>& errors);

private:
  QtWebsocket::QWsServer* server;
  QList<QtWebsocket::QWsSocket*> clients;
  void frameReceived(QtWebsocket::QWsSocket* socket, QByteArray ba, bool binary);
};

extern WsSvr *wssvr;

#endif
