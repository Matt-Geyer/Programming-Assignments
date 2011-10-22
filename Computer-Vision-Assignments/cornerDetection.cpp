#include <cv.h>
#include <iostream>
#include <highgui.h>
#include <vector>

using namespace std;
using namespace cv;

void gaussPyramid( Mat& );
void dogPyramid();
void detectCorners();
void nmSupressThresh( int );
void printPoints( string );
void dogMaxima();
void cvtToFloat( Mat& , Mat& );
void matchPoints();


// establish some parameters
const int SIGMA_MAX = 10;
const int SIGMA_ZERO = 2;
const int LEVELS = 12;
const float K = 1.4; //pow( ( SIGMA_MAX / SIGMA_ZERO ) , 1/LEVELS );
const float t1 = .2;
const int t2 = 0;
int num_points;

Mat G_SS[ LEVELS ];
Mat DOG_SS[ LEVELS - 1 ];
Mat corners[ LEVELS ];
Mat corners_2[ LEVELS ];
 
Mat orig;
Mat f_orig;

Mat orig2;
Mat f_orig2;

string in, out;

struct ip{
   int i, j, s;
};

vector<ip> ip1;
vector<ip> ip2;

int main( int argc , char **argv ){

    Mat temp = imread( argv[1] );
    temp.convertTo( orig , CV_32FC3 );
    cvtColor( orig , f_orig , CV_RGB2GRAY );
    orig = f_orig.clone();

    Mat temp2 = imread( argv[2] );
    temp2.convertTo( orig2 , CV_32FC3 );
    cvtColor( orig2 , f_orig , CV_RGB2GRAY );
    orig2 = f_orig.clone();
    
    if( argv[1] != NULL ) out = argv[2];
   
    for( int i = 0; i < LEVELS; i++ ){
        corners[i].create( orig.rows , orig.cols , CV_32FC1 );
    }

    // second image....
    // build gaussian pyramid with input image
    gaussPyramid( f_orig );
    // build difference of gaussian pyramid.. requires previous call to gaussPyramid
    dogPyramid();
    // detect corners using cornerHarris()
    detectCorners();
    // non-maxima supression and thresholding
    nmSupressThresh(0);
    // dog minima/maxima 
    dogMaxima();

    // copy over corners values.. that's why I do second image first
    for( int i = 0; i < LEVELS; i++ )
        corners_2[i] = corners[i].clone();

    f_orig = orig.clone();
    // build gaussian pyramid with input image
    gaussPyramid( f_orig );
    // build difference of gaussian pyramid.. requires previous call to gaussPyramid
    dogPyramid();
    // detect corners using cornerHarris()
    detectCorners();
    // non-maxima supression and thresholding
    nmSupressThresh(0);
    // dog minima/maxima 
    dogMaxima();

    matchPoints();
    // show interest points
    //printPoints( out );
 
    string filename = "0dog_circle.pgm";

    //namedWindow("corners");
    //for( int i = 1; i < LEVELS - 2; i++ ){
        //filename[0] = char( i + 48 );
        //imwrite( filename , DOG_SS[i] );
    //}
          
    //for( int i = 0; i < LEVELS - 1; i++ ){
    //    imshow("corners" , corners_2[i] * 1/255.0 );
    //    waitKey();
    //}

    return 0;
}

void detectCorners(){
    // detect corners in all levels of the gaussian pyramid
    // depends on existence of global gauss pyramid G_SS

    float sigma_i, sigma_d;
    int blockSize, appSize;

    for( int i = 0; i < LEVELS; i++ ){
        // determine sigma_i
        if( i == 0 ){
           sigma_i = SIGMA_ZERO;
        }
        else{
            sigma_i = SIGMA_ZERO * pow( K , i );
        } 
        
        sigma_d = sigma_i * 0.7;
        
        // blocksize is the nearest positive odd int to 5 * sigma_i
        blockSize = 3 * sigma_i;
        if( blockSize % 2 == 0 ) blockSize++;

        // appSize is the nearest positive odd int to 5 * sigma_d
        // the largest appSize is 31
        appSize = 5 * sigma_d;
        if( appSize % 2 == 0 ) appSize++;
        if( appSize > 31 ) appSize = 31;
     
        cornerHarris( f_orig , corners[i], blockSize ,  appSize , 0.04 );
        normalize( corners[i] , corners[i] , 100 , NORM_MINMAX );
        //imshow( "corners" , corners[i] );
        //waitKey();
    }
    return;
}


