/* buffer.h */
// Simple buffer class that is comprised of a 1D array for the buffer
// implemented to behave like a circular queue

#include <pthread.h>
#include <semaphore.h>

template<class T>
class buffer{
  
  public:
      buffer();
      buffer( int );
      ~buffer();
      void insert( T item );
      void remove( T &item );
      bool isFull();
      bool isEmpty();
 
  private:
      T* b;
      int front, rear;
      int CAPACITY;
      pthread_mutex_t mutex;
      sem_t full, empty;

};

template<class T>
buffer<T>::buffer(){
    // Default constructor included for completeness.. but should not be 
    // used because it is useless

    CAPACITY = 0;
    front = rear = -1;
    b = NULL;
    pthread_mutex_init( &mutex , NULL );
    sem_init( &full , 0 , 0 );
    sem_init( &empty , 0 , CAPACITY );
}

template<class T>
buffer<T>::buffer( int cap ){
    // practical constructor
    
    CAPACITY = cap;
    // if front and rear == -1 queue is empty.. 
    front = rear = -1;

    // declare memory for the buffer
    b = new T[ CAPACITY ];
 
    // initialize mutex lock
    pthread_mutex_init( &mutex , NULL );
}

template<class T>
buffer<T>::~buffer(){
    // de-allocate memory for the buffer
    delete b;
}

template<class T>
bool buffer<T>::isEmpty(){
    // check if the buffer is empty.. returns true if empty, false otherwise
    if( rear == -1 && front == -1 )
        return true;
    else
        return false;
}

template<class T>
bool buffer<T>::isFull(){
    // check if the buffer is full.. returns true if full, false otherwise
    if(( ( front == 0 ) && ( rear == CAPACITY - 1 ) ) || ( front == rear + 1 ))
        return true;
    else
        return false;
}

template<class T>
void buffer<T>::insert( T item ){
    // insert an item into the queue
 
    // aquire mutex lock so that only one insertion/removal can happen at a time
    pthread_mutex_lock( &mutex );
   
    // update index as per circular queue specs 
    if( front == - 1 )
        front = rear = 0;
    else if( rear == CAPACITY - 1 )
        rear = 0;
    else
        rear++;

    // insert item into the buffer
    b[rear] = item;
  
    // release mutex lock because we're done
    pthread_mutex_unlock( &mutex );

    return;
}

template<class T>
void buffer<T>::remove( T &item ){

    // aquire mutex lock
    pthread_mutex_lock( &mutex );    

    // "remove" an item from the array
    item = b[front];

    // update index
    if( front == rear )
        front = rear = -1;
    else if( front == CAPACITY - 1 )
        front = 0;
    else
        front++;
    // release mutex lock
    pthread_mutex_unlock( &mutex );

    return;
}
    

