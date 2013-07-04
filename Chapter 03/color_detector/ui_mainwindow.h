/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Sep 21 18:32:00 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_Image;
    QAction *actionChoose_Color;
    QAction *actionProcess;
    QAction *actionQuit;
    QWidget *centralWidget;
    QPushButton *pushButton_openImage;
    QPushButton *pushButton_color;
    QPushButton *pushButton_process;
    QLabel *label;
    QSlider *verticalSlider_Threshold;
    QLabel *label_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 394);
        actionOpen_Image = new QAction(MainWindow);
        actionOpen_Image->setObjectName(QString::fromUtf8("actionOpen_Image"));
        actionChoose_Color = new QAction(MainWindow);
        actionChoose_Color->setObjectName(QString::fromUtf8("actionChoose_Color"));
        actionProcess = new QAction(MainWindow);
        actionProcess->setObjectName(QString::fromUtf8("actionProcess"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton_openImage = new QPushButton(centralWidget);
        pushButton_openImage->setObjectName(QString::fromUtf8("pushButton_openImage"));
        pushButton_openImage->setGeometry(QRect(10, 10, 81, 23));
        pushButton_color = new QPushButton(centralWidget);
        pushButton_color->setObjectName(QString::fromUtf8("pushButton_color"));
        pushButton_color->setGeometry(QRect(10, 40, 81, 23));
        pushButton_process = new QPushButton(centralWidget);
        pushButton_process->setObjectName(QString::fromUtf8("pushButton_process"));
        pushButton_process->setGeometry(QRect(10, 320, 81, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(100, 10, 491, 331));
        label->setFrameShape(QFrame::Box);
        verticalSlider_Threshold = new QSlider(centralWidget);
        verticalSlider_Threshold->setObjectName(QString::fromUtf8("verticalSlider_Threshold"));
        verticalSlider_Threshold->setGeometry(QRect(40, 80, 20, 151));
        verticalSlider_Threshold->setMaximum(442);
        verticalSlider_Threshold->setOrientation(Qt::Vertical);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 240, 81, 71));
        label_2->setWordWrap(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_Image);
        menuFile->addAction(actionChoose_Color);
        menuFile->addAction(actionProcess);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Color Detector (CIE Lab)", 0, QApplication::UnicodeUTF8));
        actionOpen_Image->setText(QApplication::translate("MainWindow", "Open Image", 0, QApplication::UnicodeUTF8));
        actionChoose_Color->setText(QApplication::translate("MainWindow", "Choose Color", 0, QApplication::UnicodeUTF8));
        actionProcess->setText(QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        pushButton_openImage->setText(QApplication::translate("MainWindow", "Open Image", 0, QApplication::UnicodeUTF8));
        pushButton_color->setText(QApplication::translate("MainWindow", "Select Color", 0, QApplication::UnicodeUTF8));
        pushButton_process->setText(QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Image", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Color Distance Threshold: 0", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
