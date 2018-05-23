#include "qtopencvwidget.hpp"

QtOpencvWidget::QtOpencvWidget(QWidget * parent) : 
	QWidget(parent)
{
	ui.setupUi(this);
	ui.labelView->setScaledContents(true);

	ui.textEditConsoleOutput->setText(QDateTime::currentDateTime().toString());
	ui.textEditConsoleOutput->append("=================================");
	
	/*
	for each (QString qstr in QStyleFactory::keys())
		ui.textEditConsoleOutput->append(qstr);
	*/

	QApplication::setStyle(QStyleFactory::create("Fusion"));
	//QApplication::setPalette(QApplication::style()->standardPalette());
	
	setup();
}

QtOpencvWidget::~QtOpencvWidget()
{
	thread->quit();
	while (!thread->isFinished());
	delete thread;
}

void QtOpencvWidget::setup()
{
	thread = new QThread();
	
	// if you create the worker on the stack (i.e. ...
	// WorkerOpenCV worker;
	// ...) then the moment you exit the scope of this 
	// function it would no longer exist
	worker = new WorkerOpenCV();

	workerTrigger = new QTimer();
	workerTrigger->setInterval(1); // in milliseconds

	connect(this, SIGNAL(sendSetup(int)), worker, SLOT(receiveSetup(int)));

	connect(workerTrigger, SIGNAL(timeout()), worker, SLOT(receiveGrabFrame()));
	connect(worker, SIGNAL(sendFrame(QImage)), this, SLOT(receiveFrame(QImage)));

	// ...........................
	// PushButton - Play
	// ...........................
	connect(this, SIGNAL(sendToggleStream()), worker, SLOT(receiveToggleStream()));
	connect(ui.pushButtonPlay, SIGNAL(clicked(bool)), this, SLOT(receiveToggleStream()));

	// ============================================================

	// ...........................
	// PushButton - Record
	// ...........................
	connect(this, SIGNAL(sendToggleRecord()), worker, SLOT(receiveToggleRecord()));
	connect(ui.pushButtonRecord, SIGNAL(clicked(bool)), this, SLOT(receiveToggleRecord()));

	// ...........................
	// PushButton - Rotate Counter Clockwise
	// ...........................
	connect(this, SIGNAL(sendRotateCounterClockwise()), worker, SLOT(receiveRotateCounterClockwise()));
	connect(ui.pushButtonRotateCounterClockwise, SIGNAL(clicked(bool)), this, SLOT(receiveRotateCounterClockwise()));

	// ...........................
	// PushButton - Rotate Clockwise
	// ...........................
	connect(this, SIGNAL(sendRotateClockwise()), worker, SLOT(receiveRotateClockwise()));
	connect(ui.pushButtonRotateClockwise, SIGNAL(clicked(bool)), this, SLOT(receiveRotateClockwise()));

	// ...........................
	// PushButton - Flip Horizontal
	// ...........................
	connect(this, SIGNAL(sendFlipHorizontal()), worker, SLOT(receiveFlipHorizontal()));
	connect(ui.pushButtonFlipHorizontal, SIGNAL(clicked(bool)), this, SLOT(receiveFlipHorizontal()));

	// ...........................
	// PushButton - Flip Vertical
	// ...........................
	connect(this, SIGNAL(sendFlipVertical()), worker, SLOT(receiveFlipVertical()));
	connect(ui.pushButtonFlipVertical, SIGNAL(clicked(bool)), this, SLOT(receiveFlipVertical()));

	// ------------------------------------------------------------
	// TAB - COLOR FORMAT
	// ------------------------------------------------------------

	// ...........................
	// QImage::Format
	// ...........................
	connect(ui.radioButtonAlpha8, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatAlpha8()));
	connect(ui.radioButtonARGB32, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatARGB32()));
	connect(ui.radioButtonARGB32_Premultiplied, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatARGB32Premultiplied()));
	connect(ui.radioButtonBGR30, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatBGR30()));
	connect(ui.radioButtonIndexed8, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatIndexed8()));
	connect(ui.radioButtonMonoLSB, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatMonoLSB()));
	connect(ui.radioButtonRGB16, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGB16()));
	connect(ui.radioButtonRGB30, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGB30()));
	connect(ui.radioButtonRGB32, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGB32()));
	connect(ui.radioButtonRGB444, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGB444()));
	connect(ui.radioButtonRGB555, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGB555()));
	connect(ui.radioButtonRGB666, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGB666()));
	connect(ui.radioButtonRGB888, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGB888()));
	connect(ui.radioButtonRGBA8888, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGBA8888()));
	connect(ui.radioButtonRGBA8888_Premultiplied, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGBA8888Premultiplied()));
	connect(ui.radioButtonRGBX8888, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableImageFormatRGBX8888()));

	// ...........................
	// cv::cvtColor
	// ...........................
	connect(ui.radioButtonBGR555, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeBGR555()));
	connect(ui.radioButtonBGRA, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeBGRA()));
	connect(ui.radioButtonGRAY, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeGRAY()));
	connect(ui.radioButtonHLS, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeHLS()));
	connect(ui.radioButtonHSV, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeHSV()));
	connect(ui.radioButtonLab, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeLab()));
	connect(ui.radioButtonLuv, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeLuv()));
	connect(ui.radioButtonRGB, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeRGB()));
	connect(ui.radioButtonRGBA, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeRGBA()));
	connect(ui.radioButtonXYZ, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeXYZ()));
	connect(ui.radioButtonYCrCb, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeYCrCb()));
	connect(ui.radioButtonYUV, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeYUV()));
	connect(ui.radioButtonYUV_I420, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeYUV_I420()));
	connect(ui.radioButtonYUV_IYUV, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeYUV_IYUV()));
	connect(ui.radioButtonYUV_YV12, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorCodeYUV_YV12()));

	// ...........................
	// Image Correction
	// ...........................
	connect(ui.checkBoxEnableContrast, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableContrast()));
	connect(ui.horizontalSliderContrast, SIGNAL(valueChanged(int)), worker, SLOT(receiveContrastValue(int)));
	connect(ui.checkBoxEnableContrast, SIGNAL(toggled(bool)), ui.horizontalSliderContrast, SLOT(setEnabled(bool)));

	connect(ui.checkBoxEnableBrightness, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableBrightness()));
	connect(ui.horizontalSliderBrightness, SIGNAL(valueChanged(int)), worker, SLOT(receiveBrightnessValue(int)));
	connect(ui.checkBoxEnableBrightness, SIGNAL(toggled(bool)), ui.horizontalSliderBrightness, SLOT(setEnabled(bool)));

	connect(ui.checkBoxEnableGamma, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableGamma()));
	connect(ui.horizontalSliderGamma, SIGNAL(valueChanged(int)), worker, SLOT(receiveGammaValue(int)));
	connect(ui.checkBoxEnableGamma, SIGNAL(toggled(bool)), ui.horizontalSliderGamma, SLOT(setEnabled(bool)));

	// ...........................
	// Equalize Histogram
	// ...........................
	connect(ui.checkBoxEnableEqualizeHistogram, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableEqualizeHistogram()));

	// ...........................
	// Adaptive Threshold
	// ...........................
	connect(ui.checkBoxEnableAdaptiveThreshold, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableAdaptiveThreshold()));
	connect(ui.spinBoxAdaptiveThresholdBlockSize, SIGNAL(valueChanged(int)), worker, SLOT(receiveAdaptiveThresholdBlockSize(int)));
	connect(ui.checkBoxEnableAdaptiveThreshold, SIGNAL(toggled(bool)), ui.spinBoxAdaptiveThresholdBlockSize, SLOT(setEnabled(bool)));

	// ...........................
	// Binary Threshold
	// ...........................
	connect(ui.checkBoxEnableBinaryThreshold, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableBinaryThreshold()));
	connect(ui.spinBoxBinaryThreshold, SIGNAL(valueChanged(int)), worker, SLOT(receiveBinaryThreshold(int)));
	connect(ui.checkBoxEnableBinaryThreshold, SIGNAL(toggled(bool)), ui.spinBoxBinaryThreshold, SLOT(setEnabled(bool)));

	// ------------------------------------------------------------
	// TAB - PROCESSING 1
	// ------------------------------------------------------------

	// ...........................
	// Contours
	// ...........................
	connect(ui.checkBoxEnableContours, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableContours()));
	connect(ui.horizontalSliderContoursThreshold, SIGNAL(valueChanged(int)), worker, SLOT(receiveContoursThreshold(int)));
	connect(ui.checkBoxEnableContours, SIGNAL(toggled(bool)), ui.horizontalSliderContoursThreshold, SLOT(setEnabled(bool)));

	// ...........................
	// Convex Hull
	// ...........................
	connect(ui.checkBoxEnableConvexHull, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableConvexHull()));
	connect(ui.horizontalSliderConvexHullThreshold, SIGNAL(valueChanged(int)), worker, SLOT(receiveConvexHullThreshold(int)));
	connect(ui.checkBoxEnableConvexHull, SIGNAL(toggled(bool)), ui.horizontalSliderConvexHullThreshold, SLOT(setEnabled(bool)));

	// ...........................
	// Corner Detection
	// ...........................
	connect(ui.checkBoxEnableCornerDetection, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableCornerDetection()));
	connect(ui.horizontalSliderCornerDetectionMaxCorners, SIGNAL(valueChanged(int)), worker, SLOT(receiveCornerDetectionMaxCorners(int)));
	connect(ui.doubleSpinBoxCornerDetectionQualityLevel, SIGNAL(valueChanged(double)), worker, SLOT(receiveCornerDetectionQualityLevel(double)));
	connect(ui.spinBoxCornerDetectionMinDistance, SIGNAL(valueChanged(int)), worker, SLOT(receiveCornerDetectionMinDistance(int)));
	connect(ui.spinBoxCornerDetectionBlockSize, SIGNAL(valueChanged(int)), worker, SLOT(receiveCornerDetectionBlockSize(int)));

	connect(ui.checkBoxEnableCornerDetection, SIGNAL(toggled(bool)), ui.horizontalSliderCornerDetectionMaxCorners, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableCornerDetection, SIGNAL(toggled(bool)), ui.doubleSpinBoxCornerDetectionQualityLevel, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableCornerDetection, SIGNAL(toggled(bool)), ui.spinBoxCornerDetectionMinDistance, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableCornerDetection, SIGNAL(toggled(bool)), ui.spinBoxCornerDetectionBlockSize, SLOT(setEnabled(bool)));

	// ...........................
	// Hough Circles
	// ...........................
	connect(ui.checkBoxEnableHoughCircles, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableHoughCircles()));
	connect(ui.horizontalSliderHoughCirclesCannyThreshold, SIGNAL(valueChanged(int)), worker, SLOT(receiveHoughCirclesCannyThreshold(int)));
	connect(ui.horizontalSliderHoughCirclesAccumulatorThreshold, SIGNAL(valueChanged(int)), worker, SLOT(receiveHoughCirclesAccumulatorThreshold(int)));
	connect(ui.spinBoxHoughCirclesMinRadius, SIGNAL(valueChanged(int)), worker, SLOT(receiveHoughCirclesMinRadius(int)));
	connect(ui.spinBoxHoughCirclesMaxRadius, SIGNAL(valueChanged(int)), worker, SLOT(receiveHoughCirclesMaxRadius(int)));

	connect(ui.checkBoxEnableHoughCircles, SIGNAL(toggled(bool)), ui.horizontalSliderHoughCirclesCannyThreshold, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableHoughCircles, SIGNAL(toggled(bool)), ui.horizontalSliderHoughCirclesAccumulatorThreshold, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableHoughCircles, SIGNAL(toggled(bool)), ui.spinBoxHoughCirclesMinRadius, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableHoughCircles, SIGNAL(toggled(bool)), ui.spinBoxHoughCirclesMaxRadius, SLOT(setEnabled(bool)));

	// ...........................
	// Hough Lines
	// ...........................
	connect(ui.checkBoxEnableHoughLines, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableHoughLines()));
	connect(ui.horizontalSliderHoughLinesThreshold, SIGNAL(valueChanged(int)), worker, SLOT(receiveHoughLinesThreshold(int)));
	connect(ui.spinBoxHoughLinesMinLineLength, SIGNAL(valueChanged(int)), worker, SLOT(receiveHoughLinesMinLineLength(int)));
	connect(ui.spinBoxHoughLinesMaxLineGap, SIGNAL(valueChanged(int)), worker, SLOT(receiveHoughLinesMaxLineGap(int)));
	connect(ui.comboBoxHoughLinesType, SIGNAL(currentIndexChanged(int)), worker, SLOT(receiveHoughLinesType(int)));

	connect(ui.checkBoxEnableHoughLines, SIGNAL(toggled(bool)), ui.horizontalSliderHoughLinesThreshold, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableHoughLines, SIGNAL(toggled(bool)), ui.spinBoxHoughLinesMinLineLength, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableHoughLines, SIGNAL(toggled(bool)), ui.spinBoxHoughLinesMaxLineGap, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableHoughLines, SIGNAL(toggled(bool)), ui.comboBoxHoughLinesType, SLOT(setEnabled(bool)));

	// ...........................
	// Laplacian
	// ...........................
	connect(ui.checkBoxEnableLaplacian, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableLaplacian()));
	connect(ui.spinBoxLaplacianKernelSize, SIGNAL(valueChanged(int)), worker, SLOT(receiveLaplacianKernelSize(int)));
	connect(ui.checkBoxEnableLaplacian, SIGNAL(toggled(bool)), ui.spinBoxLaplacianKernelSize, SLOT(setEnabled(bool)));

	// ...........................
	// Canny Edge Detection
	// ...........................
	connect(ui.checkBoxEnableCannyDetection, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableCannyDetection()));
	connect(ui.horizontalSliderCannyDetectionThreshold, SIGNAL(valueChanged(int)), worker, SLOT(receiveCannyDetectionThreshold(int)));
	connect(ui.spinBoxCannyDetectionRatio, SIGNAL(valueChanged(int)), worker, SLOT(receiveCannyDetectionRatio(int)));
	connect(ui.spinBoxCannyDetectionKernelSize, SIGNAL(valueChanged(int)), worker, SLOT(receiveCannyDetectionKernelSize(int)));

	connect(ui.checkBoxEnableCannyDetection, SIGNAL(toggled(bool)), ui.horizontalSliderCannyDetectionThreshold, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableCannyDetection, SIGNAL(toggled(bool)), ui.spinBoxCannyDetectionRatio, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableCannyDetection, SIGNAL(toggled(bool)), ui.spinBoxCannyDetectionKernelSize, SLOT(setEnabled(bool)));

	// ...........................
	// Sobel
	// ...........................
	connect(ui.checkBoxEnableSobel, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableSobel()));
	connect(ui.spinBoxSobelKernelSize, SIGNAL(valueChanged(int)), worker, SLOT(receiveSobelKernelSize(int)));
	connect(ui.checkBoxEnableSobel, SIGNAL(toggled(bool)), ui.spinBoxSobelKernelSize, SLOT(setEnabled(bool)));

	// ...........................
	// Homogeneous Blur
	// ...........................
	connect(ui.checkBoxEnableHomogeneousBlur, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableHomogeneousBlur()));
	connect(ui.spinBoxHomogeneousBlurKernelSize, SIGNAL(valueChanged(int)), worker, SLOT(receiveHomogeneousBlurKernelSize(int)));
	connect(ui.checkBoxEnableHomogeneousBlur, SIGNAL(toggled(bool)), ui.spinBoxHomogeneousBlurKernelSize, SLOT(setEnabled(bool)));
	
	// ...........................
	// Gaussian Blur
	// ...........................
	connect(ui.checkBoxEnableGaussianBlur, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableGaussianBlur()));
	connect(ui.spinBoxGaussianBlurKernelWidth, SIGNAL(valueChanged(int)), worker, SLOT(receiveGaussianBlurKernelWidth(int)));
	connect(ui.spinBoxGaussianBlurKernelHeight, SIGNAL(valueChanged(int)), worker, SLOT(receiveGaussianBlurKernelHeight(int)));
	
	connect(ui.checkBoxEnableGaussianBlur, SIGNAL(toggled(bool)), ui.spinBoxGaussianBlurKernelWidth, SLOT(setEnabled(bool)));
	connect(ui.checkBoxEnableGaussianBlur, SIGNAL(toggled(bool)), ui.spinBoxGaussianBlurKernelHeight, SLOT(setEnabled(bool)));

	// ...........................
	// Median Blur
	// ...........................
	connect(ui.checkBoxEnableMedianBlur, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableMedianBlur()));
	connect(ui.spinBoxMedianBlurKernelSize, SIGNAL(valueChanged(int)), worker, SLOT(receiveMedianBlurKernelSize(int)));
	connect(ui.checkBoxEnableMedianBlur, SIGNAL(toggled(bool)), ui.spinBoxMedianBlurKernelSize, SLOT(setEnabled(bool)));

	// ...........................
	// Bilateral Filter
	// ...........................
	connect(ui.checkBoxEnableBilateralFilter, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableBilateralFilter()));
	connect(ui.horizontalSliderBilateralFilter, SIGNAL(valueChanged(int)), worker, SLOT(receiveBilateralFilterValue(int)));
	connect(ui.checkBoxEnableBilateralFilter, SIGNAL(toggled(bool)), ui.horizontalSliderBilateralFilter, SLOT(setEnabled(bool)));

	// ...........................
	// Object Detection
	// ...........................
	connect(ui.checkBoxEnableObjectDetection, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableObjectDetection()));

	// ...........................
	// Grid Lines
	// ...........................
	connect(ui.checkBoxEnableGrid, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableGrid()));
	connect(ui.horizontalSliderGridLineSpacing, SIGNAL(valueChanged(int)), worker, SLOT(receiveGridLineSpacingValue(int)));
	connect(ui.checkBoxEnableGrid, SIGNAL(toggled(bool)), ui.horizontalSliderGridLineSpacing, SLOT(setEnabled(bool)));

	// ------------------------------------------------------------
	// TAB - PROCESSING 2
	// ------------------------------------------------------------

	// ...........................
	// Morphology
	// ...........................
	connect(ui.checkBoxEnableErosion, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableErosion()));
	connect(ui.horizontalSliderErosion, SIGNAL(valueChanged(int)), worker, SLOT(receiveErosionValue(int)));
	connect(ui.checkBoxEnableErosion, SIGNAL(toggled(bool)), ui.horizontalSliderErosion, SLOT(setEnabled(bool)));

	connect(ui.checkBoxEnableDilation, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableDilation()));
	connect(ui.horizontalSliderDilation, SIGNAL(valueChanged(int)), worker, SLOT(receiveDilationValue(int)));
	connect(ui.checkBoxEnableDilation, SIGNAL(toggled(bool)), ui.horizontalSliderDilation, SLOT(setEnabled(bool)));

	// ...........................
	// Non-Photorealistic Rendering
	// ...........................
	connect(ui.checkBoxEnableNormalizedConvolutionFilter, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableNormalizedConvolutionFilter()));

	connect(ui.checkBoxEnableRecursiveFilter, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableRecursiveFilter()));

	connect(ui.checkBoxEnableDetailEnhancement, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableDetailEnhancement()));

	connect(ui.checkBoxEnablePencilSketch, SIGNAL(toggled(bool)), worker, SLOT(receiveEnablePencilSketch()));

	connect(ui.checkBoxEnableColorPencilDrawing, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableColorPencilDrawing()));

	connect(ui.checkBoxEnableStylization, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableStylization()));

	// ------------------------------------------------------------
	// TAB - CONSOLE
	// ------------------------------------------------------------

	connect(this, SIGNAL(sendConsoleInput(std::string)), worker, SLOT(receiveConsoleInput(std::string)));
	connect(ui.pushButtonConsoleEnter, SIGNAL(clicked(bool)), this, SLOT(receiveConsoleInput()));
	
	// ============================================================

	workerTrigger->start();
	worker->moveToThread(thread);
	workerTrigger->moveToThread(thread);

	thread->start();

	emit sendSetup(0); // video capture device
}

