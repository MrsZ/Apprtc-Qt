#include "videowindow.h"

VideoWindow::VideoWindow(QWidget *parent) : QMainWindow(parent)
{

}

VideoWindow::VideoWindow(int width, int height, webrtc::VideoTrackInterface *track_to_render)
{
    resize(600, 400);
    ::InitializeCriticalSection(&buffer_lock_);
    ZeroMemory(&bmi_, sizeof(bmi_));
      bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmi_.bmiHeader.biPlanes = 1;
      bmi_.bmiHeader.biBitCount = 32;
      bmi_.bmiHeader.biCompression = BI_RGB;
      bmi_.bmiHeader.biWidth = width;
      bmi_.bmiHeader.biHeight = -height;
      bmi_.bmiHeader.biSizeImage = width * height *
                                  (bmi_.bmiHeader.biBitCount >> 3);
    rendered_track_ = track_to_render;
    rendered_track_->AddOrUpdateSink(this, rtc::VideoSinkWants());
}

VideoWindow::~VideoWindow()
{
    rendered_track_->RemoveSink(this);
    ::DeleteCriticalSection(&buffer_lock_);
}

void VideoWindow::paintEvent(QPaintEvent* event)
{

    const uint8_t* image = image_.get();
    if (image != NULL) {
        std::cout << "paint" << std::endl;
        QImage *local_video = new QImage(image, bmi_.bmiHeader.biWidth, abs(bmi_.bmiHeader.biHeight),
                                         QImage::Format_RGB32);
        QPainter painter(this);
        QRect rect = this->rect();
        painter.drawImage(rect, *local_video);
    }
}

void VideoWindow::closeEvent(QCloseEvent *event)
{

}

void VideoWindow::OnFrame(const cricket::VideoFrame &video_frame)
{
    {
    AutoLock<VideoWindow> lock(this);

    const cricket::VideoFrame* frame = video_frame.GetCopyWithRotationApplied();

    SetSize(frame->width(), frame->height());

    ASSERT(image_.get() != NULL);
    frame->ConvertToRgbBuffer(cricket::FOURCC_ARGB,
                              image_.get(),
                              bmi_.bmiHeader.biSizeImage,
                              bmi_.bmiHeader.biWidth *
                              bmi_.bmiHeader.biBitCount / 8
                              );
    }

    this->update();
}

void VideoWindow::SetSize(int width, int height)
{
    AutoLock<VideoWindow> lock(this);

    if (width == bmi_.bmiHeader.biWidth && height == bmi_.bmiHeader.biHeight) {
        return;
      }

    bmi_.bmiHeader.biWidth = width;
     bmi_.bmiHeader.biHeight = -height;
     bmi_.bmiHeader.biSizeImage = width * height *
                                  (bmi_.bmiHeader.biBitCount >> 3);
    image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
}
