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

#ifdef QT_OS_ANDROID
extern "C" void android_exec_host(char*,char*,char*,int);
#endif

using namespace std;

void helpabout();
void helpcontext(int c,Bedit *);
void helpcontextnuvoc(int c,Bedit *);
void htmlhelp(QString s);
void helplabs();
void helplabsadvance();
void helplabschapters();
void wiki(QString s);
QAction* Menu::sharedHelpAboutAct;

void Menu::createhelpActions()
{
#ifdef Q_OS_MAC
  if (sharedHelpAboutAct == NULL) { // QTBUG-17941
    sharedHelpAboutAct = makeact("helpaboutAct","&About","");
  }
  helpaboutAct = sharedHelpAboutAct;
#else
  helpaboutAct = makeact("helpaboutAct","&About","");
#endif
  helpconstantsAct = makeact("helpconstantsAct","&Constants","");
  helpcontextAct = makeact("helpcontextAct","&Context Sensitive","Ctrl+F1");
  helpcontextnuvocAct = makeact("helpcontextnuvocAct","&NuVoc Context Sensitive","Ctrl+Shift+F1");
  helpcontrolsAct = makeact("helpcontrolsAct","&Control Structures","");
  helpdemoqtAct = makeact("helpdemoqtAct","&Qt Demos...","");
  helpdemowdAct = makeact("helpdemowdAct","&Showcase...","");
  helpdictionaryAct = makeact("helpdictionaryAct","&Dictionary","");
  helpforeignsAct = makeact("helpforeignsAct","&Foreign Conjunction","");
  helpgeneralAct = makeact("helpgeneralAct","&General Info","");
  helphelpAct = makeact("helphelpAct","&Help","");
  helplabsAct = makeact("helplabsAct","&Labs...","");
  helplabsadvanceAct = makeact("helplabsadvanceAct","Advance","Ctrl+J");
  helplabschaptersAct = makeact("helplabschaptersAct","Chapters...","");
  helpreleaseAct = makeact("helpreleaseAct","&Release Highlights","");
  helprelnotesAct = makeact("helprelnotesAct","&Release Notes","");
  helpvocabAct = makeact("helpvocabAct","&Vocabulary","F1");
  helpvocabnuvocAct = makeact("helpvocabnuvocAct","&NuVoc","Shift+F1");
}

void Menu::createhelpMenu()
{
  helpMenu = addMenu(tr("&Help"));
  helpMenu->addAction(helphelpAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpgeneralAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpvocabAct);
  helpMenu->addAction(helpvocabnuvocAct);
  helpMenu->addAction(helpconstantsAct);
  helpMenu->addAction(helpcontrolsAct);
  helpMenu->addAction(helpforeignsAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpdictionaryAct);
  helpMenu->addSeparator();
  QMenu *m = helpMenu->addMenu(tr("Studio"));
  helpMenu->addSeparator();
  m->addAction(helplabsAct);
  m->addAction(helplabsadvanceAct);
  m->addAction(helplabschaptersAct);
  m->addSeparator();
  m->addAction(helpdemoqtAct);
  m->addSeparator();
  m->addAction(helpdemowdAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpcontextAct);
  helpMenu->addAction(helpcontextnuvocAct);
#ifndef __MACH__
  helpMenu->addSeparator();
#endif
  helpMenu->addAction(helpaboutAct);
  helpMenu->menuAction()->setMenuRole(QAction::NoRole);
}

// ---------------------------------------------------------------------
void Note::on_helpaboutAct_triggered()
{
  term->on_helpaboutAct_triggered();
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
void Note::on_helpcontextnuvocAct_triggered()
{
  helpcontextnuvoc(1,(Bedit *)editPage());
}

// ---------------------------------------------------------------------
void Note::on_helpcontrolsAct_triggered()
{
  term->on_helpcontrolsAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helpdemoqtAct_triggered()
{
  term->on_helpdemoqtAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helpdemowdAct_triggered()
{
  term->on_helpdemowdAct_triggered();
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
void Note::on_helpvocabnuvocAct_triggered()
{
  term->on_helpvocabnuvocAct_triggered();
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
void Term::on_helpcontextnuvocAct_triggered()
{
  helpcontextnuvoc(0,(Bedit *)tedit);
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
void Term::on_helpdemoqtAct_triggered()
{
  tedit->docmds("demoqt_jqtide_$0", false);
}

// ---------------------------------------------------------------------
void Term::on_helpdemowdAct_triggered()
{
  tedit->docmds("demowd_jqtide_$0", false);
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
void Term::on_helpreleaseAct_triggered()
{
  htmlhelp("user/relhigh.htm");
}

// ---------------------------------------------------------------------
void Term::on_helprelnotesAct_triggered()
{
  htmlhelp("release/contents");
}

// ---------------------------------------------------------------------
void Term::on_helpvocabAct_triggered()
{
  htmlhelp("dictionary/vocabul");
}

// ---------------------------------------------------------------------
void Term::on_helpvocabnuvocAct_triggered()
{
  wiki("NuVoc");
}

// ---------------------------------------------------------------------
void helpcontext(int c,Bedit *e)
{
  var_set("arg_jqtide_",e->readhelptext(c));
  QString s=var_cmdr("helpcontext0_jqtide_ arg_jqtide_");
  if (s.size())
    htmlhelp(s);
}

// ---------------------------------------------------------------------
void helpcontextnuvoc(int c,Bedit *e)
{
  var_set("arg_jqtide_",e->readhelptext(c));
  QString s=var_cmdr("helpcontext1_jqtide_ arg_jqtide_");
  if (s.size())
    wiki("Vocabulary/" + s);
}

// ---------------------------------------------------------------------
void helplabs()
{
  tedit->docmds("labs_run_jqtide_$0", false);
}

// ---------------------------------------------------------------------
void helplabsadvance()
{
  tedit->docmds("labs_run_jqtide_ 0", false);
}

// ---------------------------------------------------------------------
void helplabschapters()
{
  tedit->docmds("labs_run_jqtide_ 1", false);
}

// ---------------------------------------------------------------------
void htmlhelp(QString s)
{
  QString t=cpath("~addons/docs/help/") + s + ".htm";
  QString w="http://www.jsoftware.com/docs/help804/";
#ifdef QT_OS_ANDROID
  if (QFile(t).exists())
    tedit->docmds("browse_j_ '" + t.prepend("file://") + "'", false);
  else tedit->docmds("browse_j_ '" + s.prepend(w).append(".htm") + "'", false);
#else
  if (QFile(t).exists())
    QDesktopServices::openUrl(QUrl::fromLocalFile(t));
  else QDesktopServices::openUrl(QUrl(s.prepend(w).append(".htm"),QUrl::TolerantMode));
#endif
}

// ---------------------------------------------------------------------
void wiki(QString s)
{
  QString w="http://www.jsoftware.com/jwiki/";
#ifdef QT_OS_ANDROID
  tedit->docmds("browse_j_ '" + s.prepend(w) + "'", false);
#else
  QDesktopServices::openUrl(QUrl(s.prepend(w),QUrl::TolerantMode));
#endif
}
