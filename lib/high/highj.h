#ifndef HIGHJ_H
#define HIGHJ_H

#include <QSyntaxHighlighter>
#include "../base/base.h"
#include "../base/style.h"

class Highj : public QSyntaxHighlighter
{

  Q_OBJECT

public:
  Highj(QTextDocument *parent=0);
  void init();
  void init1(QTextCharFormat *f,Style s);

protected:
  void highlightBlock(const QString &text);

private:
  struct HighlightingRule {
    QRegExp pattern;
    QTextCharFormat format;
  };
  QVector<HighlightingRule> highlightingRules;

  QRegExp commentStartExpression;
  QRegExp commentEndExpression;
  QRegExp noundefStartExpression;
  QRegExp noundefEndExpression;
  QRegExp NBPattern;

  QTextCharFormat controlFormat;
  QTextCharFormat numberFormat;
  QTextCharFormat nounFormat;
  QTextCharFormat verbFormat;
  QTextCharFormat adverbFormat;
  QTextCharFormat conjunctionFormat;
  QTextCharFormat noundefFormat;
  QTextCharFormat singleLineCommentFormat;
  QTextCharFormat multiLineCommentFormat;
  QTextCharFormat stringFormat;
  QTextCharFormat functionFormat;
};

#endif
