// Matt Geyer 
// CS 485 Computer Vision
//
// This program does Gaussian Pyramids, Laplacian Pyramids
// and Multi-scale Edge Detection
//
//
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <string>

using namespace cv;
using namespace std;

float* gauss( float *h , float sigma, int &hSize);
void apply_gauss( Mat& src, Mat& dst, float sigma ); 
void apply_lap( Mat& src, Mat& dst );
void med( Mat& src, Mat& dst , int thresh);

int hSize = 0;

int main( int argc, char** argv ){
       
    // Load images and convert to grayscale
    Mat tmp = imread( "lapbother5.pgm" );
    Mat img;
    cvtColor( tmp, img, CV_BGR2GRAY );
    tmp = img.clone();
    Mat lap = img.clone();    
    Mat msed = img.clone();

    med( tmp , msed ,5.5 );
    namedWindow("ed");
    imshow("ed" , msed );
    imwrite( "med_sf5_5p5.pgm" , msed );
    waitKey();
  
   
    
    // Base strings for saving / showing to the window
    string lapb = "lapblenna0.pgm";
    string gpb = "gpbother0.pgm";
    
    // Build Part B pyramids.. I didn't have time to implement a menu
    // So things need to be commented or uncommented as required for the 
    // different effects
    for( int i = 1; i < 6; i++ ){
        // Apply a gauss mask with the passed sigma.. ie sqrt( 1 ) , sqrt( 2 )... etc
        //apply_gauss( tmp, img, (float)sqrt( i ) );
        apply_gauss( tmp , img , 1.0 );
        // Subtract two gaussian images to approximate laplacian 
        subtract( img, tmp, lap );
        tmp = img.clone();
        normalize( lap , lap , 0 , 255 , NORM_MINMAX );
        // Update strings to make new filenames
        //gpb[8] = char(48 + i );
        lapb[9] = char(48 + i);
        
        // Different Write/Show operations.. uncomment as needed
        //namedWindow( gpb );
        namedWindow( lapb );
        imshow( lapb , lap );
        imwrite( lapb, lap );
        //imshow( gpb , img );
        //imwrite( gpb, img );
        
        // Report sigma and mask size 
        //cout << "Sigma is " << (float)sqrt( i ) << " mask size is " << hSize << endl;
        waitKey();
    }

    // Laplacian and Gaussian Pyramid part a code
    string gpa = "gpaother0.pgm";
    string lapa = "lapaother0.pgm";
    for( int i = 0; i < 5; i++ ){
        // Apply gauss or laplacian to the image.. sigma is kept static at 1.0
        apply_gauss( tmp , img , 1.0 );
        apply_lap( img , lap );
        tmp = img.clone();
     
        gpa[8] = char(49 + i);
        lapa[9] = char(49 + i);
       
        //namedWindow( lapa );
        //imshow( lapa , lap );
        //namedWindow( gpa );
        //imshow( "Control" , stable );
        //imshow( gpa , img );
        //imwrite(gpa , img);
        //imwrite( lapa , lap );
        //waitKey();
     }
    
  
    return 0;
}

void apply_gauss( Mat& src, Mat& dst, float sigma ){
    // Apply a gaussian mask to the src image with the
    // passed standard deviation

    //int hSize = 0;
    float *mask, sum;
    // get a gaussian mask     
    mask = gauss( mask , sigma, hSize );
    Mat tmp = src.clone();

    // 1D gaussian across rows
    for( int i = 0; i < src.rows; i++ ){
        for( int j = 0; j < src.cols; j++ ){

            for( int a = floor( double(-hSize/2) ); a <= floor( double(hSize/2)); a++ ){
                if( j + a >= 0 && j + a < src.cols ){
                    sum += (int)src.at<uchar>(i, j + a) * mask[ a + hSize/2 ];
                }
            }
            tmp.at<uchar>(i,j) = sum;
            sum = 0;
        }
    }
   
    // 1D Gaussian across cols of resulting image from previous 
    sum = 0;
    for( int j = 0; j < tmp.cols; j++ ){
        for( int i = 0; i < tmp.rows; i++ ){

            for( int a = floor( double(-hSize/2)); a <= floor( double(hSize/2)); a++ ){
                if( i + a >= 0 && i + a < tmp.rows){
                    sum += (int)tmp.at<uchar>(i + a, j) * mask[a + hSize/2 ]; 
                }
            }
            dst.at<uchar>(i,j) = sum;
            sum = 0;
        }
    }

    // normalize 
    normalize( dst , dst , 0 , 255, NORM_MINMAX );
    return;
}

