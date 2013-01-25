
#include <QtWebKit>

#include "wd.h"
#include "webview.h"
#include "form.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Webview::Webview(string s, string p, Form *f) : Child(s,p,f)
{
  type="webview";
  QWebView *w=new QWebView;
  widget=(QWidget *) w;
  QString qs=s2q(s);
  w->setObjectName(qs);
  baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html"));
}

// ---------------------------------------------------------------------
void Webview::set(string p)
{
  ((QWebView *)widget)->load(QUrl(s2q(p)));
  ((QWebView *)widget)->show();
}

// ---------------------------------------------------------------------
void Webview::setp(string p,string v)
{
  if (p=="baseurl") {
    baseUrl = QUrl(s2q(v));
  } else if (p=="html") {
    ((QWebView *)widget)->setHtml(s2q(v), baseUrl);
    ((QWebView *)widget)->show();
  } else Child::setp(p,v);
}

