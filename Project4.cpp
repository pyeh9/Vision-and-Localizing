#include "Robot.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "image.h"
#include <cstdlib>
#include <string>
using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

//int readImageHeader(string, int&, int&, int&, bool&);
//int readImage(string, ImageType&);
//int writeImage(string, ImageType&);

int main()
{
	int i, j; 
 	int M, N, Q;
 	bool type;
 	int val;
 	int thresh;
	string outputfilename;
	int count = 0;
	
	ifstream inputFile("input.dat");
	ofstream outputFile("output.dat");
	string line;
	inputFile >> line;
	Robot rob = Robot();
	
	outputfilename = "testout .pgm";
	readImageHeader((char*)line.c_str(), N, M, Q, type);
	ImageType image(N, M, Q);
	while(line != "ENDOFINPUT"){
	 	//rob.readLine(line);
	 	
		readImage((char*)line.c_str(), image);
		
		// 2 stage filter:
		// 1 - isolates circles, possibly with some artifacts
		// 2 - attempts to get rid of artifacts
		rob.filter(image);
		rob.threshold(image, 155); // 155 known a priori
		rob.filter(image);
		rob.threshold(image, 255);
		
		for(int i = 0; i<7; i++) rob.growCircles(image); // should find better solution
	
		rob.findCircles(image, 3);
		//cout << "Circle" << count << ": "; 
		//rob.printCircles();
		rob.clearCircles();
		outputFile << "For the robot in " << line << ":\n";
		rob.doCalc(outputFile);
				
		//outputfilename[outputfilename.length()-5] = '0'+count;
		//writeImage((char*)outputfilename.c_str(), image);
		inputFile >> line;
		count++;
	}
	return 0;
}

/****** readImage ************************************************************
 * reads in image specified by filename into image array
 *****************************************************************************/
int readImage(char fname[], ImageType& image)
{
	int i, j;
	int N, M, Q;
 	unsigned int intImage;
 	char header [100], *ptr;
 	ifstream ifp;
 	int val;

	ifp.open(fname, ios::in);

 	if (!ifp) {
   		cout << "Can't read image: " << fname << endl;
   		exit(1);
 	}

 	// read header
 	ifp.getline(header,100,'\n');
	if ( (header[0]!='P') || (header[1]!='2') ) {   /* '2' */
    	cout << "Image " << fname << " is not PGM" << endl;
    	exit(1);
 	}

	ifp.getline(header,100,'\n');
 	while(header[0]=='#') ifp.getline(header,100,'\n');

 	M=strtol(header,&ptr,0);
 	N=atoi(ptr);

 	ifp.getline(header,100,'\n');
 	Q=strtol(header,&ptr,0);
 
 	for(i=0; i<N; i++)
   		for(j=0; j<M; j++) {
     		ifp >> intImage;
	 		val = intImage;
		    image.setPixelVal(i, j, val);   
   		}
 	ifp.close();

 	return (1);
}

/****** readImageHeader*******************************************************
 * reads in image header - returns rows, cols, gray levels 
 *****************************************************************************/
int readImageHeader(char fname[], int& N, int& M, int& Q, bool& type)
{
	int i, j;
 	unsigned char *charImage;
 	char header [100], *ptr;
 	ifstream ifp;

 	ifp.open(fname, ios::in);
 	if (!ifp) {
   		cout << "Can't read image: " << fname << endl;
   		exit(1);
 	}

 	// read header
 	type = false; // PGM

 	ifp.getline(header,100,'\n');
 	if ( (header[0] == 'P') && (header[1]== '2') ) {  /* '2' */
    	type = false;
 	} else if ( (header[0] == 80) &&  /* 'P' */
    	(header[1] == 54) ) {        /* '6' */
     	type = true;
 	} else {
   		cout << "Image " << fname << " is not PGM or PPM" << endl;
   		exit(1);
 	}

	ifp.getline(header,100,'\n');
 	while(header[0]=='#') ifp.getline(header,100,'\n');
	
	M=strtol(header,&ptr,0);
 	N=atoi(ptr);
	ifp.getline(header,100,'\n');
	Q=strtol(header,&ptr,0);
	ifp.close();
	return(1);
}

/****** writeImage ***********************************************************
 * reads in image specified by filename into image array
 *****************************************************************************/
 int writeImage(char fname[], ImageType& image)
 {
 	int i, j;
 	int N, M, Q;
 	ofstream ofp;
 	int val;

 	image.getImageInfo(N, M, Q);

 	ofp.open(fname, ios::out);
 
 	ofp << "P2" << endl; // P2 indicates gray scale image
 	ofp << M << " " << N << endl;
 	ofp << Q << endl;

 	if (!ofp) {
   		cout << "Can't open file: " << fname << endl;
   		exit(1);
 	}

 	for(i=0; i<N; i++) {
   		for(j=0; j<M; j++) {
     		image.getPixelVal(i, j, val);
     		if (j<M-1) ofp << (unsigned int) val << " ";
	 		else ofp << (unsigned int) val;
	 	}
	 	if (i<N-1) ofp << endl;
   	}

 	ofp.close();
 	return(1);
 }
