/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 1 of the cookbook:  
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
     tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));

    image= cv::imread(fileName.toAscii().data());

    if (image.data) {
        cv::namedWindow("Original Image");
        cv::imshow("Original Image", image);
        ui->pushButton_2->setEnabled(true);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    cv::flip(image,image,1); // process the image

    cv::cvtColor(image,image,CV_BGR2RGB);  // change color channel ordering
    QImage img= QImage((const unsigned char*)(image.data),  // Qt image structure
                       image.cols,image.rows,QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));  // display on label
    ui->label->resize(ui->label->pixmap()->size()); // resize the label to fit the image
}
