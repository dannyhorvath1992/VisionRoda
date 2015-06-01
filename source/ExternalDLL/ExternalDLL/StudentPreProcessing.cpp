#include "StudentPreProcessing.h"
#include "IntensityImageStudent.h"
#include "basetimer.h"
#include <iostream>
#include "ImageFactory.h"
#include "ImageIO.h"

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

	BaseTimer bt;
	// start de timer
	bt.start();

	double imageWidth = image.getWidth();
	double imageHeight = image.getHeight();
	double oldDimensions = imageWidth * imageHeight;
	double maxHeight = 200;
	double maxWidth = 200;
	double newHeight;
	double newWidth;
	double newDimensions = maxWidth * maxHeight;
	
	double ratio;
	IntensityImage * cop;

	if (newDimensions < oldDimensions) {
		// resize
		// Get aspect ratio and new dimensions
		if (imageWidth > maxWidth) {
			ratio = maxWidth / imageWidth;
			newHeight = imageHeight * ratio;
			newWidth = imageWidth * ratio;
		}
		if (imageHeight > maxHeight) {
			ratio = maxHeight / imageHeight;
			newWidth = imageWidth * ratio;
			newHeight = imageHeight * ratio;
		}

		newDimensions = newWidth * newHeight;

		cop = ImageFactory::newIntensityImage();
		cop->set(newWidth, newHeight);

		// scale
		int A, B, C, D, x, y, index, gray;
		float x_ratio = ((float)(imageWidth - 1)) / newWidth;
		float y_ratio = ((float)(imageHeight - 1)) / newHeight;
		float x_diff, y_diff, ya, yb;
		int offset = 0;

		for (int i = 0; i < newHeight; ++i) {
			for (int j = 0; j < newWidth-1; ++j) {
				x = (int)(x_ratio * j);
				y = (int)(y_ratio * i);
				x_diff = (x_ratio * j) - x;
				y_diff = (y_ratio * i) - y;
				index = y*imageWidth + x;

				// range is 0 to 255 thus bitwise AND with 0xff
				A = image.getPixel(index) & 0xff;
				B = image.getPixel(index + 1) & 0xff;
				C = image.getPixel(index + imageWidth) & 0xff;
				D = image.getPixel(index + imageWidth + 1) & 0xff;

				// Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
				gray = (int)(
					A*(1 - x_diff)*(1 - y_diff) + B*(x_diff)*(1 - y_diff) +
					C*(y_diff)*(1 - x_diff) + D*(x_diff*y_diff)
					);

				cop->setPixel(j,i, gray);
			}
		}
		
	}
	else {
		// return original
		cop = ImageFactory::newIntensityImage(image);

	}

	// stop de timer
	bt.stop();

	// rapporteer door de elapsed...() functies aan te roepen
	std::cout << "Scale Time for the operation was: " << bt.elapsedMicroSeconds() << " microseconds" << std::endl;

	return cop;

}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {

	BaseTimer bt;
	// start de timer
	bt.start();

	int imageWidth = image.getWidth();
	int imageHeight = image.getHeight();
	IntensityImage * cop = ImageFactory::newIntensityImage();
	cop->set(imageWidth, imageHeight);


	long valX, valY = 0; 
	unsigned int GX[3][3]; 
	unsigned int GY[3][3];

	//Sobel Horizontal Mask     
	GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
	GX[1][0] = -2; GX[1][1] = 0; GX[1][2] = 2;
	GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;

	//Sobel Vertical Mask   
	GY[0][0] = 1; GY[0][1] = 2; GY[0][2] = 1;
	GY[1][0] = 0; GY[1][1] = 0; GY[1][2] = 0;
	GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;

	int SUM;

	//SOBEL edge detector implementation. 
	for (int i = 0; i < imageHeight; i++)
	{
		for (int j = 0; j < imageWidth; j++)
		{

			//setting the pixels around the border to 0, 
			//because the Sobel kernel cannot be allied to them
			if ((i == 0) || (i == imageHeight - 1) || (j == 0) || (j == imageWidth - 1))
			{
				valX = 0;
				valY = 0;
			}

			else
			{
				valX = 0;
				valY = 0;

				//calculating the X and Y convolutions
				for (int x = -1; x <= 1; x++)
				{
					for (int y = -1; y <= 1; y++)
					{
						valX = valX + image.getPixel(j+y, i+x) * GX[1 + x][1 + y];
						valY = valY + image.getPixel(j+y, i+x) * GY[1 + x][1 + y];
					
					}
				}

			}

			SUM = abs(valX) + abs(valY);
			SUM = SUM > 255 ? 255 : SUM;
			SUM = SUM < 0 ? 0 : SUM;

			cop->setPixel(j, i, SUM);

		}
	}

	// stop de timer
	bt.stop();

	// rapporteer door de elapsed...() functies aan te roepen
	std::cout << "Edge detection Time for the operation was: " << bt.elapsedMicroSeconds() << " microseconds" << std::endl;

	return cop;

}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	int imageWidth = image.getWidth();
	int imageHeight = image.getHeight();
	IntensityImage * cop = ImageFactory::newIntensityImage();
	cop->set(imageWidth, imageHeight);
	int val;
	int thresh = 159;

	for (int i = 0; i < imageHeight; i++)
	{
		for (int j = 0; j < imageWidth; j++)
		{

			val = image.getPixel(j, i);
			val = val > thresh ? 0 : 255;
			cop->setPixel(j, i, val);

		}
	}

	return cop;
}
