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
  QTextCharFormat classFormat;
  QTextCharFormat singleLineCommentFormat;
  QTextCharFormat multiLineCommentFormat;
  QTextCharFormat quotationFormat;
  QTextCharFormat functionFormat;
};

#endif
