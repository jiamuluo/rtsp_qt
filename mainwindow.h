#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videortspclient.h"
#include <QImage>
#include <QTimer>
#include <QVideoWidget>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showFrame(QImage *);
    void getFrameSize(QSize size);
    void getTimerOut(void);

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void on_pushButtonStart_toggled(bool checked);
private:
    bool startVideo(void);
    bool stopVideo(void);
private:
    VideoRtspClient *videoRtsp;
    QList<QImage *> frames;
    QTimer frameTimer;
    int count;
    int frameCounts;
    QVideoWidget *videoWidget;
    QMediaPlayer *videoPlayer;
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
