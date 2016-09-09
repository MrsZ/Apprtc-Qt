#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QPainter>
#include <QImage>

#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/base/win32.h"
#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/media/base/mediachannel.h"
#include "webrtc/media/base/videocommon.h"
#include "webrtc/media/base/videoframe.h"

class VideoWindow : public QMainWindow,
                    public rtc::VideoSinkInterface<cricket::VideoFrame>
{
    Q_OBJECT
public:
    explicit VideoWindow(QWidget *parent = 0);
    VideoWindow(int width, int height, webrtc::VideoTrackInterface* track_to_render);
    virtual ~VideoWindow();

protected:
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

public:
    void Lock() {
          ::EnterCriticalSection(&buffer_lock_);
        }

        void Unlock() {
          ::LeaveCriticalSection(&buffer_lock_);
        }
    void SetSize(int width, int height);
    void OnFrame(const cricket::VideoFrame &video_frame) override;

    const uint8_t* image() const { return image_.get(); }
    const BITMAPINFO& bmi() const { return bmi_; }

    // A little helper class to make sure we always to proper locking and
      // unlocking when working with VideoRenderer buffers.
    template <typename T>
    class AutoLock {
    public:
        explicit AutoLock(T* obj) : obj_(obj) { obj_->Lock(); }
        ~AutoLock() { obj_->Unlock(); }
    protected:
        T* obj_;
    };

private:
    std::unique_ptr<uint8_t[]> image_;
    BITMAPINFO bmi_;
    CRITICAL_SECTION buffer_lock_;
    rtc::scoped_refptr<webrtc::VideoTrackInterface> rendered_track_;
signals:

public slots:
};

#endif // VIDEOWINDOW_H
