#pragma once
#include <QObject>
#include <QImage>

#include <iostream>
#include <vector>

#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include "processingfunction.hpp"

#define VERTICAL_FLIP 0
#define HORIZONTAL_FLIP 1

class WorkerOpenCV : public QObject {
	Q_OBJECT

public:
	WorkerOpenCV(QObject * parent = Q_NULLPTR);
	~WorkerOpenCV();

private:
	cv::Mat _frameOriginal;
	//cv::Mat _frameOriginalGrayscale;
	cv::Mat _frameProcessed;
	cv::VideoCapture *cap;

	bool status;
	bool toggleStream;
	bool toggleRecord;

	// ============================================================

	int rotationValue;
	bool flipHorizontal;
	bool flipVertical;

	// processing functions will execute in this order
	// duplicates are allowed
	std::vector<ProcessingFunction> processingExecutionOrder = { 
		// Image Correction
		/*
		ProcessingFunction::BRIGHTNESS,
		ProcessingFunction::CONTRAST,
		ProcessingFunction::GAMMA,
		*/

		// Smooth or Blur Image
		ProcessingFunction::BILATERAL_FILTER,
		ProcessingFunction::GAUSSIAN_BLUR,
		ProcessingFunction::HOMOGENEOUS_BLUR,
		ProcessingFunction::MEDIAN_BLUR,

		// Morphology
		ProcessingFunction::EROSION,
		ProcessingFunction::DILATION,

		// Non-Photorealistic Rendering
		ProcessingFunction::NORMALIZED_CONVOLUTION_FILTER,
		ProcessingFunction::RECURSIVE_FILTER,
		ProcessingFunction::DETAIL_ENHANCEMENT,
		ProcessingFunction::PENCIL_SKETCH,
		ProcessingFunction::COLOR_PENCIL_DRAWING,
		ProcessingFunction::STYLIZATION,

		// Edge Detection
		ProcessingFunction::CANNY_EDGE_DETECTION,
		ProcessingFunction::SOBEL_EDGE_DETECTION,

		// Miscellaneous
		ProcessingFunction::ADAPTIVE_THRESHOLD,
		ProcessingFunction::BINARY_THRESHOLD,
		ProcessingFunction::CONTOURS,
		ProcessingFunction::CONVEX_HULL,
		ProcessingFunction::CORNER_DETECTION,
		ProcessingFunction::HOUGH_CIRCLES,
		ProcessingFunction::HOUGH_LINES,
		ProcessingFunction::LAPLACIAN,
		ProcessingFunction::EQUALIZE_HISTOGRAM,
		ProcessingFunction::OBJECT_DETECTION
	};

	// ------------------------------------------------------------
	// TAB - COLOR FORMAT
	// ------------------------------------------------------------

	QImage::Format qImageFormat;
	cv::ColorConversionCodes cvColorCode;

	bool contrastEnable;
	int contrastValue;

	bool brightnessEnable;
	int brightnessValue;

	bool gammaEnable;
	int gammaValue;

	bool equalizeHistogramEnable;

	bool adaptiveThresholdEnable;
	int adaptiveMethod;
	int adaptiveThresholdType;
	int adaptiveThresholdBlockSize; // odd number

	bool binaryThresholdEnable;
	int binaryThreshold;

	// ------------------------------------------------------------
	// TAB - PROCESSING 1
	// ------------------------------------------------------------

	bool contoursEnable;
	int contoursThreshold;

	bool convexHullEnable;
	int convexHullThreshold;

	bool cornerDetectionEnable;
	int cornerDetectionMaxCorners;
	double cornerDetectionQualityLevel;
	double cornerDetectionMinDistance;
	int cornerDetectionBlockSize;

	bool houghCirclesEnable;
	int houghCirclesCannyThreshold;
	int houghCirclesAccumulatorThreshold;
	int houghCirclesMinRadius;
	int houghCirclesMaxRadius;

	bool houghLinesEnable;
	int houghLinesThreshold;
	int houghLinesMinLineLength;
	int houghLinesMaxLineGap;
	bool useStandardHoughLines;

	bool laplacianEnable;
	int laplacianKernelSize;

	bool cannyDetectionEnable;
	int cannyDetectionThreshold;
	int cannyDetectionRatio;
	int cannyDetectionKernelSize; // kernel size must be 3, 5, or 7
	cv::Mat cannyEdges;

	bool sobelEnable;
	int sobelKernelSize; // kernel size must be 1, 3, 5, or 7
	int sobelScaleValue;
	int sobelDeltaValue;
	const int SOBEL_DDEPTH_VALUE = CV_16S;
	cv::Mat sobelGradX, sobelGradY;
	cv::Mat sobelAbsGradX, sobelAbsGradY;
	cv::Mat sobelTotalGradient;

	bool homogeneousBlurEnable;
	int homogeneousBlurKernelSize;

	bool gaussianBlurEnable;
	int gaussianBlurKernelWidth;
	int gaussianBlurKernelHeight;
	
	bool medianBlurEnable;
	int medianBlurKernelSize; // must be odd and greater than 1
	
	bool bilateralFilterEnable;
	int bilateralFilterValue;

	bool objectDetectionEnable;

	bool gridEnable;
	int gridLineSpacing;

	// ------------------------------------------------------------
	// TAB - PROCESSING 2
	// ------------------------------------------------------------

	bool erosionEnable;
	int erosionValue;
	cv::Mat erosionKernel;
	cv::MorphShapes erosionShape;

	bool dilationEnable;
	int dilationValue;
	cv::Mat dilationKernel;
	cv::MorphShapes dilationShape;

	bool normalizedConvolutionFilterEnable;

	bool recursiveFilterEnable;

