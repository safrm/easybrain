/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "aboutdialog.h"
#include "version.h"
#include "weblink.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
QLabel * iconLabel = new QLabel();
iconLabel->setPixmap(QIcon(":/images/tray_icon.svg").pixmap(30,30));

QHBoxLayout *versionLayout = new QHBoxLayout();
QLabel* pVersionLabel = new QLabel("version: ");
QLineEdit* pVersionLineEdit = new QLineEdit(Version::getFullString());
pVersionLineEdit->setReadOnly(true);
pVersionLineEdit->selectAll();
versionLayout->addWidget(pVersionLabel);
versionLayout->addWidget(pVersionLineEdit);

//changelog
WebLink* pChangelogFilelabel = new WebLink("Changelog",QUrl("/usr/share/doc/easybrain/todo.txt"),this);

//webpage
WebLink* pOfficialWebPageLabel = new WebLink("Easybrain",QUrl("http://easybrain.safrm.net"));
QString sMailTo = "mailto:easybrain@safrm.net?subject=Easybrain: version: " + QString(Version::getFullString());
WebLink* pSupportemailWebPageLabel = new WebLink("Send email to author",QUrl(sMailTo),this);

QLabel * logoLabel = new QLabel();
logoLabel->setPixmap(QIcon(":/images/tray_icon.svg").pixmap(143,73));

QHBoxLayout *licenseLayout = new QHBoxLayout();
licenseLayout->setAlignment(Qt::AlignHCenter);
QLabel* copyrightLabel = new QLabel();
QString sCopyright =  "Copyright " +  QString(Qt::Key_copyright) + QString(" ") + QString().setNum(int(COMPILE_TIME_YEAR));
copyrightLabel->setTextFormat(Qt::RichText);
copyrightLabel->setText(sCopyright);
WebLink* webPageLabel = new WebLink("safrm.net",QUrl("http://www.safrm.net"),this);
licenseLayout->addWidget(webPageLabel);
licenseLayout->addWidget(copyrightLabel);
WebLink* licenseFilelabel = new WebLink("License",QUrl("/usr/share/doc/easybrain/license.txt"),this);

//BUTTONS
QPushButton* closeButton = new QPushButton(tr("&Close"));
connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
//ALL TOGETHER
QVBoxLayout *mainLayout = new QVBoxLayout(this);
mainLayout->addWidget(logoLabel,0,Qt::AlignHCenter);
mainLayout->addLayout(versionLayout);
mainLayout->addWidget(pChangelogFilelabel,0,Qt::AlignHCenter);
mainLayout->addWidget(pOfficialWebPageLabel,0,Qt::AlignHCenter);
mainLayout->addWidget(pSupportemailWebPageLabel,0,Qt::AlignHCenter);
mainLayout->addWidget(new QWidget());
mainLayout->addLayout(licenseLayout, 0);
mainLayout->addWidget(licenseFilelabel, 0, Qt::AlignHCenter);
mainLayout->addWidget(new QWidget());
mainLayout->addWidget(closeButton,0,Qt::AlignHCenter);
setLayout(mainLayout);

setWindowTitle("About Easybrain ...");
setWindowIcon(QIcon(":/images/easybrain.svg"));
setModal(true);
}

QSize AboutDialog::sizeHint() const
{
    return QSize(400,200);
}
