#ifndef KALMANFILTER_H
#define KALMANFILTER_H


#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Font.h"
#include "cinder/Vector.h"
#include "cinder/Path2d.h"
#include "cinder/gl/gl.h"
#include "CinderOpenCV.h"
#include "cinder/app/AppNative.h"


using namespace ci;
using namespace ci::app;
using namespace std;


typedef struct
{
	Vec2f	p1;
	Vec2f	p2;
	Vec2f	p3;
} QuadraticCurve;

class KalmanFilter
{

public:

	KalmanFilter()
	{};

	~KalmanFilter()
	{}

public:

	void init(Vec2f position, float smoothness = 1e-4, float rapidness = 1e-1)
	{
		kalmanFilter.init(4, 2, 0);
		kalmanFilter.transitionMatrix = *(cv::Mat_<float>(4, 4) << 1, 0, 1, 0,
			0, 1, 0, 1,
			0, 0, 1, 0,
			0, 0, 0, 1);

		measurementMat = cv::Mat_<float>(2, 1);
		measurementMat.setTo(cv::Scalar(0));

		kalmanFilter.statePre.at<float>(0) = position.x;
		kalmanFilter.statePre.at<float>(1) = position.y;
		kalmanFilter.statePre.at<float>(2) = 0;
		kalmanFilter.statePre.at<float>(3) = 0;

		kalmanFilter.statePost.at<float>(0) = position.x;
		kalmanFilter.statePost.at<float>(1) = position.y;
		kalmanFilter.statePost.at<float>(2) = 0;
		kalmanFilter.statePost.at<float>(3) = 0;

		cv::setIdentity(kalmanFilter.measurementMatrix);
		cv::setIdentity(kalmanFilter.processNoiseCov, cv::Scalar::all(smoothness));
		cv::setIdentity(kalmanFilter.measurementNoiseCov, cv::Scalar::all(rapidness));
		cv::setIdentity(kalmanFilter.errorCovPost, cv::Scalar::all(0.1f));
	}

	void update(const Vec2f & newPoint)
	{
		predictionMat = kalmanFilter.predict();

		measurementMat.at<float>(0, 0) = newPoint.x;
		measurementMat.at<float>(1, 0) = newPoint.y;

		estimatedMat = kalmanFilter.correct(measurementMat);
	}

	Vec2f getPrediction()
	{
		return Vec2f(predictionMat.at<float>(0, 0), predictionMat.at<float>(1, 0));
	};

	Vec2f getEstimation()
	{
		return Vec2f(estimatedMat.at<float>(0, 0), estimatedMat.at<float>(1, 0));
	};

	Vec2f getVelocity()
	{
		return Vec2f(estimatedMat.at<float>(3, 0), estimatedMat.at<float>(4, 0));
	}

private:

	cv::KalmanFilter	kalmanFilter;
	cv::Mat				measurementMat;
	cv::Mat				predictionMat;
	cv::Mat				estimatedMat;
};

#endif