	bool detailEnhancementEnable;

	bool pencilSketchEnable;

	bool colorPencilDrawingEnable;

	bool stylizationEnable;

	// ============================================================

	void checkIfDeviceAlreadyOpened(const int device);
	void process();

	void GridLines(cv::Mat img,
		int lineThickness = 1,
		int lineType = cv::LINE_8,
		cv::Scalar lineColor = cv::Scalar(0, 0, 0),
		int lineSpacing = 100, 
		const cv::Point* start = NULL, 
		const cv::Point* end = NULL);

	bool isGrayscale();

signals:
	void sendFrame(QImage frameProcessed);

public slots:
	void receiveGrabFrame();
	void receiveSetup(const int device);
	void receiveToggleStream();
	void receiveToggleRecord();
	void receiveRotateCounterClockwise();
	void receiveRotateClockwise();
	void receiveFlipHorizontal();
	void receiveFlipVertical();
	void receiveConsoleInput(std::string consoleInput);

	// ============================================================

	// ------------------------------------------------------------
	// TAB - COLOR FORMAT
	// ------------------------------------------------------------

	void receiveEnableImageFormatAlpha8();
	void receiveEnableImageFormatARGB32();
	void receiveEnableImageFormatARGB32Premultiplied();
	void receiveEnableImageFormatBGR30();
	void receiveEnableImageFormatIndexed8();
	void receiveEnableImageFormatMonoLSB();
	void receiveEnableImageFormatRGB16();
	void receiveEnableImageFormatRGB30();
	void receiveEnableImageFormatRGB32();
	void receiveEnableImageFormatRGB444();
	void receiveEnableImageFormatRGB555();
	void receiveEnableImageFormatRGB666();
	void receiveEnableImageFormatRGB888();
	void receiveEnableImageFormatRGBA8888();
	void receiveEnableImageFormatRGBA8888Premultiplied();
	void receiveEnableImageFormatRGBX8888();

	void receiveEnableColorCodeBGR555();
	void receiveEnableColorCodeBGRA();
	void receiveEnableColorCodeGRAY();
	void receiveEnableColorCodeHLS();
	void receiveEnableColorCodeHSV();
	void receiveEnableColorCodeLab();
	void receiveEnableColorCodeLuv();
	void receiveEnableColorCodeRGB();
	void receiveEnableColorCodeRGBA();
	void receiveEnableColorCodeXYZ();
	void receiveEnableColorCodeYCrCb();
	void receiveEnableColorCodeYUV();
	void receiveEnableColorCodeYUV_I420();
	void receiveEnableColorCodeYUV_IYUV();
	void receiveEnableColorCodeYUV_YV12();

	void receiveEnableContrast();
	void receiveContrastValue(int value);

	void receiveEnableBrightness();
	void receiveBrightnessValue(int value);

	void receiveEnableGamma();
	void receiveGammaValue(int value);

	void receiveEnableEqualizeHistogram();

	void receiveEnableAdaptiveThreshold();
	void receiveAdaptiveThresholdBlockSize(int blockSize);

	void receiveEnableBinaryThreshold();
	void receiveBinaryThreshold(int threshold);

	// ------------------------------------------------------------
	// TAB - PROCESSING 1
	// ------------------------------------------------------------

	void receiveEnableContours();
	void receiveContoursThreshold(int value);

	void receiveEnableConvexHull();
	void receiveConvexHullThreshold(int value);

	void receiveEnableCornerDetection();
	void receiveCornerDetectionMaxCorners(int value);
	void receiveCornerDetectionQualityLevel(double value);
	void receiveCornerDetectionMinDistance(int value);
	void receiveCornerDetectionBlockSize(int value);

	void receiveEnableHoughCircles();
	void receiveHoughCirclesCannyThreshold(int value);
	void receiveHoughCirclesAccumulatorThreshold(int value);
	void receiveHoughCirclesMinRadius(int value);
	void receiveHoughCirclesMaxRadius(int value);

	void receiveEnableHoughLines();
	void receiveHoughLinesThreshold(int value);
	void receiveHoughLinesMinLineLength(int value);
	void receiveHoughLinesMaxLineGap(int value);
	void receiveHoughLinesType(int index);

	void receiveEnableLaplacian();
	void receiveLaplacianKernelSize(int value);

	void receiveEnableCannyDetection();
	void receiveCannyDetectionThreshold(int value);
	void receiveCannyDetectionRatio(int value);
	void receiveCannyDetectionKernelSize(int value);

	void receiveEnableSobel();
	void receiveSobelKernelSize(int value);

	void receiveEnableHomogeneousBlur();
	void receiveHomogeneousBlurKernelSize(int value);

	void receiveEnableGaussianBlur();
	void receiveGaussianBlurKernelWidth(int value);
	void receiveGaussianBlurKernelHeight(int value);

	void receiveEnableMedianBlur();
	void receiveMedianBlurKernelSize(int value);

	void receiveEnableBilateralFilter();
	void receiveBilateralFilterValue(int value);

	void receiveEnableObjectDetection();

	void receiveEnableGrid();
	void receiveGridLineSpacingValue(int value);

	// ------------------------------------------------------------
	// TAB - PROCESSING 2
	// ------------------------------------------------------------

	void receiveEnableErosion();
	void receiveErosionValue(int value);

	void receiveEnableDilation();
	void receiveDilationValue(int value);

	void receiveEnableNormalizedConvolutionFilter();

	void receiveEnableRecursiveFilter();

	void receiveEnableDetailEnhancement();

	void receiveEnablePencilSketch();

	void receiveEnableColorPencilDrawing();

	void receiveEnableStylization();

	// ============================================================

};
