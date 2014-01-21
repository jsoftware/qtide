#ifndef WSCLN_H
#define WSCLN_H

#include <QtCore>
#include <QtNetwork>

#include "../QtWebsocket/QWsSocket.h"

#include "jsvr.h"

class WsCln : public QObject
{
  Q_OBJECT

public:
  WsCln();
  ~WsCln();
  void * connect(QString ipaddr);
  void disconnect(void * server);
  void write(void * server, const char * msg, I len, bool binary);
  std::string queryServer();

public slots:
  void onOpen();
  void onClose();
  void onMessage(QString message);
  void onMessage(QByteArray message);
  void onStateChange(QAbstractSocket::SocketState socketState);
  void onError(const QList<QSslError>& errors);

private:
  QList<QtWebsocket::QWsSocket*> servers;
  void frameReceived(QtWebsocket::QWsSocket* socket, QByteArray ba, bool binary);
};

extern WsCln *wscln;

#endif
