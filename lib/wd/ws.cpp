
#include "wd.h"
#include "cmd.h"
#include "../base/wssvr.h"
#include "../base/wscln.h"

using namespace std;

WsSvr *wssvr;
WsCln *wscln;

string ws(string p);
static string wsconnect();
static string wsclose();
static string wslisten();
static string wsquery();
static string wssend(int binary);

static QStringList arg;

// ---------------------------------------------------------------------
string ws(string p)
{
  arg=qsplit(p);
  if (arg.size()<1) {
    error("missing ws cmd");
    return "";
  }

  QString type=arg.first();
  arg.removeFirst();
  if (type=="listen")
    return wslisten();
  if (type=="connect")
    return wsconnect();
  if (type=="send")
    return wssend(0);
  if (type=="sendb")
    return wssend(1);
  if (type=="close")
    return wsclose();
  if (type=="query")
    return wsquery();
  error("invalid ws cmd: " + q2s(type));
  return "";
}

// ---------------------------------------------------------------------
string wsconnect()
{
  int port=0, protocol=0;
  string q;

  if (arg.size()==1) {
    q=q2s(arg.at(0));
  } else if (arg.size()==2) {
    q=q2s(arg.at(0));
    port=c_strtoi(q2s(arg.at(1)));
  } else {
    error("Need url [port]: "+q2s(arg.join(" ")));
    return "";
  }
  if (q.substr(0,5)=="ws://") {
    protocol=0;
  } else if (q.substr(0,6)=="wss://") {
    protocol=1;
  } else {
    error("url not ws:// or wss:// : "+q2s(arg.join(" ")));
    return "";
  }
  if (!port) port = (0==protocol) ? 80 : 443;
  if (!wscln)
    wscln = new WsCln();
  return p2s(wscln->connect(s2q(q),port));
}

// ---------------------------------------------------------------------
string wsclose()
{
  if (arg.size()==0) {
    return "";
  }
  I socket=c_strtol(q2s(arg.at(0)));
  if ((wssvr) && wssvr->hasSocket((void *)socket)) {
    wssvr->disconnect((void *)socket);
  } else if ((wscln) && wscln->hasSocket((void *)socket)) {
    wscln->disconnect((void *)socket);
  }
  return "";
}

// ---------------------------------------------------------------------
string wslisten()
{
  int port=0,protocol=0;

  if (arg.size()==1) {
    port=c_strtoi(q2s(arg.at(0)));
  } else if (arg.size()==2) {
    port=c_strtoi(q2s(arg.at(0)));
    protocol=c_strtoi(q2s(arg.at(1)));
  } else {
    error("Need port [protocol]: "+q2s(arg.join(" ")));
    return "";
  }
  if (wssvr) {
    delete wssvr;
    wssvr = 0;
  }
  if (port) {
    wssvr = new WsSvr(port,1+protocol);
    if (!(wssvr->errstring.empty())) {
      error("ws listen failed: " + wssvr->errstring);
      delete wssvr;
      wssvr = 0;
    }
  }
  return "";
}

// ---------------------------------------------------------------------
string wsquery()
{
  int type=0;
  string r="";
  if (arg.size()!=0)
    type=c_strtoi(q2s(arg.at(0)));
  if (0==type) {
    if (wssvr)
      r=wssvr->querySocket();
  } else {
    if (wscln)
      r=wscln->querySocket();
  }
  return r;
}

// ---------------------------------------------------------------------
string wssend(int binary)
{
  I socket=0;
  string r, data="";

  if (arg.size()==1) {
    socket=c_strtol(q2s(arg.at(0)));
  } else if (arg.size()==2) {
    socket=c_strtol(q2s(arg.at(0)));
    data=q2s(arg.at(1));
  } else {
    error("Need socket [data]: "+q2s(arg.join(" ")));
    return "";
  }
  if ((wssvr) && (0==socket))
    r=  p2s((void *)wssvr->write((void *)0, data.c_str(), data.size(), binary));
  else if ((wscln) && (1==socket))
    r=  p2s((void *)wscln->write((void *)0, data.c_str(), data.size(), binary));
  else if ((wssvr) && wssvr->hasSocket((void *)socket))
    r=  p2s((void *)wssvr->write((void *)socket, data.c_str(), data.size(), binary));
  else if ((wscln) && wscln->hasSocket((void *)socket))
    r=  p2s((void *)wscln->write((void *)socket, data.c_str(), data.size(), binary));
  else {
    error("Need active websocket connection: "+q2s(arg.join(" ")));
    return "";
  }
  return r;
}

