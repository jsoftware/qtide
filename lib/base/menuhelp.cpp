#include <QApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

#include "base.h"
#include "menu.h"
#include "note.h"
#include "state.h"
#include "svr.h"
#include "nedit.h"
#include "term.h"
#include "tedit.h"

// using namespace std;

void helpabout();
void helpcontext(int c,QWidget *);
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
  helpcontrolsAct = makeact("helpcontrolsAct","&Control Structures","");
  helpdemoqtAct = makeact("helpdemoqtAct","&Qt Demos...","");
  helpdemowdAct = makeact("helpdemowdAct","&Showcase...","");
  helpstdlibAct = makeact("helpstdlibAct","&Standard Library","");
  helpforeignsAct = makeact("helpforeignsAct","&Foreigns","");
  helplabsAct = makeact("helplabsAct","&Labs...","");
  helplabsadvanceAct = makeact("helplabsadvanceAct","Advance","Ctrl+J");
  helplabschaptersAct = makeact("helplabschaptersAct","Chapters...","");
  helprelnotesAct = makeact("helprelnotesAct","&Release Notes","");
  helpvocabAct = makeact("helpvocabAct","&Vocabulary","F1");
  helpwdAct = makeact("helpwdAct","&Window Driver","");
  helpwikiAct = makeact("helpwikiAct","&Wiki","");
}

void Menu::createhelpMenu()
{
  helpMenu = addMenu(tr("&Help"));
  helpMenu->addAction(helpwikiAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpvocabAct);
  helpMenu->addAction(helpconstantsAct);
  helpMenu->addAction(helpcontrolsAct);
  helpMenu->addAction(helpforeignsAct);
  helpMenu->addAction(helprelnotesAct);
  helpMenu->addSeparator();
  helpMenu->addAction(helpstdlibAct);
  helpMenu->addAction(helpwdAct);
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
  helpcontext(1,editPage());
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
void Note::on_helpstdlibAct_triggered()
{
  term->on_helpstdlibAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helpforeignsAct_triggered()
{
  term->on_helpforeignsAct_triggered();
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
void Note::on_helpwdAct_triggered()
{
  term->on_helpwdAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_helpwikiAct_triggered()
{
  term->on_helpwikiAct_triggered();
}

// ---------------------------------------------------------------------
void Term::on_helpaboutAct_triggered()
{
  helpabout();
}

// ---------------------------------------------------------------------
void Term::on_helpcontextAct_triggered()
{
  helpcontext(0,tedit);
}

// ---------------------------------------------------------------------
void Term::on_helpconstantsAct_triggered()
{
  wiki("Vocabulary/Constants");
}

// ---------------------------------------------------------------------
void Term::on_helpcontrolsAct_triggered()
{
  wiki("Vocabulary/ControlStructures");
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
void Term::on_helpstdlibAct_triggered()
{
  wiki("Standard_Library/Overview");
}

// ---------------------------------------------------------------------
void Term::on_helpforeignsAct_triggered()
{
  wiki("Vocabulary/Foreigns");
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
void Term::on_helprelnotesAct_triggered()
{
  wiki("System/ReleaseNotes/J904");
}

// ---------------------------------------------------------------------
void Term::on_helpvocabAct_triggered()
{
  wiki("NuVoc");
}

// ---------------------------------------------------------------------
void Term::on_helpwdAct_triggered()
{
  wiki("Guides/Window_Driver");
}

// ---------------------------------------------------------------------
void Term::on_helpwikiAct_triggered()
{
  wiki("");
}

// ---------------------------------------------------------------------
void helpcontext(int c,QWidget *e)
{
  jcon->set("arg_jqtide_",q2s(readhelptext(e,c)));
  QString s=jcon->cmdr("helpcontext1_jqtide_ arg_jqtide_");
  if (s.size())
    wiki("Vocabulary/" + s);
}

// ---------------------------------------------------------------------
void helplabs()
{
  tedit->docmddo("labs_run_jqtide_$0");
}

// ---------------------------------------------------------------------
void helplabsadvance()
{
  tedit->docmddo("labs_run_jqtide_ 0");
}

// ---------------------------------------------------------------------
void helplabschapters()
{
  tedit->docmddo("labs_run_jqtide_ 1");
}

// ---------------------------------------------------------------------
void wiki(QString s)
{
  QString w="http://code.jsoftware.com/wiki/";
  QDesktopServices::openUrl(QUrl(s.prepend(w),QUrl::TolerantMode));
}
