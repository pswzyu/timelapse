#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include "utility.h"
#include <QPixmap>
#include <QDateTime>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // make sure the dir_name exists
    dir_name = QString("workout_images").append(QDir::separator());
    if (!QDir(dir_name).exists())
        QDir().mkdir(dir_name);

    setCounter(0);

    // add selection item to the combo box
    ui->cb_Action->addItem("Take Pic");
    ui->cb_Action->addItem("Create Mask");

    cap.open(1);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

    timer = new QTimer(this);
    timer->setInterval(30);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDisplayImage()));
    timer->start();

    connect(ui->btn_ResetCount, SIGNAL(released()), this, SLOT(resetCounterSlot()));
    connect(ui->btn_Shot, SIGNAL(released()), this, SLOT(takePictureSlot()));

}

int MainWindow::updateDisplayImage()
{

    bool bSuccess = cap.read(img_origin); // read a new frame from video
    if (!bSuccess)
        return -2;

    cv::Mat img_seg_blend; // for blending with segmentation map
    cv::Mat img_tran_blend; // for blending with

    if (img_mask.data == NULL) // no mask for this counter
    {
        img_seg_blend = img_origin;
        img_tran_blend = img_origin;
    }else{
        cv::addWeighted(img_origin, 0.5, img_seg, 0.5, 0.0, img_seg_blend);
        cv::addWeighted(img_origin, 0.5, img_mask, 0.5, 0.0, img_tran_blend);
    }

    // make a smaller image to display
    cv::resize(img_seg_blend, img_seg_blend,
               cv::Size(img_seg_blend.cols/2, img_seg_blend.rows/2));

    cv::transpose(img_seg_blend, img_seg_blend);
    //cv::flip(img_seg_blend, img_seg_blend, 1);

    // convert the mat to a qimage and display it in the qgraphicview
    QImage qimg_seg = Utility::convertImage(img_seg_blend);
    ui->lbl_Segment->setPixmap(QPixmap::fromImage(qimg_seg));

    //-----------------

    // make a smaller image to display
    cv::resize(img_tran_blend, img_tran_blend,
               cv::Size(img_tran_blend.cols/2, img_tran_blend.rows/2));

    cv::transpose(img_tran_blend, img_tran_blend);
    //cv::flip(img_tran_blend, img_tran_blend, 1);


    // convert the mat to a qimage and display it in the qgraphicview
    QImage qimg_tr = Utility::convertImage(img_tran_blend);
    ui->lbl_Transparent->setPixmap(QPixmap::fromImage(qimg_tr));

    return 0;
}

int MainWindow::resetCounterSlot()
{
    setCounter(0);
    return 0;
}

int MainWindow::takePictureSlot()
{
    qDebug() << QDateTime::currentDateTime().toString();
    // check the action in the combobox
    if (ui->cb_Action->currentIndex() == 0) // take pic
    {
        cv::imwrite((dir_name+"Image_"+QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+"_"+
                    ui->lbl_counter->text()+".jpg").toStdString(), img_origin);
    }else{ // make mask
        cv::imwrite((dir_name+"Mask_"+
                    ui->lbl_counter->text()+".jpg").toStdString(), img_origin);
    }

    setCounter(getCounter()+1);

    return 0;
}

void MainWindow::setCounter(int cnt)
{
    // set counter to be that number
    counter = cnt;

    // try to load the mask of that counter, if failed, set the mask
    // to be an empty mat
    img_mask = cv::imread((dir_name+"Mask_"+QString::number(counter)+".jpg").toStdString());
    // get the segmentation map

    if (img_mask.data != NULL)
    {

        cv::cvtColor( img_mask, img_seg, CV_BGR2GRAY );
        cv::blur( img_seg, img_seg, cv::Size(3,3) );

        // Canny detector
        cv::Canny( img_seg, img_seg, 10, 30, 3 );
        cv::cvtColor( img_seg, img_seg, CV_GRAY2BGR );
    }

    // set the label
    ui->lbl_counter->setText(QString::number(counter));
}

int MainWindow::getCounter()
{
    return counter;
}

void MainWindow::cleanUp()
{
    delete timer;
    cap.release();
}

MainWindow::~MainWindow()
{
    delete ui;
}




