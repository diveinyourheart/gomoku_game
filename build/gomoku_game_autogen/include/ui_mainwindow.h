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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
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
    QAction *action_Back_to_Menu;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QStackedWidget *stackedWidget;
    QWidget *modeSelectionPage;
    QVBoxLayout *modeLayout;
    QSpacerItem *verticalSpacer_t;
    QLabel *label_title;
    QPushButton *btn_normalMode;
    QPushButton *btn_aiMode;
    QPushButton *btn_onlineMode;
    QSpacerItem *verticalSpacer_b;
    QWidget *gamePage;
    QVBoxLayout *gameLayout;
    GomokuWidget *gomokuWidget;
    QWidget *buttonWidget;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_Undo;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menuBar;
    QMenu *menu_Game;
    QMenu *menu_Help;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 650);
        action_New_Game = new QAction(MainWindow);
        action_New_Game->setObjectName(QString::fromUtf8("action_New_Game"));
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName(QString::fromUtf8("action_Exit"));
        action_About = new QAction(MainWindow);
        action_About->setObjectName(QString::fromUtf8("action_About"));
        action_Back_to_Menu = new QAction(MainWindow);
        action_Back_to_Menu->setObjectName(QString::fromUtf8("action_Back_to_Menu"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        modeSelectionPage = new QWidget();
        modeSelectionPage->setObjectName(QString::fromUtf8("modeSelectionPage"));
        modeLayout = new QVBoxLayout(modeSelectionPage);
        modeLayout->setSpacing(20);
        modeLayout->setObjectName(QString::fromUtf8("modeLayout"));
        modeLayout->setContentsMargins(100, -1, 100, -1);
        verticalSpacer_t = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        modeLayout->addItem(verticalSpacer_t);

        label_title = new QLabel(modeSelectionPage);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        QFont font;
        font.setPointSize(24);
        font.setBold(true);
        label_title->setFont(font);
        label_title->setAlignment(Qt::AlignCenter);

        modeLayout->addWidget(label_title);

        btn_normalMode = new QPushButton(modeSelectionPage);
        btn_normalMode->setObjectName(QString::fromUtf8("btn_normalMode"));
        btn_normalMode->setMinimumSize(QSize(0, 50));

        modeLayout->addWidget(btn_normalMode);

        btn_aiMode = new QPushButton(modeSelectionPage);
        btn_aiMode->setObjectName(QString::fromUtf8("btn_aiMode"));
        btn_aiMode->setMinimumSize(QSize(0, 50));

        modeLayout->addWidget(btn_aiMode);

        btn_onlineMode = new QPushButton(modeSelectionPage);
        btn_onlineMode->setObjectName(QString::fromUtf8("btn_onlineMode"));
        btn_onlineMode->setEnabled(false);
        btn_onlineMode->setMinimumSize(QSize(0, 50));

        modeLayout->addWidget(btn_onlineMode);

        verticalSpacer_b = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        modeLayout->addItem(verticalSpacer_b);

        stackedWidget->addWidget(modeSelectionPage);
        gamePage = new QWidget();
        gamePage->setObjectName(QString::fromUtf8("gamePage"));
        gameLayout = new QVBoxLayout(gamePage);
        gameLayout->setObjectName(QString::fromUtf8("gameLayout"));
        gomokuWidget = new GomokuWidget(gamePage);
        gomokuWidget->setObjectName(QString::fromUtf8("gomokuWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gomokuWidget->sizePolicy().hasHeightForWidth());
        gomokuWidget->setSizePolicy(sizePolicy);

        gameLayout->addWidget(gomokuWidget);

        buttonWidget = new QWidget(gamePage);
        buttonWidget->setObjectName(QString::fromUtf8("buttonWidget"));
        buttonLayout = new QHBoxLayout(buttonWidget);
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        btn_Undo = new QPushButton(buttonWidget);
        btn_Undo->setObjectName(QString::fromUtf8("btn_Undo"));

        buttonLayout->addWidget(btn_Undo);

        horizontalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(horizontalSpacer_2);


        gameLayout->addWidget(buttonWidget);

        stackedWidget->addWidget(gamePage);

        mainLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 22));
        menu_Game = new QMenu(menuBar);
        menu_Game->setObjectName(QString::fromUtf8("menu_Game"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QString::fromUtf8("menu_Help"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_Game->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menu_Game->addAction(action_Back_to_Menu);
        menu_Game->addAction(action_New_Game);
        menu_Game->addSeparator();
        menu_Game->addAction(action_Exit);
        menu_Help->addAction(action_About);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gomoku Master", nullptr));
        action_New_Game->setText(QCoreApplication::translate("MainWindow", "New Game", nullptr));
        action_Exit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        action_About->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        action_Back_to_Menu->setText(QCoreApplication::translate("MainWindow", "Back to Menu", nullptr));
        label_title->setText(QCoreApplication::translate("MainWindow", "GOMOKU GAME", nullptr));
        btn_normalMode->setText(QCoreApplication::translate("MainWindow", "\346\231\256\351\200\232\346\250\241\345\274\217 (Local PvP)", nullptr));
        btn_aiMode->setText(QCoreApplication::translate("MainWindow", "AI \345\257\271\346\210\230\346\250\241\345\274\217 (PvE)", nullptr));
        btn_onlineMode->setText(QCoreApplication::translate("MainWindow", "\350\201\224\347\275\221\345\257\271\346\210\230 (\345\215\263\345\260\206\346\216\250\345\207\272...)", nullptr));
        btn_Undo->setText(QCoreApplication::translate("MainWindow", "\346\202\224\346\243\213", nullptr));
        menu_Game->setTitle(QCoreApplication::translate("MainWindow", "Game", nullptr));
        menu_Help->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
