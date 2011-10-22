// Face Normalization Program

#include <iostream>
#include "face.h"
#include <cv.h>
#include <highgui.h>
#include <string>

using namespace cv;
using namespace std;

void loadP( Mat& , Face& );
void loadAinverse( Mat& , Face& , float& );

int main(){
    
    // Same letter as matrices in the write-up
    Mat P( 5 , 3 , CV_32FC1 );
    Mat px( 5 , 1 , CV_32FC1 );
    Mat py( 5 , 1 , CV_32FC1 );
    Mat c1( 3, 1 , CV_32FC1 );
    Mat c2( 3 , 1 , CV_32FC1 );

    // A Matrices for computing the inverse transform
    Mat Ai( 3 , 3 , CV_32FC1 );
    Mat xy1p( 1 , 3 , CV_32FC1 );
    Mat xy1( 1 , 3 , CV_32FC1 );

    // F bar.. holds the average feature locations across
    // all face images..
    Face F_bar;
    Face F_bar_prev;
 
    float det;
    float avg[] = { 0 , 0 , 0 , 0 , 0,
                    0 , 0 , 0 , 0 , 0  }; 

    // Filename containers for automatic file loading and saving..    
    string in_file = "0.pgm";
    string out_file = "0_5nc.pgm";

    // Mat to store the face images.. F1....F10
    Mat tmp = imread( "1.pgm" );
    Mat Fi = tmp.clone();
    cvtColor( tmp, Fi, CV_BGR2GRAY );
    Mat Fnorm( 48 , 40 , CV_8UC1 );

    Face Ffinal = Face( 10,8, 30,8, 20,26, 20,37, 20,48  );
  
    Face faces[] = {
              //start of face 1
              Face(25,51,    63,49,    46,68,    45,88,    45,110),
              Face(31,44,    75,42,    65,64,    59,86,    53,111),
              Face(24,47,    61,44,    45,73,    45,89,    46,110),
              Face(15,44,    55,41,    28,62,    31,83,    36,109),
              Face(35,44,    76,42,    69,62,    64,83,    62,109),
              Face(11,47,    52,43,    25,66,    28,84,    35,110),
              Face(24,45,    61,42,    43,58,    43,80,    45,111),
              Face(28,45,    64,43,    49,65,    48,84,    49,110),
              Face(30,36,    67,35,    53,46,    52,72,    52,106),
              Face(34,51,    75,49,    63,75,    57,91,    54,110),

            //start of face 2
              Face(28,52,    60,48,    46,61,    48,82,    50,105),
              Face(32,50,    64,51,    40,62,    49,83,    47,109),
              Face(29,52,    61,50,    45,64,    45,85,    45,109),
              Face(33,48,    66,50,    51,61,    48,82,    48,107),
              Face(28,50,    61,47,    47,59,    47,82,    47,109),
              Face(34,51,    68,55,    53,64,    50,83,    45,107),
              Face(27,53,    61,51,    45,66,    46,85,    45,109),
              Face(29,52,    63,52,    46,65,    46,85,    44,108),
              Face(34,47,    67,51,    53,60,    48,81,    43,107),
              Face(26,53,    61,52,    44,66,    44,86,    44,110),
              //start of face 3
              Face(31,54,    66,56,    50,73,    47,88,    46,109),
              Face(40,54,    73,55,    63,70,    57,87,    56,108),
              Face(38,54,    73,55,    60,72,    57,90,    54,110),
              Face(16,53,    52,57,    28,70,    30,88,    27,110),
              Face(13,52,    48,54,    25,69,    28,88,    28,110),
              Face(24,57,    61,59,    42,75,    41,90,    37,110),
              Face(21,54,    55,57,    34,76,    33,90,    33,110),
              Face(21,55,    54,57,    36,76,    35,89,    37,110),
              Face(30,53,    65,53,    50,72,    49,90,    49,110),
              Face(33,51,    68,51,    55,70,    53,89,    53,107),

              //start of face 4
              Face(24,52,    59,53,    40,67,    39,89,    38,109),
              Face(34,51,    68,52,    55,65,    52,89,    51,109),
              Face(20,51,    55,53,    33,66,    33,87,    32,108),
              Face(32,53,    65,52,    50,68,    49,89,    50,110),
              Face(37,53,    73,52,    61,68,    58,89,    58,109),
              Face(16,52,    50,53,    27,68,    28,88,    30,110),
              Face(29,53,    61,53,    45,69,    44,89,    43,110),
              Face(22,50,    57,52,    35,67,    36,89,    35,110),
              Face(33,55,    67,56,    52,69,    49,91,    49,110),
              Face(44,53,    77,52,    72,67,    66,88,    66,107),

              //start of face 5
              Face(15,57,    50,57,    29,72,    33,87,    34,110),
              Face(27,56,    62,57,    46,72,    45,87,    45,108),
              Face(32,58,    67,58,    55,71,    52,88,    56,109),
              Face(14,57,    48,56,    27,70,    31,86,    34,107),
              Face(12,58,    47,58,    25,73,    30,88,    33,108),
              Face(15,57,    50,58,    29,74,    32,88,    34,108),
              Face(12,56,    45,56,    23,71,    28,86,    31,109),
              Face(23,61,    58,63,    38,79,    39,91,    40,110),
              Face(23,60,    57,60,    39,78,    40,90,    42,110),
              Face(21,55,    56,53,    38,65,    41,84,    44,106) };
              

    // initialize F_bar to the features from the first face
    F_bar = faces[40];
   
    int q = 0;
    float diff = 999;
    // SVD step 2
    while( q < 6){    
 
    // populate px , py with the given fixed feature coordinates
    px = Ffinal.F_x.clone();
    py = Ffinal.F_y.clone();

    // Load P with F_bar's values to solve the transformation
    // that maps F_bar with F_final
    loadP( P , F_bar );
   
    // SVD solve
    solve( P , px , c1 , DECOMP_SVD );
    solve( P , py , c2 , DECOMP_SVD );
  
    // Apply transformation on F_bar to get F_bar_prime
    px = P * c1;
    py = P * c2;
 
    // Update F to F_bar
    F_bar.F_x = px.clone();
    F_bar.F_y = py.clone();
    F_bar.F_c1 = c1.clone();
    F_bar.F_c2 = c2.clone();  

 
    // Check to make sure the transformation maps to the fixed values
    cout << "Check! 10,8 30,8 20,26 20,37 20,48 " << endl;
    for( int i = 0; i < 5; i++ ){
        cout << (int)F_bar.F_x.at<float>( i , 0 ) << "," << (int)F_bar.F_y.at<float>( i , 0 ) << " ";
    }
    cout << endl;

    //  Step 3
    
    // For every Face image Fi use SVD to compute the affine transform
    // that maps the features of Fi to F
    for( int i = 40; i < 50; i++ ){
    
        // Load P with appropriate values
        loadP( P , faces[i] );

        // Load Px and Py with average feature locations 
        px = F_bar.F_x.clone();
        py = F_bar.F_y.clone();       
 
        // Solve to find parameters of affine transform
        solve( P , px , c1 , DECOMP_SVD);
        solve( P , py , c2 , DECOMP_SVD);

        // Apply transformation on coords Fi to get Fi prime
        faces[i].F_xp = P * c1;
        faces[i].F_yp = P * c2;

        // Save transformation coefficients for later when 
        // the inverse is applied
        faces[i].F_c1 = c1.clone();
        faces[i].F_c2 = c2.clone();
       
    }         
   
    // zero out avg array
    for( int i = 0; i < 10; i++ ){
        avg[i] = 0;
    }
    
    // Compute average feature locations F_prime for every image
    for( int i = 40; i < 50; i++ ){
        // P1 , P2 .. P5
        avg[0] += faces[i].F_xp.at<float>( 0 , 0 ) / 10;
        avg[1] += faces[i].F_yp.at<float>( 0 , 0 ) / 10;
  
        avg[2] += faces[i].F_xp.at<float>( 1 , 0 ) / 10;
        avg[3] += faces[i].F_yp.at<float>( 1 , 0 ) / 10;
        
        avg[4] += faces[i].F_xp.at<float>( 2 , 0 ) / 10;
        avg[5] += faces[i].F_yp.at<float>( 2 , 0 ) / 10;

        avg[6] += faces[i].F_xp.at<float>( 3 , 0 ) / 10;
        avg[7] += faces[i].F_yp.at<float>( 3 , 0 ) / 10;

        avg[8] += faces[i].F_xp.at<float>( 4 , 0 ) / 10;
        avg[9] += faces[i].F_yp.at<float>( 4 , 0 ) / 10;
   
    }
    
    // save a copy of F_bar for comparison
    F_bar_prev = F_bar;
     
    // Update F_bar to be the average
    F_bar.F_x.at<float>( 0 , 0 ) = avg[0];
    F_bar.F_y.at<float>( 0 , 0 ) = avg[1];

    F_bar.F_x.at<float>( 1 , 0 ) = avg[2];
    F_bar.F_y.at<float>( 1 , 0 ) = avg[3];

    F_bar.F_x.at<float>( 2 , 0 ) = avg[4];
    F_bar.F_y.at<float>( 2 , 0 ) = avg[5];

    F_bar.F_x.at<float>( 3 , 0 ) = avg[6];
    F_bar.F_y.at<float>( 3 , 0 ) = avg[7];

    F_bar.F_x.at<float>( 4 , 0 ) = avg[8];
    F_bar.F_y.at<float>( 4 , 0 ) = avg[9];
    
    diff = F_bar - F_bar_prev;
    cout << q << " " << diff <<  endl;
    
    q++;
    
    }// end of while loop
    
    // Apply inverse transformation on all images to normalize them  
    for( int k = 40; k < 50; k++ ){

    // update filenames
    if ( k == 49 ){
        in_file = "10.pgm";
        out_file = "10_5nc.pgm";
    }
    else{
        in_file[0] = char( 49 + k - 40 );
        out_file[0] = char( 49 + k - 40);
    }

    // read images and convert to grayscale
    tmp = imread( in_file );
    Fi = tmp.clone();
    cvtColor( tmp, Fi, CV_BGR2GRAY );
    
    // Populate the inverse transformation matrix
    loadAinverse( Ai , faces[k]  , det ); 

    int tx , ty;
    xy1p.at<float>( 0 , 2 ) = 1.f;

    // Iterate through every pixel in the output image
    for( int i = 0; i < Fnorm.rows; i++ ){
        for( int j = 0; j < Fnorm.cols; j++ ){
            // [ X' , Y' , 1 ]
            xy1p.at<float>( 0 , 0 ) = j;
            xy1p.at<float>( 0 , 1 ) = i;
           
            // Apply inverse transform
            xy1 = ( xy1p * Ai ) /  det;
          
            // Check bounds to avoid wrap-around
            tx = xy1.at<float>( 0 , 0 );
            ty = xy1.at<float>( 0 , 1 );
            if( tx > Fi.cols ) tx = Fi.cols - 1;
            if( tx < 0 ) tx = 0;
            if( ty > Fi.rows ) ty = Fi.rows - 1;
            if( ty < 0 ) ty = 0;
 
            // Set pixel in output image    
            Fnorm.at<uchar>( i , j ) = Fi.at<uchar>( ty  , tx );
            
        }
    }

    // write results to file
    imwrite( out_file , Fnorm );

    // display results
    namedWindow( "Standard" );
    namedWindow( "Normalized" );
    imshow( "Standard" , Fi ); 

    // Draw circles where features should be mapped to
    circle( Fnorm , Point( F_bar.F_x.at<float>( 0 , 0 ) , F_bar.F_y.at<float>( 0 , 0 ) ), 3 ,  Scalar(255 , 0 , 0) , 1 );
    circle( Fnorm , Point( F_bar.F_x.at<float>( 1 , 0 ) , F_bar.F_y.at<float>( 1 , 0 ) ), 3 ,  Scalar(255 , 0 , 0) , 1 );
    circle( Fnorm , Point( F_bar.F_x.at<float>( 2 , 0 ) , F_bar.F_y.at<float>( 2 , 0 ) ), 3 ,  Scalar(255 , 0 , 0) , 1 );
    circle( Fnorm , Point( F_bar.F_x.at<float>( 3 , 0 ) , F_bar.F_y.at<float>( 3 , 0 ) ), 3 ,  Scalar(255 , 0 , 0) , 1 );
    circle( Fnorm , Point( F_bar.F_x.at<float>( 4 , 0 ) , F_bar.F_y.at<float>( 4 , 0 ) ), 3 ,  Scalar(255 , 0 , 0) , 1 );
    imshow( "Normalized" , Fnorm );
    imwrite( out_file , Fnorm );
    waitKey();
    }    
    
    return 0;
}

