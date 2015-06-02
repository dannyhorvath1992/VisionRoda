#include "StudentLocalization.h"
#include "ImageIO.h"
#include "ImageFactory.h"

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
	RGBImage * debugImage = ImageFactory::newRGBImage();
	ImageIO::intensityToRGB(image, *debugImage);

	//Create the eye features to return
	Feature featureLeftEye = Feature(Feature::FEATURE_EYE_LEFT_RECT);
   Feature featureRightEye = Feature(Feature::FEATURE_EYE_RIGHT_RECT);

   Feature top_head = features.getFeature(Feature::FEATURE_HEAD_TOP);
   Feature bottom_nose_left = features.getFeature(Feature::FEATURE_NOSE_END_LEFT);
   Feature bottom_nose_right = features.getFeature(Feature::FEATURE_NOSE_END_RIGHT);
   Feature bottom_head_left = features.getFeature(Feature::FEATURE_HEAD_LEFT_NOSE_BOTTOM);
   Feature bottom_head_right = features.getFeature(Feature::FEATURE_HEAD_RIGHT_NOSE_BOTTOM);

	//Add the left eye rect
   int x_top_left_left_eye = bottom_head_left.getPoints()[0].getX();
   int y_top_left_left_eye = top_head.getPoints()[0].getY();
   int x_bottom_right_left_eye = bottom_nose_left.getPoints()[0].getX();
   int y_bottom_right_left_eye = bottom_nose_left.getPoints()[0].getY();

   double IntensityEyeLocalizationStart = 0.3;
   double IntensityEyeLocalizationEnd = 0.07;

   bool found = false;
   for (int i = y_bottom_right_left_eye; i > y_top_left_left_eye; i--){
      int intensity = 0;
      for (int j = x_top_left_left_eye; j < x_bottom_right_left_eye; j++){
         if (debugImage->getPixel(j, i).b == 0){
            intensity++;
         }
      }
      double result = (double)intensity / (double)(x_bottom_right_left_eye - x_top_left_left_eye);
      if (!found){
         if (result >= IntensityEyeLocalizationStart){
            y_bottom_right_left_eye = i;
            found = true;
         }
      }
      else {
         if (result <= IntensityEyeLocalizationEnd){
            y_top_left_left_eye = i;
            break;
         }
      }
   }

   for (int i = x_top_left_left_eye; i < x_bottom_right_left_eye; i++){
      int intensity = 0;
      for (int j = y_top_left_left_eye; j < y_bottom_right_left_eye; j++){
         if (debugImage->getPixel(i, j).b == 0){
            intensity++;
         }
      }
      double result = (double)intensity / (double)(y_bottom_right_left_eye - y_top_left_left_eye);
      if (result > 0){
         x_top_left_left_eye = i;
         break;
      }
   }

   featureLeftEye.addPoint(Point2D<double>(x_top_left_left_eye, y_top_left_left_eye));
   featureLeftEye.addPoint(Point2D<double>(x_bottom_right_left_eye, y_bottom_right_left_eye));

	//Add the right eye rect
   int x_top_left_right_eye = bottom_nose_right.getPoints()[0].getX();
   int y_top_left_right_eye = top_head.getPoints()[0].getY();
   int x_bottom_right_right_eye = bottom_head_right.getPoints()[0].getX();
   int y_bottom_right_right_eye = bottom_nose_left.getPoints()[0].getY();


   found = false;
   for (int i = y_bottom_right_right_eye; i > y_top_left_right_eye; i--){
      int intensity = 0;
      for (int j = x_top_left_right_eye; j < x_bottom_right_right_eye; j++){
         if (debugImage->getPixel(j, i).b == 0){
            intensity++;
         }
      }
      double result = (double)intensity / (double)(x_bottom_right_right_eye - x_top_left_right_eye);
      if (!found){
         if (result >= IntensityEyeLocalizationStart){
            y_bottom_right_right_eye = i;
            found = true;
         }
      }
      else {
         if (result <= IntensityEyeLocalizationEnd){
            y_top_left_right_eye = i;
            break;
         }
      }
   }

   for (int i = x_bottom_right_right_eye; i > x_top_left_right_eye; i--){
      int intensity = 0;
      for (int j = y_top_left_right_eye; j < y_bottom_right_right_eye; j++){
         if (debugImage->getPixel(i, j).b == 0){
            intensity++;
         }
      }
      double result = (double)intensity / (double)(y_bottom_right_right_eye - y_top_left_right_eye);
      if (result > 0){
         x_bottom_right_right_eye = i;
         break;
      }
   }

   featureRightEye.addPoint(Point2D<double>(x_top_left_right_eye, y_top_left_right_eye));
   featureRightEye.addPoint(Point2D<double>(x_bottom_right_right_eye, y_bottom_right_right_eye));

   //Draw rectangles on RGB_Map
   for (int i = x_top_left_left_eye-1; i <= x_bottom_right_left_eye+1; i++){
      debugImage->setPixel(i, y_top_left_left_eye-1, RGB(255, 255, 0));
      debugImage->setPixel(i, y_bottom_right_left_eye+1, RGB(255, 255, 0));
   }
   for (int i = y_top_left_left_eye-1; i <= y_bottom_right_left_eye+1; i++){
      debugImage->setPixel(x_top_left_left_eye-1, i, RGB(255, 255, 0));
      debugImage->setPixel(x_bottom_right_left_eye+1, i, RGB(255, 255, 0));
   }

   for (int i = x_top_left_right_eye-1; i <= x_bottom_right_right_eye+1; i++){
      debugImage->setPixel(i, y_top_left_right_eye-1, RGB(255, 255, 0));
      debugImage->setPixel(i, y_bottom_right_right_eye+1, RGB(255, 255, 0));
   }
   for (int i = y_top_left_right_eye-1; i <= y_bottom_right_right_eye+1; i++){
      debugImage->setPixel(x_top_left_right_eye-1, i, RGB(255, 255, 0));
      debugImage->setPixel(x_bottom_right_right_eye+1, i, RGB(255, 255, 0));
   }

	//Put the eye features
	features.putFeature(featureRightEye);
	features.putFeature(featureLeftEye);

	//Save debug image
	ImageIO::saveRGBImage(*debugImage, ImageIO::getDebugFileName("Localization-5/debug.png"));
	delete debugImage;
	return true;
}