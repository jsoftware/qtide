
#include "wd.h"
#include "cmd.h"

string WS=" \f\n\r\t\v";
char DEL='\177';

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
void Cmd::init(char *s)
{
  str=s;
  len=str.size();
  while (len>0) {
    if (!contains(WS,str[len-1])) break;
    len--;
  }
  str=str.substr(0,len);
  bgn=pos=0;
}

// ---------------------------------------------------------------------
// split on h p v s z and remove blanks
QStringList Cmd::bsplits()
{
  QStringList r;
  str=remws(str);
  len=str.size();
  while (pos<len) {
    bgn=pos++;
    pos=str.find_first_of("hpsvz",pos);
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
  if (str[pos]=='\n') pos++;
  if (pos==len) return "";
  bgn=pos;
  pos=str.find_first_of('\n',pos);
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
// split on WS, or paired "" or DEL (=ascii 127)
QStringList Cmd::qsplits()
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
// split on h v z and remove blanks
QStringList bsplit(string s)
{
  Cmd c;
  c.init((char*)s.c_str());
  return c.bsplits();
}

// ---------------------------------------------------------------------
// split on blank (except in quotes) and LF
// *indicates rest is a single string
QStringList qsplit(string s)
{
  Cmd c;
  c.init((char*)s.c_str());
  return c.qsplits();
}

// ---------------------------------------------------------------------
string Cmd::getcmdstr()
{
  return str.substr(pos);
}