void nmSupressThresh( int thresh ){
    // perform non-maxima supression and thresholding to the detected corners

    bool ismax = true,
         ismin = true;

    
    for( int k = 0; k < LEVELS; k++ ){
    for( int i = 1; i < corners[k].rows - 1; i++ ){
        for( int j = 1; j < corners[k].cols - 1; j++ ){
            for( int a = -1; a < 2; a++ ){
                for( int b = -1; b < 2; b++ ){
                    // the value only gets kept if it is a max or a min
                    if( corners[k].at<float>( i + a , j + b ) > corners[k].at<float>( i , j ) ){
                        corners[k].at<float>( i , j ) = 0.0;
                    } 
                    else if( corners[k].at<float>( i , j ) < t1 ){
                        corners[k].at<float>( i , j ) = 0.0;
                    }
                }                    
            }
        }
    }
    }   
    return;
}

void dogMaxima(){    
    // perform non-maxima supression and thresholding to the detected corners
    // using the DOG_SS 
    
    bool ismax = true,
         ismin = true;

    for( int k = 1; k < LEVELS - 2; k++ ){
    // for every non-zero pixel in the corner matrix
    for( int i = 0; i < corners[k].rows; i++ ){
    for( int j = 0; j < corners[k].cols; j++ ){
        if( corners[k].at<float>( i , j ) != 0 ){
        // loop through the pixels 26 neighbors to see if it is a max
        for( int l = -1; l < 2; l++ ){
        for( int a = -1; a < 2; a++ ){
        for( int b = -1; b < 2; b++ ){
                    // check bounds
            if( i + a >= 0 && i + a < corners[k].rows && j + b >= 0 && j + b < corners[k].cols & a != 0 && b != 0){
            // the value only gets kept if it is a max or a min
            if( DOG_SS[k + l].at<float>( i + a , j + b ) >= DOG_SS[k].at<float>( i , j ) ){
                ismax = false;
            } 
            else if( DOG_SS[k + l].at<float>( i + a , j + b ) <= DOG_SS[k].at<float>( i , j ) ){
                ismin = false;
            }
            else if( DOG_SS[k].at<float>( i , j ) < t2 ){
                    //corners[k].at<float>( i , j ) = 0;
            }
        }
        }
        }
        }
        }
            if( !ismin && !ismax ){
                corners[k].at<float>( i , j ) = 0.0;
            }
            else{
                  //
            }
            ismin = ismax = true;
    }
    }
    }           
    
    return;
}

void gaussPyramid( Mat& src ){
    double sigma;

    for( int i = 0; i < LEVELS; i++ ){
        // determine sigma value for this level
        if( i == 0 ){
            sigma = SIGMA_ZERO;
        }
        else{
            sigma = SIGMA_ZERO * pow( K , i );
        }
        // apply gauss with the given sigma        
        GaussianBlur( src , G_SS[i] , Size( 0 , 0 ) , sigma , 0 , BORDER_DEFAULT );
    }
    return;
}

void dogPyramid(){
    // depends on existence of global G_SS array pre-populated with levels of the gauss pyramid
 
    for( int i = 0; i < LEVELS - 1; i++ ){
        DOG_SS[i] = G_SS[i] - G_SS[ i + 1];
    }
    return;
}

