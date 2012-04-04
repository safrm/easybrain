/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#ifndef DEFINES_H
#define DEFINES_H
#include <QMetaType>
class QString;
typedef QString ProjectString;
typedef QString TimeStampString;

enum ItemType {
      Working,
      Archive,
      Backup
};

namespace EditorView {
    enum Type {
        Default, //from settings
        XmlSource,
        Text,
        RichFormat,
        OnlyPlainData, //data as a text
        CountOfTypes
    };
}
Q_DECLARE_METATYPE(EditorView::Type)
//Q_ENUMS(EditorView::Type)

#endif // DEFINES_H
