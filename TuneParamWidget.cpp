#include "TuneParamWidget.h"
#include "TuneParamTreeView.h"
#include <QPushButton>
#include <QBoxLayout>

TuneParamWidget::TuneParamWidget(QAbstractItemModel* sourceModel, QWidget* parent):
    QWidget(parent)
{
    auto tuneParamTreeView = new TuneParamTreeView(sourceModel, this);
    connect(this, &TuneParamWidget::currentChanged,
            tuneParamTreeView, &TuneParamTreeView::setRootIndex);

    auto pushButtonEditMode = new QPushButton(tr("Режим редактирования"), this);
    auto pushButtonApply = new QPushButton(tr("Применить"), this);
    auto pushButtonApplySave = new QPushButton(tr("Применить и сохранить"), this);
    connect(pushButtonEditMode, &QAbstractButton::toggled,
            pushButtonApply, &QAbstractButton::setEnabled);
    connect(pushButtonEditMode, &QAbstractButton::toggled,
            pushButtonApplySave, &QAbstractButton::setEnabled);
    connect(pushButtonEditMode, &QAbstractButton::toggled,
            tuneParamTreeView, [tuneParamTreeView] (bool checked)
    {
        tuneParamTreeView->setColumnHidden(2, !checked);
    });
    connect(pushButtonApply, &QAbstractButton::clicked,
            pushButtonEditMode, &QAbstractButton::setChecked);
    connect(pushButtonApplySave, &QAbstractButton::clicked,
            pushButtonEditMode, &QAbstractButton::setChecked);
    connect(pushButtonApply, &QAbstractButton::clicked,
            tuneParamTreeView, &TuneParamTreeView::sendChangedTP);
    pushButtonEditMode->setCheckable(true);
    pushButtonApply->setEnabled(false);
    pushButtonApplySave->setEnabled(false);

    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(pushButtonEditMode);
    buttonLayout->addWidget(pushButtonApply);
    buttonLayout->addWidget(pushButtonApplySave);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(tuneParamTreeView);
    mainLayout->addLayout(buttonLayout);
}
