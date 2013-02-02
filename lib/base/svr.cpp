/* J svr */

#include <QApplication>
#include <QDataStream>
#include <QEventLoop>
#include <QString>
#include <QTimer>

#include <csignal>
#include <unistd.h>

#include "base.h"
#include "base.h"
#include "jsvr.h"
#include "svr.h"
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

C* _stdcall Jinput(C*);
void _stdcall Joutput(J jt, int type, C* s);

static bool active=true;
static bool inputready=false;
static int inputs=0;
static QString inputx;
static bool logged=false;
static bool quitx=false;

void logbin(const char*s,int n);
void logcs(char *msg);
Jcon *jcon=0;
QEventLoop *evloop;
int cnt=0;

// ---------------------------------------------------------------------
void Jcon::cmd(QString s)
{
  jedo((char *)q2s(s).c_str());
}

// ---------------------------------------------------------------------
QString Jcon::cmdr(QString s)
{
  return dors(s);
}

// ---------------------------------------------------------------------
int Jcon::exec()
{
  QString s;

  while(1) {
    cnt++;
    Jinput((char *)"   ");
    if (quitx) break;
    while(!Sentence.isEmpty()) {
      s=Sentence.at(0);
      Sentence.removeFirst();
      strcpy(inputline,q2s(s).c_str());
      jedo(inputline);
    }
  }
  jefree();
  return 0;
}

// ---------------------------------------------------------------------
int Jcon::init(int argc, char* argv[])
{

  void* callbacks[] = {(void*)Joutput,0,(void*)Jinput,0,(void*)SMCON};
  int type;

  evloop=new QEventLoop();

  jepath(argv[0]);     // get path to JFE folder
  jt=jeload(callbacks);
  if(!jt) {
    char m[1000];
    jefail(m), fputs(m,stdout);
#ifdef ANDROID
    info("Server","jeload failed: " + QString(m));
#endif
    exit(1);
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
// run command
void Jcon::immex(QString s)
{
  Sentence.append(s);
  QTimer *timer = new QTimer(this);
  timer->setSingleShot(true);
  connect(timer, SIGNAL(timeout()), jcon, SLOT(input()));
  timer->start();
}

// ---------------------------------------------------------------------
void Jcon::input()
{
  evloop->exit();
}

// ---------------------------------------------------------------------
void Jcon::quit()
{
  quitx=true;
  input();
}

// ---------------------------------------------------------------------
void Jcon::set(QString s, QString t)
{
  sets(s,t);
}

// ---------------------------------------------------------------------
//J calls for input (debug suspension and 1!:1[1) and we call for input
char* _stdcall Jinput(char* p)
{
  tedit->prompt=c2q(p);
  tedit->setprompt();
  inputready=false;
  inputs=0;
  logged=true;
  evloop->exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
  return (char*) 0;
}

// ---------------------------------------------------------------------
// J calls for output
// logged isn't used
// active isnt used
// inputs isnt used

void _stdcall Joutput(J jt,int type, char* s)
{
  Q_UNUSED(jt);

  if(MTYOEXIT==type) {
    exit((int)(intptr_t)s);
    // term->filequit();
  }

  int n=strlen(s);
  if (n==0) return;
  if (s[n-1]=='\n') s[n-1]='\0';
  QString t=QString::fromUtf8(s);

  if (!active) return;

  if(MTYOLOG!=type) {
    tedit->append(t);
  } else {
    if (inputs) return;
    if (logged) {
      tedit->append(t);
    } else {
      logged=true;
      tedit->append("");
    }
  }
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
