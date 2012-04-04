/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#include <QDesktopServices>
#include "weblink.h"

WebLink::WebLink(const QString &text,const QUrl &url,QWidget *parent) :
    QLabel(parent)
{
    setTextFormat(Qt::RichText);
    QString sWithFormat = tr("<font color=blue><u>") + text + tr("</u></color>");
    setText(sWithFormat);
    m_url = url;
    connect(this,SIGNAL(clicked()),this,SLOT(openUrl()));
}

void WebLink::mousePressEvent(QMouseEvent *ev)
{
    emit clicked();
    QLabel::mousePressEvent(ev);
}

void WebLink::openUrl()
{
    QDesktopServices::openUrl(m_url);
}
