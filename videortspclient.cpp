#include "videortspclient.h"

#include <qDebug>
VideoRtspClient::VideoRtspClient(const QString &url)
    :valid(false),frameNum(1)
{
    if(url.isEmpty())
        return;
    if(videoCap.open(url.toStdString()) != true)
         return;

    valid = true;
}

VideoRtspClient::~VideoRtspClient()
{
    videoCap.release();
}

QImage* VideoRtspClient::changeMat2Image(cv::Mat &frame)
{
    cv::cvtColor(frame,frame,cv::COLOR_BGRA2RGB);
    QImage * getImage = new QImage(frame.data,frame.cols,frame.rows,QImage::Format_RGB888);
    if(getImage->isNull())
    {
        delete getImage;
        getImage = nullptr;
    }

    return getImage;
}

void VideoRtspClient::run()
{
    cv::Mat frame;
    while(1)
    {
        videoCap.read(frame);
        if(frame.empty())
             continue;
        if(frameNum == 1)
        {
            emit VideoGetSize(QSize(frame.cols,frame.rows));
        }
        frameNum++;

        if(frameNum % 2)
            continue;

        QImage* getImage = changeMat2Image(frame);
        if(getImage)
            emit VideoGetImage(getImage);


    }
}

