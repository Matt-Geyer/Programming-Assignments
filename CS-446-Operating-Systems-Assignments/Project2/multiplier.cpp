// CS 446
// Matt Geyer
//
// Programming Assignment #2 
// Multi-Threaded Matrix Multiplication
//

#include <pthread.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

// Struct containing i , j values
// needed since you can only pass 1 parameter to 
// thread function
struct val{
    int i;
    int j;
};

void *mult( void *ptr );
int** readMatrix( char*  , bool  );

// Global Matrix Pointers
int **A;
int **B;
int **C;
int N, M, K = 0;

int main( int argc, char **argv ){
    int *ret;
    val *v = new val;
    ofstream fout;
   
    // Populate matrix A and B from file 
    // the bool is true to indicate that it is the
    // first matrix to be multiplied ( order matters )    
    A = readMatrix( argv[1] , true );
    B = readMatrix( argv[2] , false );
        
    // allocate memory for C
    C = new int*[M];
    for( int row = 0; row < M; row++ )
        C[row] = new int[N];
    
    // initialize C to zero
    for( int row = 0; row < M; row++ )
        for( int col = 0; col < N; col++ )
            C[row][col] = 0;

    // Create an array of M * N threads
    pthread_t thread[ M * N ];
    ret = new int[ M * N ];
   
    // thread counter
    int num_threads = 0;
    // Calculate every location in the matrix with a separate thread
    for( int row = 0; row < M; row++ ){
        for( int col = 0; col < N; col++ ){
            // Initialize a new struct so that the threads aren't 
            // competing for the same struct
            v = new val;
            v->i = row;
            v->j = col;
            // Try to create a new thread and pass the struct to the thread function
            // if we fail to create a new thread, the counter isn't increased.. necessary to avoid segfault
            if( (ret[num_threads] = pthread_create( &thread[num_threads] , NULL, mult, (void*)v )))
                cout << "Tried to make too many threads! Thread creation failed: " << ret[num_threads] << endl;
            else
                num_threads++;
        }
    }

    // Wait for all the threads to finish before printing out the array           
    for( int h = 0; h < num_threads; h++ ){
        pthread_join( thread[h] , NULL );
    }

    // Print the array to ouput file c.txt and print to screen
    cout << "A * B = C = " << endl << endl;

    fout.open("c.txt");
    fout << M << endl;
    fout << N << endl;
    for( int row = 0; row < M; row++ ){
        for( int col = 0; col < N; col++ ){
            cout << C[row][col] << " ";
            fout << C[row][col] << " ";
        }
        cout << endl;
        fout << endl;
    }
    fout.close();
 
    cout << endl << endl << "Printed output to c.txt..." << endl << endl;
    
    return 0;
}

void *mult( void* ptr ){
    // thread function.. determines the value
    // for the (i,j) location in the output matrix
  
    // typecast back to a struct pointer
    val* v = (val*)ptr;
    
    int ai,bj,cval;
    cval = 0;
    ai = v->i;
    bj = v->j; 
    
    // do summation formula for C(i,j)
    for( int n = 0; n < K; n++ )
        cval += A[ai][n] * B[n][bj];
    
    // store into output array
    C[ai][bj] = cval; 
 
}
    
int** readMatrix( char* file , bool isA ){
    // Routine for reading in a matrix from a file
    // File must conform to specific template as outlined in writeup

    int rows, cols, kk, **mat;

    ifstream fin( file );

    if( isA ){
        // Read in dimension values for matrix A
        fin >> M;
        fin >> K;
        rows = M;
        cols = K;
    }
    else{
        fin >> kk;
        fin >> N;
        // if kk != K then matrix multiplication isn't possible
        if( kk != K ){
            cout << "Incompatible Dimensions.. Exiting.. " << endl;
            exit( 0 );
        }
    
        rows = K;
        cols = N;
    }

    // allocate some new memory
    mat = new int*[rows];
    for( int i = 0; i < rows; i++ )
        mat[i] = new int[cols];

    // finally, begin reading the values into the matrix
    for( int i = 0; i < rows; i++ ){
        for( int j = 0; j < cols; j++ ){
            fin >> mat[i][j];
        }
    }
  
    fin.close();
    return mat;
}


    


