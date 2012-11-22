/******************************************************************************
 * robot Class.
 * Specification file for robot class. 
 * This class finds a robot in image inputs
 *  *****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "image.h"

using namespace std;
class Robot
{
	private:
		int centerX;
		int centerY;
		double angle;
		vector<int> circleX;
		vector<int> circleY;
	public:
		Robot();
		
		int getCenterX() const { return centerX; }
		int getCenterY() const { return centerY; }
		double getAngle() const { return angle; }
		
		void filter(ImageType&);
		void threshold(ImageType&, int);
		void findCircles(ImageType&, int);
		void growCircles(ImageType&);
		void doCalc(ofstream& );
		void printCircles();
		void clearCircles();
};