void QtOpencvWidget::receiveFrame(QImage frame)
{
	ui.labelView->setPixmap(QPixmap::fromImage(frame));
}

void QtOpencvWidget::receiveToggleStream()
{
	if (ui.pushButtonPlay->isChecked())
	{
		// *play pressed*

		ui.pushButtonPlay->setIcon(QIcon("Images/pause.png"));
	}
	else
	{
		// *pause pressed*

		ui.pushButtonPlay->setIcon(QIcon("Images/play.png"));

		// if recording, stop
		if (ui.pushButtonRecord->isChecked())
		{
			ui.pushButtonRecord->animateClick();
		}
	}

	emit sendToggleStream();
}

void QtOpencvWidget::receiveToggleRecord()
{
	if (ui.pushButtonRecord->isChecked())
	{
		// *record pressed*

		ui.pushButtonRecord->setIcon(QIcon("Images/stop.png"));

		// if NOT playing, play
		if (ui.pushButtonPlay->isChecked() == false)
		{
			ui.pushButtonPlay->animateClick();
		}

		// TODO:: receiveToggleRecord() ... START ... recording video
	}
	else
	{
		// *stop pressed*

		ui.pushButtonRecord->setIcon(QIcon("Images/record.png"));
		// TODO:: receiveToggleRecord() ... STOP ... recording video
	}

	emit sendToggleRecord();
}

void QtOpencvWidget::receiveRotateCounterClockwise()
{
	emit sendRotateCounterClockwise();
}

void QtOpencvWidget::receiveRotateClockwise()
{
	emit sendRotateClockwise();
}

void QtOpencvWidget::receiveFlipHorizontal()
{
	emit sendFlipHorizontal();
}

void QtOpencvWidget::receiveFlipVertical()
{
	emit sendFlipVertical();
}

void QtOpencvWidget::receiveConsoleInput()
{
	QString consoleInput = ui.lineEditConsoleInput->text();
	ui.lineEditConsoleInput->clear();

	if (consoleInput == "exit")
	{
		QApplication::quit();
	}

	ui.textEditConsoleOutput->append(consoleInput);

	emit sendConsoleInput(consoleInput.toStdString());
}