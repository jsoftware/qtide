
#include "cmd.h"
#include "wd.h"
#include "../base/wscln.h"
#include "../base/wssvr.h"

// using namespace std;

static WsSvr *wssvr;
static WsCln *wscln;

std::string ws(std::string p);
static std::string wsconnect();
static std::string wsclose();
static std::string wslisten();
static std::string wsquery();
#ifdef QT50
static std::string wsstate();
#endif
static std::string wssend(int binary);

static std::vector<std::string> arg;
static std::string argjoin;

// ---------------------------------------------------------------------
std::string ws(std::string p)
{
  argjoin=p;
  arg=ssplit(p);
  if (arg.size()<1) {
    error("missing ws cmd");
    return "";
  }

  std::string type=arg.front();
  arg.erase(arg.begin());
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
#ifdef QT50
  if (type=="state")
    return wsstate();
#endif
  error("invalid ws cmd: " + type);
  return "";
}

// ---------------------------------------------------------------------
std::string wsconnect()
{
  std::string q;

  if (arg.size()==1) {
    q=arg.at(0);
  } else {
    error("Need url: "+argjoin);
    return "";
  }
  if (!((q.substr(0,5)=="ws://")||(q.substr(0,6)=="wss://"))) {
    error("bad url: "+argjoin);
    return "";
  }
  if (!wscln)
    wscln = new WsCln();
  return p2s(wscln->openurl(s2q(q)));
}

// ---------------------------------------------------------------------
std::string wsclose()
{
  if (arg.size()==0) {
    return "";
  }
  I socket=c_strtol(arg.at(0));
  if ((wssvr) && wssvr->hasSocket((void *)socket)) {
    wssvr->disconnect((void *)socket);
  } else if ((wscln) && wscln->hasSocket((void *)socket)) {
    wscln->disconnect((void *)socket);
  }
  return "";
}

// ---------------------------------------------------------------------
std::string wslisten()
{
  int port=0,protocol=0;

  if (arg.size()==1) {
    port=c_strtoi(arg.at(0));
  } else if (arg.size()==2) {
    port=c_strtoi(arg.at(0));
    protocol=c_strtoi(arg.at(1));
  } else {
    error("Need port [protocol]: "+argjoin);
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
std::string wsquery()
{
  int type=0;
  std::string r="";
  if (arg.size()!=0)
    type=c_strtoi(arg.at(0));
  if (0==type) {
    if (wssvr)
      r=wssvr->querySocket();
  } else {
    if (wscln)
      r=wscln->querySocket();
  }
  return r;
}

#ifdef QT50
// ---------------------------------------------------------------------
std::string wsstate()
{
  I socket=0;
  std::string r="";
  if (arg.size()==0) {
    error("Need socket: "+argjoin);
    return "";
  }
  socket=c_strtol(arg.at(0));
  if ((wssvr) && wssvr->hasSocket((void *)socket)) {
    r=wssvr->state((void *)socket);
  } else if ((wscln) && wscln->hasSocket((void *)socket)) {
    r=wscln->state((void *)socket);
  }
  return r;
}
#endif

// ---------------------------------------------------------------------
std::string wssend(int binary)
{
  I socket=0;
  std::string r, data="";

  if (arg.size()==1) {
    socket=c_strtol(arg.at(0));
  } else if (arg.size()==2) {
    socket=c_strtol(arg.at(0));
    data=arg.at(1);
  } else {
    error("Need socket [data]: "+argjoin);
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
    error("Need active websocket connection: "+argjoin);
    return "";
  }
  return r;
}

