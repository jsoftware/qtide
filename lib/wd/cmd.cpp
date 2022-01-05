
#include "cmd.h"
#include "wd.h"

static char DEL='\177';
static char LF='\n';
static char SOH='\001';
static std::string WS=" \f\r\t\v";
static std::string WSLF=WS+LF;

static bool contains(std::string s,char c);
static std::string toLF(std::string s);

// ---------------------------------------------------------------------
void Cmd::end()
{
  pos=str.size();
}

// ---------------------------------------------------------------------
void Cmd::init(char *s,int slen)
{
  str=std::string(s,slen);
  str=toLF(str);
  len=str.size();
  bgn=pos=pos0=0;
}

// ---------------------------------------------------------------------
// split on g h m p v s z and remove blanks
QStringList Cmd::bsplits()
{
  QStringList r;
  str=remws(str);
  len=str.size();
  while (pos<len) {
    bgn=pos++;
    pos=str.find_first_of("ghmpsvz",pos);
    if (pos==std::string::npos) {
      r.append(s2q(str.substr(bgn)));
      break;
    } else
      r.append(s2q(str.substr(bgn,pos-bgn)));
  }
  return r;
}

// ---------------------------------------------------------------------
// if std::string delimited by LF
bool Cmd::delimLF(std::string s)
{
  char c;
  int n=(int)s.size();
  for (int i=0; i<n; i++) {
    c=s[i];
    if (c==LF) return true;
    if (c=='*' || c==SOH) return false;
    if (c=='"' || c==DEL)
      while (s[++i]!=c);
  }
  return false;
}

// ---------------------------------------------------------------------
std::string Cmd::getid()
{
  char c;
  skips(WSLF+';');
  bgn=pos;
  while (pos<len) {
    c=str[pos];
    if (contains(WSLF,c) || c==';') break;
    pos++;
  }
  return remquotes(str.substr(bgn,pos-bgn));
}

// ---------------------------------------------------------------------
// get to next LF
std::string Cmd::getline()
{
  std::string r;
  if (pos==len) return "";
  if (str[pos]==LF) pos++;
  if (pos==len) return "";
  bgn=pos;
  pos=str.find_first_of(LF,pos);
  if (pos==std::string::npos)
    pos=str.size();
  else
    pos++;
  return str.substr(bgn,pos-bgn-1);
}
// ---------------------------------------------------------------------
// to next ; else return rest of string
// if star, preserve leading *
std::string Cmd::getparms(bool star)
{
  char c;

  if (pos==len)
    return "";

  if (str[pos]==';') {
    pos++;
    return "";
  }

  skips(WSLF);
  if (pos==len)
    return "";
  if (str[pos]=='*') {
    std::string r=str.substr(pos+(star?0:1));
    pos=len;
    return r;
  }
  bgn=pos;
  while (pos<len) {
    c=str[pos];
    if (c==';') break;
    pos++;
    if (c=='"' || c==DEL) {
      skippast(c);
      continue;
    }
  }
  return str.substr(bgn,pos-bgn);
}

// ---------------------------------------------------------------------
// if there is a starred parameter (i.e. beginning *)
// this also means string not delimited by SOH or LF
bool Cmd::ifstarred(std::string s)
{
  char c;
  int n=(int)s.size();
  for (int i=0; i<n; i++) {
    c=s[i];
    if (c=='*') return true;
    if (c==LF || c==SOH) return false;
    if (c=='"' || c==DEL)
      while (s[++i]!=c)
        if (s[i]==SOH) return false;
  }
  return false;
}
// ---------------------------------------------------------------------
bool Cmd::more()
{
  return pos<len;
}

// ---------------------------------------------------------------------
// split on character
QStringList Cmd::qsplitby(char c)
{
  std::string s;
  if (len==0 || pos==len)
    return QStringList();
  if (str[len-1]==c)
    s=str.substr(pos,len-1);
  else
    s=str.substr(pos);
  pos=len;
  return s2q(s).split(c);
}

// ---------------------------------------------------------------------
// split parameters
// standard is split on WS, or paired "" or DEL
// if starred, do standard
// otherwise if has SOH, split on SOH
// otherwise if has LF not in paired delimiters, split on LF
// otherwise do standard
QStringList Cmd::qsplits()
{
  skips(WS);
  if (ifstarred(str))
    return qsplitstd();
  std::string s=str.substr(pos);
  if (contains(s,SOH))
    return qsplitby(SOH);
  if (delimLF(s))
    return qsplitby(LF);
  return qsplitstd();
}

