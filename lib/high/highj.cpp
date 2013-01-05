
/*
 * note this only done as an experiment for simple control words
 * rest is just boilerplate cpp highlighting...
 */

#include "high/high.h"

Highj::Highj(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
  HighlightingRule rule;

  controlFormat.setForeground(Qt::red);
  controlFormat.setFontWeight(QFont::Bold);

  QStringList controlPatterns;

  controlPatterns
      << "\\bassert\\." << "\\bbreak\\." << "\\bcontinue\\."
      << "\\breturn\\." << "\\bdo\\." << "\\bif\\."
      << "\\belse\\." << "\\belseif\\." << "\\bend\\."
      << "\\bfor\\." << "\\bselect\\." << "\\bcase\\."
      << "\\bfcase\\." << "\\bthrow\\." << "\\btry\\."
      << "\\bcatch\\." << "\\bcatchd\\." << "\\bcatcht\\."
      << "\\bwhile\\." << "\\bwhilst\\." << "\\bfor_\\%{valid-name}?\\."
      ;

// not done yet...
// "\\bgoto_\\%{valid-name}?\\.\\s" << "\\blabel_\\%{valid-name}?\\.\\s"


  foreach (const QString &pattern, controlPatterns) {
    rule.pattern = QRegExp(pattern);
    rule.format = controlFormat;
    highlightingRules.append(rule);
  }

  classFormat.setFontWeight(QFont::Bold);
  classFormat.setForeground(Qt::darkMagenta);
  rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
  rule.format = classFormat;
  highlightingRules.append(rule);

  singleLineCommentFormat.setForeground(Qt::red);
  rule.pattern = QRegExp("NB\\.[^\n]*");
  rule.format = singleLineCommentFormat;
  highlightingRules.append(rule);

  multiLineCommentFormat.setForeground(Qt::red);

  quotationFormat.setForeground(Qt::darkGreen);
  rule.pattern = QRegExp("\'.*\'");
  rule.format = quotationFormat;
  highlightingRules.append(rule);

  functionFormat.setFontItalic(true);
  functionFormat.setForeground(Qt::blue);
  rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
  rule.format = functionFormat;
  highlightingRules.append(rule);

  commentStartExpression = QRegExp("/\\*");
  commentEndExpression = QRegExp("\\*/");
}


void Highj::highlightBlock(const QString &text)
{
  foreach (const HighlightingRule &rule, highlightingRules) {
    QRegExp expression(rule.pattern);
    int index = expression.indexIn(text);
    while (index >= 0) {
      int length = expression.matchedLength();
      setFormat(index, length, rule.format);
      index = expression.indexIn(text, index + length);
    }
  }
  setCurrentBlockState(0);

  int startIndex = 0;
  if (previousBlockState() != 1)
    startIndex = commentStartExpression.indexIn(text);

  while (startIndex >= 0) {
    int endIndex = commentEndExpression.indexIn(text, startIndex);
    int commentLength;
    if (endIndex == -1) {
      setCurrentBlockState(1);
      commentLength = text.length() - startIndex;
    } else {
      commentLength = endIndex - startIndex
                      + commentEndExpression.matchedLength();
    }
    setFormat(startIndex, commentLength, multiLineCommentFormat);
    startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
  }
}
