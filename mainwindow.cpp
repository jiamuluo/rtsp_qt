#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frameCounts = 0;
    ui->widgetShow->installEventFilter(this);

    videoRtsp = new VideoRtspClient("rtsp://192.168.2.53/0");
    if(videoRtsp->isValid())
    {
        connect(videoRtsp,&VideoRtspClient::VideoGetImage,this,&MainWindow::showFrame);
        connect(videoRtsp,&VideoRtspClient::VideoGetSize,this,&MainWindow::getFrameSize);
        videoRtsp->start();
    }
    connect(&frameTimer,&QTimer::timeout,this,&MainWindow::getTimerOut);
    frameTimer.start(1000);
    //videoPlayer = new QMediaPlayer(this);
//    videoWidget = new QVideoWidget(this);
//    this->setCentralWidget(videoWidget);
//    videoPlayer->setVideoOutput(videoWidget);
//    videoPlayer->setMedia(QUrl("rtsp://192.168.2.53/0"));
//    videoPlayer->play();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(videoRtsp)
        delete  videoRtsp;
}

void MainWindow::showFrame(QImage *frame) //激活一个重绘事件?
{
    if(frames.size() > 20)
    {
        QImage  *first = frames.takeAt(0);
        delete first;
    }

    frameCounts++;
    frames.append(frame);
    ui->widgetShow->repaint();
}

void MainWindow::getFrameSize(QSize size)
{
//    QSize mainSize(size.width() + 20,size.height() + 20);
//    this->resize(mainSize);
//    ui->widgetShow->resize(size);
}

void MainWindow::getTimerOut()
{
    ui->labelFps->setText(QString("FPS:%1").arg(frameCounts));
    frameCounts = 0;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
#if 1
    if(watched == ui->widgetShow)
    {

        if(event->type() != QEvent::Paint)
            return false;

         QImage  *first = frames.takeAt(0);
         QWidget *video          = static_cast< QWidget*>(watched);

         first->scaled(video->size());
         QPainter painter(video);
         painter.drawImage(video->rect(),*first);
         if(first->save(QString("G:/tmp/output/%1.jpg").arg(count)) == false)
         {
             qDebug() << "Now save failed";
         }
         count++;
         delete first;

         return true;
    }
    else
    {
        return QWidget::eventFilter(watched,event);
    }
#endif
}

void MainWindow::on_pushButtonStart_released()
{
    qDebug() << ui->urlAddr->text();

}

void MainWindow::paintEvent(QPaintEvent *event)
{

}
