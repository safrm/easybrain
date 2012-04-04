/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#ifndef XMLHIGHTLIGHTER_H
#define XMLHIGHTLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QColor>

class XmlHighlighter : public QSyntaxHighlighter
{
public:
    explicit XmlHighlighter(QTextDocument* parent);

    enum HighlightType
    {
        SyntaxChar,
        ElementName,
        Comment,
        AttributeName,
        AttributeValue,
        Error,
        Other
    };

    void setHighlightColor(HighlightType type, QColor color, bool foreground = true);
    void setHighlightFormat(HighlightType type, QTextCharFormat format);

protected:
    void highlightBlock(const QString& rstrText);
    int  processDefaultText(int i, const QString& rstrText);

private:

    QTextCharFormat fmtSyntaxChar;
    QTextCharFormat fmtElementName;
    QTextCharFormat fmtComment;
    QTextCharFormat fmtAttributeName;
    QTextCharFormat fmtAttributeValue;
    QTextCharFormat fmtError;
    QTextCharFormat fmtOther;

    enum ParsingState
    {
        NoState = 0,
        ExpectElementNameOrSlash,
        ExpectElementName,
        ExpectAttributeOrEndOfElement,
        ExpectEqual,
        ExpectAttributeValue
    };

    enum BlockState
    {
        NoBlock = -1,
        InComment,
        InElement
    };

    ParsingState state;
};

#endif // XMLHIGHTLIGHTER_H
