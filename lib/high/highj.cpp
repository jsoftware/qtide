
/*
 * Note this contains experimental highlighting for J.
 * Current implementation attempts to suppport:
 * strings, numbers, primitive nouns, control words,
 * comments, multiline Note.
 * There is currently no context aware support (for example to show that control
 * words are not valid outside an explicit definition).
 */

#include "high/high.h"

Highj::Highj(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
  HighlightingRule rule;

  controlFormat.setForeground(Qt::red);

  QStringList controlPatterns;

  controlPatterns
      << "\\bassert\\." << "\\bbreak\\." << "\\bcontinue\\."
      << "\\breturn\\." << "\\bdo\\." << "\\bif\\."
      << "\\belse\\." << "\\belseif\\." << "\\bend\\."
      << "\\bfor\\." << "\\bselect\\." << "\\bcase\\."
      << "\\bfcase\\." << "\\bthrow\\." << "\\btry\\."
      << "\\bcatch\\." << "\\bcatchd\\." << "\\bcatcht\\."
      << "\\bwhile\\." << "\\bwhilst\\." << "\\bfor_[a-zA-Z][a-zA-Z0-9_]*\\."
      << "\\bgoto_[a-zA-Z][a-zA-Z0-9_]*\\." << "\\blabel_[a-zA-Z][a-zA-Z0-9_]*\\."
      ;

  foreach (const QString &pattern, controlPatterns) {
    rule.pattern = QRegExp(pattern);
    rule.format = controlFormat;
    highlightingRules.append(rule);
  }

  numberFormat.setForeground(QColor(160,32,240));
  rule.pattern = QRegExp("\\b[_0-9][_0-9\\.a-zA-Z]*\\b");
  rule.format = numberFormat;
  highlightingRules.append(rule);

  nounFormat.setFontWeight(QFont::Bold);
  nounFormat.setForeground(Qt::blue);
  rule.pattern = QRegExp("(_\\.|a\\.|a:)(?![\\.\\:])");
  rule.format = nounFormat;
  highlightingRules.append(rule);
  
  verbFormat.setForeground(QColor(0,153,102));
  rule.pattern = QRegExp("((_?[0-9]:)|(\\bp\\.\\.)|(\\b[AcCeEiIjLopr]\\.)|(\\b[ipqsux]:)|(\\{::)|([\\<\\>\\+\\*\\-\\%\\^\\$\\~\\|\\,\\#\\{\\}\\\"\\;\\?]\\.)|([\\<\\>\\_\\+\\*\\-\\%\\$\\~\\|\\,\\;\\#\\/\\\\[\\{\\}\\\"]:)|([\\<\\>\\=\\+\\*\\-\\%\\^\\$\\|\\,\\;\\#\\!\\[\\]\\{\\?]))(?![\\.\\:])");

  /* The line continuations below seem to break the RegExp so it doesn't work.
  rule.pattern = QRegExp("((_?[0-9]:)|             \
                          (\\bp\\.\\.)|            \
                          (\\b[AcCeEiIjLopr]\\.)|  \
                          (\\b[ipqsux]:)|          \
                          (\\{::)|                 \
                          ([\\<\\>\\+\\*\\-\\%\\^\\$\\~\\|\\,\\#\\{\\}\\\"\\;\\?]\\.)|   \
                          ([\\<\\>\\_\\+\\*\\-\\%\\$\\~\\|\\,\\;\\#\\/\\\\[\\{\\}\\\"]:)| \
                          ([\\<\\>\\=\\+\\*\\-\\%\\^\\$\\|\\,\\;\\#\\!\\[\\]\\{\\?]))    \
                          (?![\\.\\:])");
  */

  /*  The original gtk regular expressions for verbs.
        ((_?[0-9]:)|
        (\\bp\\.\\.)|              
        (\\b[AcCeEiIjLopr]\\.)|
        (\\b[ipqsux]:)|
        ({::)|
        ([&lt;&gt;\+\*\-\%\^\$\~\|\,\#\{\}&quot;\?]\.)|
        ([&lt;&gt;\_\+\*\-\%\$\~\|\,\;\#\/\\\[\{\}&quot;]:)|
        ([&lt;&gt;\=\+\*\-\%\^\$\|\,\;\#\!\[\]\{\?]))
        (?![\.\:]) 
  */
  rule.format = verbFormat;
  highlightingRules.append(rule);

  adverbFormat.setForeground(QColor(221,68,68));
  rule.pattern = QRegExp("(([\\/\\\\]\\.)|(\\b[bfMt]\\.)|(\\bt:)|([\\~\\/\\\\}]))(?![\\.\\:])");

  /* The line continuations below seem to break the RegExp so it doesn't work.
  rule.pattern = QRegExp("(([\\/\\]\\.)|    \
                           (\\b[bfMt]\\.)|  \
                           (\\bt:)|         \
                           ([\\~\\/\\\\}])) \
                          (?![\\.\\:])");
  */
  /* The original gtk regular expressions for adverbs.
        (([\/\\]\.)| 
        (\%[[bfMt]\.)|
        (\%[t:)|
        ([\~\/\\\}]))
        (?![\.\:])

  */
  rule.format = adverbFormat;
  highlightingRules.append(rule);

  conjunctionFormat.setForeground(QColor(221,153,0));
  rule.pattern = QRegExp("((\\b[dDHT]\\.)|(\\b[DLS]:)|(\\&\\.:)|([\\;\\!\\@\\&]\\.)|([\\^\\!\\`\\@\\&]:)|([\\\"\\`\\@\\&])|(\\s[\\.\\:][\\.\\:])|(\\s[\\.\\:]))(?![\\.\\:])");

  /* The line continuations below seem to break the RegExp so it doesn't work.
  rule.pattern = QRegExp("((\\b[dDHT]\\.)|
                           (\\b[DLS]:)|
                           (\\&\\.:)|
                           ([\\;\\!\\@\\&]\\.)|
                           ([\\^\\!\\`\\@\\&]:)|
                           ([\\\"\\`\\@\\&])|
                           (\\s[\\.\\:][\\.\\:])|
                           (\\s[\\.\\:]))
                          (?![\\.\\:])");  
  */

  /*  The original gtk regular expressions for conjunctions.
        ((\%[[dDHT]\.)|
        (\%[[DLS]:)|
        (&amp;\.:)|
        ([\;\!\@&amp;]\.)|
        ([\^\!\`\@&amp;]:)|
        ([\"\`\@&amp;])|
        (\s[\.\:][\.\:])|
        (\s[\.\:]))
        (?![\.\:])
  */
  rule.format = conjunctionFormat;
  highlightingRules.append(rule);


  stringFormat.setForeground(Qt::blue);
  rule.pattern = QRegExp("\'.*\'");
  rule.format = stringFormat;
  highlightingRules.append(rule);

  singleLineCommentFormat.setFontItalic(true);
  singleLineCommentFormat.setForeground(Qt::gray);
  rule.pattern = QRegExp("\\bNB\\.[^\n]*");
  rule.format = singleLineCommentFormat;
  highlightingRules.append(rule);

  multiLineCommentFormat.setFontItalic(true);
  multiLineCommentFormat.setForeground(Qt::gray);

  functionFormat.setFontItalic(true);
  functionFormat.setForeground(Qt::blue);
  rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
  rule.format = functionFormat;
  highlightingRules.append(rule);

  commentStartExpression = QRegExp("^\\s*\\bNote\\b(?!\\s*\\=[:.])\\s*['\\d].*$");
  commentEndExpression = QRegExp("^\\s*\\)\\s*$");
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
