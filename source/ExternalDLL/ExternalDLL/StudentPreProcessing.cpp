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

		std::cout << newWidth << " x " << newHeight;


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

	return cop;

}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	//double gaussianMask[5][5];		// Gaussian mask
	double gaussianMask[9][9];
	//double sum = 159.0;
	double sum = 4.0;

	int iOffset;
	int newPixel;			// Sum pixel values for gaussian
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
	unsigned int val1, val2, val3;

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

				//SUM = abs(valX) + abs(valY);

			}

			SUM = abs(valX) + abs(valY);
			SUM = SUM > 255 ? 255 : SUM;
			SUM = SUM < 0 ? 0 : SUM;

			//Gradient magnitude
			//val1 = sqrt(valX*valX + valY*valY);

			//if (SUM>255) SUM = 255;
			//if (SUM<0) SUM = 0;
			//unsigned char tst = 255 - (unsigned char)(SUM);

			cop->setPixel(j, i, SUM);

		}
	}

	std::cout << "klaar";

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

///// end -> BACKUP BELOW
//
//#include "StudentPreProcessing.h"
//#include "IntensityImageStudent.h"
//#include "basetimer.h"
//#include <iostream>
//#include "ImageFactory.h"
//#include "ImageIO.h"
//
//
//IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
//	// maak een timer object aan
//	BaseTimer bt;
//
//	IntensityImageStudent *intensity_image = new IntensityImageStudent(image.getWidth(), image.getHeight());
//
//	// start de timer
//	bt.start();
//
//	for (int i = 0; i < image.getWidth()*image.getHeight(); i++){
//		//Luminosity formula 0.21 R + 0.72 G + 0.07 B
//		unsigned char sum = (image.getPixel(i).r * 0.21) + (image.getPixel(i).g * 0.72) + (image.getPixel(i).b * 0.07);
//		//unsigned char sum = (image.getPixel(i).r  + image.getPixel(i).g  + image.getPixel(i).b) / 3;
//
//		Intensity it = sum;
//		intensity_image->setPixel(i, it);
//	}
//
//	// stop de timer
//	bt.stop();
//
//	// rapporteer door de elapsed...() functies aan te roepen
//	std::cout << "Time for the operation was: " << bt.elapsedMicroSeconds() << " microseconds" << std::endl;
//
//	return intensity_image;
//}
//
//IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
//	return nullptr;
//}
//
//IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
//	//double gaussianMask[5][5];		// Gaussian mask
//	double gaussianMask[9][9];
//	//double sum = 159.0;
//	double sum = 4.0;
//
//	int iOffset;
//	int newPixel;			// Sum pixel values for gaussian
//	int imageWidth = image.getWidth();
//	int imageHeight = image.getHeight();
//	IntensityImage * cop = ImageFactory::newIntensityImage();
//	cop->set(imageWidth, imageHeight);
//
//
//	/* Declare Gaussian mask */
//	/*gaussianMask[0][0] = 2;	 gaussianMask[0][1] = 4;  gaussianMask[0][2] = 5;  gaussianMask[0][3] = 4;  gaussianMask[0][4] = 2;
//	gaussianMask[1][0] = 4;	 gaussianMask[1][1] = 9;  gaussianMask[1][2] = 12; gaussianMask[1][3] = 9;  gaussianMask[1][4] = 4;
//	gaussianMask[2][0] = 5;	 gaussianMask[2][1] = 12; gaussianMask[2][2] = 15; gaussianMask[2][3] = 12; gaussianMask[2][4] = 2;
//	gaussianMask[3][0] = 4;	 gaussianMask[3][1] = 9;  gaussianMask[3][2] = 12; gaussianMask[3][3] = 9;  gaussianMask[3][4] = 4;
//	gaussianMask[4][0] = 2;	 gaussianMask[4][1] = 4;  gaussianMask[4][2] = 5;  gaussianMask[4][3] = 4;  gaussianMask[4][4] = 2;*/
//
//	gaussianMask[0][0] = 0;	 gaussianMask[0][1] = 0;  gaussianMask[0][2] = 3;  gaussianMask[0][3] = 2;		gaussianMask[0][4] = 2;		gaussianMask[0][5] = 2;			gaussianMask[0][6] = 3;  gaussianMask[0][7] = 0;  gaussianMask[0][8] = 0;
//	gaussianMask[1][0] = 0;	 gaussianMask[1][1] = 2;  gaussianMask[1][2] = 3;  gaussianMask[1][3] = 5;		gaussianMask[1][4] = 5;		gaussianMask[1][5] = 5;			gaussianMask[1][6] = 3;  gaussianMask[1][7] = 2;  gaussianMask[1][8] = 0;
//	gaussianMask[2][0] = 3;	 gaussianMask[2][1] = 3;  gaussianMask[2][2] = 5;  gaussianMask[2][3] = 3;		gaussianMask[2][4] = 0;		gaussianMask[2][5] = 3;			gaussianMask[2][6] = 5;  gaussianMask[2][7] = 3;  gaussianMask[2][8] = 3;
//	gaussianMask[3][0] = 2;	 gaussianMask[3][1] = 5;  gaussianMask[3][2] = 3;  gaussianMask[3][3] = -12;	gaussianMask[3][4] = -23;	gaussianMask[3][5] = -12;		gaussianMask[3][6] = 3;  gaussianMask[3][7] = 5;  gaussianMask[3][8] = 2;
//	gaussianMask[4][0] = 2;	 gaussianMask[4][1] = 5;  gaussianMask[4][2] = 0;  gaussianMask[4][3] = -23;	gaussianMask[4][4] = -40;	gaussianMask[4][5] = -23;		gaussianMask[4][6] = 0;  gaussianMask[4][7] = 5;  gaussianMask[4][8] = 2;
//	gaussianMask[5][0] = 2;	 gaussianMask[5][1] = 5;  gaussianMask[5][2] = 3;  gaussianMask[5][3] = -12;	gaussianMask[5][4] = -23;	gaussianMask[5][5] = 12;		gaussianMask[5][6] = 3;  gaussianMask[5][7] = 5;  gaussianMask[5][8] = 2;
//	gaussianMask[6][0] = 3;	 gaussianMask[6][1] = 3;  gaussianMask[6][2] = 5;  gaussianMask[6][3] = 3;		gaussianMask[6][4] = 0;		gaussianMask[6][5] = 3;			gaussianMask[6][6] = 5;  gaussianMask[6][7] = 3;  gaussianMask[6][8] = 3;
//	gaussianMask[7][0] = 0;	 gaussianMask[7][1] = 2;  gaussianMask[7][2] = 3;  gaussianMask[7][3] = 5;		gaussianMask[7][4] = 5;		gaussianMask[7][5] = 5;			gaussianMask[7][6] = 3;  gaussianMask[7][7] = 2;  gaussianMask[7][8] = 0;
//	gaussianMask[8][0] = 0;	 gaussianMask[8][1] = 0;  gaussianMask[8][2] = 3;  gaussianMask[8][3] = 2;		gaussianMask[8][4] = 2;		gaussianMask[8][5] = 2;			gaussianMask[8][6] = 3;  gaussianMask[8][7] = 0;  gaussianMask[8][8] = 0;
//
//
//	/*for(int i = 0; i < 5; ++i) {
//	for (int j = 0; j < 5; ++j) {
//	gaussianMask[i][j] /= sum;
//	std::cout << gaussianMask[i][j] << '\t';
//	}
//	std::cout << std::endl;
//	}*/
//
//	for (int i = 0; i < 9; ++i) {
//		for (int j = 0; j < 9; ++j) {
//			gaussianMask[i][j] /= sum;
//			std::cout << gaussianMask[i][j] << '\t';
//		}
//		std::cout << std::endl;
//	}
//
//	// Gaussian blur
//	// loop image pixels
//	//for (int row = 2; row < imageHeight - 2; row++) {
//	//	for (int col = 2; col < imageWidth - 2; col++) {
//	//		newPixel = 0;
//	//		// loop kernel
//	//		for (int rowOffset = -2; rowOffset <= 2; rowOffset++) {
//	//			for (int colOffset = -2; colOffset <= 2; colOffset++) {
//	//				//int rowTotal = row + rowOffset;
//	//				//int colTotal = col + colOffset;
//	//				//iOffset = (rowTotal * 3 * imageWidth + colTotal * 3);
//	//				newPixel += image.getPixel(row - colOffset, col - rowOffset) * gaussianMask[2 + rowOffset][2 + colOffset];
//
//	//			}
//	//		}
//
//	//		//int z = (row * 3 * imageWidth + col * 3);
//	//		//Intensity inten = newPixel / 159;
//	//		Intensity inten = newPixel;
//	//		//int posx = ((row-2)*imageWidth) + (col - 2);
//	//		//int posy = row;
//	//		cop->setPixel(row, col, inten);
//
//	//	}
//	//}
//
//	for (int row = 4; row < imageHeight - 4; row++) {
//		for (int col = 4; col < imageWidth - 4; col++) {
//			newPixel = 0;
//			// loop kernel
//			for (int rowOffset = -4; rowOffset <= 4; rowOffset++) {
//				for (int colOffset = -4; colOffset <= 4; colOffset++) {
//					//int rowTotal = row + rowOffset;
//					//int colTotal = col + colOffset;
//					//iOffset = (rowTotal * 3 * imageWidth + colTotal * 3);
//					newPixel += image.getPixel(row - colOffset, col - rowOffset) * gaussianMask[4 + rowOffset][4 + colOffset];
//
//				}
//			}
//
//			//int z = (row * 3 * imageWidth + col * 3);
//			//Intensity inten = newPixel / 159;
//			Intensity inten = newPixel;
//			//int posx = ((row-2)*imageWidth) + (col - 2);
//			//int posy = row;
//			cop->setPixel(row, col, inten);
//
//		}
//	}
//
//	std::cout << "klaar";
//
//	return cop;
//
//	//return nullptr;
//}
//
//IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
//	return nullptr;
//}