#ifndef VIDEORTSPCLIENT_H
#define VIDEORTSPCLIENT_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>

class MeothdImage
{
public:
    MeothdImage(cv::Mat &in,cv::Mat &out);
    ~MeothdImage(){}

};
class VideoRtspClient : public QThread
{
    Q_OBJECT

public:
    VideoRtspClient(const QString &url);
    ~VideoRtspClient();
public:
    bool isValid(void){return valid;}
    static QImage *changeMat2Image(cv::Mat &);
signals:
    void VideoGetImage(QImage *getImage);
    void VideoGetSize(QSize size);
protected:
    virtual void run();
private:
    bool valid;
    cv::VideoCapture videoCap;
    int frameNum;
};

#endif // VIDEORTSPCLIENT_H
