
#include <QtWebKit>

#include "wd.h"
#include "webview.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Webview::Webview(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="webview";
  QWebView *w=new QWebView(p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html"));
}

// ---------------------------------------------------------------------
void Webview::set(string p,string v)
{
  QWebView *w = (QWebView *)widget;
  if (p=="baseurl") {
    baseUrl = QUrl(s2q(v));
  } else if (p=="html") {
    w->setHtml(s2q(v), baseUrl);
    w->show();
  } else if (p=="url") {
    w->load(QUrl(s2q(v)));
    w->show();
  } else Child::set(p,v);
}
