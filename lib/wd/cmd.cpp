
#include "wd.h"
#include "cmd.h"

char DEL='\177';
char LF='\n';
char SOH='\001';
string WS=" \f\r\t\v";
string WSLF=WS+LF;

bool contains(string s,char c);
QStringList qsplitby(string s, char c);
vector<string> ssplitby(string s, char c);
string toLF(string s);

// ---------------------------------------------------------------------
void Cmd::end()
{
  pos=str.size();
}

// ---------------------------------------------------------------------
void Cmd::init(char *s,int slen)
{
  str=string(s,slen);
  str=toLF(str);
  len=str.size();
  while (len>0) {
    if (!contains(WS,str[len-1])) break;
    len--;
  }
  str=str.substr(0,len);
  bgn=pos=0;
}

// ---------------------------------------------------------------------
// split on h m p v s z and remove blanks
QStringList Cmd::bsplits()
{
  QStringList r;
  str=remws(str);
  len=str.size();
  while (pos<len) {
    bgn=pos++;
    pos=str.find_first_of("hmpsvz",pos);
    if (pos==string::npos) {
      r.append(s2q(str.substr(bgn)));
      break;
    } else
      r.append(s2q(str.substr(bgn,pos-bgn)));
  }
  return r;
}

// ---------------------------------------------------------------------
bool Cmd::delimLF(string s)
{
  char c;
  int n=s.size();
  for (int i=0; i<n; i++) {
    c=s[i];
    if (c==LF) return true;
    if (c=='"' || c==DEL)
      while (s[++i]!=c);
  }
  return false;
}

// ---------------------------------------------------------------------
string Cmd::getid()
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
string Cmd::getline()
{
  string r;
  if (pos==len) return "";
  if (str[pos]==LF) pos++;
  if (pos==len) return "";
  bgn=pos;
  pos=str.find_first_of(LF,pos);
  if (pos==string::npos)
    pos=str.size();
  else
    pos++;
  return str.substr(bgn,pos-bgn-1);
}
// ---------------------------------------------------------------------
// to next ; else return rest of string
string Cmd::getparms()
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
    string r=str.substr(pos+1);
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
bool Cmd::more()
{
  return pos<len;
}

// ---------------------------------------------------------------------
// split parameters
// if has SOH, split on SOH
// if has LF not in paired delimiters, split on LF
// otherwise split on WS, or paired "" or DEL
QStringList Cmd::qsplits()
{
  skips(WS);
  if (contains(str,SOH))
    return qsplitby(str,SOH);
  if (delimLF(str))
    return qsplitby(str,LF);
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
// split as qsplits, but returning vector of string
vector<string> Cmd::ssplits()
{
  skips(WS);
  if (contains(str,SOH))
    return ssplitby(str,SOH);
  if (delimLF(str))
    return ssplitby(str,LF);
  char c;
  vector<string> r;
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
string Cmd::remws(const string s)
{
  string r;
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
void Cmd::skips(string s)
{
  pos=str.find_first_not_of(s,pos);
  if (pos==string::npos) pos=len;
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
// split on bin commands and remove blanks
QStringList bsplit(string s)
{
  Cmd c;
  c.init((char*)s.c_str(),(int)s.size());
  return c.bsplits();
}

// ---------------------------------------------------------------------
bool contains(string s,char c)
{
  return string::npos != s.find_first_of(c);
}

// ---------------------------------------------------------------------
// split parameters
QStringList qsplit(string s)
{
  Cmd c;
  c.init((char*)s.c_str(),(int)s.size());
  return c.qsplits();
}

// ---------------------------------------------------------------------
// split on character
QStringList qsplitby(string s,char c)
{
  int n=s.size();
  if (n==0)
    return QStringList();
  if (s[n-1]==c)
    s=s.substr(0,n-1);
  return s2q(s).split(c);
}

// ---------------------------------------------------------------------
// as qsplit but returning vector of string
vector<string> ssplit(string s)
{
  Cmd c;
  c.init((char*)s.c_str(),(int)s.size());
  return c.ssplits();
}

// ---------------------------------------------------------------------
// split on char
vector<string> ssplitby(string s,char c)
{
  int i,p;
  vector<string> r;
  int n=s.size();
  if (n==0) return r;
  if (s[n-1]==c)
    s=s.substr(0,--n);
  for (i=p=0; i<n; i++)
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
// convert CRLF to LF
string toLF(string s)
{
  if (!contains(s,'\r'))
    return s;
  int n=s.size();
  int p=0;
  size_t t;
  string r;
  while (string::npos != (t=s.find("\r\n",p))) {
    r.append(s.substr(p,t-p));
    p=t+1;
  }
  if (p<n)
    r.append(s.substr(p,n-p));
  return r;
}

// ---------------------------------------------------------------------
string Cmd::getcmdstr()
{
  return str.substr(pos);
}
