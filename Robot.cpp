#include "Robot.h"
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>

/******************************************************************************
 * robot Class
 * Implementation and methods for robot class. 
 * This class finds a robot in image inputs
 *****************************************************************************/
 Robot::Robot()
 {
 }

 
/****** filter(ImageType&) ****************************************************
 * filters an image with a simple averaging filter
 *****************************************************************************/
 void Robot::filter(ImageType& image)
 {
 	int N2, M2, Q2;
 	image.getImageInfo(N2,M2,Q2);
 	ImageType copyImg(N2+2, M2+2, Q2);
 	int i,j, val;
 	
 	int filter[3][3] = {{1, 1, 1},
 					 	{1, 1, 1},
 					 	{1, 1, 1}};
	int windowx = 3;
	int windowy = 3;
	int windowsize = windowx*windowy;
	int temp;
	
 	for(i=0; i<N2; i++){
		for(j=0; j<M2; j++){
			image.getPixelVal(i, j, val);
			copyImg.setPixelVal(i+1, j+1, val);
		}
 	}
	// border padding
	for(j=0; j<M2+2; j++){ // first row
		copyImg.getPixelVal(1, j, val);
		copyImg.setPixelVal(0, j, val);
	} 
	for(j=0; j<M2+2; j++){ // last row 
	 	copyImg.getPixelVal(N2, j, val);
	 	copyImg.setPixelVal(N2+1, j, val);
	}
	for(i=0; i<N2+2; i++){ // first column
	 	copyImg.getPixelVal(i, 1, val);
	 	copyImg.setPixelVal(i, 0, val);
	}
	for(i=0; i<N2+2; i++){ // last column
		copyImg.getPixelVal(i, M2, val);
	 	copyImg.setPixelVal(i, M2+1, val);
	}
	
	// filter padded image
	for(i=1; i<N2+1; i++){
	 	for(j=1; j<M2+1; j++){
	 		temp=0;
	 		for(int di=-1; di<2; di++){
	 			for(int dj=-1; dj<2; dj++){
	 				copyImg.getPixelVal(i+di,j+dj,val);
	 				temp+=val*filter[di+1][dj+1];
	 			}
	 		}
	 		temp/=windowsize;
	 		copyImg.setPixelVal(i,j,temp);
	 	}
	}
	// copy back
	for(i=0; i<N2; i++){
		for(j=0; j<M2; j++){
			copyImg.getPixelVal(i+1, j+1, val);
			image.setPixelVal(i, j, val);
		}
 	}
 }
 
/****** threshold(ImageType&, int ) *******************************************
 * sets all pixels within a range of a specified threshold to 255 - white
 *****************************************************************************/
 void Robot::threshold(ImageType& image, int T)
 {
 	int N, M, Q, val;
 	image.getImageInfo(N,M,Q);
 	for(int i=0; i<N; i++){
   		for(int j=0; j<M; j++) {
       		image.getPixelVal(i,j,val);
     		if(abs(val - T) < 25) 
     			image.setPixelVal(i,j,255);
     		else
   				image.setPixelVal(i,j,0);
   		}
 	}
 }

/****** findCircles(ImageType&, int ) ****************************************
 * sets all pixels within a range of a specified threshold to 255 - white
 *****************************************************************************/
 void Robot::findCircles(ImageType& image, int num)
 {
 	int foundCircles = 0;
 	int N, M, Q, val;
 	image.getImageInfo(N,M,Q);
 	int i, j; 
 	int curR, curC, tempR, tempC, maxR, maxC, minR, minC;
 	int dr, dc;
 	int done;
 	while(foundCircles != num){
 		// scans each line, stopping at the first white pixel
 		done = 0;
 		for(i=0; i<N; i++){
 			if(done) break;
 			for(j=0; j<M; j++){
 				image.getPixelVal(i,j,val);
 				if(val==255){
 					minR = i;
 					minR = i;  
 					minC = j; 
 					done = 1;
 					break;
 				}
 			}
 		}
 		
 		image.getPixelVal(minR, minC, val);
		dc = 0;
 		while(val==255){
 			dc++;
 			image.getPixelVal(minR, minC+dc, val);
 		}
 		maxC = minC+dc-1;
 		image.getPixelVal(minR, minC, val);
		dr = 0;
 		while(val==255){
 			dr++;
 			image.getPixelVal(minR+dr, minC, val);
 		}
 		maxR = minR+dr-1;
 		//printf("(minR, minC):(%d,%d), (maxR, maxC):(%d,%d)\n", minR, minC, maxR, maxC);
 		for(i=minR; i<=maxR; i++){
 			for(j=minC; j<=maxC; j++){
 				image.setPixelVal(i, j, 0);
 			}
 		}
 		
 		circleY.push_back((minR+maxR)/2);
 		circleX.push_back((minC+maxC)/2);
 		foundCircles++;
 	}	
 }
 
