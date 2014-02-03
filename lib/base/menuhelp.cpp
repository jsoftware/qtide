#include <QApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

#include "base.h"
#include "menu.h"
#include "note.h"
#include "state.h"
#include "bedit.h"
#include "term.h"
#include "tedit.h"

using namespace std;

void helpabout();
void helpcontext(int c,Bedit *);

void htmlhelp(QString s);
void helplabs();
void helplabsadvance();
void helplabschapters();
void helplabsauthor();

void Menu::createhelpActions()
{
  helpaboutAct = makeact("helpaboutAct","&About","");
  helpconstantsAct = makeact("helpconstantsAct","&Constants","");
  helpcontextAct = makeact("helpcontextAct","&Context Sensitive","Ctrl+F1");
  helpcontrolsAct = makeact("helpcontrolsAct","&Control Structures","");
  helpdictionaryAct = makeact("helpdictionaryAct","&Dictionary","");
  helpforeignsAct = makeact("helpforeignsAct","&Foreign Conjunction","");
  helpgeneralAct = makeact("helpgeneralAct","&General Info","");
  helphelpAct = makeact("helphelpAct","&Help","");
  helplabsAct = makeact("helplabsAct","&Labs...","");
  helplabsadvanceAct = makeact("helplabsadvanceAct","Advance","Ctrl+J");
  helplabschaptersAct = makeact("helplabschaptersAct","Chapters...","");
  helplabsauthorAct = makeact("helplabsauthorAct","Author...","");
  helpreleaseAct = makeact("helpreleaseAct","&Release Highlights","");
  helprelnotesAct = makeact("helprelnotesAct","&Release Notes","");
  helpvocabAct = makeact("helpvocabAct","&Vocabulary","F1");
}

void Menu::createhelpMenu()
{
  helpMenu = addMenu(tr("Help"));
  helpMenu->addAction(helphelpAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpgeneralAct);
//  helpMenu->addAction(helpreleaseAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpvocabAct);
  helpMenu->addAction(helpconstantsAct);
  helpMenu->addAction(helpcontrolsAct);
  helpMenu->addAction(helpforeignsAct);
  helpMenu->addSeparator();
//  helpMenu->addAction(helprelnotesAct);
  helpMenu->addAction(helpdictionaryAct);
  helpMenu->addSeparator();
  QMenu *m = helpMenu->addMenu(tr("Studio"));
  helpMenu->addSeparator();
  m->addAction(helplabsAct);
  m->addAction(helplabsadvanceAct);
  m->addAction(helplabschaptersAct);
  m->addSeparator();
  m->addAction(helplabsauthorAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpcontextAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpaboutAct);
}

// ---------------------------------------------------------------------
void Note::on_helpaboutAct_triggered()
{
  helpabout();
}