void loadP( Mat& dst , Face& f  ){
    // function to load the matrix P
    for( int i = 0; i < 5; i++ ){
        dst.at<float>( i , 0 ) = f.F_x.at<float>( i , 0 );
        dst.at<float>( i , 1 ) = f.F_y.at<float>( i , 0 );
        dst.at<float>( i , 2 ) = 1.f;
    }
    
}
  
void loadAinverse( Mat& dst , Face& f , float& det){
    // load inverse matrix for transformation
    
    float a11, a12, a21, a22, b1, b2;
    a11 = f.F_c1.at<float>( 0 , 0 );
    a12 = f.F_c1.at<float>( 1 , 0 );
    b1 = f.F_c1.at<float>( 2 , 0 ); 
    a21 = f.F_c2.at<float>( 0 , 0 );
    a22 = f.F_c2.at<float>( 1 , 0 );
    b2 = f.F_c2.at<float>( 2 , 0 );
    det = ( a11 * a22 ) - ( a21 * a12 );

    // row 1 
    dst.at<float>( 0 , 0 ) = a22;
    dst.at<float>( 0 , 1 ) = -a21;
    dst.at<float>( 0 , 2 ) = 0;
 
    // row 2 
    dst.at<float>( 1 , 0 ) = -a12;
    dst.at<float>( 1 , 1 ) = a11;
    dst.at<float>( 1 , 2 ) = 0;
    
    // row 3
    dst.at<float>( 2 , 0 ) = ( a12 * b2 ) - ( a22 * b1 );
    dst.at<float>( 2 , 1 ) = ( a21 * b1 ) - ( a11 * b2 );
    dst.at<float>( 2 , 2 ) = ( a11 * a22 ) - ( a21 * a12 );
    
    return;
}