/****** growCircles(ImageType&) **********************************************
 * fills circles to the smallest box that contains it
 *****************************************************************************/
 void Robot::growCircles(ImageType& image){
 	int N, M, Q, val, val2, val3;
 	image.getImageInfo(N,M,Q);
 	int i, j;
 	for(i=1; i<N-1; i++){
 		for(j=1; j<N-1; j++){
 			image.getPixelVal(i,j,val);
 			if(val==0){
 				image.getPixelVal(i+1,j,val2); 
 				image.getPixelVal(i,j+1,val3);
 				if(val2 == 255 && val3 == 255){ // right and bottom neighbors
 					image.setPixelVal(i,j, 255);
 					continue;
 				} 
 				image.getPixelVal(i+1,j,val2);
 				image.getPixelVal(i,j-1,val3);
 				if(val2 == 255 && val3 == 255){ // left and bottom neighbors
 					image.setPixelVal(i,j, 255);
 					continue;
 				} 
 				image.getPixelVal(i-1,j,val2); 
 				image.getPixelVal(i,j+1,val3);
 				if(val2 == 255 && val3 == 255){ // right and top neighbors
 					image.setPixelVal(i,j, 255);
 					continue;
 				} 
 				image.getPixelVal(i-1,j,val2);
 				image.getPixelVal(i,j-1,val3);
 				if(val2 == 255 && val3 == 255){ // left and top neighbors
 					image.setPixelVal(i,j, 255);
 					continue;
 				} 
 				
 			}
 		}
 	}
 }
 
 /****** printCircles() ******************************************************
 * 
 *****************************************************************************/
 void Robot::printCircles()
 {
 	cout << "centers: ";
 	for(int i=0; i<circleX.size(); i++){
 		cout << "(" << circleX[i] << "," << circleY[i] << ") "; 
 	}
 	cout << endl;
 }
 
 /****** clearCircles() ******************************************************
 * 
 *****************************************************************************/
 void Robot::clearCircles()
 {
 	circleX.clear();
 	circleY.clear();
 }
 
 /****** doCalc(ofstream&) ***************************************************
 * perform the final calculations - orientation and center of robot
 * and prints it to output
 *****************************************************************************/
 void Robot::doCalc(ofstream& file)
 {
 	int line01 = pow(circleX[0]-circleX[1],2)+pow(circleY[0]-circleY[1],2);
 	int line12 = pow(circleX[1]-circleX[2],2)+pow(circleY[1]-circleY[2],2);
 	int line02 = pow(circleX[0]-circleX[2],2)+pow(circleY[0]-circleY[2],2);
 	//printf("Triangle lengths: %d, %d, and %d. ", line01, line12, line02);
 	int baseMidX, baseMidY;
 	double pi = atan(1)*4;
 	// find which segment is the base, and extend altitude from midpoint
 	if(line01<line12 && line01<line02){ // line01 is the base
 		baseMidX = (circleX[0]+circleX[1])/2;
 		baseMidY = (circleY[0]+circleY[1])/2;
 		centerX = (baseMidX+circleX[2])/2;
 		centerY = (baseMidY+circleY[2])/2;
 		angle = atan2(circleY[2]-centerY, circleX[2]-centerX);
 	} else if(line12<line01 && line12<line02){ // line12 is base
 		baseMidX = (circleX[1]+circleX[2])/2;
 		baseMidY = (circleY[1]+circleY[2])/2;
 		centerX = (baseMidX+circleX[0])/2;
 		centerY = (baseMidY+circleY[0])/2;
 		angle = atan2(circleY[0]-centerY, circleX[0]-centerX);
 	} else if(line02<line12 && line02<line01){ // line02 is base
 		baseMidX = (circleX[0]+circleX[2])/2;
 		baseMidY = (circleY[0]+circleY[2])/2;
 		centerX = (baseMidX+circleX[1])/2;
 		centerY = (baseMidY+circleY[1])/2;
 		angle = atan2(circleY[1]-centerY, circleX[1]-centerX);
 	}
 	angle=(angle*180/pi);
 	while(angle<0) angle+=360.0;
 	
 	file << "\tRobot center (X,Y): (" << centerX << "," << centerY << "), angle: " << (int)angle << " degrees.\n\n";
 }
 
