#include "StudentPreProcessing.h"
#include "IntensityImageStudent.h"


IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	IntensityImageStudent *intensity_image = new IntensityImageStudent(image.getWidth(), image.getHeight());

	for (int i = 0; i < image.getWidth()*image.getHeight(); i++){
		//Luminosity formula 0.21 R + 0.72 G + 0.07 B
		unsigned char sum = (image.getPixel(i).r * 0.21) + (image.getPixel(i).g * 0.72) + (image.getPixel(i).b * 0.07);
		Intensity it = sum;
		intensity_image->setPixel(i, it);
	}

	return intensity_image;
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	return nullptr;
}