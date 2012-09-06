/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://easybrain.safrm.net/
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

#include <QStringList>
#include <QTextStream>
#include <QApplication>
#include <QStatusBar>
#include <QMessageBox>
#include <QTabBar>
#include <QMenu>
#include <QClipboard>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QMetaType>
#include <QPrintDialog>
#include <QPrinter>

#include "editortabwidget.h"
#include "dataowner.h"
#include "mainwindow.h"
#include "settings.h"
#include "ebtextedit.h"
#include "filegroupwidget.h"

namespace EditorView {
    namespace String {
    const QString XmlSource("XmlSource");
    const QString Text("Text");
    const QString RichFormat("RichFormat");
    const QString OnlyPlainData("OnlyPlainData");
} //namespace String

Type Int2Enum(int iType)
{
    //qRegisterMetaType<EditorView::Type>("EditorView::Type");
    //todo some better check for validity
    Q_ASSERT(iType<=EditorView::CountOfTypes);
    return static_cast<EditorView::Type>(iType);
}

Type String2Enum(const QString & sType)
{
    if (sType == EditorView::String::XmlSource)
        return EditorView::XmlSource;
    if (sType == EditorView::String::Text)
        return EditorView::Text;
    if (sType == EditorView::String::RichFormat)
        return EditorView::RichFormat;
    if (sType == EditorView::String::OnlyPlainData)
        return EditorView::OnlyPlainData;

    Q_ASSERT(0); //also default is not allowed
    return EditorView::XmlSource; //default
}

const QString Enum2String(Type  eType)
{
    switch(eType) {
    case EditorView::XmlSource:
        return EditorView::String::XmlSource;
    case EditorView::Text:
        return EditorView::String::Text;
    case EditorView::RichFormat:
        return EditorView::String::RichFormat;
    case EditorView::OnlyPlainData:
        return EditorView::String::OnlyPlainData;
    default:
        Q_ASSERT(0);
    }
    return EditorView::String::XmlSource;
}

} //namespace EditorView



class EditorTabWidget_private : public QObject
{
public:
    explicit EditorTabWidget_private(QObject* parent) :
        QObject(parent),
        m_RightClickMenu(NULL),
        textViewAction(NULL),
        sourceViewAction(NULL),
        dataViewAction(NULL),
        changeViewActionGroup(NULL),
        m_pPrinter(NULL)
    {};

    QIcon iconAccordingViewType(EditorView::Type viewType) const;
    QMenu* m_RightClickMenu;
    QAction* textViewAction;
    QAction* sourceViewAction;
    QAction* dataViewAction;
    QActionGroup* changeViewActionGroup;
    QPrinter* m_pPrinter;
};

QIcon EditorTabWidget_private::iconAccordingViewType(EditorView::Type viewType) const
{
    QString sIcon(":/images/def.svg");
    switch(viewType) {
    case EditorView::XmlSource:
        sIcon = ":/images/view_source.svg";
        break;
    case EditorView::Text:
        sIcon = ":/images/view_text.svg";
        break;
    case EditorView::RichFormat:
        sIcon = ":/images/view_rich.svg";
        break;
    default:
        Q_ASSERT(viewType>=EditorView::RichFormat);
    }
    return QIcon(sIcon);
}

EditorTabWidget::EditorTabWidget(QWidget *parent) :
    QTabWidget(parent),
    d(new EditorTabWidget_private(this))
{

   connect(this, SIGNAL(currentChanged(int)), this, SLOT(enableToolbars(int)));
   connect(this, SIGNAL(currentViewChanged(EditorView::Type)), this, SLOT(updateViewActions(EditorView::Type)));
}

QActionGroup* EditorTabWidget::changeViewActionGroup()
{
    if (!d->changeViewActionGroup) {
        d->sourceViewAction = new QAction(QIcon(":/images/view_source.svg"), tr("&Source"), this);
        d->sourceViewAction->setObjectName("SourceViewAction");
        d->sourceViewAction->setCheckable(true);

        d->textViewAction = new QAction(QIcon(":/images/view_text.svg"), tr("&Text"), this);
        d->textViewAction->setObjectName("TextViewAction");
        d->textViewAction->setCheckable(true);

        d->dataViewAction = new QAction(QIcon(":/images/view_data.svg"), tr("&Only data"), this);
        d->dataViewAction->setObjectName("DataViewAction");
        d->dataViewAction->setCheckable(true);

        d->changeViewActionGroup = new QActionGroup(this);
        d->changeViewActionGroup->addAction(d->sourceViewAction);
        d->changeViewActionGroup->addAction(d->textViewAction);
        d->changeViewActionGroup->addAction(d->dataViewAction);
        d->changeViewActionGroup->setExclusive(true);
connect(d->changeViewActionGroup,SIGNAL(triggered(QAction *)),this,SLOT(switchView(QAction*)));
    }
    return d->changeViewActionGroup;
}

