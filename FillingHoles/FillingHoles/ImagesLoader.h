#pragma once
#include <string>
#include<opencv2/opencv.hpp>
constexpr int IMAGE_TYPE_FLOAT32 = 5;
class ImagesLoader
{
private:

	int imageCounter;
	float holeVal;
	float z;
	float epsilon;
	bool isEightBoundaryConnected;
	cv::Mat image;
	cv::Mat mask;
	using pixel = std::array<uint, 2>;

	//Returns if pixel defineds as hole.
	bool IsHolePixel(uint x, uint y) const;

	//Calculates the weight of two pixels
	float Weight(uint x1, uint y1, uint x2, uint y2)const; 
	//Returns the coordinates of the boundary's pixels
	std::vector<pixel> GetBoundaryPixels() const;

	//Loading a mask image wich defines the hole by black pixels and merge it with the original image
	void LoadMask(std::string path);
	//Fills the image's hole by the given algorithm
	void FillHoles(); 
public:
	ImagesLoader(std::string imagePath, std::string maskPath, float z, float epsilon, bool isEightBoundaryConnected, float holeVal = 0);
	//Shows the image on a window
	void ShowImage();

	void GetImage(cv::Mat& mat) const; 

};

