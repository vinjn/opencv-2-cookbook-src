/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 3 of the cookbook:  
   Computer Vision Programming using the OpenCV Library. 
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //select color
    connect(ui->pushButton_color, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->actionChoose_Color, SIGNAL(triggered()), this, SLOT(setColor()));

    //open image
    connect(ui->pushButton_openImage, SIGNAL(clicked()), this, SLOT(setImage()));
    connect(ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(setImage()));

    //process Color Detection
    connect(ui->pushButton_process, SIGNAL(clicked()), this, SLOT(processColorDetection()));
    connect(ui->actionProcess, SIGNAL(triggered()), this, SLOT(processColorDetection()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::setImage()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open Image Files"),
                                "",
                                tr("Image files (*.jpg *.jpeg *.png *.gif *.bmp)"),
                                &selectedFilter,
                                options);
    if (!fileName.isEmpty()){
        cv::Mat img_mat = cv::imread(fileName.toStdString(),1); //0 for grayscale
        displayMat(img_mat);
    }
    //Set Filename
    ColorDetectController::getInstance()->setInputImage(fileName.toStdString());
}

//Convert cv::Mat to QImage and display
void MainWindow::displayMat(const cv::Mat& image){

    //BGR openCV Mat to QImage
    QImage img_qt = QImage((const unsigned char*)image.data,image.cols, image.rows, image.step, QImage::Format_RGB888);

    //For Binary Images
    if (img_qt.isNull()){
        //ColorTable for Binary Images
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++)
            colorTable.push_back(qRgb(i, i, i));

        img_qt = QImage((const unsigned char*)image.data,image.cols, image.rows, QImage::Format_Indexed8);
        img_qt.setColorTable(colorTable);
        }

    //Display the QImage in the Label
    QPixmap img_pix = QPixmap::fromImage(img_qt.rgbSwapped()); //BGR to RGB
    this->ui->label->setPixmap(img_pix.scaled(ui->label->size(), Qt::KeepAspectRatio));
}

void MainWindow::on_verticalSlider_Threshold_valueChanged(int value)
{
    QString cdt("Color Distance Threshold: ");
    cdt.append(QString::number(value));
    this->ui->label_2->setText(cdt);
}

void MainWindow::setColor()
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
       ColorDetectController::getInstance()->setTargetColor(color.red(),color.green(),color.blue());
    }
}

void MainWindow::processColorDetection()
{
    ColorDetectController::getInstance()->setColorDistanceThreshold(ui->verticalSlider_Threshold->value());
    ColorDetectController::getInstance()->process();

    cv::Mat resulting = ColorDetectController::getInstance()->getLastResult();
    if (!resulting.empty())
        displayMat(resulting);

}
