#-------------------------------------------------
#
# Project created by QtCreator 2016-09-02T09:17:58
#
#-------------------------------------------------

QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppRTC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpchannelclient.cpp \
    directrtcclient.cpp \
    apprtcclient.cpp \
    peerconnectionclient.cpp \
    videowindow.cpp

HEADERS  += mainwindow.h \
    tcpchannelclient.h \
    directrtcclient.h \
    apprtcclient.h \
    peerconnectionclient.h \
    videowindow.h

FORMS    += mainwindow.ui


CONFIG += warn_off \ static \ c++11

DEFINES += _AMD64_
DEFINES += _DEBUG
DEFINES += V8_DEPRECATION_WARNINGS
DEFINES += _WIN32_WINNT=0x0A00
DEFINES += WINVER=0x0A00
DEFINES += WIN32
DEFINES += _WINDOWS
DEFINES += NOMINMAX
DEFINES += PSAPI_VERSION=1
DEFINES += _CRT_RAND_S
DEFINES += CERT_CHAIN_PARA_HAS_EXTRA_FIELDS
DEFINES += WIN32_LEAN_AND_MEAN
DEFINES += _ATL_NO_OPENGL
DEFINES += _SECURE_ATL
DEFINES += _HAS_EXCEPTIONS=0
DEFINES += _WINSOCK_DEPRECATED_NO_WARNINGS
DEFINES += CHROMIUM_BUILD
DEFINES += CR_CLANG_REVISION=270823-1
DEFINES += USE_AURA=1
DEFINES += USE_DEFAULT_RENDER_THEME=1
DEFINES += USE_LIBJPEG_TURBO=1
DEFINES += ENABLE_WEBRTC=1
DEFINES += ENABLE_MEDIA_ROUTER=1
DEFINES += ENABLE_PEPPER_CDMS
DEFINES += ENABLE_NOTIFICATIONS
DEFINES += FIELDTRIAL_TESTING_ENABLED
DEFINES += NO_TCMALLOC
DEFINES += __STD_C
DEFINES += _CRT_SECURE_NO_DEPRECATE
DEFINES += _SCL_SECURE_NO_DEPRECATE
DEFINES += NTDDI_VERSION=0x0A000000
DEFINES += _USING_V110_SDK71_
DEFINES += ENABLE_TASK_MANAGER=1
DEFINES += ENABLE_EXTENSIONS=1
DEFINES += ENABLE_PDF=1
DEFINES += ENABLE_PLUGIN_INSTALLATION=1
DEFINES += ENABLE_PLUGINS=1
DEFINES += ENABLE_SESSION_SERVICE=1
DEFINES += ENABLE_THEMES=1
DEFINES += ENABLE_PRINTING=1
DEFINES += ENABLE_BASIC_PRINTING=1
DEFINES += ENABLE_PRINT_PREVIEW=1
DEFINES += ENABLE_SPELLCHECK=1
DEFINES += ENABLE_CAPTIVE_PORTAL_DETECTION=1
DEFINES += ENABLE_SUPERVISED_USERS=1
DEFINES += ENABLE_MDNS=1
DEFINES += ENABLE_SERVICE_DISCOVERY=1
DEFINES += V8_USE_EXTERNAL_STARTUP_DATA
DEFINES += FULL_SAFE_BROWSING
DEFINES += SAFE_BROWSING_CSD
DEFINES += SAFE_BROWSING_DB_LOCAL
DEFINES += EXPAT_RELATIVE_PATH
DEFINES += WEBRTC_WIN
DEFINES += WEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE
DEFINES += QT_NO_EMIT
DEFINES += _CRT_SECURE_NO_WARNINGS
DEFINES += USE_LIBPCI=1
DEFINES += __STDC_CONSTANT_MACROS
DEFINES += __STDC_FORMAT_MACROS
DEFINES += DYNAMIC_ANNOTATIONS_ENABLED=1
DEFINES += WTF_USE_DYNAMIC_ANNOTATIONS=1
DEFINES += WEBRTC_EXTERNAL_JSON

INCLUDEPATH = D:/webrtc/webrtc_checkout/src \
            D:/opencv3.1/sources/include


