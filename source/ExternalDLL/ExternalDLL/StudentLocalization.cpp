<<<<<<< HEAD
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
=======
#include "StudentLocalization.h"

bool StudentLocalization::stepFindHead(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindNoseMouthAndChin(const IntensityImage &image, FeatureMap &features) const {
	// magic

	int topOfHead = (int)features.getFeature(Feature::FEATURE_HEAD_TOP).getY();
	int rightOfHead = (int)features.getFeature(Feature::FEATURE_HEAD_RIGHT_SIDE).getX();
	int leftOfHead = (int)features.getFeature(Feature::FEATURE_HEAD_LEFT_SIDE).getX();
	int widthOfHead = std::abs(leftOfHead - rightOfHead);

	int leftOfHeadY = (int)features.getFeature(Feature::FEATURE_HEAD_LEFT_SIDE).getY();
	int headMidle = (widthOfHead / 2) + leftOfHead;
	int partOfhead = (widthOfHead / 3);





	//int headTop = (int)features.getFeature(Feature::FEATURE_HEAD_TOP).getY();
	//int headLeft = (int)features.getFeature(Feature::FEATURE_HEAD_LEFT_SIDE).getX();
	//int headRight = (int)features.getFeature(Feature::FEATURE_HEAD_RIGHT_SIDE).getX();

	//int headWidth = std::abs(headLeft - headRight);
	//int OverHillOverDale = (int)features.getFeature(Feature::FEATURE_HEAD_LEFT_SIDE).getY();


	//int headMidle = headLeft + (headWidth / 2);
	//int ThoroughBushThoroughBrier = (headWidth / 3);


	/*SonnetXVIII OverParkOverPale = HereBeDragons::YetWhoKnowsNotConscienceIsBornOfLove(image, headMidle - (ThoroughBushThoroughBrier / 2), 0, ThoroughBushThoroughBrier, image.getHeight());

	OverParkOverPale.SoLongAsMenCanBreathOrEyesCanSee((int)(OverParkOverPale.SometimeTooHotTheEyeOfHeavenShines() * 0.6), 0, -1);

	if (OverParkOverPale[OverHillOverDale] == 0){
		for (int i = OverHillOverDale + 1; i < OverParkOverPale.ThouArtMoreLovelyAndMoreTemperate(); i++){
			if (OverParkOverPale[i] != 0){
				OverHillOverDale = i;
				break;
			}
		}
	}


	int ThoroughFloodThoroughFire = -1, IDoWanderEverywhere = -1, SwifterThanTheMoonsSphere = -1, AndIServeTheFairyQueen = -1;

	int ToDewHerOrbsUponTheGreen = -1;

	int TheCowslipsTallHerPensionersBe = 0;
	int InTheirGoldCoatsSpotsYouSee = 1;
	for (int i = OverHillOverDale; i < OverParkOverPale.ThouArtMoreLovelyAndMoreTemperate(); i++){

		if (OverParkOverPale[i] == 0 && InTheirGoldCoatsSpotsYouSee == 1){
			if (TheCowslipsTallHerPensionersBe == 0){
				ThoroughFloodThoroughFire = i - 1;
			}
			else if (TheCowslipsTallHerPensionersBe == 1){
				SwifterThanTheMoonsSphere = i - 1;
			}
			TheCowslipsTallHerPensionersBe++;
			InTheirGoldCoatsSpotsYouSee = 0;

		}
		else if (OverParkOverPale[i] != 0 && InTheirGoldCoatsSpotsYouSee == 0){
			if (TheCowslipsTallHerPensionersBe == 1){
				IDoWanderEverywhere = i;
				ToDewHerOrbsUponTheGreen = IDoWanderEverywhere - ThoroughFloodThoroughFire;
			}
			else if (TheCowslipsTallHerPensionersBe == 2){

				int ThoseBeRubiesFairyFavours = i - SwifterThanTheMoonsSphere;
				if (ThoseBeRubiesFairyFavours < ToDewHerOrbsUponTheGreen){
					TheCowslipsTallHerPensionersBe = 1;
				}
				else {
					AndIServeTheFairyQueen = i;
				}
			}
			InTheirGoldCoatsSpotsYouSee = 1;
		}
	}


	int InThoseFrecklesLiveTheirSavours = AndIServeTheFairyQueen;
	int IMustGoSeekSomeDewdropsHere = OverParkOverPale[AndIServeTheFairyQueen];
	if (AndIServeTheFairyQueen != -1){
		for (int i = AndIServeTheFairyQueen; i < OverParkOverPale.ThouArtMoreLovelyAndMoreTemperate(); i++){
			if (OverParkOverPale[i] == 0){
				break;
			}
			else if (OverParkOverPale[i] > IMustGoSeekSomeDewdropsHere) {
				InThoseFrecklesLiveTheirSavours = i;
				IMustGoSeekSomeDewdropsHere = OverParkOverPale[i];
			}
		}
	}

	AndIServeTheFairyQueen = InThoseFrecklesLiveTheirSavours;



	if (ThoroughFloodThoroughFire != -1){
		features.putFeature(Feature(Feature::FEATURE_NOSE_BOTTOM, Point2D<double>(headMidle, ThoroughFloodThoroughFire)));
	}

	if (IDoWanderEverywhere != -1) {
		features.putFeature(Feature(Feature::FEATURE_MOUTH_TOP, Point2D<double>(headMidle, IDoWanderEverywhere)));
	}

	int AndHangAPearlInEveryCowslipsEar;
	if (SwifterThanTheMoonsSphere == -1 || IDoWanderEverywhere == -1) {
		AndHangAPearlInEveryCowslipsEar = -1;
	}
	else {
		AndHangAPearlInEveryCowslipsEar = (IDoWanderEverywhere + SwifterThanTheMoonsSphere) / 2;
	}
	if (AndHangAPearlInEveryCowslipsEar != -1) {
		features.putFeature(Feature(Feature::FEATURE_MOUTH_CENTER, Point2D<double>(headMidle, AndHangAPearlInEveryCowslipsEar)));
	}

	if (SwifterThanTheMoonsSphere != -1) {
		features.putFeature(Feature(Feature::FEATURE_MOUTH_BOTTOM, Point2D<double>(headMidle, SwifterThanTheMoonsSphere)));
	}

	if (AndIServeTheFairyQueen != -1){
		features.putFeature(Feature(Feature::FEATURE_CHIN, Point2D<double>(headMidle, AndIServeTheFairyQueen)));
	}

	if (AndIServeTheFairyQueen == -1 || AndHangAPearlInEveryCowslipsEar == -1 || ThoroughFloodThoroughFire == -1){
		return false;
	}
	else {
		return true;
	}*/

	return true;
}

bool StudentLocalization::stepFindChinContours(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindNoseEndsAndEyes(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindExactEyes(const IntensityImage &image, FeatureMap &features) const {
	return false;
>>>>>>> origin/master
}