void matchPoints(){
    // match points across two images..
   
    Mat b1to3( 3 , 3 , CV_32FC1 );
    Mat pix1( 1 , 3 , CV_32FC1 );
    pix1.at<float>( 0 , 2 ) = 1;
    Mat pix1p( 1 , 3 , CV_32FC1 );

    ip *ipptr;

    b1to3.at<float>( 0 , 0 ) = 5.6887079e-1;
    b1to3.at<float>( 1 , 0 ) = -4.6783159e-1;
    b1to3.at<float>( 2 , 0 ) = 6.4697420e-06;
    b1to3.at<float>( 0 , 1 ) = 4.6997572e-1;
    b1to3.at<float>( 1 , 1 ) = 5.6548769e-1;
    b1to3.at<float>( 2 , 1 ) = -1.1704138e-6;
    b1to3.at<float>( 0 , 2 ) = 2.551564e+1;
    b1to3.at<float>( 1 , 2 ) = 3.4819925e2;
    b1to3.at<float>( 2 , 2 ) = 1.f;

    // fill ip vectors
    for( int k = 1; k < LEVELS - 2; k++ ){
        for( int i = 1; i < corners[k].rows - 1; i++ ){
            for( int j = 1; j < corners[k].rows - 1; j++ ){
            
                if( corners[k].at<float>( i , j ) > 0 ){
                    ipptr = new ip;
                    ipptr->i = i;
                    ipptr->j = j;
                    ipptr->s = k;
                    ip1.push_back( *ipptr );
                }
                if( corners_2[k].at<float>( i , j ) > 0 ){
                    ipptr = new ip;
                    ipptr->i = i;
                    ipptr->j = j;
                    ipptr->s = k;
                    ip2.push_back( *ipptr );
                }
            }
        }
    }
 
    float err;
    float i1 , j1;

    cout << ip1.size() << endl;
    cout << ip2.size() << endl;

    // for every interest point in image 1
    for( int i = 0; i < ip1.size(); i++ ){
            pix1.at<float>( 0 , 0 ) = ip1.at( i ).j;
            pix1.at<float>( 0 , 1 ) = ip1.at( i ).i;
            pix1p = pix1 * b1to3;

        // for every interest point in image 2
        for( int j = 0; j < ip2.size(); j++ ){
            i1 = ip2.at( j ).i - pix1p.at<float>( 0 , 1 );
            j1 = ip2.at( j ).j - pix1p.at<float>( 0 , 0 );

            err = sqrt( (i1 * i1) + (j1 * j1 ) );
        }
    }

    /*
    circle( orig , Point( 600 , 200 ) , 20 , Scalar( 255 , 0 , 0 ) , 1 );
    imshow( "lol" , orig * 1/255.0 );

    pix1.at<float>( 0 , 0 ) = 600;
    pix1.at<float>( 0 , 1 ) = 200;

    pix1p = pix1 * b1to3;
    circle( orig2 , Point( (int)pix1p.at<float>( 0 , 0 ) , (int)pix1p.at<float>( 0 , 0 ) ) , 20 , Scalar( 255 , 0 , 0 ) , 1 );
    imshow("lol2" , orig2 *1/255.0 );

    waitKey();       
            
   */      
    

                

    return;
}

void printPoints( string out ){
    // display the images with the detected points
  
    float sigma = SIGMA_ZERO;
   
    for( int k = 1; k < LEVELS - 2; k ++ ){
        // determine sigma value for this level
        if( k == 0 ){
            sigma = SIGMA_ZERO;
        }
        else{
            sigma = SIGMA_ZERO * pow( K , k );
        }
    for( int i = 1; i < orig.rows - 1; i++ ){
        for( int j = 1; j < orig.cols - 1; j++ ){
            // draw a circle at each points
            if( corners[k].at<float>( i , j ) > 0 ){
                circle( orig , Point( j , i ) , 1 , Scalar( 255 ,0 , 0 ) , 1 );
                circle( orig , Point( j , i ) , sigma , Scalar( 255 , 255 , 0 ) , 1 );           
            }
        } 
    }
    }
    
    namedWindow( "Points" );
    imshow( "Points" , orig * 1/255.0 );
    imwrite( out , orig  );
    waitKey();
    return;
}
