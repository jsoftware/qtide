
#include "wd.h"
#include "cmd.h"

char DEL='\177';
char LF='\n';
string WN=" \f\r\t\v";
string WS=WN+LF;

// ---------------------------------------------------------------------
bool Cmd::contains(string s,char c)
{
  return string::npos != s.find_first_of(c);
}

// ---------------------------------------------------------------------
void Cmd::end()
{
  pos=str.size();
}

// ---------------------------------------------------------------------
void Cmd::init(char *s,int slen)
{
  str=string(s,slen);
  len=str.size();
  while (len>0) {
    if (!contains(WN,str[len-1])) break;
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
string Cmd::getid()
{
  char c;
  skips(WS+';');
  bgn=pos;
  while (pos<len) {
    c=str[pos];
    if (contains(WS,c) || c==';') break;
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

  skips(WS);
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
// split on LF, or WS, or paired "" or DEL
QStringList Cmd::qsplits()
{
  char c;
  QStringList r;
  while (pos<len) {
    skips(WN);
    bgn=pos;
    c=str[pos++];
    if (c=='*') {
      r.append(s2q(str.substr(pos)));
      break;
    }
    if (c==LF)
      r.append("");
    else  if (c=='"' || c==DEL) {
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
// split on LF, or WS, or paired "" or DEL
vector<string> Cmd::ssplits()
{
  char c;
  vector<string> r;
  while (pos<len) {
    skips(WN);
    bgn=pos;
    c=str[pos++];
    if (c=='*') {
      r.push_back(str.substr(pos));
      break;
    }
    if (c==LF)
      r.push_back("");
    else if (c=='"' || c==DEL) {
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
    if (!contains(WS,s[i]))
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
// split on blank (except in quotes) and LF
// *indicates rest is a single string
QStringList qsplit(string s)
{
  Cmd c;
  c.init((char*)s.c_str(),(int)s.size());
  return c.qsplits();
}

// ---------------------------------------------------------------------
// split on LF if present, otherwise do qsplit
// this is for list and comboboxes that would not have LF in items
QStringList rsplit(string s)
{
  int n=s.size();
  if (n==0)
    return QStringList();
  if (string::npos==s.find(LF))
    return qsplit(s);
  if (s[n-1]==LF)
    s=s.substr(0,n-1);
  return s2q(s).split(LF);
}

// ---------------------------------------------------------------------
// split on blank (except in quotes) and LF
// *indicates rest is a single string
vector<string> ssplit(string s)
{
  Cmd c;
  c.init((char*)s.c_str(),(int)s.size());
  return c.ssplits();
}

// ---------------------------------------------------------------------
string Cmd::getcmdstr()
{
  return str.substr(pos);
}
