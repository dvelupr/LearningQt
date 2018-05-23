#pragma once
#include <QWidget>
#include <QThread>
#include <QTimer>
#include <QStyleFactory>
#include <QDateTime>
#include <iostream>
#include "ui_qtopencvwidget.h"
#include "workeropencv.hpp"

class QtOpencvWidget : 
	public QWidget
{
	Q_OBJECT

public:
	QtOpencvWidget(QWidget * parent = Q_NULLPTR);
	~QtOpencvWidget();

private:
	Ui::QtOpencvWidget ui;
	QThread *thread;
	WorkerOpenCV *worker;
	QTimer *workerTrigger;

	void setup();

signals:
	void sendSetup(int device);
	void sendToggleStream();
	void sendToggleRecord();
	void sendRotateCounterClockwise();
	void sendRotateClockwise();
	void sendFlipHorizontal();
	void sendFlipVertical();
	void sendConsoleInput(std::string consoleInput);

private slots:
	void receiveFrame(QImage frame);
	void receiveToggleStream();
	void receiveToggleRecord();
	void receiveRotateCounterClockwise();
	void receiveRotateClockwise();
	void receiveFlipHorizontal();
	void receiveFlipVertical();
	void receiveConsoleInput();
};
