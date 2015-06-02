#include "StudentLocalization.h"
#include "ImageIO.h"
#include "ImageFactory.h"
#include "basetimer.h"

bool StudentLocalization::stepFindHead(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindNoseMouthAndChin(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindChinContours(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindNoseEndsAndEyes(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindExactEyes(const IntensityImage &image, FeatureMap &features) const {

	// maak een timer object aan
	BaseTimer bt;

	// start de timer
	bt.start();

	// Eyes
	Feature leftEyeFeat = Feature(Feature::FEATURE_EYE_LEFT_RECT);
	Feature rightEyeFeat = Feature(Feature::FEATURE_EYE_RIGHT_RECT);

	//Head
	Feature headTop = features.getFeature(Feature::FEATURE_HEAD_TOP);
	Feature leftHeadBottom = features.getFeature(Feature::FEATURE_HEAD_LEFT_NOSE_BOTTOM);
	Feature rightHeadBottom = features.getFeature(Feature::FEATURE_HEAD_RIGHT_NOSE_BOTTOM);

	// Nose
	Feature leftNoseBottom = features.getFeature(Feature::FEATURE_NOSE_END_LEFT);
	Feature rightNoseBottom = features.getFeature(Feature::FEATURE_NOSE_END_RIGHT);


	int leftEyeBottomRightX = leftNoseBottom.getPoints()[0].getX();
	int leftEyeBottomRightY = leftNoseBottom.getPoints()[0].getY();

	int leftEyeTopLeftX = leftHeadBottom.getPoints()[0].getX();
	int leftEyeTopLeftY = headTop.getPoints()[0].getY();

	double eyeLocaBegin = 0.3;
	double eyeLocaFinish = 0.07;

	RGBImage *tempImg = ImageFactory::newRGBImage();
	ImageIO::intensityToRGB(image, *tempImg);

	bool isFound = false;
	for (int i = leftEyeBottomRightY; i > leftEyeTopLeftY; i--){
		int intens = 0;
		for (int j = leftEyeTopLeftX; j < leftEyeBottomRightX; j++){
			if (tempImg->getPixel(j, i).b == 0){
				intens++;
			}
		}
		double theRes = (double)intens / (double)(leftEyeBottomRightX - leftEyeTopLeftX);
		if (!isFound){
			if (theRes >= eyeLocaBegin){
				leftEyeBottomRightY = i;
				isFound = true;
			}
		}
		else {
			if (theRes <= eyeLocaFinish){
				leftEyeTopLeftY = i;
				break;
			}
		}
	}

	for (int i = leftEyeTopLeftX; i < leftEyeBottomRightX; i++){
		int intens = 0;
		for (int j = leftEyeTopLeftY; j < leftEyeBottomRightY; j++){
			if (tempImg->getPixel(i, j).b == 0){
				intens++;
			}
		}
		double theRes = (double)intens / (double)(leftEyeBottomRightY - leftEyeTopLeftY);
		if (theRes > 0){
			leftEyeTopLeftX = i;
			break;
		}
	}


	int rightEyeBottomRightX = rightHeadBottom.getPoints()[0].getX();
	int rightEyeBottomRightY = leftNoseBottom.getPoints()[0].getY();

	int rightEyeTopLeftX = rightNoseBottom.getPoints()[0].getX();
	int rightEyeTopLeftY = headTop.getPoints()[0].getY();

	leftEyeFeat.addPoint(Point2D<double>(leftEyeTopLeftX, leftEyeTopLeftY));
	leftEyeFeat.addPoint(Point2D<double>(leftEyeBottomRightX, leftEyeBottomRightY));

	isFound = false;
	for (int i = rightEyeBottomRightY; i > rightEyeTopLeftY; i--){
		int intens = 0;
		for (int j = rightEyeTopLeftX; j < rightEyeBottomRightX; j++){
			if (tempImg->getPixel(j, i).b == 0){
				intens++;
			}
		}
		double theRes = (double)intens / (double)(rightEyeBottomRightX - rightEyeTopLeftX);
		if (!isFound){
			if (theRes >= eyeLocaBegin){
				rightEyeBottomRightY = i;
				isFound = true;
			}
		}
		else {
			if (theRes <= eyeLocaFinish){
				rightEyeTopLeftY = i;
				break;
			}
		}
	}

	for (int i = rightEyeBottomRightX; i > rightEyeTopLeftX; i--){
		int intens = 0;
		for (int j = rightEyeTopLeftY; j < rightEyeBottomRightY; j++){
			if (tempImg->getPixel(i, j).b == 0){
				intens++;
			}
		}
		double theRes = (double)intens / (double)(rightEyeBottomRightY - rightEyeTopLeftY);
		if (theRes > 0){
			rightEyeBottomRightX = i;
			break;
		}
	}

	rightEyeFeat.addPoint(Point2D<double>(rightEyeTopLeftX, rightEyeTopLeftY));
	rightEyeFeat.addPoint(Point2D<double>(rightEyeBottomRightX, rightEyeBottomRightY));

	for (int i = leftEyeTopLeftX - 1; i <= leftEyeBottomRightX + 1; i++){
		tempImg->setPixel(i, leftEyeTopLeftY - 1, RGB(255, 255, 0));
		tempImg->setPixel(i, leftEyeBottomRightY + 1, RGB(255, 255, 0));
	}
	for (int i = leftEyeTopLeftY - 1; i <= leftEyeBottomRightY + 1; i++){
		tempImg->setPixel(leftEyeTopLeftX - 1, i, RGB(255, 255, 0));
		tempImg->setPixel(leftEyeBottomRightX + 1, i, RGB(255, 255, 0));
	}

	for (int i = rightEyeTopLeftX - 1; i <= rightEyeBottomRightX + 1; i++){
		tempImg->setPixel(i, rightEyeTopLeftY - 1, RGB(255, 255, 0));
		tempImg->setPixel(i, rightEyeBottomRightY + 1, RGB(255, 255, 0));
	}
	for (int i = rightEyeTopLeftY - 1; i <= rightEyeBottomRightY + 1; i++){
		tempImg->setPixel(rightEyeTopLeftX - 1, i, RGB(255, 255, 0));
		tempImg->setPixel(rightEyeBottomRightX + 1, i, RGB(255, 255, 0));
	}

	features.putFeature(leftEyeFeat);
	features.putFeature(rightEyeFeat);

	// save img
	ImageIO::saveRGBImage(*tempImg, ImageIO::getDebugFileName("Localization-5/debug.png"));
	// Delete temp img
	delete tempImg;

	// stop de timer
	bt.stop();

	// rapporteer door de elapsed...() functies aan te roepen
	std::cout << "Eyesfind Time for the operation was: " << bt.elapsedMicroSeconds() << " microseconds" << std::endl;

	return true;
}