bool EditorTabWidget::addNewItem()
{
    QString newFullFileName = DO->newItemFile();
    if (newFullFileName.isEmpty())
        return false;

    MW->currentProjectFileGroup()->addNewFile(newFullFileName);
    MW->statusBar()->showMessage(tr("New Item %1").arg(newFullFileName));

    //use default view
    EBTextEdit *textEdit = new EBTextEdit(newFullFileName, Settings::General.eDefaultOpenViewType, this);
    if (!textEdit->readFile())
        return false;

    int iTabIndex = addTab(textEdit,QFileInfo(newFullFileName).baseName());
    setTabToolTip(iTabIndex,  textEdit->itemNameFromDocument());
    setTabIcon(iTabIndex,d->iconAccordingViewType(Settings::General.eDefaultOpenViewType));
    setCurrentIndex(iTabIndex);
    emit currentViewChanged(Settings::General.eDefaultOpenViewType);
    return true;
}

bool EditorTabWidget::addOpenItem(const QString &sFileName, EditorView::Type viewType)
{
    if (viewType== EditorView::Default)
        viewType = Settings::General.eDefaultOpenViewType;

    //is already open?
    EBTextEdit *textEdit = textEditByFilename(sFileName);
    if (textEdit) {
       setCurrentIndex(indexOf(textEdit));
       return true;
    }

    textEdit = new EBTextEdit(sFileName, viewType, this);
    if (!textEdit->readFile())
        return false;


    int iTabIndex = addTab(textEdit, QFileInfo(sFileName).baseName());
    setTabToolTip(iTabIndex,   textEdit->itemNameFromDocument());
    setTabIcon(iTabIndex,d->iconAccordingViewType(viewType));
    setCurrentIndex(iTabIndex);
    emit currentViewChanged(viewType);
    return true;
}

bool EditorTabWidget::saveCurrentItem()
{
    EBTextEdit *textEdit = currentTextEdit();
    if (!textEdit)
        return false;
    if (!textEdit->document()->isModified())
        return true;
    if (textEdit->writeFile())  {
        QString sFileName = currentTabFileName();
        QString sItemName = textEdit->itemNameFromDocument();
        MW->statusBar()->showMessage(sFileName + tr(" saved at ") + DO->now(DataOwnerSingl::TIME_SIMPLE_FORMAT));
        setTabToolTip(currentIndex(), sItemName);
        emit refreshFileRow(sFileName, sItemName);
        return true;
    }
    return false;
}

bool EditorTabWidget::saveAsCurrentItem(const QString & sFileName)
{
    if (sFileName.isEmpty())
        return false;

    EBTextEdit *textEdit = currentTextEdit();
    if (!textEdit)
        return false;

    if (textEdit->writeFile(sFileName))  {
        textEdit->setDocumentTitle(sFileName);
        setTabToolTip(currentIndex(),   textEdit->itemNameFromDocument());
        MW->statusBar()->showMessage(tr("Exported to ") + sFileName);
        return true;
    }
    QMessageBox::warning(this, tr("EB"),
        tr("Cannot export to file %1").arg(sFileName));
    return false;
}

bool EditorTabWidget::saveAll()
{
    EBTextEdit *textEdit(NULL);
    for(int i=0;i< count();++i) {
        textEdit = qobject_cast<EBTextEdit*>(widget(i));
        Q_ASSERT(textEdit);
        if (!textEdit->writeFile())
            return false;
    }
    return true;
}

