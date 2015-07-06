#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

class QTimer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer* timer;
    cv::Mat img_origin;
    cv::Mat img_mask;
    cv::Mat img_seg;
    cv::VideoCapture cap;

    int counter; // the index of pic working now
    QString dir_name;

    void cleanUp();
    void setCounter(int cnt);
    int getCounter();


private:
    Ui::MainWindow *ui;


public slots:
    int updateDisplayImage();
    int resetCounterSlot();
    int takePictureSlot();

};

#endif // MAINWINDOW_H
