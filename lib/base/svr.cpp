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

// using namespace std;

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
void Jcon::cmd(std::string s)
{
#ifdef DEBUG_JDO
  qDebug() << "jcon cmd "+s2q(s)+ " jecallback "+ jecallback;
#endif
  jedo((char *)s.c_str());
}

// ---------------------------------------------------------------------
void Jcon::runimmx0(std::string s)
{
#ifdef DEBUG_JDO
  qDebug() << "jcon runimmx0 "+s2q(s)+ " jecallback "+ jecallback;
#endif
  jcon->set("IMMX_jrx_",s);
  jedo((char *)"9!:29[1[9!:27'0!:100 IMMX_jrx_'");
}

// ---------------------------------------------------------------------
void Jcon::runimmx1(std::string s)
{
#ifdef DEBUG_JDO
  qDebug() << "jcon runimmx1 "+s2q(s)+ " jecallback "+ jecallback;
#endif
  jcon->set("IMMX_jrx_",s);
  jedo((char *)"9!:29[1[9!:27'0!:101 IMMX_jrx_'");
}

// ---------------------------------------------------------------------
// add expression to Sentence and run all Sentence
void Jcon::cmddo(std::string s)
{
#ifdef DEBUG_JDO
  qDebug() << "jcon cmddo "+s2q(s)+ " jecallback "+ jecallback;
#endif
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
void Jcon::cmddop(std::string s)
{
  runcmd=false;
  cmdSentence(s);
  jevloop->exit();
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
void Jcon::cmdSentence(std::string s)
{
  size_t n=s.find('\0');
  if (n==std::string::npos)
    cmd(s);
  else {
    wdQuery=s.substr(n+1,std::string::npos);
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
  std::string s;
  while (!Sentence.empty()) {
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
QString Jcon::cmdr(std::string s)
{
  return s2q(dors(s));
}

// ---------------------------------------------------------------------
int Jcon::init(int argc, char* argv[], uintptr_t stackinit)
{
  void* callbacks[] = {(void*)Joutput,0,(void*)Jinput,(void*)stackinit,(void*)SMQT};
  int type;

  int i,poslib=0,poslibpath=0,posnorl=0,posprmpt=0; // assume all absent
  for(i=1; i<argc; i++) {
    if(!poslib&&!strcmp(argv[i],"-lib")) {
      poslib=i;
      if((i<argc-1)&&('-'!=*(argv[i+1])))poslibpath=i+1;
    } else if(!posnorl&&!strcmp(argv[i],"-norl")) {
      posnorl=i;
//      norl=1;
    } else if(!posprmpt&&!strcmp(argv[i],"-prompt")) {
      posprmpt=i;
//      forceprmpt=1;
    }
  }
  if (!jdllproc && (void *)-1==jdlljt) jepath(argv[0],(poslibpath)?argv[poslibpath]:(char *)"");
// remove processed arg
  if(poslib||poslibpath||posnorl||posprmpt) {
    int j=0;
    char **argvv = (char **)malloc(argc*sizeof(char*));
    argvv[j++]=argv[0];
    for(i=1; i<argc; i++) {
      if(!(i==poslib||i==poslibpath||i==posnorl||i==posprmpt))argvv[j++]=argv[i];
    }
    argc=j;
    for(i=1; i<argc; ++i)argv[i]=argvv[i];
    free(argvv);
  }

  jt=jeload(callbacks);
  if (!jt && (void *)-1==jdlljt) {
    char m[1000];
    jefail(m);
    fputs(m,stderr);
    exit(1);
  }

  if (jdllproc || (void *)-1!=jdlljt) {
    *inputline=0;
    return 0;
  }
  adadbreak=(char**)jt; // first address in jt is address of breakdata
  signal(SIGINT,sigint);

  if (argc==2&&!strcmp(argv[1],"-jprofile"))
    type=3;
  else if (argc>2&&!strcmp(argv[1],"-jprofile"))
    type=1;
  else
    type=0;
  addargv(argc,argv,inputline+strlen(inputline));
  jefirst(type,inputline);

  return 0;
}

// ---------------------------------------------------------------------
// run immex command
void Jcon::immex(std::string s)
{
  Sentence.push_back(s);
  cmddos();
}

// ---------------------------------------------------------------------
void Jcon::set(QString s, std::string t)
{
  sets(s,t);
}

// ---------------------------------------------------------------------
// J calls for input (debug suspension and 1!:1[1) and we call for input
char* _stdcall Jinput(J jt, char* p)
{
  Q_UNUSED(jt);
  Q_ASSERT(tedit);
  std::string s;
  if (!jecallback) {
    // On initial entry to suspension, purge sentences typed ahead by the user.
    // but DO NOT remove calls to wdhandler[x], because some event sequences
    // such as mbldown-mblup-mbldbl must be kept intact
    for (size_t i = jcon->Sentence.size(); i>0; --i) {
      std::string s=jcon->Sentence.front();
      jcon->Sentence.pop_front();
      if (s.find("wdhandler") != std::string::npos)jcon->Sentence.push_back(s);
    }
    jecallback=true;
  }
  tedit->prompt=c2q(p);
  if (runterm==1 || 0==strlen(p)) {
    tedit->setprompt();
  }
  runterm=qMax(0,runterm-1);
#ifdef DEBUG_JDO
  qDebug() << "jcon Jinput loop begin";
#endif
  if (jcon->Sentence.empty()) {
    jevloop->exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
  }
#ifdef DEBUG_JDO
  qDebug() << "jcon Jinput loop end";
#endif
  jecallback=false;
  if (!jcon->Sentence.empty()) {
    s=jcon->Sentence.front();
    jcon->Sentence.pop_front();
  }
  size_t n=s.find('\0');
  if (n!=std::string::npos)
    wdQuery=s.substr(n+1,std::string::npos);
  s=s.substr(0,n);
  if ((int)sizeof(inputline)<s.size()) exit(100);
  strcpy(inputline,s.c_str());
#ifdef DEBUG_JDO
  qDebug() << "jcon Jinput return "+s2q(s);
#endif
  return inputline;
}

// ---------------------------------------------------------------------
// J calls for output
void _stdcall Joutput(J jt,int type, char* s)
{
  Q_UNUSED(jt);

  if (MTYOEXIT==type) {
    state_quit();
    evloop->exit((intptr_t)s);
//#ifdef _WIN32
    jefree();
    exit((intptr_t)s);
//#endif
    return;
  }

  Q_ASSERT(tedit);
  int n=(int)strlen(s);
  if (n==0) return;
//  if (s[n-1]=='\n') s[n-1]='\0';
  QString t=QString::fromUtf8(s,(s[n-1]=='\n')?n-1:n);

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
  return (s);
}

// ---------------------------------------------------------------------
bool svr_init(int argc, char* argv[], uintptr_t stackinit)
{
  jcon=new Jcon();
  int r=jcon->init(argc,argv,stackinit);
  if (r)
    info("Server","svr_init result: " + QString::number(r));
  return r==0;
}
