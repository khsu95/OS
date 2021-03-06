#include "rw_lock.h"

void init_rwlock(struct rw_lock * rw)
{
  //	Write the code for initializing your read-write lock.
	pthread_mutex_init(&rw->lock, NULL);
	pthread_cond_init(&rw->read, NULL);
	pthread_cond_init(&rw->write, NULL);
	rw -> readers =0;
	rw -> read_waiters = 0;
	rw -> write_waiters = 0;
	rw -> writers = 0;
}

void r_lock(struct rw_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
//	printf("Read Lock\n ");
//	printf("Write_Waiters : %d", rw->write_waiters);
	pthread_mutex_lock(&rw->lock);
	//Because Write preference
	while(rw->write_waiters){	
		pthread_cond_signal(&rw->write);
		pthread_cond_wait(&rw->read, &rw->lock);
	}
	rw->readers++;
	pthread_mutex_unlock(&rw->lock);
}

void r_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
//	printf("Read Unlock\n ");
	pthread_mutex_lock(&rw->lock);
	rw->readers--;
	//Wake writers up
	if(rw->readers==0)	pthread_cond_signal(&rw->write);
	pthread_cond_signal(&rw->read);
	pthread_mutex_unlock(&rw->lock);

}

void w_lock(struct rw_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
//	printf("Write lock\n ");
	pthread_mutex_lock(&rw->lock);
	rw->write_waiters++;
	//Wait until Unlock 
	if(rw->readers)	pthread_cond_wait(&rw->write, &rw->lock);
	rw->write_waiters--;
	rw->writers++;
}

void w_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
//	printf("Write Unlock\n ");
	rw->writers--;
	pthread_cond_signal(&rw->read);
	pthread_cond_signal(&rw->write);

	pthread_mutex_unlock(&rw->lock);
}