void apply_lap( Mat& src , Mat& dst ){
    // Apply the laplacian to an image

    int mSize = 3;
    float mask[9] = { -0.125 , -0.125 , -0.125,
                      -0.125 ,  1.000 , -0.125,
                      -0.125 , -0.125 , -0.125 };

    float sum = 0;

    for( int i = 0; i < src.rows; i++ ){
        for( int j = 0; j < src.cols; j++ ){
   
            for( int a = floor( double(-mSize/2)); a <= floor( double(mSize/2)); a++ ){
            for( int b = floor( double(-mSize/2)); b <= floor( double(mSize/2)); b++ ){
                if( i + a >= 0 && i + a < src.rows && j + b >= 0 && j + b < src.cols ){
                   sum += (int)src.at<uchar>(i + a, j + b) * mask[ mSize * (mSize/2 + a ) + (mSize/2 + b)];
                }
            }
            }
            dst.at<uchar>(i,j) = sum; 
            sum = 0;
        }
    }
    normalize( dst , dst , 0 , 255 , NORM_MINMAX );
    return;
}


float*  gauss( float *h, float sigma, int &hSize ){
    // create a 1D gauss sample 
    // Code taken from slides..
 
    int i;
    float cst, tssq, x, sum;
    float PI = 3.14159265;
    
    int halfSize = (int)(2.5 * sigma );
    hSize = 2 * halfSize;
    if( hSize % 2 == 0 ) hSize++;
    
    h = new float[ hSize ]; 
   
    cst = 1 / ( sigma * sqrt( 2.0 * PI ) );
    
    tssq = 1 / ( 2 * sigma * sigma );

    for( int i = 0; i < hSize; i++ ){
        x = (float)( i - hSize / 2 );
        h[i] = ( cst * exp( -( x * x * tssq ) ) );
    }

    sum = 0;
   
    for( int i = 0; i < hSize; i++ )
        sum += h[i];
 
    for( int i = 0; i < hSize; i++ )
       h[i] /= sum; 
       
    

    return h;
}

void med( Mat& src , Mat& dst , int thresh ){
    // Multi-scale Edge Detection method


    float mean, var;
    int k = 0;
    
    // array to store locations of zero crossings..
    int **zerox = new int*[src.rows];
    for( int i = 0; i < src.rows; i++ ){
        zerox[i] = new int[src.cols];
    }
   
    // initialize all values to 0
    for( int i = 0; i < src.rows; i++ ){
        for( int j = 0; j < src.cols; j++ ){
            zerox[i][j] = 0;
        }
    } 
    
    // Segment Second Order Derivative Image
    for( int i = 0; i < 256; i++ ){
        for( int j = 0; j < 256; j++){
            if( src.at<uchar>(i,j) <= 0 ) dst.at<uchar>(i,j) = 0;
            else if( src.at<uchar>(i,j) > 0 ) dst.at<uchar>(i,j) = 1;
        }
     }

    
     // Detect Zero crossings 
    for( int i = 0; i < dst.rows; i++ ){
        for( int j = 0; j < dst.cols; j++ ){
            for( int a = -1; a <= 1; a++ ){
                for( int b = -1; b <= 1; b++ ){
                    // check bounds
                    if( i + a >= 0 && i + a < dst.rows && j + b >= 0 && j + b < dst.cols ){
                        // if a pixel has a neighbor that is a different value than itself, then it is a zero crossing
                        if( dst.at<uchar>(i + a, j + b) != dst.at<uchar>(i,j) ) zerox[i][j] = 1;
                    }
                }
            }
        }
    }

    // calculate variance in local 5x5 neighborhood around zero crossing pixels
    for( int i = 0; i < dst.rows; i++ ){
        for( int j = 0; j < dst.cols; j++ ){
            // Only look at neighborhoods around zero crossing pixels
            if( zerox[i][j] == 1 ){
                mean = var = k = 0;
                // compute mean in 5 x 5 neighborhood
                for( int a = -2; a <= 2; a++ ){
                for( int b = -2; b <= 2; b++ ){
                    // check bounds
                    if( i + a >= 0 && i + a < dst.rows && j + b >= 0 && j + b < dst.cols ){
                        mean += (int)src.at<uchar>(i + a, j + b );
                        // k counts number of items added so that the mean
                        // is accurately represented for the border cases
                        k++;
                    }
                }
                }

                mean /= k;
                // loop again to compute variance 
                for( int a = -2; a <= 2; a++ ){
                for( int b = -2; b <= 2; b++ ){
                    // check bounds
                    if( i + a >= 0 && i + a < dst.rows && j + b >= 0 && j + b < dst.cols ){
                        var += pow( ((int)src.at<uchar>(i + a, j + b) - mean) , 2.0 );
                    }
                }
                }
           
                var /= k;
                var = sqrt( var );
                // if the standard deviation is above the threshold it is an edge pixel..
                if( var >= thresh ) dst.at<uchar>(i,j) = 255;
            }
        }
    }
   
    return;
}                
            


    
                 



