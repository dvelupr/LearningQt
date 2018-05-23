/********************************************************************************
** Form generated from reading UI file 'LayersWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYERSWIDGET_H
#define UI_LAYERSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QListView *listViewLayers;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonRemoveAll;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(260, 447);
        widget = new QWidget(Form);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 0, 258, 441));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        listViewLayers = new QListView(widget);
        listViewLayers->setObjectName(QStringLiteral("listViewLayers"));

        verticalLayout->addWidget(listViewLayers);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButtonAdd = new QPushButton(widget);
        pushButtonAdd->setObjectName(QStringLiteral("pushButtonAdd"));

        horizontalLayout->addWidget(pushButtonAdd);

        pushButtonRemoveAll = new QPushButton(widget);
        pushButtonRemoveAll->setObjectName(QStringLiteral("pushButtonRemoveAll"));

        horizontalLayout->addWidget(pushButtonRemoveAll);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", Q_NULLPTR));
        pushButtonAdd->setText(QApplication::translate("Form", "Add", Q_NULLPTR));
        pushButtonRemoveAll->setText(QApplication::translate("Form", "Remove All", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERSWIDGET_H
