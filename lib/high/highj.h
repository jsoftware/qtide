#ifndef HIGHJ_H
#define HIGHJ_H

#include <QSyntaxHighlighter>
#include "../base/base.h"

class Highj : public QSyntaxHighlighter
{

  Q_OBJECT

public:
  Highj(QTextDocument *parent=0);

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

  QTextCharFormat controlFormat;
  QTextCharFormat numberFormat;
  QTextCharFormat nounFormat;
  QTextCharFormat singleLineCommentFormat;
  QTextCharFormat multiLineCommentFormat;
  QTextCharFormat stringFormat;
  QTextCharFormat functionFormat;
};

#endif
