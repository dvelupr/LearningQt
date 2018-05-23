#include "workeropencv.hpp"

WorkerOpenCV::WorkerOpenCV(QObject * parent) :
	QObject(parent),
	status(false),
	toggleStream(false),
	toggleRecord(false),
	// ============================================================

	rotationValue(0),
	flipHorizontal(false),
	flipVertical(false),

	// ------------------------------------------------------------
	// TAB - COLOR FORMAT
	// ------------------------------------------------------------

	qImageFormat(QImage::Format::Format_RGB888),
	cvColorCode(cv::ColorConversionCodes::COLOR_BGR2RGB),

	contrastEnable(false),
	contrastValue(0),

	brightnessEnable(false),
	brightnessValue(0),

	gammaEnable(false),
	gammaValue(0),

	equalizeHistogramEnable(false),

	binaryThresholdEnable(false),
	binaryThreshold(127),

	adaptiveThresholdEnable(false),
	adaptiveMethod(cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C),
	adaptiveThresholdType(cv::ThresholdTypes::THRESH_BINARY),
	adaptiveThresholdBlockSize(15),

	// ------------------------------------------------------------
	// TAB - PROCESSING 1
	// ------------------------------------------------------------

	contoursEnable(false),
	contoursThreshold(100),

	convexHullEnable(false),
	convexHullThreshold(100),

	cornerDetectionEnable(false),
	cornerDetectionMaxCorners(4),
	cornerDetectionQualityLevel(0.01),
	cornerDetectionMinDistance(10),
	cornerDetectionBlockSize(3),

	houghCirclesEnable(false),
	houghCirclesCannyThreshold(100),
	houghCirclesAccumulatorThreshold(50),
	houghCirclesMinRadius(20),
	houghCirclesMaxRadius(100),

	houghLinesEnable(false),
	houghLinesThreshold(50),
	houghLinesMinLineLength(30),
	houghLinesMaxLineGap(10),
	useStandardHoughLines(true),

	laplacianEnable(false),
	laplacianKernelSize(3),

	cannyDetectionEnable(false),
	cannyDetectionThreshold(0),
	cannyDetectionRatio(3),
	cannyDetectionKernelSize(3),

	sobelEnable(false),
	sobelKernelSize(3),
	sobelScaleValue(1),
	sobelDeltaValue(0),

	homogeneousBlurEnable(false),
	homogeneousBlurKernelSize(3),

	gaussianBlurEnable(false),
	gaussianBlurKernelWidth(3),
	gaussianBlurKernelHeight(3),

	medianBlurEnable(false),
	medianBlurKernelSize(3),

	bilateralFilterEnable(false),
	bilateralFilterValue(1),

	objectDetectionEnable(false),

	gridEnable(false),
	gridLineSpacing(5),

	// ------------------------------------------------------------
	// TAB - PROCESSING 2
	// ------------------------------------------------------------

	erosionEnable(false),
	erosionValue(1),

	dilationEnable(false),
	dilationValue(1),

	normalizedConvolutionFilterEnable(false),

	recursiveFilterEnable(false),

	detailEnhancementEnable(false),

	pencilSketchEnable(false),

	colorPencilDrawingEnable(false),

	stylizationEnable(false)

	// ============================================================
{
	cap = new cv::VideoCapture();
}

WorkerOpenCV::~WorkerOpenCV()
{
	if (cap->isOpened())
		cap->release();
	
	delete cap;
}

void WorkerOpenCV::receiveGrabFrame()
{
	if (!toggleStream)
		return;

	(*cap) >> _frameOriginal;
	
	if (_frameOriginal.empty())
		return;

	// TODO:: if there is no change in the frame then don't process?
	process();

	// ..................................................
	//cv::ColorConversionCodes::COLOR_BGR2RGBA
	// ..................................................
	//QImage::Format::Format_ARGB32
	//QImage::Format::Format_ARGB32_Premultiplied
	//QImage::Format::Format_RGB32
	//QImage::Format::Format_RGBA8888
	//QImage::Format::Format_RGBA8888_Premultiplied
	//QImage::Format::Format_RGBX8888
	// ..................................................

	// ..................................................
	//cv::ColorConversionCodes::COLOR_BGR2BGR555
	// ..................................................
	//QImage::Format::Format_ARGB8555_Premultiplied
	// ..................................................

	//QImage::Format::Format_A2BGR30_Premultiplied
	//QImage::Format::Format_A2RGB30_Premultiplied
	//QImage::Format::Format_Alpha8
	//QImage::Format::Format_ARGB32
	//QImage::Format::Format_ARGB32_Premultiplied
	//QImage::Format::Format_ARGB4444_Premultiplied
	//QImage::Format::Format_ARGB6666_Premultiplied
	//QImage::Format::Format_ARGB8555_Premultiplied
	//QImage::Format::Format_ARGB8565_Premultiplied
	//QImage::Format::Format_BGR30
	//QImage::Format::Format_Grayscale8
	//QImage::Format::Format_Indexed8
	//QImage::Format::Format_Invalid
	//QImage::Format::Format_Mono
	//QImage::Format::Format_MonoLSB
	//QImage::Format::Format_RGB16
	//QImage::Format::Format_RGB30
	//QImage::Format::Format_RGB32
	//QImage::Format::Format_RGB444
	//QImage::Format::Format_RGB555
	//QImage::Format::Format_RGB666
	//QImage::Format::Format_RGB888
	//QImage::Format::Format_RGBA8888
	//QImage::Format::Format_RGBA8888_Premultiplied
	//QImage::Format::Format_RGBX8888
	//QImage::Format::NImageFormats

	// TODO:: should process() go here?

	QImage output(
		(const unsigned char*)_frameProcessed.data,
		_frameProcessed.cols,
		_frameProcessed.rows,
		_frameProcessed.step,
		qImageFormat);
		//QImage::Format_Indexed8); // assuming we are working with 8 BGR from opencv image

	emit sendFrame(output);
}

