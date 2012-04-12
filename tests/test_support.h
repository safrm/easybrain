/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://safrm.net/easybrain
** Source: https://github.com/safrm/easybrain
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/
#ifndef TEST_SUPPORT_H
#define TEST_SUPPORT_H

namespace QTest {
/*
    template <>
    inline char *toString<long double>(const long double &t)
    {
        char *msg = new char[128];
        ::snprintf(msg, 128, "%.123Lg", t);
        return msg;
    }

    static inline bool qFuzzyCompare(long double p1, long double p2)
    {
        return (qAbs(p1 - p2) <= 0.00000000000001 * qMin(qAbs(p1), qAbs(p2)));
    }

    template <>
    inline bool Q_TESTLIB_EXPORT qCompare<long double>(long double const &t1,
            long double const &t2, const char *actual, const char *expected, const char *file, int line)
    {
        return qFuzzyCompare(t1, t2)
            ? compare_helper(true, "COMPARE()", file, line)
            : compare_helper(false, "Compared long doubles are not the same (fuzzy compare)",
                    toString(t1), toString(t2), actual, expected, file, line);
    }
    */
}

bool compareFiles(const QString &filename, const QString &correctFilename)
{
    bool filesAreTheSame = true;

    QFile newFile(filename);
    if (!newFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "a Could not open" << filename;
        return false;
    }
    QTextStream newIn(&newFile);

    QFile correctFile(correctFilename);
    if (!correctFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "b Could not open" << correctFilename;
        return false;
    }
    QTextStream correctIn(&correctFile);

    do {
        if (newIn.atEnd() || correctIn.atEnd()) {
            bool oneFileFinishedBeforeOther = (!newIn.atEnd() || !correctIn.atEnd());
            if (oneFileFinishedBeforeOther) {
                qDebug( "one file finished before the other" );
                filesAreTheSame = false;
            }
            break;
        }

        QString newLine     = newIn.readLine();
        QString correctLine = correctIn.readLine();

        // skip exceptional lines
        QString headerGuard(QFileInfo(filename).fileName().toUpper().replace(".", "_"));
        bool lineIsExceptional =
            newLine.contains(headerGuard) ||
            newLine.contains("** Created: ") ||
            newLine.contains("**      by: The Qt Meta Object Compiler version ") ||
            newLine.contains("#error \"This file was generated using the moc from") ||
            newLine.contains("#include ") ||
            newLine.contains("** Meta object code from reading C++ file ") ||
            newLine.contains("ft_mservicefwgen")
          ;
        if (lineIsExceptional) {
            continue;
        }

        bool linesAreIdentical = (newLine == correctLine);
        if ( ! linesAreIdentical )
        {
          qDebug() << "got these different lines: new:\n"
                   << filename
                   << "\n"
                   << newLine
                   << "\nexpected:\n"
                   << correctFilename
                   << "\n"
                   << correctLine;
        }
        filesAreTheSame = linesAreIdentical;
    } while (filesAreTheSame);

    correctFile.close();
    newFile.close();

    return filesAreTheSame;
}
#endif // TEST_SUPPORT_H
