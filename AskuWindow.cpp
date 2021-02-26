#include "AskuWindow.h"
#include "SchemeTreeModel.h"
#include "ElemTreeView.h"
#include "CtrlParamTableView.h"
#include "TuneParamWidget.h"
#include "ElemListView.h"
#include "PathComboBox.h"
#include <QSplitter>
#include <QLayout>
#include <QJsonValue>
#include <QDebug>

AskuWindow::AskuWindow(QWidget* parent):
    QWidget(parent)
{
    auto sourceModel = new SchemeTreeModel(this);
    connect(this, &AskuWindow::schemesReceived, sourceModel, &SchemeTreeModel::buildSchemes);
    //connect(this, &AskuWindow::elemDatasReceived, sourceModel, &SchemeTreeModel::updateElems);

    auto elemTreeView = new ElemTreeView(sourceModel, this);

    auto ctrlParamTableView = new CtrlParamTableView(sourceModel, this);
    connect(elemTreeView, QOverload<const QModelIndex&>::of(&ElemTreeView::currentChanged),
            ctrlParamTableView, &CtrlParamTableView::setRootIndex);

    auto tuneParamWidget = new TuneParamWidget(sourceModel, this);
    connect(elemTreeView, QOverload<const QModelIndex&>::of(&ElemTreeView::currentChanged),
            tuneParamWidget, &TuneParamWidget::currentChanged);

    auto elemListView = new ElemListView(sourceModel, this);
    connect(elemTreeView, QOverload<const QModelIndex&>::of(&ElemTreeView::currentChanged),
            elemListView, &ElemListView::setRootIndex);

    auto pathComboBox = new PathComboBox(elemTreeView->model(), this);
    connect(elemTreeView, QOverload<const QModelIndex&>::of(&ElemTreeView::currentChanged),
            pathComboBox, &PathComboBox::updateItem);
    connect(pathComboBox, &PathComboBox::activated, elemTreeView, &ElemTreeView::setCurrentIndex);
    //connect(pathComboBox, &PathComboBox::activated, elemTreeView, [elemTreeView]
    //{
    //    qDebug() << __PRETTY_FUNCTION__;
    //    elemTreeView->setFocus();
    //});

    auto viewSplitter = new QSplitter(Qt::Horizontal, this);
    viewSplitter->setChildrenCollapsible(false);
    viewSplitter->addWidget(elemTreeView);
    viewSplitter->addWidget(ctrlParamTableView);
    viewSplitter->addWidget(tuneParamWidget);
    viewSplitter->addWidget(elemListView);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(pathComboBox);
    mainLayout->addWidget(viewSplitter);
}