void WorkerOpenCV::checkIfDeviceAlreadyOpened(const int device)
{
	if (cap->isOpened())
		cap->release();

	cap->open(device);
}

void WorkerOpenCV::process()
{
	std::vector<std::string> args;
	//cv::cvtColor(_frameOriginal, _frameOriginalGrayscale, cv::COLOR_BGR2GRAY);

	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2BGR555);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2BGR565);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2BGRA);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2HLS);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2HLS_FULL);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2HSV);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2HSV_FULL);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2Lab);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2Luv);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2RGB);
	
	cv::cvtColor(_frameOriginal, _frameProcessed, cvColorCode);

	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2XYZ);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2YCrCb);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2YUV);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2YUV_I420);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2YUV_IYUV);
	//cv::cvtColor(_frameOriginal, _frameProcessed, cv::ColorConversionCodes::COLOR_BGR2YUV_YV12);

	// TODO:: scale image

	// ============================================================
	// ============================================================
	// ============================================================

	// TODO:: rotate original & grayscale ??
	if (rotationValue != 0)
	{
		switch (rotationValue)
		{
		case 90:
			cv::rotate(_frameProcessed, _frameProcessed, cv::ROTATE_90_CLOCKWISE);
			break;

		case 180:
			cv::rotate(_frameProcessed, _frameProcessed, cv::ROTATE_180);
			break;

		case 270:
			cv::rotate(_frameProcessed, _frameProcessed, cv::ROTATE_90_COUNTERCLOCKWISE);
			break;

		default:
			break;
		}
	}

	if (flipHorizontal)
	{
		cv::flip(_frameProcessed, _frameProcessed, HORIZONTAL_FLIP);
	}

	if (flipVertical)
	{
		cv::flip(_frameProcessed, _frameProcessed, VERTICAL_FLIP);
	}

	// ...........................
	// Image Correction
	// ...........................
	double alpha_value = contrastValue / 100.0;
	int beta_value = brightnessValue - 100;
	double gamma_value = gammaValue / 100.0;

	if (contrastEnable && brightnessEnable)
	{
		cv::Mat res;
		_frameProcessed.convertTo(res, -1, alpha_value, beta_value);
		_frameProcessed += res;

		// create new image to the right of original image
		//cv::hconcat(_frameProcessed, res, _frameProcessed);
	}
	else if (contrastEnable && brightnessEnable == false)
	{
		cv::Mat res;
		_frameProcessed.convertTo(res, -1, alpha_value, 0);
		_frameProcessed += res;
	}
	else if (contrastEnable == false && brightnessEnable)
	{
		cv::Mat res;
		_frameProcessed.convertTo(res, -1, 0, beta_value);
		_frameProcessed += res;
	}

	if (gammaEnable)
	{
		CV_Assert(gamma_value >= 0);
		cv::Mat lookUpTable(1, 256, CV_8U);
		uchar* p = lookUpTable.ptr();
		for (int i = 0; i < 256; ++i)
			p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_value) * 255.0);

		cv::Mat res = _frameProcessed.clone();
		LUT(_frameProcessed, lookUpTable, res);
		_frameProcessed += res;
	}

	// ------------------------------------------------------------
	// EXECUTE PROCESSING FUNCTIONS
	// ------------------------------------------------------------

	for (size_t i = 0; i < processingExecutionOrder.size(); i++)
	{
		switch (processingExecutionOrder[i])
		{
			// ....................................................
		case ProcessingFunction::BILATERAL_FILTER:
			// ....................................................
			if (bilateralFilterEnable)
			{
				cv::Mat inputImage = _frameProcessed.clone();
				cv::Mat outputImage;
				cv::bilateralFilter(
					inputImage,
					outputImage,
					bilateralFilterValue,
					(bilateralFilterValue * 2.0),
					(bilateralFilterValue / 2.0));

				_frameProcessed += outputImage;
			}
			break;

			// ....................................................
		case ProcessingFunction::GAUSSIAN_BLUR:
			// ....................................................
			if (gaussianBlurEnable)
			{
				cv::GaussianBlur(
					_frameProcessed,
					_frameProcessed,
					cv::Size(gaussianBlurKernelWidth, gaussianBlurKernelHeight),
					0,
					0,
					cv::BorderTypes::BORDER_DEFAULT);
			}
			break;

			// ....................................................
		case ProcessingFunction::HOMOGENEOUS_BLUR:
			// ....................................................
			if (homogeneousBlurEnable)
			{
				cv::blur(
					_frameProcessed,
					_frameProcessed,
					cv::Size(homogeneousBlurKernelSize, homogeneousBlurKernelSize),
					cv::Point(-1, -1),
					cv::BorderTypes::BORDER_DEFAULT);
			}
			break;

			// ....................................................
		case ProcessingFunction::MEDIAN_BLUR:
			// ....................................................
			if (medianBlurEnable)
			{
				cv::medianBlur(_frameProcessed, _frameProcessed, medianBlurKernelSize);
			}
			break;

			// ....................................................
		case ProcessingFunction::EROSION:
			// ....................................................
			if (erosionEnable)
			{
				erosionShape = cv::MorphShapes::MORPH_CROSS;
				erosionKernel = getStructuringElement(erosionShape,
					cv::Size(2 * erosionValue + 1, 2 * erosionValue + 1),
					cv::Point(erosionValue, erosionValue));

				cv::erode(_frameProcessed, _frameProcessed, erosionKernel);
			}
			break;

			// ....................................................
		case ProcessingFunction::DILATION:
			// ....................................................
			if (dilationEnable)
			{
				dilationShape = cv::MorphShapes::MORPH_CROSS;
				dilationKernel = getStructuringElement(dilationShape,
					cv::Size(2 * dilationValue + 1, 2 * dilationValue + 1),
					cv::Point(dilationValue, dilationValue));

				cv::dilate(_frameProcessed, _frameProcessed, dilationKernel);
			}
			break;

			// ....................................................
		case ProcessingFunction::NORMALIZED_CONVOLUTION_FILTER:
			// ....................................................
			if (normalizedConvolutionFilterEnable)
			{
				cv::edgePreservingFilter(_frameProcessed, _frameProcessed, cv::NORMCONV_FILTER);
			}
			break;

			// ....................................................
		case ProcessingFunction::RECURSIVE_FILTER:
			// ....................................................
			if (recursiveFilterEnable)
			{
				cv::edgePreservingFilter(_frameProcessed, _frameProcessed, cv::RECURS_FILTER);
			}
			break;

			// ....................................................
		case ProcessingFunction::DETAIL_ENHANCEMENT:
			// ....................................................
			if (detailEnhancementEnable)
			{
				cv::detailEnhance(_frameProcessed, _frameProcessed);
			}
			break;

			// ....................................................
		case ProcessingFunction::PENCIL_SKETCH:
			// ....................................................
			if (pencilSketchEnable)
			{
				cv::Mat pencilSketchImage;
				cv::Mat colorPencilDrawingImage;
				pencilSketch(_frameProcessed, pencilSketchImage, colorPencilDrawingImage, 10, 0.1f, 0.03f);

				if (isGrayscale())
				{
					_frameProcessed = pencilSketchImage.clone();
				}
				else
				{
					cv::cvtColor(pencilSketchImage, pencilSketchImage, cv::COLOR_GRAY2RGB);
					_frameProcessed = pencilSketchImage.clone();
				}
			}
			break;

			// ....................................................
		case ProcessingFunction::COLOR_PENCIL_DRAWING:
			// ....................................................
			if (colorPencilDrawingEnable)
			{
				cv::Mat pencilSketchImage;
				pencilSketch(_frameProcessed, pencilSketchImage, _frameProcessed, 10, 0.1f, 0.03f);
			}
			break;

			// ....................................................
		case ProcessingFunction::STYLIZATION:
			// ....................................................
			if (stylizationEnable)
			{
				cv::stylization(_frameProcessed, _frameProcessed);
			}
			break;

			// ....................................................
		case ProcessingFunction::CANNY_EDGE_DETECTION:
			// ....................................................
			if (cannyDetectionEnable)
			{
				cv::cvtColor(_frameProcessed.clone(), cannyEdges, cv::COLOR_BGR2GRAY);

				/// Reduce noise with a kernel 3x3
				//cv::blur(cannyEdges, cannyEdges, cv::Size(cannyDetectionKernelSize, cannyDetectionKernelSize));

				cv::Canny(
					cannyEdges,
					cannyEdges,
					cannyDetectionThreshold,
					cannyDetectionThreshold * cannyDetectionRatio,
					cannyDetectionKernelSize, 
					true);

				/// Using Canny's output as a mask, we display our result
				cv::Mat dst;
				dst.create(_frameProcessed.size(), _frameProcessed.type());
				dst = cv::Scalar::all(0);

				_frameProcessed.copyTo(dst, cannyEdges);
				cv::cvtColor(cannyEdges, cannyEdges, cv::COLOR_GRAY2RGBA);

				// combine processed image with canny image
				_frameProcessed += dst;

				// TODO... add ui components to... blend src image with canny image
				//cv::addWeighted(dst, 0.5, cannyEdges, 0.5, 0.0, _frameProcessed);
			}
			break;

			// ....................................................
		case ProcessingFunction::SOBEL_EDGE_DETECTION:
			// ....................................................
			if (sobelEnable)
			{
				// sobel works for rgba

				int borderType = cv::BorderTypes::BORDER_DEFAULT;

				cv::cvtColor(_frameProcessed.clone(), sobelTotalGradient, cv::COLOR_BGR2GRAY);

				// reduce noise
				/*
				cv::GaussianBlur(sobelTotalGradient, sobelTotalGradient, 
					cv::Size(sobelKernelSize, sobelKernelSize), 
					0, 0, borderType);
				*/

				// TODO:: change ... SOBEL_DDEPTH_VALUE ... depending on image depth value otherwise will crash

				/// Gradient X
				cv::Sobel(
					sobelTotalGradient,
					sobelGradX,
					SOBEL_DDEPTH_VALUE,
					1,
					0,
					sobelKernelSize,
					sobelScaleValue,
					sobelDeltaValue,
					borderType);

				/// Gradient Y
				cv::Sobel(
					sobelTotalGradient,
					sobelGradY,
					SOBEL_DDEPTH_VALUE,
					0,
					1,
					sobelKernelSize,
					sobelScaleValue,
					sobelDeltaValue,
					borderType);

				cv::convertScaleAbs(sobelGradX, sobelAbsGradX);
				cv::convertScaleAbs(sobelGradY, sobelAbsGradY);

				/// Total Gradient (approximate)
				cv::addWeighted(sobelAbsGradX, 0.5, sobelAbsGradY, 0.5, 0, sobelTotalGradient);

				cv::cvtColor(sobelTotalGradient, sobelTotalGradient, cv::COLOR_GRAY2RGBA);

				//sobelTotalGradient.copyTo(_frameProcessed);
				_frameProcessed += sobelTotalGradient;
			}
			break;

			// ....................................................
		case ProcessingFunction::ADAPTIVE_THRESHOLD:
			// ....................................................
			if (adaptiveThresholdEnable)
			{
				int maxValue = 255;
				const int c = -2; // constant subtracted from the mean or weighted mean
				cv::Mat inputImage;

				if (isGrayscale())
				{
					inputImage = _frameProcessed.clone();
				}
				else
				{
					cv::cvtColor(_frameProcessed.clone(), inputImage, cv::COLOR_BGR2GRAY);
				}

				cv::adaptiveThreshold(
					inputImage,
					_frameProcessed,
					maxValue,
					adaptiveMethod,
					adaptiveThresholdType,
					adaptiveThresholdBlockSize,
					c);
			}
			break;

			// ....................................................
		case ProcessingFunction::BINARY_THRESHOLD:
			// ....................................................
			if (binaryThresholdEnable)
			{
				int maxValue = 255;
				int type = cv::ThresholdTypes::THRESH_BINARY;
				cv::threshold(_frameProcessed, _frameProcessed, binaryThreshold, maxValue, type);
			}
			break;

			// ....................................................
		case ProcessingFunction::CONTOURS:
			// ....................................................
			if (contoursEnable)
			{
				int type = cv::ThresholdTypes::THRESH_BINARY;

				cv::Mat inputImage;
				cv::Mat cannyEdges;
				std::vector<std::vector<cv::Point> > contours;
				std::vector<cv::Vec4i> hierarchy;
				cv::RNG rng(12345);

				if (isGrayscale())
				{
					inputImage = _frameProcessed.clone();
				}
				else
				{
					cv::cvtColor(_frameProcessed.clone(), inputImage, cv::COLOR_BGR2GRAY);
				}

				/// Detect edges using canny
				cv::Canny(inputImage, cannyEdges, contoursThreshold, contoursThreshold * 2, 3);

				findContours(
					cannyEdges,
					contours,
					hierarchy,
					cv::RetrievalModes::RETR_TREE,
					cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE,
					cv::Point(0, 0));

				/// Draw contours
				cv::Mat drawing = cv::Mat::zeros(cannyEdges.size(), CV_8UC3);
				for (size_t i = 0; i< contours.size(); i++)
				{
					cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
					drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, cv::Point());
				}

				_frameProcessed += drawing;
			}
			break;

			// ....................................................
		case ProcessingFunction::CONVEX_HULL:
			// ....................................................
			if (convexHullEnable)
			{
				// !! IMPORTANT !! need to be in release mode for visual studio

				cv::Mat inputImage;
				cv::Mat binaryImage;
				std::vector<std::vector<cv::Point>> contours;
				std::vector<cv::Vec4i> hierarchy;
				int maxValue = 255;
				int type = cv::ThresholdTypes::THRESH_BINARY;
				cv::RNG rng(12345);

				if (isGrayscale())
				{
					inputImage = _frameProcessed.clone();
				}
				else
				{
					cv::cvtColor(_frameProcessed.clone(), inputImage, cv::COLOR_BGR2GRAY);
				}

				/// Detect edges using Threshold
				cv::threshold(inputImage, binaryImage, convexHullThreshold, maxValue, type);
				
				cv::findContours(
					binaryImage, 
					contours, 
					hierarchy, 
					cv::RetrievalModes::RETR_TREE,
					cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE, 
					cv::Point(0, 0));
				
				/// Find the convex hull object for each contour
				std::vector<std::vector<cv::Point> >hull(contours.size());
				for (size_t i = 0; i < contours.size(); i++)
				{
					convexHull(cv::Mat(contours[i]), hull[i], false);
				}

				/// Draw contours + hull results
				cv::Mat drawing = cv::Mat::zeros(binaryImage.size(), CV_8UC3);
				for (size_t i = 0; i< contours.size(); i++)
				{
					cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
					drawContours(drawing, contours, (int)i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
					drawContours(drawing, hull, (int)i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
				}

				_frameProcessed += drawing;
			}
			break;

			// ....................................................
		case ProcessingFunction::CORNER_DETECTION:
			// ....................................................
			if (cornerDetectionEnable)
			{
				cv::Mat inputImage;

				if (isGrayscale())
				{
					inputImage = _frameProcessed.clone();
				}
				else
				{
					cv::cvtColor(_frameProcessed.clone(), inputImage, cv::COLOR_BGR2GRAY);
				}

				/// Parameters for Shi-Tomasi algorithm
				std::vector<cv::Point2f> corners;
				bool useHarrisDetector = false;
				double k = 0.04;
				cv::RNG rng(12345);

				/// Copy the source image
				cv::Mat copy;
				copy = _frameProcessed.clone();

				/// Apply corner detection
				goodFeaturesToTrack(
					inputImage,
					corners,
					cornerDetectionMaxCorners,
					cornerDetectionQualityLevel,
					cornerDetectionMinDistance,
					cv::Mat(),
					cornerDetectionBlockSize,
					useHarrisDetector,
					k);

				/// Draw corners detected
				int r = 4;
				for (size_t i = 0; i < corners.size(); i++)
				{
					circle(copy, corners[i], r, cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), -1, 8, 0);
				}

				_frameProcessed += copy;

				// TODO:: harris corner detector
				/*
				cv::Mat myHarris_copy;
				cv::Mat Mc;
				cv::Mat myHarris_dst;

				int max_qualityLevel = 100;
				double myHarris_minVal; double myHarris_maxVal;
				//cv::Mat cornerShiTomasi;

				/// Set some parameters
				int blockSize = 3; int apertureSize = 3;

				/// My Harris matrix -- Using cornerEigenValsAndVecs
				myHarris_dst = cv::Mat::zeros(_frameOriginalGrayscale.size(), CV_32FC(6));
				Mc = cv::Mat::zeros(_frameOriginalGrayscale.size(), CV_32FC1);

				cornerEigenValsAndVecs(_frameOriginalGrayscale, myHarris_dst, blockSize, apertureSize, cv::BORDER_DEFAULT);

				// calculate Mc
				for (int j = 0; j < _frameOriginalGrayscale.rows; j++)
				{
					for (int i = 0; i < _frameOriginalGrayscale.cols; i++)
					{
						float lambda_1 = myHarris_dst.at<cv::Vec6f>(j, i)[0];
						float lambda_2 = myHarris_dst.at<cv::Vec6f>(j, i)[1];
						Mc.at<float>(j, i) = lambda_1*lambda_2 - 0.04f*pow((lambda_1 + lambda_2), 2);
					}
				}

				minMaxLoc(Mc, &myHarris_minVal, &myHarris_maxVal, 0, 0, cv::Mat());

				cv::RNG rng(12345);
				myHarris_copy = _frameProcessed.clone();

				if (cornerDetectorValue < 1) { cornerDetectorValue = 1; }

				for (int j = 0; j < _frameOriginalGrayscale.rows; j++)
				{
					for (int i = 0; i < _frameOriginalGrayscale.cols; i++)
					{
						if (Mc.at<float>(j, i) > myHarris_minVal + (myHarris_maxVal - myHarris_minVal)*cornerDetectorValue / max_qualityLevel)
						{
							cv::circle(myHarris_copy, cv::Point(i, j), 4, cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), -1, 8, 0);
						}
					}
				}

				_frameProcessed += myHarris_copy;
				*/
			}
			break;

			// ....................................................
		case ProcessingFunction::HOUGH_CIRCLES:
			// ....................................................
			if (houghCirclesEnable)
			{
				cv::Mat inputImage;

				if (isGrayscale())
				{
					inputImage = _frameProcessed.clone();
				}
				else
				{
					cv::cvtColor(_frameProcessed.clone(), inputImage, cv::COLOR_BGR2GRAY);
				}

				// will hold the results of the detection
				std::vector<cv::Vec3f> circles;

				// runs the actual detection
				cv::HoughCircles(
					inputImage, 
					circles, 
					cv::HoughModes::HOUGH_GRADIENT, 
					1, 
					inputImage.rows / 8,
					houghCirclesCannyThreshold,
					houghCirclesAccumulatorThreshold,
					houghCirclesMinRadius,
					houghCirclesMaxRadius);

				// clone the colour, input image for displaying purposes
				cv::Mat display = _frameProcessed.clone();
				for (size_t i = 0; i < circles.size(); i++)
				{
					cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
					int radius = cvRound(circles[i][2]);
					// circle center
					cv::circle(display, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
					// circle outline
					cv::circle(display, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
				}

				_frameProcessed += display;
			}
			break;

			// ....................................................
		case ProcessingFunction::HOUGH_LINES:
			// ....................................................
			if (houghLinesEnable)
			{
				cv::Mat inputImage;

				if (isGrayscale())
				{
					inputImage = _frameProcessed.clone();
				}
				else
				{
					cv::cvtColor(_frameProcessed.clone(), inputImage, cv::COLOR_BGR2GRAY);
				}

				/// Apply Canny edge detector
				cv::Mat edges;
				cv::Canny(inputImage, edges, 50, 200, 3, true);

				if (useStandardHoughLines)
				{
					cv::Mat standardHough;
					std::vector<cv::Vec2f> sLines;
					cv::cvtColor(edges, standardHough, cv::COLOR_GRAY2BGR);

					/// 1. Use Standard Hough Transform
					cv::HoughLines(edges, sLines, 1, CV_PI / 180, houghLinesThreshold, 0, 0);

					/// Show the result
					for (size_t i = 0; i < sLines.size(); i++)
					{
						float r = sLines[i][0], t = sLines[i][1];
						double cos_t = cos(t), sin_t = sin(t);
						double x0 = r*cos_t, y0 = r*sin_t;
						double alpha = 1000;

						cv::Point pt1(cvRound(x0 + alpha*(-sin_t)), cvRound(y0 + alpha*cos_t));
						cv::Point pt2(cvRound(x0 - alpha*(-sin_t)), cvRound(y0 - alpha*cos_t));
						cv::line(standardHough, pt1, pt2, cv::Scalar(255, 0, 0), 3, cv::LINE_AA);
					}

					_frameProcessed += standardHough;
				}
				else
				{
					cv::Mat probabilisticHough;
					cv::cvtColor(edges, probabilisticHough, cv::COLOR_GRAY2BGR);
					std::vector<cv::Vec4i> pLines;

					/// 2. Use Probabilistic Hough Transform
					cv::HoughLinesP(edges, pLines, 1, CV_PI / 180, houghLinesThreshold, houghLinesMinLineLength, houghLinesMaxLineGap);

					/// Show the result
					for (size_t i = 0; i < pLines.size(); i++)
					{
						cv::Vec4i l = pLines[i];
						cv::line(probabilisticHough, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 3, cv::LINE_AA);
					}

					_frameProcessed += probabilisticHough;
				}
			}
			break;

			// ....................................................
		case ProcessingFunction::LAPLACIAN:
			// ....................................................
			if (laplacianEnable)
			{
				cv::Mat laplacianImage;
				int ddepthValue;

				if (isGrayscale())
				{
					laplacianImage = _frameProcessed.clone();
					ddepthValue = _frameProcessed.depth();
				}
				else
				{
					cv::cvtColor(_frameProcessed.clone(), laplacianImage, cv::COLOR_BGR2GRAY);
					ddepthValue = CV_16U;
				}
				
				// blur image to reduce noise

				cv::Laplacian(
					laplacianImage,
					_frameProcessed,
					ddepthValue,
					laplacianKernelSize,
					1,
					0,
					cv::BorderTypes::BORDER_DEFAULT);

				// TODO:: laplacian ... convertScaleAbs(dst, abs_dst);
			}
			break;

			// ....................................................
		case ProcessingFunction::EQUALIZE_HISTOGRAM:
			// ....................................................
			if (equalizeHistogramEnable)
			{
				if (isGrayscale())
				{
					cv::equalizeHist(_frameProcessed, _frameProcessed);
				}
			}
			break;

			// ....................................................
		case ProcessingFunction::OBJECT_DETECTION:
			// ....................................................
			if (objectDetectionEnable)
			{
				// TODO:: objectDetection
			}
			break;

		default:
			break;
		}
	}

	// ============================================================
	// ============================================================
	// ============================================================

	if (gridEnable)
	{
		//GridLines(_frameProcessed);

		int gridWidth = 200;
		int gridHeight = 200;
		int gridLineThickness = 1;
		int gridLineType = isGrayscale() ? cv::LineTypes::LINE_8 : cv::LineTypes::LINE_AA;
		cv::Scalar gridLineColor(110, 255, 50);
		//cv::Point gridStartPoint(_frameProcessed.cols / 2 - gridWidth/2, _frameProcessed.rows / 2 - gridHeight/2);
		//cv::Point gridEndPoint(_frameProcessed.cols / 2 + gridWidth/2, _frameProcessed.rows / 2 + gridHeight/2);

		GridLines(_frameProcessed,
			gridLineThickness,
			gridLineType,
			gridLineColor,
			gridLineSpacing);
	}

	// TODO:: ColorMap()
	//cv::applyColorMap(_frameProcessed, _frameProcessed, cv::COLORMAP_JET);
}

void WorkerOpenCV::receiveSetup(const int device)
{
	checkIfDeviceAlreadyOpened(device);
	if (!cap->isOpened())
	{
		status = false;
		return;
	}

	status = true;
}

void WorkerOpenCV::receiveToggleStream()
{
	toggleStream = !toggleStream;
}

void WorkerOpenCV::receiveToggleRecord()
{
	toggleRecord = !toggleRecord;
}

void WorkerOpenCV::receiveRotateCounterClockwise()
{
	switch (rotationValue)
	{
	case 0:
		rotationValue = 270;
		break;

	case 90:
		rotationValue = 0;
		break;

	case 180:
		rotationValue = 90;
		break;

	case 270:
		rotationValue = 180;
		break;

	default:
		break;
	}
}

void WorkerOpenCV::receiveRotateClockwise()
{
	switch (rotationValue)
	{
	case 0:
		rotationValue = 90;
		break;

	case 90:
		rotationValue = 180;
		break;

	case 180:
		rotationValue = 270;
		break;

	case 270:
		rotationValue = 0;
		break;

	default:
		break;
	}
}

void WorkerOpenCV::receiveFlipHorizontal()
{
	flipHorizontal = !flipHorizontal;
}

void WorkerOpenCV::receiveFlipVertical()
{
	flipVertical = !flipVertical;
}

void WorkerOpenCV::receiveConsoleInput(std::string consoleInput)
{
	// TODO:: receiveConsoleInput() ... tokenize string

	// TODO:: receiveConsoleInput() ... isValid command?
	// TODO:: receiveConsoleInput() ... interpret command
}

// ============================================================
// ============================================================
// ============================================================

void WorkerOpenCV::receiveEnableImageFormatAlpha8()
{
	qImageFormat = QImage::Format::Format_Alpha8;
}

void WorkerOpenCV::receiveEnableImageFormatARGB32()
{
	qImageFormat = QImage::Format::Format_ARGB32;
}

void WorkerOpenCV::receiveEnableImageFormatARGB32Premultiplied()
{
	qImageFormat = QImage::Format::Format_ARGB32_Premultiplied;
}

void WorkerOpenCV::receiveEnableImageFormatBGR30()
{
	qImageFormat = QImage::Format::Format_BGR30;
}

void WorkerOpenCV::receiveEnableImageFormatIndexed8()
{
	qImageFormat = QImage::Format::Format_Indexed8;
}

void WorkerOpenCV::receiveEnableImageFormatMonoLSB()
{
	qImageFormat = QImage::Format::Format_MonoLSB;
}

void WorkerOpenCV::receiveEnableImageFormatRGB16()
{
	qImageFormat = QImage::Format::Format_RGB16;
}

void WorkerOpenCV::receiveEnableImageFormatRGB30()
{
	qImageFormat = QImage::Format::Format_RGB30;
}

void WorkerOpenCV::receiveEnableImageFormatRGB32()
{
	qImageFormat = QImage::Format::Format_RGB32;
}

void WorkerOpenCV::receiveEnableImageFormatRGB444()
{
	qImageFormat = QImage::Format::Format_RGB444;
}

void WorkerOpenCV::receiveEnableImageFormatRGB555()
{
	qImageFormat = QImage::Format::Format_RGB555;
}

void WorkerOpenCV::receiveEnableImageFormatRGB666()
{
	qImageFormat = QImage::Format::Format_RGB666;
}

void WorkerOpenCV::receiveEnableImageFormatRGB888()
{
	qImageFormat = QImage::Format::Format_RGB888;
}

void WorkerOpenCV::receiveEnableImageFormatRGBA8888()
{
	qImageFormat = QImage::Format::Format_RGBA8888;
}

void WorkerOpenCV::receiveEnableImageFormatRGBA8888Premultiplied()
{
	qImageFormat = QImage::Format::Format_RGBA8888_Premultiplied;
}

void WorkerOpenCV::receiveEnableImageFormatRGBX8888()
{
	qImageFormat = QImage::Format::Format_RGBX8888;
}

// ============================================================

void WorkerOpenCV::receiveEnableColorCodeBGR555()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2BGR555;
}

