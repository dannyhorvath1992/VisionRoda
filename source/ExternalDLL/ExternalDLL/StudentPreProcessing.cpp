#include "StudentPreProcessing.h"
#include "IntensityImageStudent.h"
#include "basetimer.h"
#include <iostream>

IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	// maak een timer object aan
	BaseTimer bt;

	IntensityImageStudent *intensity_image = new IntensityImageStudent(image.getWidth(), image.getHeight());

	// start de timer
	bt.start();

	for (int i = 0; i < image.getWidth()*image.getHeight(); i++){
		//Luminosity formula 0.21 R + 0.72 G + 0.07 B
		unsigned char sum = (image.getPixel(i).r * 0.21) + (image.getPixel(i).g * 0.72) + (image.getPixel(i).b * 0.07);
		//unsigned char sum = (image.getPixel(i).r  + image.getPixel(i).g  + image.getPixel(i).b) / 3;

		Intensity it = sum;
		intensity_image->setPixel(i, it);
	}

	// stop de timer
	bt.stop();

	// rapporteer door de elapsed...() functies aan te roepen
	std::cout << "Time for the operation was: " << bt.elapsedMicroSeconds() << " microseconds" << std::endl;

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