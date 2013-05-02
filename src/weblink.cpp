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