void WorkerOpenCV::receiveEnableColorCodeBGRA()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2BGRA;
}

void WorkerOpenCV::receiveEnableColorCodeGRAY()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2GRAY;
}

void WorkerOpenCV::receiveEnableColorCodeHLS()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2HLS;
}

void WorkerOpenCV::receiveEnableColorCodeHSV()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2HSV;
}

void WorkerOpenCV::receiveEnableColorCodeLab()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2Lab;
}

void WorkerOpenCV::receiveEnableColorCodeLuv()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2Luv;
}

void WorkerOpenCV::receiveEnableColorCodeRGB()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2RGB;
}

void WorkerOpenCV::receiveEnableColorCodeRGBA()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2RGBA;
}

void WorkerOpenCV::receiveEnableColorCodeXYZ()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2XYZ;
}

void WorkerOpenCV::receiveEnableColorCodeYCrCb()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2YCrCb;
}

void WorkerOpenCV::receiveEnableColorCodeYUV()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2YUV;
}

void WorkerOpenCV::receiveEnableColorCodeYUV_I420()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2YUV_I420;
}

void WorkerOpenCV::receiveEnableColorCodeYUV_IYUV()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2YUV_IYUV;
}

void WorkerOpenCV::receiveEnableColorCodeYUV_YV12()
{
	cvColorCode = cv::ColorConversionCodes::COLOR_BGR2YUV_YV12;
}

