/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gomoku_widget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_New_Game;
    QAction *action_Exit;
    QAction *action_About;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    GomokuWidget *gomokuWidget;
    QStatusBar *statusBar;
    QMenuBar *menuBar1;
    QMenu *menu_Game;
    QMenu *menu_Help;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        action_New_Game = new QAction(MainWindow);
        action_New_Game->setObjectName(QString::fromUtf8("action_New_Game"));
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName(QString::fromUtf8("action_Exit"));
        action_About = new QAction(MainWindow);
        action_About->setObjectName(QString::fromUtf8("action_About"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        menuBar = new QMenuBar(centralWidget);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));

        verticalLayout->addWidget(menuBar);

        gomokuWidget = new GomokuWidget(centralWidget);
        gomokuWidget->setObjectName(QString::fromUtf8("gomokuWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gomokuWidget->sizePolicy().hasHeightForWidth());
        gomokuWidget->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(gomokuWidget);

        statusBar = new QStatusBar(centralWidget);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));

        verticalLayout->addWidget(statusBar);

        MainWindow->setCentralWidget(centralWidget);
        menuBar1 = new QMenuBar(MainWindow);
        menuBar1->setObjectName(QString::fromUtf8("menuBar1"));
        menuBar1->setGeometry(QRect(0, 0, 800, 22));
        menu_Game = new QMenu(menuBar1);
        menu_Game->setObjectName(QString::fromUtf8("menu_Game"));
        menu_Help = new QMenu(menuBar1);
        menu_Help->setObjectName(QString::fromUtf8("menu_Help"));
        MainWindow->setMenuBar(menuBar1);

        menuBar1->addAction(menu_Game->menuAction());
        menuBar1->addAction(menu_Help->menuAction());
        menu_Game->addAction(action_New_Game);
        menu_Game->addAction(action_Exit);
        menu_Help->addAction(action_About);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gomoku Game", nullptr));
        action_New_Game->setText(QCoreApplication::translate("MainWindow", "New Game", nullptr));
        action_Exit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        action_About->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        menu_Game->setTitle(QCoreApplication::translate("MainWindow", "Game", nullptr));
        menu_Help->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
