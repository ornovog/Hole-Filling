#include "ImagesLoader.h"

//Returns if pixel defineds as hole.
bool ImagesLoader::IsHolePixel(uint x, uint y) const {
	return mask.at<float>(x, y) == holeVal;
}

//Calculates the weight of two pixels
float ImagesLoader::Weight(uint x1, uint y1, uint x2, uint y2) const {
	return 1 / (std::powf((float)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)), z / 2.0) + epsilon);
}

//Returns the coordinates of the boundary's pixels
std::vector<ImagesLoader::pixel> ImagesLoader::GetBoundaryPixels() const {
	std::vector<pixel> result;
	std::vector<char> IsBoundary(long(image.cols * image.rows));

	 auto PixelToIndex = [&](int x, int y) {
		return y * image.rows + x;
	};

	 // In the exercise specification sheet we assumed no holes on image's boundaries. 
	for (uint i = 1; i < image.rows - 1; i++) {
		for (uint j = 1; j < image.cols - 1; j++) {
			if (IsHolePixel(i, j)) {

				 auto MarkBoundary  = [&](int di, int dj) {
					if (!IsHolePixel(i + di, j + dj)) {
						IsBoundary[PixelToIndex(i + di, j + dj)] = true;
					}
				};

				if (isEightBoundaryConnected) {
					for (int di : {-1, 1}) {
						for (int dj : {-1, 1}) {
							MarkBoundary(di, dj);
						}
					}
				}

				for (int di : {-1, 1}) {
					MarkBoundary(di, 0);
				}
				for (int dj : {-1, 1}) {
					MarkBoundary(0, dj);
				}
			}
		}
	}

	for (uint i = 0; i < image.rows; i++) {
		for (uint j = 0; j < image.cols; j++) {

			if (IsBoundary[PixelToIndex(i, j)]) {
				result.push_back({ i,j });
			}
		}
	}
	return result;
}

//Loading a mask image wich defines the hole by black pixels and merge it with the original image
void ImagesLoader::LoadMask(std::string path) {
	cv::Mat charMask = cv::imread(path, cv::IMREAD_GRAYSCALE);
	charMask.convertTo(mask, IMAGE_TYPE_FLOAT32, 1 / 255.);
}

//Fills the image's hole by the given algorithm
void ImagesLoader::FillHoles() {
	auto boundary = GetBoundaryPixels();
	for (uint i = 1; i < image.rows - 1; i++) {
		for (uint j = 1; j < image.cols - 1; j++) {

			if (IsHolePixel(i, j)) {
				float sumW = 0;
				float sumWI = 0;
				for (pixel v : boundary) {
					float w = Weight(i, j, v[0], v[1]);
					sumW += w;
					sumWI += (w * image.at<float>(v[0], v[1]));
				}

				// We are guaranteed SumW > 0
				image.at<float>(i, j) = (sumWI / sumW);
			}
		}
	}

}

ImagesLoader::ImagesLoader(std::string imagePath, std::string maskPath, float z, float epsilon, bool isEightBoundaryConnected, float holeVal) {

		this->isEightBoundaryConnected = isEightBoundaryConnected;
		this->holeVal = holeVal;
		this->z = z;
		this->epsilon = epsilon;
		cv::Mat charImage = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
		if (charImage.data == NULL) {
			std::cout << "Couldn't load image." << std::endl;
			return;
		}

		charImage.convertTo(image, IMAGE_TYPE_FLOAT32, 1 / 255.);
		LoadMask(maskPath);

		if (mask.data == NULL) {
			std::cout << "Couldn't load mask." << std::endl;
			return;
		}

		if (mask.rows != image.rows || mask.cols != image.cols) {
			std::cout << "Image and mask are not at the same size." << std::endl;
			return;
		}
		FillHoles();
	}

	//Shows the image on a window
	void ImagesLoader::ShowImage() {
		std::string windowName = "image" + std::to_string(++imageCounter);
		cv::namedWindow(windowName, cv::WINDOW_NORMAL);
		cv::imshow(windowName, image);
	}

	void ImagesLoader::GetImage(cv::Mat & mat)const {
		mat = image;
	}