// ---------------------------------------------------------------------
QStringList Cmd::qsplitstd()
{
  char c;
  QStringList r;
  while (pos<len) {
    skips(WS);
    bgn=pos;
    c=str[pos++];
    if (c=='*') {
      r.append(s2q(str.substr(pos)));
      break;
    }
    if (c=='"' || c==DEL) {
      skippast(c);
      r.append(s2q(str.substr(bgn+1,pos-bgn-2)));
    } else {
      skiptows();
      r.append(s2q(str.substr(bgn,pos-bgn)));
      if (pos<len && str[pos]==LF)
        pos++;
    }
  }
  return r;
}

// ---------------------------------------------------------------------
std::string Cmd::remws(const std::string s)
{
  std::string r;
  for (size_t i=0; i<s.size(); i++)
    if (!contains(WSLF,s[i]))
      r.push_back(s[i]);
  return r;
}

// ---------------------------------------------------------------------
// start after the delimiter, move past the closing delimiter
void Cmd::skippast(char c)
{
  while (len>pos)
    if (str[pos++]==c) return;
}

// ---------------------------------------------------------------------
void Cmd::skips(std::string s)
{
  pos=str.find_first_not_of(s,pos);
  if (pos==std::string::npos) pos=len;
}

// ---------------------------------------------------------------------
// move to next whitespace
void Cmd::skiptows()
{
  while (len>pos) {
    if (contains(WS,str[pos])) return;
    pos++;
  }
}

// ---------------------------------------------------------------------
// split on char
std::vector<std::string> Cmd::ssplitby(char c)
{
  int i,p;
  std::vector<std::string> r;
  std::string s;
  if (len==0 || pos==len)
    return r;
  if (str[len-1]==c)
    s=str.substr(pos,len-1);
  else
    s=str.substr(pos);
  pos=len;
  int n=(int)s.size();
  for(i=p=0; i<n; i++)
    if (s[i]==c) {
      r.push_back(s.substr(p,i-p));
      p=i+1;
    }
  if (p==n)
    r.push_back("");
  else
    r.push_back(s.substr(p,n-p));
  return r;
}

// ---------------------------------------------------------------------
// split as qsplits, but returning vector of string
std::vector<std::string> Cmd::ssplits()
{
  skips(WS);
  if (ifstarred(str))
    return ssplitstd();
  std::string s=str.substr(pos);
  if (contains(s,SOH))
    return ssplitby(SOH);
  if (delimLF(s))
    return ssplitby(LF);
  return ssplitstd();
}

// ---------------------------------------------------------------------
std::vector<std::string> Cmd::ssplitstd()
{
  char c;
  std::vector<std::string> r;
  while (pos<len) {
    skips(WS);
    bgn=pos;
    c=str[pos++];
    if (c=='*') {
      r.push_back(str.substr(pos));
      break;
    }
    if (c=='"' || c==DEL) {
      skippast(c);
      r.push_back(str.substr(bgn+1,pos-bgn-2));
    } else {
      skiptows();
      r.push_back(str.substr(bgn,pos-bgn));
      if (pos<len && str[pos]==LF)
        pos++;
    }
  }
  return r;
}

// ---------------------------------------------------------------------
// split on bin commands and remove blanks
QStringList bsplit(std::string s)
{
  Cmd c;
  c.init((char*)s.c_str(),(int)s.size());
  return c.bsplits();
}

// ---------------------------------------------------------------------
bool contains(std::string s,char c)
{
  return std::string::npos != s.find_first_of(c);
}

// ---------------------------------------------------------------------
// split parameters
// if star and first non WS is *, then return rest as single parameter
QStringList qsplit(std::string s,bool star)
{
  if (star) {
    size_t p=s.find_first_not_of(WS);
    if (p!=std::string::npos && s[p]=='*')
      return QStringList(s2q(s.substr(p+1)));
  }
  Cmd c;
  c.init((char*)s.c_str(),(int)s.size());
  return c.qsplits();
}

// ---------------------------------------------------------------------
// as qsplit but returning vector of string
std::vector<std::string> ssplit(std::string s)
{
  Cmd c;
  c.init((char*)s.c_str(),(int)s.size());
  return c.ssplits();
}

// ---------------------------------------------------------------------
// convert CRLF to LF
std::string toLF(std::string s)
{
  if (!contains(s,'\r'))
    return s;
  int n=(int)s.size();
  int p=0;
  int t;
  std::string r;
  while ((int)std::string::npos != (t=(int)s.find("\r\n",p))) {
    r.append(s.substr(p,t-p));
    p=t+1;
  }
  if (p<n)
    r.append(s.substr(p,n-p));
  return r;
}

// ---------------------------------------------------------------------
void Cmd::markpos()
{
  pos0=pos;
}

// ---------------------------------------------------------------------
void Cmd::rewindpos()
{
  pos=pos0;
}
