#include "ImagesLoader.h"
int main(int argc, char** argv)
{
	std::string path;
	std::string mask;
	float z;
	float epsilon;
	int isEightBoundaryConnected;
	
	if (argc != 6)
	{
		std::cout << "Error: call FillingHoles with the following arguments:" << std::endl
			      << "path-to-image, path-to-mask, z, epsilon, is-8-connectivity" << std::endl;
		return 0;
	}

	path = argv[1];
	mask = argv[2];
	z = atof(argv[3]);
	epsilon = atof(argv[4]);
	isEightBoundaryConnected = atoi(argv[5]);
	
	ImagesLoader imgLD(path, mask, z, epsilon, isEightBoundaryConnected == 1);
	imgLD.ShowImage();
	cv::waitKey(0);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
