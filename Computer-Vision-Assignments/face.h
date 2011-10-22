// Face class used for the face normalization program..

#include <cv.h>

using namespace cv;

class Face{

public:
      Face();
      Face( const Face& );
      float operator - ( const Face& );
      Face( int,int, int,int, int,int, int,int, int,int );
      Face& operator = ( const Face& );
      void print( const int );
      int* getCoordArr();
      int p1x, p1y,
          p2x, p2y,
          p3x, p3y,
          p4x, p4y,
          p5x, p5y;

      Mat F_x;
      Mat F_y;
      Mat F_xp;
      Mat F_yp;
      Mat F_c1;
      Mat F_c2; 

};

Face::Face(){
    F_x.create( 5 , 1 , CV_32FC1 );
    F_y.create( 5 , 1 , CV_32FC1 );
    F_xp.create( 5 , 1 , CV_32FC1 );
    F_yp.create( 5 , 1 , CV_32FC1 );
    F_c1.create( 3 , 1 , CV_32FC1 );
    F_c2.create( 3 , 1 , CV_32FC1 );
}

Face::Face( const Face& src ){
    F_x.create( 5 , 1 , CV_32FC1 );
    F_y.create( 5 , 1 , CV_32FC1 );
    F_xp.create( 5 , 1 , CV_32FC1 );
    F_yp.create( 5 , 1 , CV_32FC1 );
    F_c1.create( 3 , 1 , CV_32FC1 );
    F_c2.create( 3 , 1 , CV_32FC1 );
    
    F_x = src.F_x.clone();
    F_y = src.F_y.clone();
    F_yp = src.F_y.clone();
    F_xp = src.F_x.clone();
    F_c1 = src.F_c1.clone();
    F_c2 = src.F_c2.clone();

}

Face::Face( int i1x,int i1y, int i2x,int i2y, int i3x,int i3y, int i4x,int i4y, int i5x, int i5y ){

    F_x.create( 5 , 1 , CV_32FC1 );
    F_y.create( 5 , 1 , CV_32FC1 );
    F_xp.create( 5 , 1 , CV_32FC1 );
    F_yp.create( 5 , 1 , CV_32FC1 );
    F_c1.create( 3 , 1 , CV_32FC1 );
    F_c2.create( 3 , 1 , CV_32FC1 );
   
    p1x = i1x; p1y = i1y;
    p2x = i2x; p2y = i2y;
    p3x = i3x; p3y = i3y;
    p4x = i4x; p4y = i4y;
    p5x = i5x; p5y = i5y;
 
    F_x.at<float>( 0 , 0 ) = p1x; F_y.at<float>( 0 , 0 ) = p1y;
    F_x.at<float>( 1 , 0 ) = p2x; F_y.at<float>( 1 , 0 ) = p2y;
    F_x.at<float>( 2 , 0 ) = p3x; F_y.at<float>( 2 , 0 ) = p3y;
    F_x.at<float>( 3 , 0 ) = p4x; F_y.at<float>( 3 , 0 ) = p4y;
    F_x.at<float>( 4 , 0 ) = p5x; F_y.at<float>( 4 , 0 ) = p5y;

}

Face& Face::operator = ( const Face& rhs ){
    
    F_x.create( 5 , 1 , CV_32FC1 );
    F_y.create( 5 , 1 , CV_32FC1 );
    F_c1.create( 3 , 1 , CV_32FC1 );
    F_c2.create( 3 , 1 , CV_32FC1 );
    F_xp.create( 5 , 1 , CV_32FC1 );
    F_yp.create( 5 , 1 , CV_32FC1 );

    F_x = rhs.F_x.clone();
    F_y = rhs.F_y.clone();
    F_xp = rhs.F_xp.clone();
    F_yp = rhs.F_yp.clone();
    F_c1 = rhs.F_c1.clone();
    F_c2 = rhs.F_c2.clone();
    
    return *this;
}

float Face::operator - ( const Face& rhs ){
    // compute difference between two matrices in terms of their F_x and F_y 
    // components

    float diff = 0;
 
    for( int i = 0; i < 5; i++ ){
        diff += pow( F_x.at<float>( i , 0 ) - rhs.F_x.at<float>( i , 0 ) , 2 );
        diff += pow( F_y.at<float>( i , 0 ) - rhs.F_y.at<float>( i , 0 ) , 2 );
    }    

    return sqrt( diff );
}
   
    

int* Face::getCoordArr(){
    // Return an array of the point coordinates

    int *arr = new int[10];

    arr[0] = p1x; arr[1] = p1y;
    arr[2] = p2x; arr[3] = p2y;
    arr[4] = p3x; arr[5] = p3y;
    arr[6] = p4x; arr[7] = p4y;
    arr[8] = p5x; arr[9] = p5y;

    return arr;
}

void Face::print( const int flag ){
    // print F_x and F_y

    if( flag == 1 ){
        cout << endl;
        cout << F_x.at<float>( 0 , 0 ) << " " << F_y.at<float>( 0 , 0 ) << endl;
        cout << F_x.at<float>( 1 , 0 ) << " " << F_y.at<float>( 1 , 0 ) << endl;
        cout << F_x.at<float>( 2 , 0 ) << " " << F_y.at<float>( 2 , 0 ) << endl;
        cout << F_x.at<float>( 3 , 0 ) << " " << F_y.at<float>( 3 , 0 ) << endl;
        cout << F_x.at<float>( 4 , 0 ) << " " << F_y.at<float>( 4 , 0 ) << endl;
        cout << endl;
    }
    else if( flag == 2 ){
        cout << endl;
        cout << F_xp.at<float>( 0 , 0 ) << " " << F_yp.at<float>( 0 , 0 ) << endl;
        cout << F_xp.at<float>( 1 , 0 ) << " " << F_yp.at<float>( 1 , 0 ) << endl;
        cout << F_xp.at<float>( 2 , 0 ) << " " << F_yp.at<float>( 2 , 0 ) << endl;
        cout << F_xp.at<float>( 3 , 0 ) << " " << F_yp.at<float>( 3 , 0 ) << endl;
        cout << F_xp.at<float>( 4 , 0 ) << " " << F_yp.at<float>( 4 , 0 ) << endl;
        cout << endl;
    }
    else if( flag == 3 ){
        cout << endl;
        cout << F_c1.at<float>( 0 , 0 ) << " " << F_c1.at<float>( 1 , 0 ) << " " << F_c1.at<float>( 2 , 0 ) << endl;
        cout << F_c2.at<float>( 0 , 0 ) << " " << F_c2.at<float>( 1 , 0 ) << " " << F_c2.at<float>( 2 , 0 ) << endl;
        cout << endl;
    }
    else
        cout << "Invalid flag.." << endl;

    return;
} 
