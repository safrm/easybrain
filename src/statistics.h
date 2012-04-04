/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
