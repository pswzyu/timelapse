#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include "utility.h"
#include <QPixmap>
#include <QDateTime>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCounter(0);

    // make sure the dir_name exists
    dir_name = "images"+QDir::separator();
    if (!QDir(dir_name).exists())
        QDir().mkdir(dir_name);

    // add selection item to the combo box
    ui->cb_Action->addItem("Take Pic");
    ui->cb_Action->addItem("Create Mask");

    cap.open(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

    timer = new QTimer(this);
    timer->setInterval(30);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDisplayImage()));
    timer->start();

}

int MainWindow::updateDisplayImage()
{

    bool bSuccess = cap.read(img_origin); // read a new frame from video
    if (!bSuccess)
        return -2;

    cv::Mat img_seg_blend; // for blending with segmentation map
    cv::Mat img_tran_blend; // for blending with

    if (img_mask.data = NULL) // no mask for this counter
    {

    }else{
        cv::addWeighted(img_origin, 0.5, img_seg, 0.5, 0.0, img_seg_blend);
        cv::addWeighted(img_origin, 0.5, img_mask, 0.5, 0.0, img_tran_blend);
    }

    // make a smaller image to display
    cv::resize(img_seg_blend, img_seg_blend,
               cv::Size(img_seg_blend.cols/4, img_seg_blend.rows/4));

    cv::transpose(img_seg_blend, img_seg_blend);
    cv::flip(img_seg_blend, img_seg_blend, 1);

    // convert the mat to a qimage and display it in the qgraphicview
    QImage qimg_seg = Utility::convertImage(img_seg_blend);
    ui->lbl_Segment->setPixmap(QPixmap::fromImage(qimg_seg));

    //-----------------

    // make a smaller image to display
    cv::resize(img_tran_blend, img_tran_blend,
               cv::Size(img_tran_blend.cols/4, img_tran_blend.rows/4));

    cv::transpose(img_tran_blend, img_tran_blend);
    cv::flip(img_tran_blend, img_tran_blend, 1);


    // convert the mat to a qimage and display it in the qgraphicview
    QImage qimg_tr = Utility::convertImage(img_tran_blend);
    ui->lbl_Transparent->setPixmap(QPixmap::fromImage(qimg_tr));

    return 0;
}

int MainWindow::resetCounterSlot()
{
    setCounter(0);
}

int MainWindow::takePictureSlot()
{
    // check the action in the combobox
    if (ui->cb_Action->currentIndex() == 0) // take pic
    {
        cv::imwrite(dir_name+"Image_"+QDateTime().toString()+"_"+
                    ui->lbl_counter->text()+".jpg", img_origin);
    }else{ // make mask
        cv::imwrite(dir_name+"Mask_"+
                    ui->lbl_counter->text()+".jpg", img_origin);
    }

    setCounter(getCounter()+1);
}

void MainWindow::setCounter(int cnt)
{
    // set counter to be that number
    counter = cnt;

    // try to load the mask of that counter, if failed, set the mask
    // to be an empty mat
    img_mask = cv::imread(dir_name+"Mask_"+QString::number(counter)+".jpg");
    // get the segmentation map

    cv::cvtColor( img_mask, img_seg, CV_BGR2GRAY );
    cv::blur( img_seg, img_seg, Size(3,3) );

    // Canny detector
    cv::Canny( img_seg, img_seg, 10, 300, 3 );
    cv::cvtColor( img_seg, img_seg, CV_GRAY2BGR );

    // set the label
    ui->lbl_counter->setText(QString::number(counter));
}

void MainWindow::getCounter()
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




