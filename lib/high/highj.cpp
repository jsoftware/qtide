
/*
 * Note this contains experimental highlighting for J.
 * Current implementation attempts to suppport:
 * std::strings, numbers, primitive nouns, control words,
 * comments, multiline Note.
 * There is currently no context aware support (for example to show that control
 * words are not valid outside an explicit definition).
 */

#include "base/state.h"
#include "high/high.h"

// ---------------------------------------------------------------------
Highj::Highj(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
  init();
  HighlightingRule rule;

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

  rule.pattern = QRegExp("\\b[_0-9][_0-9\\.a-zA-Z]*\\b");
  rule.format = numberFormat;
  highlightingRules.append(rule);

  rule.pattern = QRegExp("(_\\.|a\\.|a:)(?![\\.\\:])");
  rule.format = nounFormat;
  highlightingRules.append(rule);

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

  rule.pattern = QRegExp("\\{\\{");
  rule.format = controlFormat;
  highlightingRules.append(rule);

  rule.pattern = QRegExp("\\}\\}");
  rule.format = controlFormat;
  highlightingRules.append(rule);

  rule.pattern = QRegExp("'[^']*'");
  rule.format = stringFormat;
  highlightingRules.append(rule);

  rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
  rule.format = functionFormat;
  highlightingRules.append(rule);

  rule.pattern = QRegExp("\\bNB\\.[^\n]*");
  NBPattern = rule.pattern;
  rule.format = singleLineCommentFormat;
  highlightingRules.append(rule);

  noundefStartExpression = QRegExp("\\b(0\\s+:\\s*0|noun\\s+define)\\b.*$");
  noundefEndExpression = QRegExp("^\\s*\\)\\s*$");

  commentStartExpression = QRegExp("^\\s*\\bNote\\b(?!\\s*\\=[:.])\\s*['\\d].*$");
  commentEndExpression = QRegExp("^\\s*\\)\\s*$");
}

// ---------------------------------------------------------------------
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
  int NBIndex = 0;
  int NBLength = 0;
  int NBL;
  if (previousBlockState() != 1) {
    NBIndex = NBPattern.indexIn(text);
    NBLength = NBPattern.matchedLength();
    startIndex = noundefStartExpression.indexIn(text);
    if ((NBIndex > -1) && (NBIndex < startIndex)) {
      startIndex = noundefStartExpression.indexIn(text, NBLength+NBIndex);
    }
  }

  while (startIndex >= 0) {
    int endIndex = noundefEndExpression.indexIn(text, startIndex);
    int noundefLength;
    if (endIndex == -1) {
      setCurrentBlockState(1);
      if (NBLength > 0) noundefLength = (text.length() - startIndex) - NBLength;
      else
        noundefLength = text.length() - startIndex;
    } else {
      if (NBLength > 0) NBL = noundefEndExpression.matchedLength() - NBLength;
      else
        NBL = noundefEndExpression.matchedLength();
      noundefLength = endIndex - startIndex
                      + NBL;
    }
    setFormat(startIndex, noundefLength, noundefFormat);
    startIndex = noundefStartExpression.indexIn(text, startIndex + noundefLength);
  }

}

// ---------------------------------------------------------------------
// set up styles
void Highj::init()
{
  init1(&adverbFormat, config.adverbStyle);
  init1(&singleLineCommentFormat, config.commentStyle);
  init1(&multiLineCommentFormat, config.commentStyle);
  init1(&conjunctionFormat, config.conjunctionStyle);
  init1(&controlFormat, config.controlStyle);
  init1(&functionFormat, config.functionStyle);
  init1(&nounFormat, config.nounStyle);
  init1(&noundefFormat, config.noundefStyle);
  init1(&numberFormat, config.numberStyle);
  init1(&stringFormat, config.stringStyle);
  init1(&verbFormat, config.verbStyle);
}

// ---------------------------------------------------------------------
void Highj::init1(QTextCharFormat *f, Style s)
{

  f->setForeground(s.color);
  f->setFontItalic(s.italic);
  f->setFontWeight(s.weight);

}
