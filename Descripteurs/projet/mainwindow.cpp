#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QResizeEvent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCentralWidget(label);
    resize(600, 600);
    generateAndDisplayImage();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::generateAndDisplayImage()
{
    cv::Mat image = cv::Mat(600, 600, CV_8UC3, cv::Scalar(255, 0, 0));
    std::string text = "Hello OpenCV!";
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 2.0;
    int thickness = 3;
    cv::Scalar textColor(255, 255, 255);
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness,
                                        &baseline);
    cv::Point textOrg((image.cols - textSize.width) / 2, (image.rows +
                                                          textSize.height) / 2);
    cv::putText(image, text, textOrg, fontFace, fontScale, textColor, thickness);
    QImage qimage = QImage(image.data, image.cols, image.rows, image.step,
                           QImage::Format_BGR888).copy();
    label->setPixmap(QPixmap::fromImage(qimage).scaled(label->size(),
                                                       Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    generateAndDisplayImage();
}
