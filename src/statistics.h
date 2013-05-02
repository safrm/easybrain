/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://easybrain.safrm.net/
** Source: http://safrm.net/projects/easybrain
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

#ifndef STATISTICS_H
#define STATISTICS_H
#include <QDockWidget>
#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
#  include <QTime>
typedef QTime QElapsedTimer;
#else
#  include <QElapsedTimer>
#endif

class QLabel;
class StatisticsDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit StatisticsDock(QWidget *parent = 0);

private slots:
    void updateStatistics();

private:
    QLabel * bootedUpTimeLabel;
    QLabel * appRunningTimeLabel;
    QLabel * lastUpdateLabel;
    QElapsedTimer* elapsedTime;
};

#endif // STATISTICS_H