bool EditorTabWidget::closeTab(QString sFileName)
{
    if (sFileName.isEmpty())
        sFileName = currentTabFileName();
    EBTextEdit *textEdit = textEditByFilename(sFileName);
    Q_ASSERT(textEdit);

    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton reply = QMessageBox::question(MW, tr("Close file"),
            tr("Do you want to <b>save file</b> %1 before closing?").arg(sFileName),
            QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel );
        if (reply == QMessageBox::Cancel)
            return false;
        else if (reply == QMessageBox::Yes )
            textEdit->writeFile();
        //No = continue closing
    }
    removeTab(indexOf(textEdit));
    delete(textEdit);
    return true;
}
void EditorTabWidget::closeOtherTabs()
{
    //if the sFileName is empty it will close all
    QString sFileName = currentTabFileName();
    EBTextEdit* textEdit(NULL);
    for(int i=count()-1;i>=0 ;--i) {
        textEdit = qobject_cast<EBTextEdit*>(widget(i));
        Q_ASSERT(textEdit);
        Q_ASSERT(textEdit->documentTitle().size());
        if (textEdit->documentTitle() != sFileName) {
            removeTab(indexOf(textEdit));
            delete(textEdit);
            textEdit = NULL;
        }
    }
}

QString EditorTabWidget::currentTabFileName() const
{
    EBTextEdit *textEdit = qobject_cast<EBTextEdit*>(widget(currentIndex()));
    Q_ASSERT(textEdit);
    Q_ASSERT(textEdit->documentTitle().size());
    return  textEdit->documentTitle();
}

EBTextEdit* EditorTabWidget::currentTextEdit()
{
  QWidget *widget = currentWidget();
  Q_ASSERT(widget);
  return qobject_cast<EBTextEdit*>(widget);
}

//todo - connect/disconnect on index change..?
void EditorTabWidget::undo()
{
    if (currentTextEdit())
        currentTextEdit()->undo();
}

void EditorTabWidget::redo()
{
    if (currentTextEdit())
        currentTextEdit()->redo();
}

void EditorTabWidget::cut()
{
    if (currentTextEdit())
        currentTextEdit()->cut();
}

void EditorTabWidget::copy()
{
    if (currentTextEdit())
        currentTextEdit()->copy();
}

void EditorTabWidget::paste()
{
    if (currentTextEdit())
        currentTextEdit()->paste();
}

void EditorTabWidget::clear()
{
    if (currentTextEdit())
        currentTextEdit()->insertPlainText("");
}

void EditorTabWidget::selectAll()
{
    if (currentTextEdit())
        currentTextEdit()->selectAll();
}

void EditorTabWidget::insetShortTime()
{
    if (currentTextEdit())
     currentTextEdit()->insertPlainText(DO->now(DataOwnerSingl::TIME_SIMPLE_FORMAT));
}

void EditorTabWidget::insetDate()
{
    if (currentTextEdit())
        currentTextEdit()->insertPlainText(DO->now(DataOwnerSingl::DATE_SIMPLE_FORMAT));
}

QPrinter* EditorTabWidget::printer()
{
  if(d->m_pPrinter== NULL)
    d->m_pPrinter = new QPrinter(QPrinter::HighResolution);
  return d->m_pPrinter;
}

void EditorTabWidget::printCurrentItem()
{
    EBTextEdit* pTextEdit = currentTextEdit();
    if(!pTextEdit)
        return;
    QPrintDialog dlg(printer(), this);
    if (dlg.exec() != QDialog::Accepted)
      return;

    pTextEdit->document()->print(printer());
}

EBTextEdit* EditorTabWidget::textEditByFilename(const QString & sFilename) const
{
    EBTextEdit* textEdit(NULL);
    for(int i=0;i< count();++i) {
        textEdit = qobject_cast<EBTextEdit*>(widget(i));
        Q_ASSERT(textEdit);
        Q_ASSERT(textEdit->documentTitle().size());
        if (textEdit->documentTitle() == sFilename) {
            return textEdit;
        }
    }
    //Q_ASSERT_X(0, "EditorTabWidget::textEdit","returns NULL - not found Textedit");
    return NULL;
}
bool EditorTabWidget::isFileOpen(const QString & sFilename) const
{
    if (textEditByFilename(sFilename))
        return true;
    return false;
}

void EditorTabWidget::copyFullFileName()
{
    QApplication::clipboard()->setText(currentTabFileName());
}

void EditorTabWidget::openFileFolder()
{
    QString sFileName =currentTabFileName();
    if (sFileName.size())   {
        QUrl url("file:///" + QFileInfo(sFileName).absolutePath());
        QDesktopServices::openUrl(url);
    }
}

