/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionasdf;
    QAction *actionasdf_2;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actiondfgh;
    QAction *actiondfgh_2;
    QAction *actiondfgh_3;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QMenu *menudsg;
    QMenu *menuasdf;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(789, 603);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setAutoFillBackground(false);
        actionasdf = new QAction(MainWindow);
        actionasdf->setObjectName("actionasdf");
        actionasdf_2 = new QAction(MainWindow);
        actionasdf_2->setObjectName("actionasdf_2");
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName("actionNew");
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actiondfgh = new QAction(MainWindow);
        actiondfgh->setObjectName("actiondfgh");
        actiondfgh_2 = new QAction(MainWindow);
        actiondfgh_2->setObjectName("actiondfgh_2");
        actiondfgh_3 = new QAction(MainWindow);
        actiondfgh_3->setObjectName("actiondfgh_3");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(-1, -1, -1, 0);
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 1)
            tableWidget->setColumnCount(1);
        if (tableWidget->rowCount() < 2)
            tableWidget->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setItem(1, 0, __qtablewidgetitem1);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy1);
        tableWidget->setMinimumSize(QSize(0, 0));
        QFont font;
        font.setFamilies({QString::fromUtf8("Cascadia Mono")});
        font.setBold(false);
        tableWidget->setFont(font);
        tableWidget->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CursorShape::IBeamCursor)));
        tableWidget->setMouseTracking(true);
        tableWidget->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        tableWidget->setEditTriggers(QAbstractItemView::EditTrigger::AllEditTriggers);
        tableWidget->setRowCount(2);
        tableWidget->setColumnCount(1);

        verticalLayout_3->addWidget(tableWidget);


        verticalLayout->addLayout(verticalLayout_3);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 789, 21));
        menudsg = new QMenu(menubar);
        menudsg->setObjectName("menudsg");
        menuasdf = new QMenu(menubar);
        menuasdf->setObjectName("menuasdf");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menudsg->menuAction());
        menubar->addAction(menuasdf->menuAction());
        menudsg->addAction(actionNew);
        menudsg->addAction(actionOpen);
        menudsg->addAction(actionSave);
        menuasdf->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "AlPo", nullptr));
        actionasdf->setText(QCoreApplication::translate("MainWindow", "asdf", nullptr));
        actionasdf_2->setText(QCoreApplication::translate("MainWindow", "asdf", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "New", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actiondfgh->setText(QCoreApplication::translate("MainWindow", "dfgh", nullptr));
        actiondfgh_2->setText(QCoreApplication::translate("MainWindow", "dfgh", nullptr));
        actiondfgh_3->setText(QCoreApplication::translate("MainWindow", "dfgh", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "File", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Run program", nullptr));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem = tableWidget->item(0, 0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "sdfg", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->item(1, 0);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "hdfg", nullptr));
        tableWidget->setSortingEnabled(__sortingEnabled);

        menudsg->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuasdf->setTitle(QCoreApplication::translate("MainWindow", "Run", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
