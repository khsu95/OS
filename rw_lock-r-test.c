#include "rw_lock.h"

void init_rwlock(struct rw_lock * rw)
{
  //	Write the code for initializing your read-write lock.
	pthread_mutex_init(&rw->lock, NULL);
	pthread_cond_init(&rw->write, NULL);
	rw-> readers=0;
	rw-> writers=0;
	rw-> write_waiters=0;
}

void r_lock(struct rw_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
	pthread_mutex_lock(&rw->lock);
	rw->readers++;
	pthread_mutex_unlock(&rw->lock);
}

void r_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
	pthread_mutex_lock(&rw->lock);
	rw->readers--;
	//This is Reader prefered, So Writer wait until given signal
	if(rw->readers==0)	pthread_cond_signal(&rw->write);
	pthread_mutex_unlock(&rw->lock);
}

void w_lock(struct rw_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
	  pthread_mutex_lock(&(rw->lock));
      //If there is data
      rw->write_waiters++;
	  //Waiter must wait
      pthread_cond_wait(&rw->write, &(rw->lock));
      rw->write_waiters--;
      rw->writers++;
	  //Lock Memory until func w_unlock be called
}

void w_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
    rw->writers--;
    //If there is Writers, Wake up writers
	pthread_cond_signal(&rw->write);
	//Unlock write. Other writers could access memory
    pthread_mutex_unlock(&rw->lock);
}
