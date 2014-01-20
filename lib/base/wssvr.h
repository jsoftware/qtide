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
  std::string queryClient();

public slots:
  void onOpen();
  void onClose();
  void onMessage(QString message);
  void onMessage(QByteArray message);
  void onPong(quint64 elapsedTime);
  void onError(const QList<QSslError>& errors);

private:
  QtWebsocket::QWsServer* server;
  QList<QtWebsocket::QWsSocket*> clients;
  void frameReceived(QtWebsocket::QWsSocket* socket, QByteArray ba, bool binary);
};

extern WsSvr *wssvr;

#endif
