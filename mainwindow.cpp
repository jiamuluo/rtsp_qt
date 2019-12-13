#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frameCounts = 0;
    ui->widgetShow->installEventFilter(this);

    connect(&frameTimer,&QTimer::timeout,this,&MainWindow::getTimerOut);
    frameTimer.start(1000);
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
         if(frames.size() == 0)
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

void MainWindow::paintEvent(QPaintEvent *event)
{

}

void MainWindow::on_pushButtonStart_toggled(bool checked)
{
    if(checked)
    {
        if(ui->urlAddr->text().isEmpty())
        {
            QMessageBox::warning(this,"警告","URL为空");
            ui->pushButtonStart->setChecked(false);
            return;
        }
        if(startVideo() == false)
        {
            QMessageBox::warning(this,"警告","打开失败");
            ui->pushButtonStart->setChecked(false);
            return;
        }
        ui->pushButtonStart->setText(tr("停止"));
        ui->urlAddr->setEnabled(false);
    }
    else
    {
        stopVideo();
        ui->pushButtonStart->setText(tr("开始"));
        ui->urlAddr->setEnabled(true);
        ui->urlAddr->clear();
    }
}

bool MainWindow::startVideo()
{

    videoRtsp = new VideoRtspClient(ui->urlAddr->text());
    if(!videoRtsp->isValid())
    {
        delete videoRtsp;
        videoRtsp = nullptr;
        return false;
    }

    connect(videoRtsp,&VideoRtspClient::VideoGetImage,this,&MainWindow::showFrame);
    connect(videoRtsp,&VideoRtspClient::VideoGetSize,this,&MainWindow::getFrameSize);
    videoRtsp->start();
    return true;
}

bool MainWindow::stopVideo()
{
    if(!videoRtsp)
        return true;
    disconnect(videoRtsp,&VideoRtspClient::VideoGetImage,this,&MainWindow::showFrame);
    disconnect(videoRtsp,&VideoRtspClient::VideoGetSize,this,&MainWindow::getFrameSize);
    connect(videoRtsp,&VideoRtspClient::finished,videoRtsp,&VideoRtspClient::deleteLater);
    videoRtsp->quit();

    return true;
}