// ---------------------------------------------------------------------
void Note::on_helpconstantsAct_triggered()
{
  term->on_helpconstantsAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helpcontextAct_triggered()
{
  helpcontext(1,(Bedit *)editPage());
}

// ---------------------------------------------------------------------
void Note::on_helpcontrolsAct_triggered()
{
  term->on_helpcontrolsAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helpdictionaryAct_triggered()
{
  term->on_helpdictionaryAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helpforeignsAct_triggered()
{
  term->on_helpforeignsAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helpgeneralAct_triggered()
{
  term->on_helpgeneralAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helphelpAct_triggered()
{
  term->on_helphelpAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helplabsAct_triggered()
{
  helplabs();
}

// ---------------------------------------------------------------------
void Note::on_helplabsadvanceAct_triggered()
{
  helplabsadvance();
}

// ---------------------------------------------------------------------
void Note::on_helplabschaptersAct_triggered()
{
  helplabschapters();
}

// ---------------------------------------------------------------------
void Note::on_helplabsauthorAct_triggered()
{
  helplabsauthor();
}

// ---------------------------------------------------------------------
void Note::on_helpreleaseAct_triggered()
{
  term->on_helpreleaseAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helprelnotesAct_triggered()
{
  term->on_helprelnotesAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helpvocabAct_triggered()
{
  term->on_helpvocabAct_triggered();
}

// ---------------------------------------------------------------------
void Term::on_helpaboutAct_triggered()
{
  helpabout();
}

// ---------------------------------------------------------------------
void Term::on_helpcontextAct_triggered()
{
  helpcontext(0,(Bedit *)tedit);
}

// ---------------------------------------------------------------------
void Term::on_helpconstantsAct_triggered()
{
  htmlhelp("dictionary/dcons");
}

// ---------------------------------------------------------------------
void Term::on_helpcontrolsAct_triggered()
{
  htmlhelp("dictionary/ctrl");
}

// ---------------------------------------------------------------------
void Term::on_helpdictionaryAct_triggered()
{
  htmlhelp("dictionary/contents");
}

// ---------------------------------------------------------------------
void Term::on_helpforeignsAct_triggered()
{
  htmlhelp("dictionary/xmain");
}

// ---------------------------------------------------------------------
void Term::on_helpgeneralAct_triggered()
{
  htmlhelp("user/product");
}

// ---------------------------------------------------------------------
void Term::on_helphelpAct_triggered()
{
  htmlhelp("index");
}

// ---------------------------------------------------------------------
void Term::on_helplabsAct_triggered()
{
  helplabs();
}

// ---------------------------------------------------------------------
void Term::on_helplabsadvanceAct_triggered()
{
  helplabsadvance();
}

// ---------------------------------------------------------------------
void Term::on_helplabschaptersAct_triggered()
{
  helplabschapters();
}

// ---------------------------------------------------------------------
void Term::on_helplabsauthorAct_triggered()
{
  helplabsauthor();
}

// ---------------------------------------------------------------------
void Term::on_helpreleaseAct_triggered()
{
  notyet("Release Highlights");
  return;
  htmlhelp("user/relhigh.htm");
}

// ---------------------------------------------------------------------
void Term::on_helprelnotesAct_triggered()
{
  notyet("Release Notes");
  return;
  htmlhelp("release/contents");
}

// ---------------------------------------------------------------------
void Term::on_helpvocabAct_triggered()
{
  htmlhelp("dictionary/vocabul");
}

// ---------------------------------------------------------------------
void helpcontext(int c,Bedit *e)
{
//  qDebug() << e->readhelptext(c);
  var_set("arg_jqtide_",e->readhelptext(c));
  QString s=var_cmdr("helpcontext0_jqtide_ arg_jqtide_");
  if (s.size())
    htmlhelp(s);
}

// ---------------------------------------------------------------------
void helplabs()
{
  tedit->docmds("labs_run_jqtide_$0");
}

// ---------------------------------------------------------------------
void helplabsadvance()
{
  tedit->docmds("labs_run_jqtide_ 0");
}

// ---------------------------------------------------------------------
void helplabschapters()
{
  tedit->docmds("labs_run_jqtide_ 1");
}

// ---------------------------------------------------------------------
void helplabsauthor()
{
  tedit->docmds("labs_run_jqtide_ 2");
}

// ---------------------------------------------------------------------
void htmlhelp(QString s)
{
  QString t=cpath("~addons/docs/help/") + s + ".htm";
#ifdef QT_OS_ANDROID
  if (QFile(t).exists())
    android_exec_host((char *)"android.intent.action.VIEW",t.prepend("file://").toUtf8().constData(),(char *)"text/html");
  else android_exec_host((char *)"android.intent.action.VIEW",s.prepend("http://www.jsoftware.com/help/").append(".htm").toUtf8().constData(),(char *)"text/html");
#else
  if (QFile(t).exists())
    QDesktopServices::openUrl(QUrl::fromLocalFile(t));
  else QDesktopServices::openUrl(QUrl(s.prepend("http://www.jsoftware.com/help/").append(".htm"),QUrl::TolerantMode));
#endif
}