#OpenCV 3.1
#LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_world310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_calib3d310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_core310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_features2d310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_flann310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_highgui310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_imgcodecs310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_imgproc310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_ml310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_objdetect310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_photo310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_shape310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_stitching310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_superres310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_videoio310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_video310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_ts310d.lib
LIBS += D:/opencv3.1/sources/build_static_64/lib/Debug/opencv_world310d.lib
#OpenCV 3.1 thrid-party
LIBS += D:/opencv3.1/sources/build_static_64/3rdparty/lib/Debug/zlibd.lib
LIBS += D:/opencv3.1/sources/build_static_64/3rdparty/lib/Debug/IlmImfd.lib
LIBS += D:/opencv3.1/sources/build_static_64/3rdparty/lib/Debug/libjasperd.lib
LIBS += D:/opencv3.1/sources/build_static_64/3rdparty/lib/Debug/libpngd.lib
LIBS += D:/opencv3.1/sources/build_static_64/3rdparty/lib/Debug/libtiffd.lib

LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/Vfw32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/ComCtl32.lib


#VC x64 LIBS
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/WinInet.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/dnsapi.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/version.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/msimg32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/ws2_32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/usp10.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/psapi.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/dbghelp.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/winmm.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/shlwapi.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/kernel32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/gdi32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/winspool.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/comdlg32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/advapi32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/shell32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/ole32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/oleaut32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/user32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/uuid.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/odbc32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/odbccp32.lib
LIBS += C:/"Program Files (x86)"/"Microsoft Visual Studio 14.0"/VC/lib/amd64/delayimp.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/credui.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/dmoguids.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/wmcodecdspuuid.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/amstrmid.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/msdmo.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/crypt32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/iphlpapi.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/secur32.lib
LIBS += C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.10586.0/um/x64/Strmiids.lib
# WEBRTC LIBS
LIBS += D:/webrtc/libs_x64/audio_coding_module.lib
LIBS += D:/webrtc/libs_x64/audio_conference_mixer.lib
LIBS += D:/webrtc/libs_x64/audio_decoder_factory_interface.lib
LIBS += D:/webrtc/libs_x64/audio_decoder_interface.lib
LIBS += D:/webrtc/libs_x64/audio_device.lib
LIBS += D:/webrtc/libs_x64/audio_encoder_interface.lib
LIBS += D:/webrtc/libs_x64/audio_processing.lib
LIBS += D:/webrtc/libs_x64/audio_processing_sse2.lib
LIBS += D:/webrtc/libs_x64/audioproc_debug_proto.lib
LIBS += D:/webrtc/libs_x64/audioproc_protobuf_utils.lib
LIBS += D:/webrtc/libs_x64/bitrate_controller.lib
LIBS += D:/webrtc/libs_x64/boringssl.lib
LIBS += D:/webrtc/libs_x64/boringssl_asm.lib
LIBS += D:/webrtc/libs_x64/builtin_audio_decoder_factory.lib
LIBS += D:/webrtc/libs_x64/bwe_simulator.lib
LIBS += D:/webrtc/libs_x64/bwe_tools_util.lib
LIBS += D:/webrtc/libs_x64/channel_transport.lib
LIBS += D:/webrtc/libs_x64/cng.lib
LIBS += D:/webrtc/libs_x64/command_line_parser.lib
LIBS += D:/webrtc/libs_x64/common_audio.lib
LIBS += D:/webrtc/libs_x64/common_audio_sse2.lib
LIBS += D:/webrtc/libs_x64/common_video.lib
LIBS += D:/webrtc/libs_x64/congestion_controller.lib
LIBS += D:/webrtc/libs_x64/desktop_capture.lib
LIBS += D:/webrtc/libs_x64/desktop_capture_differ_sse2.lib
LIBS += D:/webrtc/libs_x64/directshow_baseclasses.lib
LIBS += D:/webrtc/libs_x64/expat.lib
LIBS += D:/webrtc/libs_x64/field_trial.lib
LIBS += D:/webrtc/libs_x64/field_trial_default.lib
LIBS += D:/webrtc/libs_x64/frame_editing_lib.lib
LIBS += D:/webrtc/libs_x64/g711.lib
LIBS += D:/webrtc/libs_x64/g722.lib
LIBS += D:/webrtc/libs_x64/genperf_libs.lib
LIBS += D:/webrtc/libs_x64/gflags.lib
LIBS += D:/webrtc/libs_x64/gmock.lib
LIBS += D:/webrtc/libs_x64/gtest.lib
LIBS += D:/webrtc/libs_x64/ilbc.lib
LIBS += D:/webrtc/libs_x64/isac.lib
LIBS += D:/webrtc/libs_x64/isac_common.lib
LIBS += D:/webrtc/libs_x64/isac_fix.lib
LIBS += D:/webrtc/libs_x64/jsoncpp.lib
LIBS += D:/webrtc/libs_x64/libjingle_peerconnection.lib
LIBS += D:/webrtc/libs_x64/libjpeg.lib
LIBS += D:/webrtc/libs_x64/libsrtp.lib
LIBS += D:/webrtc/libs_x64/libstunprober.lib
LIBS += D:/webrtc/libs_x64/libvpx.lib
LIBS += D:/webrtc/libs_x64/libvpx_intrinsics_avx.lib
LIBS += D:/webrtc/libs_x64/libvpx_intrinsics_avx2.lib
LIBS += D:/webrtc/libs_x64/libvpx_intrinsics_mmx.lib
LIBS += D:/webrtc/libs_x64/libvpx_intrinsics_sse2.lib
LIBS += D:/webrtc/libs_x64/libvpx_intrinsics_sse4_1.lib
LIBS += D:/webrtc/libs_x64/libvpx_intrinsics_ssse3.lib
LIBS += D:/webrtc/libs_x64/libyuv.lib
LIBS += D:/webrtc/libs_x64/media_file.lib
LIBS += D:/webrtc/libs_x64/metrics.lib
LIBS += D:/webrtc/libs_x64/metrics_default.lib
LIBS += D:/webrtc/libs_x64/neteq.lib
LIBS += D:/webrtc/libs_x64/neteq_test_support.lib
LIBS += D:/webrtc/libs_x64/neteq_test_tools.lib
LIBS += D:/webrtc/libs_x64/openmax_dl.lib
LIBS += D:/webrtc/libs_x64/opus.lib
LIBS += D:/webrtc/libs_x64/paced_sender.lib
LIBS += D:/webrtc/libs_x64/pcm16b.lib
LIBS += D:/webrtc/libs_x64/protobuf_lite.lib
LIBS += D:/webrtc/libs_x64/protoc_lib.lib
LIBS += D:/webrtc/libs_x64/red.lib
LIBS += D:/webrtc/libs_x64/remote_bitrate_estimator.lib
LIBS += D:/webrtc/libs_x64/rent_a_codec.lib
LIBS += D:/webrtc/libs_x64/rtc_base.lib
LIBS += D:/webrtc/libs_x64/rtc_base_approved.lib
LIBS += D:/webrtc/libs_x64/rtc_event_log.lib
LIBS += D:/webrtc/libs_x64/rtc_event_log_parser.lib
LIBS += D:/webrtc/libs_x64/rtc_event_log_proto.lib
LIBS += D:/webrtc/libs_x64/rtc_event_log_source.lib
LIBS += D:/webrtc/libs_x64/rtc_media.lib
LIBS += D:/webrtc/libs_x64/rtc_p2p.lib
LIBS += D:/webrtc/libs_x64/rtc_pc.lib
LIBS += D:/webrtc/libs_x64/rtc_task_queue.lib
LIBS += D:/webrtc/libs_x64/rtc_xmllite.lib
LIBS += D:/webrtc/libs_x64/rtc_xmpp.lib
LIBS += D:/webrtc/libs_x64/rtp_rtcp.lib
LIBS += D:/webrtc/libs_x64/system_wrappers.lib
LIBS += D:/webrtc/libs_x64/usrsctplib.lib
LIBS += D:/webrtc/libs_x64/video_capture.lib
LIBS += D:/webrtc/libs_x64/video_capture_module.lib
LIBS += D:/webrtc/libs_x64/video_capture_module_internal_impl.lib
LIBS += D:/webrtc/libs_x64/video_coding_utility.lib
LIBS += D:/webrtc/libs_x64/video_processing.lib
LIBS += D:/webrtc/libs_x64/video_processing_sse2.lib
LIBS += D:/webrtc/libs_x64/voice_engine.lib
LIBS += D:/webrtc/libs_x64/webrtc.lib
LIBS += D:/webrtc/libs_x64/webrtc_common.lib
LIBS += D:/webrtc/libs_x64/webrtc_h264.lib
LIBS += D:/webrtc/libs_x64/webrtc_i420.lib
LIBS += D:/webrtc/libs_x64/webrtc_opus.lib
LIBS += D:/webrtc/libs_x64/webrtc_utility.lib
LIBS += D:/webrtc/libs_x64/webrtc_video_coding.lib
LIBS += D:/webrtc/libs_x64/webrtc_vp8.lib
LIBS += D:/webrtc/libs_x64/webrtc_vp9.lib
