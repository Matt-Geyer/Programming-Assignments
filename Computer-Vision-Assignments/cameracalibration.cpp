#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

int main( int argc, char** argv ){

    vector<Mat> imgs, rvecs, tvecs;
    Mat temp, cameraMatrix, distCoeffs;
    vector<Point2f> corners;
    vector<Point3f> pattern;
    vector< vector<Point3f> > objectPoints;
    vector< vector<Point2f> > imagePoints;
    vector< vector<Point3f> > objectPointsA, objectPointsB;
    vector< vector<Point2f> > imagePointsA, imagePointsB;

    srand( time(NULL) );

    bool found;

    // load all of the images
    string fname = "image0.bmp";
    for( int i = 0; i < 12; i++ ){
        fname[5] = char( 48 + i );
        if( i == 10 ) fname = "image10.bmp"; 
        else if( i == 11 ) fname = "image11.bmp";
        temp = imread( fname );
        imgs.push_back( temp );
    }

    // build the chess pattern ( same across all views )
    for( int y = 0; y < 7; y++ ){
        for( int x = 0; x < 7; x++ ){
            Point3f p( x , y , 0 );
            pattern.push_back( p );
        }
    } 
    
    // build objectPoints vector
    for( int i = 0; i < 12; i++ ){
        objectPoints.push_back( pattern );
    }

    // find the actual coordinates in the images
    // and build the imagePoints matrix
    string fout = "drawCorners0.bmp";
    for( int i = 0; i < imgs.size(); i++ ){
        found = findChessboardCorners( imgs[i] , cvSize( 7 , 7 ) , corners , 0 );
        //drawChessboardCorners( imgs[i] , cvSize( 7 , 7 ) , corners , found );
        imagePoints.push_back( corners );

        fout[11] = char( 48 + i );
        if( i == 10 ) fout = "drawCorners10.bmp";
        else if( i == 11 ) fout = "drawCorners11.bmp";
        //imwrite( fout , imgs[i] );
        //waitKey();
    }

    // build subsets A and B

    // keep 80% for test set A
    int keep = imagePoints[0].size() * .8;

    vector< Point2f > IPA, IPB;
    vector< Point3f > OPA, OPB;
    int k = 0;
    for( int i = 0; i < imagePoints.size(); i++ ){
        k = 0;
        // empty out the vectors
        IPA.clear();
        IPB.clear();
        OPA.clear();
        OPB.clear();

        while( IPA.size() <= keep && k < 49){
        
            // do a random check .. 
            if( (rand() % 100 + 1) <= 80 ){
                // 80% of the time ( i think ) this is true
                // so we are building the set A which has 80% of the values..
                IPA.push_back( imagePoints[i][k] );
                OPA.push_back( objectPoints[i][k] );
            }
            else{
                IPB.push_back( imagePoints[i][k] );
                OPB.push_back( objectPoints[i][k] );
            }
            k++;
        }
        // put the rest into set B
        for( int j = k; j < imagePoints[i].size(); j++ ){
            IPB.push_back( imagePoints[i][j] );
            OPB.push_back( objectPoints[i][j] );
        }

        // store sets
        imagePointsA.push_back( IPA );
        objectPointsA.push_back( OPA );
        imagePointsB.push_back( IPB );
        objectPointsB.push_back( OPB );
    }


    // find calibration parameters using set A
    Size s( imgs[0].rows , imgs[0].cols ); 
    calibrateCamera( objectPointsA , imagePointsA, s , cameraMatrix , distCoeffs , rvecs , tvecs );

    vector< Point2f > ip;

    float errA[ objectPointsA.size() ],
          errB[ objectPointsB.size() ];

    
    for( int j = 0; j < objectPointsA.size(); j++ ){
        errA[j] = 0;
        errB[j] = 0;
    }
  
    string projout = "0Aproj.bmp";
    cout << "Error for projections on set A: " << endl << endl;
    // project points set A
    for( int i = 0; i < objectPointsA.size(); i++ ){
        projectPoints( Mat( objectPointsA[i] ) , rvecs[i] , tvecs[i] , cameraMatrix , distCoeffs , ip );
 
        // calculate per image error and draw circles on output images at the project points
        for( int k = 0; k < ip.size(); k++ ){
            // this is gonna be ugly
            errA[i] +=  sqrt( ( pow( ip[k].x - imagePointsA[i][k].x , 2.0 ) + pow( ip[k].y - imagePointsA[i][k].y , 2.0) ) );
            //circle( imgs[i] , ip[k] , 4 , Scalar( 255 , 0 , 0 ) , -1 );
        }

        projout[0] = char( 48 + i );
        if( i == 10 ) projout = "10Aproj.bmp";
        else if( i == 11 ) projout = " 11Aproj.bmp";
        //imwrite( projout , imgs[i] );
        //imshow( "drawn" , imgs[i] );
        //waitKey();
        cout << "Image " << i << " projection error: " << errA[i] << endl;
    }
    // find average across all A
    float avgErrA = 0;
    for( int i = 0; i < 12; i++ ){
        avgErrA += errA[i] / 12;
    }
 
    cout << endl << "Average projection error: " << avgErrA << endl;

    cout << endl << endl;
 
    
    cout << "Verification error on set B: " << endl << endl;
    // project points set B
    projout = "0Bproj.bmp";
    for( int i = 0; i < objectPointsB.size(); i++ ){
        projectPoints( Mat( objectPointsB[i] ) , rvecs[i] , tvecs[i] , cameraMatrix , distCoeffs , ip );
 
        // calculate per image error
        for( int k = 0; k < ip.size(); k++ ){
            // this is gonna be ugly
            errB[i] +=  sqrt( ( pow( ip[k].x - imagePointsB[i][k].x , 2.0 ) + pow( ip[k].y - imagePointsB[i][k].y , 2.0) ) );
            circle( imgs[i] , ip[k] , 4 , Scalar( 255 , 0 , 0 ) , -1 );
        }

        projout[0] = char( 48 + i );
        if( i == 10 ) projout = "10Bproj.bmp";
        else if( i == 11 ) projout = " 11Bproj.bmp";
        imwrite( projout , imgs[i] );

        cout << "Image " << i << " verification error: " << errB[i] << endl;
    }
    // find average across all A
    float avgErrB = 0;
    for( int i = 0; i < 12; i++ ){
        avgErrB += errB[i] / 12;
    }
    
    cout << endl << "Average verification error: " << avgErrB << endl;
    return 0;
}