// ============================================================

void WorkerOpenCV::receiveEnableEqualizeHistogram()
{
	equalizeHistogramEnable = !equalizeHistogramEnable;
}

// ============================================================

void WorkerOpenCV::receiveEnableAdaptiveThreshold()
{
	adaptiveThresholdEnable = !adaptiveThresholdEnable;
}

void WorkerOpenCV::receiveAdaptiveThresholdBlockSize(int blockSize)
{
	adaptiveThresholdBlockSize = blockSize;
}

// ============================================================

void WorkerOpenCV::receiveEnableBinaryThreshold()
{
	binaryThresholdEnable = !binaryThresholdEnable;
}

void WorkerOpenCV::receiveBinaryThreshold(int threshold)
{
	binaryThreshold = threshold;
}

// ============================================================

void WorkerOpenCV::receiveEnableContours()
{
	contoursEnable = !contoursEnable;
}

void WorkerOpenCV::receiveContoursThreshold(int value)
{
	contoursThreshold = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableConvexHull()
{
	convexHullEnable = !convexHullEnable;
}

void WorkerOpenCV::receiveConvexHullThreshold(int value)
{
	convexHullThreshold = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableCornerDetection()
{
	cornerDetectionEnable = !cornerDetectionEnable;
}

void WorkerOpenCV::receiveCornerDetectionMaxCorners(int value)
{
	cornerDetectionMaxCorners = value;
}

void WorkerOpenCV::receiveCornerDetectionQualityLevel(double value)
{
	cornerDetectionQualityLevel = value;
}

void WorkerOpenCV::receiveCornerDetectionMinDistance(int value)
{
	cornerDetectionMinDistance = value;
}

void WorkerOpenCV::receiveCornerDetectionBlockSize(int value)
{
	cornerDetectionBlockSize = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableHoughCircles()
{
	houghCirclesEnable = !houghCirclesEnable;
}

void WorkerOpenCV::receiveHoughCirclesCannyThreshold(int value)
{
	houghCirclesCannyThreshold = value;
}

void WorkerOpenCV::receiveHoughCirclesAccumulatorThreshold(int value)
{
	houghCirclesAccumulatorThreshold = value;
}

void WorkerOpenCV::receiveHoughCirclesMinRadius(int value)
{
	houghCirclesMinRadius = value;
}

void WorkerOpenCV::receiveHoughCirclesMaxRadius(int value)
{
	houghCirclesMaxRadius = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableHoughLines()
{
	houghLinesEnable = !houghLinesEnable;
}

void WorkerOpenCV::receiveHoughLinesThreshold(int value)
{
	houghLinesThreshold = value;
}

void WorkerOpenCV::receiveHoughLinesMinLineLength(int value)
{
	houghLinesMinLineLength = value;
}

void WorkerOpenCV::receiveHoughLinesMaxLineGap(int value)
{
	houghLinesMaxLineGap = value;
}

void WorkerOpenCV::receiveHoughLinesType(int index)
{
	useStandardHoughLines = (index == 0) ? true : false;
}

// ============================================================

void WorkerOpenCV::receiveEnableLaplacian()
{
	laplacianEnable = !laplacianEnable;
}

void WorkerOpenCV::receiveLaplacianKernelSize(int value)
{
	laplacianKernelSize = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableCannyDetection()
{
	cannyDetectionEnable = !cannyDetectionEnable;
}

void WorkerOpenCV::receiveCannyDetectionThreshold(int value)
{
	cannyDetectionThreshold = value;
}

void WorkerOpenCV::receiveCannyDetectionRatio(int value)
{
	cannyDetectionRatio = value;
}

void WorkerOpenCV::receiveCannyDetectionKernelSize(int value)
{
	cannyDetectionKernelSize = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableSobel()
{
	sobelEnable = !sobelEnable;
}

void WorkerOpenCV::receiveSobelKernelSize(int value)
{
	sobelKernelSize = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableContrast()
{
	contrastEnable = !contrastEnable;
}

void WorkerOpenCV::receiveContrastValue(int value)
{
	contrastValue = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableBrightness()
{
	brightnessEnable = !brightnessEnable;
}

void WorkerOpenCV::receiveBrightnessValue(int value)
{
	brightnessValue = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableGamma()
{
	gammaEnable = !gammaEnable;
}

void WorkerOpenCV::receiveGammaValue(int value)
{
	gammaValue = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableHomogeneousBlur()
{
	homogeneousBlurEnable = !homogeneousBlurEnable;
}

void WorkerOpenCV::receiveHomogeneousBlurKernelSize(int value)
{
	homogeneousBlurKernelSize = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableGaussianBlur()
{
	gaussianBlurEnable = !gaussianBlurEnable;
}

void WorkerOpenCV::receiveGaussianBlurKernelWidth(int value)
{
	gaussianBlurKernelWidth = value;
}

void WorkerOpenCV::receiveGaussianBlurKernelHeight(int value)
{
	gaussianBlurKernelHeight = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableMedianBlur()
{
	medianBlurEnable = !medianBlurEnable;
}

void WorkerOpenCV::receiveMedianBlurKernelSize(int value)
{
	medianBlurKernelSize = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableBilateralFilter()
{
	bilateralFilterEnable = !bilateralFilterEnable;
}

void WorkerOpenCV::receiveBilateralFilterValue(int value)
{
	bilateralFilterValue = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableObjectDetection()
{
	objectDetectionEnable = !objectDetectionEnable;
}

// ============================================================

void WorkerOpenCV::receiveEnableGrid()
{
	gridEnable = !gridEnable;
}

void WorkerOpenCV::receiveGridLineSpacingValue(int value)
{
	gridLineSpacing = value;
}

// ------------------------------------------------------------
// TAB - PROCESSING 2
// ------------------------------------------------------------

void WorkerOpenCV::receiveEnableErosion()
{
	erosionEnable = !erosionEnable;
}

void WorkerOpenCV::receiveErosionValue(int value)
{
	erosionValue = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableDilation()
{
	dilationEnable = !dilationEnable;
}

void WorkerOpenCV::receiveDilationValue(int value)
{
	dilationValue = value;
}

// ============================================================

void WorkerOpenCV::receiveEnableNormalizedConvolutionFilter()
{
	normalizedConvolutionFilterEnable = !normalizedConvolutionFilterEnable;
}

void WorkerOpenCV::receiveEnableRecursiveFilter()
{
	recursiveFilterEnable = !recursiveFilterEnable;
}

void WorkerOpenCV::receiveEnableDetailEnhancement()
{
	detailEnhancementEnable = !detailEnhancementEnable;
}

void WorkerOpenCV::receiveEnablePencilSketch()
{
	pencilSketchEnable = !pencilSketchEnable;
}

void WorkerOpenCV::receiveEnableColorPencilDrawing()
{
	colorPencilDrawingEnable = !colorPencilDrawingEnable;
}

void WorkerOpenCV::receiveEnableStylization()
{
	stylizationEnable = !stylizationEnable;
}

// ============================================================
// ============================================================
// ============================================================

void WorkerOpenCV::GridLines(
	cv::Mat img, 
	int lineThickness,
	int lineType,
	cv::Scalar lineColor,
	int lineSpacing,
	const cv::Point* start, 
	const cv::Point* end)
{
	if (start == NULL && end == NULL)
	{
		for (int r = 0; r < img.rows; r += lineSpacing)
		{
			cv::line(img,
				cv::Point(0, r),
				cv::Point(img.cols, r),
				lineColor,
				lineThickness,
				lineType);
		}

		for (int c = 0; c < img.cols; c += lineSpacing)
		{
			cv::line(img,
				cv::Point(c, 0),
				cv::Point(c, img.rows),
				lineColor,
				lineThickness,
				lineType);
		}
	}
	else
	{
		// start(x,y) < end(x,y)
		// start(x,y) >= (0,0)
		// end(x,y) < image(max_x, max_y)
		if ((start->x < end->x && start->y < end->y)
			&& (start->x >= 0 && start->y >= 0)
			&& (end->y < img.rows && end->x < img.cols))
		{
			for (int r = start->y; r <= end->y; r += lineSpacing)
			{
				cv::line(img,
					cv::Point(start->x, r),
					cv::Point(end->x, r),
					lineColor,
					lineThickness,
					lineType);
			}

			for (int c = start->x; c <= end->x; c += lineSpacing)
			{
				cv::line(img,
					cv::Point(c, start->y),
					cv::Point(c, end->y),
					lineColor,
					lineThickness,
					lineType);
			}
		}
	}
}

bool WorkerOpenCV::isGrayscale()
{
	return (cvColorCode == cv::ColorConversionCodes::COLOR_BGR2GRAY);
}