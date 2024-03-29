#ifndef IMAGE_H
#define IMAGE_H

// an example - you need to add more funtions

class ImageType {
 public:
   ImageType();
   ImageType(int, int, int);
   ImageType(ImageType&);
   ~ImageType();
   void getImageInfo(int&, int&, int&);
   void setImageInfo(int, int, int);
   void setPixelVal(int, int, int);
   void getPixelVal(int, int, int&);
 private:
   int N, M, Q;
   int **pixelValue;
};

#endif
