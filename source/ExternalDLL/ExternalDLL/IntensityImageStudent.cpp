#include "IntensityImageStudent.h"
#include <iostream>

IntensityImageStudent::IntensityImageStudent() : IntensityImage() {
}

IntensityImageStudent::IntensityImageStudent(const IntensityImageStudent &other) : 
IntensityImage(other.getWidth(), other.getHeight()), 
intensity(new Intensity[other.getWidth()*other.getHeight()])
{
	for (int i = 0; i < (other.getWidth()*other.getHeight()); i++){
		intensity[i] = other.intensity[i];
	}
	//TODO: Create a copy from the other object
}

IntensityImageStudent::IntensityImageStudent(const int width, const int height) : 
IntensityImage(width, height), 
intensity(new Intensity[width*height]) {
	//int throwError = 0, e = 1 / throwError;
	//TODO: Initialize pixel storage
}

IntensityImageStudent::~IntensityImageStudent() {
	delete[] intensity;
	//int throwError = 0, e = 1 / throwError;
	//TODO: delete allocated objects
}

void IntensityImageStudent::set(const int width, const int height) {
	IntensityImage::set(width, height);
	delete[] intensity;
	intensity = new(Intensity[width*height]);
	//int throwError = 0, e = 1 / throwError;
	//TODO: resize or create a new pixel storage (Don't forget to delete the old storage)
}

void IntensityImageStudent::set(const IntensityImageStudent &other) {
	IntensityImage::set(other.getWidth(), other.getHeight());
	delete[] intensity;
	intensity = new(Intensity[other.getWidth()*other.getHeight()]);
	for (int i = 0; i < other.getWidth()*other.getHeight(); i++){
		intensity[i] = other.intensity[i];
	}
	//int throwError = 0, e = 1 / throwError;
	//TODO: resize or create a new pixel storage and copy the object (Don't forget to delete the old storage)
}

void IntensityImageStudent::setPixel(int x, int y, Intensity pixel) {
	intensity[y*getWidth() + x] = pixel;
	//std::cout << "Pixel: " << intensity[y*x] << std::endl;
	//int throwError = 0, e = 1 / throwError;
	//TODO: no comment needed :)
}

void IntensityImageStudent::setPixel(int i, Intensity pixel) {
	intensity[i] = pixel;
	//int throwError = 0, e = 1 / throwError;
	/*
	* TODO: set pixel i in "Row-Major Order"
	*
	*
	* Original 2d image (values):
	* 9 1 2
	* 4 3 5
	* 8 7 8
	*
	* 1d representation (i, value):
	* i		value
	* 0		9
	* 1		1
	* 2		2
	* 3		4
	* 4		3
	* 5		5
	* 6		8
	* 7		7
	* 8		8
	*/
}

Intensity IntensityImageStudent::getPixel(int x, int y) const {
	return intensity[y*getWidth() + x];
	//int throwError = 0, e = 1 / throwError;
	//TODO: no comment needed :)
	//return 0;
}

Intensity IntensityImageStudent::getPixel(int i) const {
	//int throwError = 0, e = 1 / throwError;
	//TODO: see setPixel(int i, RGB pixel)
	return intensity[i];
	//return 0;
}