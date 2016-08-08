/********************************************************************************
** Form generated from reading UI file 'maingui.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINGUI_H
#define UI_MAINGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainGui
{
public:
    QPushButton *pushButton;
    QListView *deviceList;

    void setupUi(QWidget *MainGui)
    {
        if (MainGui->objectName().isEmpty())
            MainGui->setObjectName(QStringLiteral("MainGui"));
        MainGui->resize(281, 251);
        pushButton = new QPushButton(MainGui);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 113, 32));
        deviceList = new QListView(MainGui);
        deviceList->setObjectName(QStringLiteral("deviceList"));
        deviceList->setGeometry(QRect(10, 50, 261, 192));

        retranslateUi(MainGui);

        QMetaObject::connectSlotsByName(MainGui);
    } // setupUi

    void retranslateUi(QWidget *MainGui)
    {
        MainGui->setWindowTitle(QApplication::translate("MainGui", "MainGui", 0));
        pushButton->setText(QApplication::translate("MainGui", "Search", 0));
    } // retranslateUi

};

namespace Ui {
    class MainGui: public Ui_MainGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINGUI_H
