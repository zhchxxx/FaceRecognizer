#ifndef TOOL_H
#define TOOL_H

//#if defined(_MSC_VER) && (_MSC_VER >= 1600)
//#pragma execution_character_set("utf-8")
//#endif

#ifdef HAVE_FFMPEG
extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#undef PixelFormat
}
#endif

#include "facerecognizer_export.h"

#include <QObject>
#include <memory>
#include <string>
#include <QWidget>
#include <QVideoFrame>
#include <QFileDialog>

/**
 * @brief 工具类。提供一此工具函数  
 * @ingroup RABBITIM_TOOL
 */
class FACERECOGNIZER_EXPORT CImageTool : public QObject
{
    Q_OBJECT
public:
    explicit CImageTool(QObject *parent = 0);
    ~CImageTool();

public:
#ifdef HAVE_FFMPEG
    //设置ffmpeg日志输出  
    static int SetFFmpegLog();

    /**
     * @brief 格式映射  
     * @param format 
     * @return 
     */
    static AVPixelFormat QVideoFrameFormatToFFMpegPixFormat(
            const QVideoFrame::PixelFormat format);
    static AVPixelFormat QImageFormatToFFMpegPixFormat(
            const QImage::Format format);

private:
    /**
     * @brief 格式转换，这个函数只CTool内部调用    
     *        
     * @param inFrame：要转换的帧（注意：需要调用者用 avpicture_fill 初始化。）  
     * @param nInWidth：转换帧的宽度  
     * @param nInHeight：转换帧的高度  
     * @param inPixelFormat：转换帧的格式  
     * @param outFrame：转换后的帧（注意：需要调用者用 avpicture_alloc 或 avpicture_fill 初始化。）    
     * @param nOutWidth：转换后帧的宽度  
     * @param nOutHeight：转换后帧的高度  
     * @param outPixelFormat：转换后帧的格式  
     * @return 
     */
    static int ConvertFormat(/*[in]*/  const AVPicture &inFrame,           /** 要转换的帧 */
                             /*[in]*/  int nInWidth,                 /** 要转换的帧的宽度 */
                             /*[in]*/  int nInHeight,                /** 要转换的帧的高度 */
                             /*[in]*/  AVPixelFormat inPixelFormat,  /** 要转换的帧的格式 */
                             /*[out]*/ AVPicture &outFrame,                /** 转换后的帧 */
                             /*[in]*/  int nOutWidth,                /** 转换后的帧的宽度 */
                             /*[in]*/  int nOutHeight,               /** 转换后的帧的高度 */  
                             /*[in]*/  AVPixelFormat outPixelFormat);/** 转换后的帧的格式 */  
    
public:

#endif

#ifdef RABBITIM_USE_OPENCV
    /** 
     * @brief 对图像进行旋转  
     * @param src:源图像
     * @param _cneter:旋转中心
     * @param angle:角度
     * @param scale:缩放  
     * 返回值:旋转后的图像  */
    static cv::Mat ImageRotate(cv::Mat & src,
                               const cv::Point &_center,
                               double angle, double scale = 1);
#endif

    /** 
     * @brief 对YUV420sp(NV12、NV21)存储格式的图像旋转+-90度
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度
     * @param mode=1为顺时针旋转90度，mode=-1为逆时针旋转90度  
     */
    static void YUV420spRotate90(uchar *dst,
                                 const uchar *src, int srcWidth,
                                 int srcHeight, int mode);

    /** 
     * @brief 对YUV420sp(NV12、NV21)存储格式的图像旋转90度
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度  
     */
    static void YUV420spRotate90(uchar *dst, const uchar *src,
                                 int srcWidth, int srcHeight);

    /** 
     * @brief 对YUV420sp(NV12、NV21)存储格式的图像旋转-90度
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度  
     */
    static void YUV420spRotateNegative90(uchar *dst, const uchar *src,
                                         int srcWidth, int srcHeight);

    /** 
     * @brief 对YUV420sp(NV12、NV21)存储格式的图像做镜像
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度
     * @param mode=1为x轴镜像，mode=0为y轴镜像，mode=-1为x、y轴都镜像。 
     */
    static void YUV420spMirror(uchar* dst, const uchar *src,
                               int srcWidth, int srcHeight, int mode = 0);

    /** 
     * @brief 以Y轴做镜像
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度   
     */
    static void YUV420spMirrorY(uchar *dst, const uchar *src,
                                int srcWidth, int srcHeight);

    /** 
     * @brief 以XY轴做镜像
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度  
     */
    static void YUV420spMirrorXY(uchar *dst, const uchar *src,
                                 int srcWidth, int srcHeight);

    /** 
     * @brief 以X轴做镜像
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度   
     */
    static void YUV420spMirrorX(uchar *dst, const uchar *src,
                                int srcWidth, int srcHeight);

    static bool isImageFile(const QString& szFile);///<判断文件是否是图片，根据扩展名进行判断  

    static void YUV420_2_RGB(unsigned char* pYUV, unsigned char* pRGB, int width, int height);
    static QImage ConverFormatToRGB888(const QVideoFrame &frame);
#ifdef HAVE_LIBYUV
    static QImage LibyuvConverFormatToRGB888(const QVideoFrame &frame);
#endif
    
signals:

public slots:

};

#endif // TOOL_H
