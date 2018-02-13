/* J svr */

#include <QApplication>
#include <QDataStream>
#include <QEventLoop>
#include <QString>
#include <QTimer>

#include <csignal>
#ifdef _WIN32
#else
#include <unistd.h>
#endif

#include "base.h"
#include "base.h"
#include "jsvr.h"
#include "svr.h"
#include "state.h"
#include "tedit.h"
#include "term.h"

// output type
#define MTYOFM		1	/* formatted result array output */
#define MTYOER		2	/* error output */
#define MTYOLOG		3	/* output log */
#define MTYOSYS		4	/* system assertion failure */
#define MTYOEXIT	5	/* exit */
#define MTYOFILE	6	/* output 1!:2[2 */

using namespace std;

C* _stdcall Jinput(J jt, C*);
void _stdcall Joutput(J jt, int type, C* s);

static QString inputx;
bool jecallback=false;
bool runcmd=false;
bool runsentences=false;
bool runshow=false;
int runterm=0;
std::string wdQuery="";

void logbin(const char*s,int n);
void logcs(char *msg);
QString runshowclean(QString s);
Jcon *jcon=0;

// ---------------------------------------------------------------------
// usual way to call J when not suspended
void Jcon::cmd(string s)
{
  jedo((char *)s.c_str());
}

// ---------------------------------------------------------------------
// add expression to Sentence and run all Sentence
void Jcon::cmddo(string s)
{
  runcmd=false;
  Sentence.push_back(s);
  if (jecallback)
    jevloop->exit();
  else
    cmdSentences();
}

// ---------------------------------------------------------------------
// execute priority expression immediately, then
// set up to run all Sentence
// e.g. used for isigraph paint event
void Jcon::cmddop(string s)
{
  runcmd=false;
  cmdSentence(s);
  cmddos();
}

// ---------------------------------------------------------------------
// run all Sentence after timeout
void Jcon::cmddos()
{
  if (jecallback)
    jevloop->exit();
  else {
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), jcon, SLOT(cmdSentences()));
    timer->start();
  }
}

// ---------------------------------------------------------------------
// run single Sentence
void Jcon::cmdSentence(string s)
{
  size_t n=s.find('\0');
  if (n==string::npos)
    cmd(s);
  else {
    wdQuery=s.substr(n+1,string::npos);
    cmd(s.substr(0,n));
  }
  jecallback=false;
}

// ---------------------------------------------------------------------
// run all Sentences
void Jcon::cmdSentences()
{
  if (runsentences) return;
  runsentences=true;
  string s;
  while(!Sentence.empty()) {
    s=Sentence.front();
    Sentence.pop_front();
    cmdSentence(s);
  }
  if (runterm==1)
    tedit->setprompt();
  runterm=0;
  runsentences=false;
}

// ---------------------------------------------------------------------
// execute J expression and return result
QString Jcon::cmdr(string s)
{
  return s2q(dors(s));
}

// ---------------------------------------------------------------------
int Jcon::init(int argc, char* argv[])
{
  void* callbacks[] = {(void*)Joutput,0,(void*)Jinput,0,(void*)SMCON};
  int type;
  int flag=0;
  int forceavx=0;

  if(argc>=3&&!strcmp(argv[1],"-lib")&&'-'!=*(argv[2])) flag=1;
  else if(!flag&&argc>=2&&!strcmp(argv[1],"-avx")) forceavx=1;  // avx
  else if(!flag&&argc>=2&&!strcmp(argv[1],"-noavx")) forceavx=2;  // no avx
  if (!jdllproc && (void *)-1==jdlljt) jepath(argv[0],flag?argv[2]:(char*)"",forceavx);  // get path to JFE folder
  if(argc>=2&&(!strcmp(argv[1],"-lib")||forceavx)) { // remove processed arg
    int i;
    int n=(flag)?2:1;
    for(i=1; i<argc-n; ++i) {
      argv[i]=argv[i+n];
    }
    argc=argc-n;
  }

  jt=jeload(callbacks);
  if(!jt && (void *)-1==jdlljt) {
    char m[1000];
    jefail(m), fputs(m,stderr);
    exit(1);
  }

  if (jdllproc || (void *)-1!=jdlljt) {
    *inputline=0;
    return 0;
  }
  adadbreak=(char**)jt; // first address in jt is address of breakdata
  signal(SIGINT,sigint);

  if(argc==2&&!strcmp(argv[1],"-jprofile"))
    type=3;
  else if(argc>2&&!strcmp(argv[1],"-jprofile"))
    type=1;
  else
    type=0;
  addargv(argc,argv,inputline+strlen(inputline));
  jefirst(type,inputline);

  return 0;
}

// ---------------------------------------------------------------------
// run immex command
void Jcon::immex(string s)
{
  Sentence.push_back(s);
  cmddos();
}

// ---------------------------------------------------------------------
void Jcon::set(QString s, string t)
{
  sets(s,t);
}

// ---------------------------------------------------------------------
// J calls for input (debug suspension and 1!:1[1) and we call for input
char* _stdcall Jinput(J jt, char* p)
{
  Q_UNUSED(jt);
  Q_ASSERT(tedit);
  string s;
  if (!jecallback) {
    // On initial entry to suspension, purge sentences typed ahead by the user.
    // but DO NOT remove calls to wdhandler[x], because some event sequences
    // such as mbldown-mblup-mbldbl must be kept intact
    for(size_t i = jcon->Sentence.size(); i>0; --i) {
      string s=jcon->Sentence.front();
      jcon->Sentence.pop_front();
      if(s.find("wdhandler") != string::npos)jcon->Sentence.push_back(s);
    }
    jecallback=true;
  }
  tedit->prompt=c2q(p);
  if (runterm==1 || 0==strlen(p)) {
    tedit->setprompt();
  }
  runterm=qMax(0,runterm-1);
  if (jcon->Sentence.empty()) {
    jevloop->exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
  }
  if (!jcon->Sentence.empty()) {
    s=jcon->Sentence.front();
    jcon->Sentence.pop_front();
  }
  size_t n=s.find('\0');
  if (n!=string::npos)
    wdQuery=s.substr(n+1,string::npos);
  s=s.substr(0,n);
  if ((int)sizeof(inputline)<s.size()) exit(100);
  strcpy(inputline,s.c_str());
  return inputline;
}

// ---------------------------------------------------------------------
// J calls for output
void _stdcall Joutput(J jt,int type, char* s)
{
  Q_UNUSED(jt);

  if(MTYOEXIT==type) {
// callback from jengine after 2!:55
    int rc=(intptr_t)s;
    jefree();
    state_quit();
    evloop->exit(rc);
    exit(rc);
    return;
  }

  Q_ASSERT(tedit);
  int n=(int)strlen(s);
  if (n==0) return;
  if (s[n-1]=='\n') s[n-1]='\0';
  QString t=QString::fromUtf8(s);

  if (MTYOER==type && runshow)
    t=runshowclean(t);

  if (runterm)
    tedit->append(t);
  else
    tedit->append_smoutput(t);
}

// ---------------------------------------------------------------------
QString runshowclean(QString s)
{
  int n=s.indexOf("output_jrx_=:");
  if (n>0)
    s.remove(n,13);
  return(s);
}

// ---------------------------------------------------------------------
bool svr_init(int argc, char* argv[])
{
  jcon=new Jcon();
  int r=jcon->init(argc,argv);
  if (r)
    info("Server","svr_init result: " + QString::number(r));
  return r==0;
}
