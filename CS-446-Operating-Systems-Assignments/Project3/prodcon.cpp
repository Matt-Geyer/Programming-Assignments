// Matt Geyer
// CS 446
// Producer - Consumer Problem
// 
// Multi-threaded resource access using mutex locks and semaphores
//
// arg1 - number of producers
// arg2 - number of consumers
// arg3 - how long to run
//


#include <iostream>
#include "buffer.h"
#include <stdlib.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
typedef int buffer_item;

using namespace std;

void *producer( void *param );
void *consumer( void *param );
int items_produced = 0, items_consumed = 0;

// declare global semaphores
sem_t full, empty;
pthread_mutex_t mutex;

bool cont = true;

// create global buffer for the producer and consumers to interact with
buffer<buffer_item> buff( BUFFER_SIZE );   

int main( int argc , char** argv ){
 
    int slp = 0,
        num_prods = 0,
        num_cons = 0;
   
    int *id = new int;

    // init full semaphore to 0
    sem_init( &full , 0 , 0 );
   
    // init empty semaphore to n or BUFFER_SIZE
    sem_init( &empty , 0 , BUFFER_SIZE );
   
    // initialize mutex lock
    pthread_mutex_init( &mutex , NULL );
    
    // Determine input parameters and convert to ints
    if( argv[1] != NULL )
        slp = atoi( argv[1] );
    if( argv[2] != NULL )
        num_prods = atoi( argv[2] );
    if( argv[3] != NULL )
        num_cons = atoi( argv[3] );

    // create array of producer and consumer threads
    pthread_t prods[ num_prods ];
    pthread_t cons[ num_cons ];

    // create producer threads.. NULL is passed for variable because it is not needed
    for( int i = 0; i < num_prods; i++ ){
        *id = i;
        pthread_create( &prods[ i ] , NULL , producer , (void*)id );
    }
    // create consumer threads
    for( int i = 0; i < num_cons; i++ ){
        *id = i;
        pthread_create( &cons[ i ] , NULL , consumer , (void*)id );
    }

    // sleep for determined amount of time and exit.
    sleep( slp );
 

    pthread_mutex_lock( &mutex );
    cont = false;
    cout << "Number of items produced: " << items_produced << endl;
    cout << "NUmber of items consumed: " << items_consumed << endl;
    exit(0); 
    pthread_mutex_unlock( &mutex );
   
    return 0;
}

void *producer( void *param ){
  
    int id = *(int*)param;
   
    // item to be produced
    buffer_item item;
 
    while( cont ){
    
        // sleep for 0 to 4 seconds
        sleep( rand() % 5 );

        // generate random item
        item = rand();
        
        // protect critical code.. since empty was initialized
        // to the number of available space in the buffer
        // when you wait on empty it essentially lowers the amount of available
        // spaces by one.. that way if a thread tries to produce and empty <= 0
        // they will hold until a new space in the buffer is available
        sem_wait( &empty );
        // aquire mutex lock
        pthread_mutex_lock( &mutex );

        if( !buff.isFull() ){
            // insert an item into the buffer ( produce an item )
            buff.insert( item );
            items_produced++;
            // print the thread id and the number it produced
            cout << "Producer " << id << " produced " << item << endl;
        }
        else{
            // this should never actually happen.. the semaphores are supposed to prevent it
            cout << "Error, buffer full" << endl;
        }
        // release mutex lock
        pthread_mutex_unlock( &mutex );

        // this increases the full semaphore which was intialized with 0
        // this indicates that an item has been produced for the consumer threads
        sem_post( &full );
    }
}

void *consumer( void *param ){

    int id = *(int*)param;

    buffer_item item;
    
    while( cont ){
 
        // sleep from 0 to 4 secs  
        sleep( rand() % 5 );
    
        // decrement the full semaphore by 1.. since full was initialized to 0
        // and full only gets incremented by a producer function finishing its critical section
        // then we know if full > 1 then we have an item to be consumed.. if full = 0 then the thread
        // blocks itself and waits..
        sem_wait( &full );
   
        // aquire mutex lock  
        pthread_mutex_lock( &mutex );

        if( !buff.isEmpty() ){
            // consume an item from the buffer
            buff.remove( item );
            items_consumed++;
            // print consumer id and item consumed
            cout << "\tConsumer " << id << " consumed " << item << endl;
        }
        else{
            // this shouldn't ever actually happen either
            cout << "Buffer empty.." << endl;
        }
    
        // release mutex lock
        pthread_mutex_unlock( &mutex );  

        // increment empty since we just took an item from the buffer..
        // if a producer thread is blocked and waiting for an empty slot it
        // should be able to unblock now since we made some free space
        sem_post( &empty );
    }
}    