void EditorTabWidget::mousePressEvent(QMouseEvent * ev)
{
  if (ev->button() == Qt::MidButton) {
    QPoint position = ev->pos();
    int c = tabBar()->count();
    int clickedItem(-1);
    int i(0);
    for (; i<c; ++i) {
      if (tabBar()->tabRect(i).contains(position)) {
        clickedItem = i;
        break;
      }
    }
    if (clickedItem != -1) {
      EBTextEdit* textEdit = qobject_cast<EBTextEdit*>(widget(i));
      Q_ASSERT(textEdit);
      Q_ASSERT(textEdit->documentTitle().size());
      closeTab(textEdit->documentTitle());
    }
  }
  else
    QTabWidget::mousePressEvent(ev);
}
void EditorTabWidget::contextMenuEvent( QContextMenuEvent * event )
{
    //first ativate right editor window
    QPoint position = event->pos();
    int c = tabBar()->count();
    int clickedItem(-1);
    int i(0);
    for (; i<c; ++i) {
    if (tabBar()->tabRect(i).contains(position)) {
      clickedItem = i;
      break;
    }
    }
    if (clickedItem != -1)
    setCurrentIndex(clickedItem);

    //execute menu
    rightClickMenu()->exec(event->globalPos());
}
QMenu* EditorTabWidget::rightClickMenu()     //context menu for tab switch
{
  if (!d->m_RightClickMenu)
  {
    d->m_RightClickMenu = new QMenu(this);
    QAction* pCopyFullFileNameAction = new QAction(QIcon(":/images/copy.svg"), tr("&Copy full file name"), this);
    connect(pCopyFullFileNameAction,SIGNAL(triggered()),this,SLOT(copyFullFileName()));
    d->m_RightClickMenu->addAction(pCopyFullFileNameAction);
    QAction* pOpenFileFolderAction = new QAction(QIcon(":/images/open.svg"), tr("&Open file folder"), this);
    connect(pOpenFileFolderAction,SIGNAL(triggered()),this,SLOT(openFileFolder()));
    d->m_RightClickMenu->addAction(pOpenFileFolderAction);
    d->m_RightClickMenu->addSeparator();
    d->m_RightClickMenu->addActions(changeViewActionGroup()->actions());
    d->m_RightClickMenu->addSeparator();

    QAction* pCloseAction = new QAction(QIcon(":/images/close.svg"), tr("&Close"), this);
    connect(pCloseAction,SIGNAL(triggered()),this,SLOT(closeTab()));
    d->m_RightClickMenu->addAction(pCloseAction);

    QAction* pCloseOtherTabsAction = new QAction(QIcon(":/images/close_other_tabs.svg"), tr("&Close other tabs"), this);
    connect(pCloseOtherTabsAction,SIGNAL(triggered()),this,SLOT(closeOtherTabs()));
    d->m_RightClickMenu->addAction(pCloseOtherTabsAction);
  }
  return d->m_RightClickMenu;
}

void EditorTabWidget::switchView(QAction* action)
{
    EditorView::Type viewType(EditorView::XmlSource);
    if (action == d->textViewAction)
        viewType = EditorView::Text;
    else if (action == d->sourceViewAction)
        viewType = EditorView::XmlSource;
    else if (action == d->dataViewAction)
        viewType = EditorView::OnlyPlainData;
    else
        Q_ASSERT(0);

    EBTextEdit* pTextEdit = currentTextEdit();
    if (pTextEdit) {
        if (!pTextEdit->changeView(viewType)) {
            pTextEdit->close();
            removeTab(indexOf(pTextEdit));
            delete pTextEdit;
        } else
            setTabIcon(currentIndex(),d->iconAccordingViewType(viewType));
    }
    emit currentViewChanged(viewType);
}

void EditorTabWidget::enableToolbars(int index)
{
    if (index == -1 )  //disable some features when there is not open any item
      changeViewActionGroup()->setDisabled(true);
    else
      changeViewActionGroup()->setEnabled(true);
}

void EditorTabWidget::updateViewActions(EditorView::Type type)
{
    switch(type) {
      case EditorView::XmlSource:
          d->sourceViewAction->setChecked(true);
          d->textViewAction->setChecked(false);
          d->dataViewAction->setChecked(false);
          break;
      case EditorView::Text:
          d->sourceViewAction->setChecked(false);
          d->textViewAction->setChecked(true);
          d->dataViewAction->setChecked(false);
          break;
      case EditorView::OnlyPlainData:
        d->sourceViewAction->setChecked(false);
        d->textViewAction->setChecked(false);
        d->dataViewAction->setChecked(true);
        break;
      case EditorView::RichFormat:
      default:
        Q_ASSERT(0);
      }

}
