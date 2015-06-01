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
}