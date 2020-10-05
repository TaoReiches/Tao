#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#define LC_INIT(s) s = 0;

#define PT_THREAD(name_args)	char name_args

#define PT_WAIT_WHILE(pt, cond)  PT_WAIT_UNTIL((pt), !(cond))

#define PT_WAIT_THREAD(pt, thread) PT_WAIT_WHILE((pt), PT_SCHEDULE(thread))

#define PT_SPAWN(pt, child, thread)		\
	do {						\
	PT_INIT((child));				\
	PT_WAIT_THREAD((pt), (thread));		\
	} while(0)

#define PT_RESTART(pt)				\
	do {						\
	PT_INIT(pt);				\
	return PT_WAITING;			\
	} while(0)

#define PT_EXIT(pt)				\
	do {						\
	PT_INIT(pt);				\
	return PT_EXITED;			\
	} while(0)

#define PT_SCHEDULE(f) ((f) < PT_EXITED)

#define PT_YIELD(pt)				\
	do {						\
	PT_YIELD_FLAG = 0;				\
	LC_SET((pt)->lc);				\
	if(PT_YIELD_FLAG == 0) {			\
	return PT_YIELDED;			\
	}						\
	} while(0)

#define PT_YIELD_UNTIL(pt, cond)		\
	do {						\
	PT_YIELD_FLAG = 0;				\
	LC_SET((pt)->lc);				\
	if((PT_YIELD_FLAG == 0) || !(cond)) {	\
	return PT_YIELDED;			\
	}						\
	} while(0)
