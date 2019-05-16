#include <stdio.h>
#include <signal.h>
#include "./defines.h"
#include "./default_functions.h"
#include "./profiler.h"
#include "./simulated_signals.h"

#ifndef ITERATION_TIME
	#define ITERATION_TIME 1000
#endif

#ifndef MAX_TIME
	#define MAX_TIME -1
#endif

#ifdef SIMULATION 
	#ifdef HARDWARE_SIMULATION
		#define HSIM // hardware simulation
	#else
		#define SSIM // software simulation
	#endif
#endif

volatile int ready = -1;

void timer_loop(){
#ifdef MPC_INTERRUPT
	if(ready == -1) return;
#ifndef TAKE_YOUR_TIME
	if(ready == 0)	
#endif
		ready = 1;
#ifndef TAKE_YOUR_TIME
	else __timeout();
#endif
#endif
}

int main_interrupt(void){
	#ifndef SSIM
	__hardware_setup();
	#else
	__sim_setup();
	#endif
	__controller_setup();
	ready = 1;
	while(1){
		#ifndef NO_DELAY
			while(ready == 0);
		#endif
		#ifdef CONTROL_DELAY
			#ifndef SSIM
				__controls();
			#else
				__sim_controls();
			#endif
		#endif
		#ifndef SSIM
			__measurements();
		#endif
		#ifdef SIMULATION
			__sim_measurements();
		#endif
		__loop();
		#ifndef CONTROL_DELAY
			#ifndef SSIM
				__controls();
			#else
				__sim_controls();
			#endif
		#endif
		__idle();
		ready = 0;
		if((MAX_TIME >= 0) && __get_time() > MAX_TIME) break;
	}
	return 0;
}

int main_counter(void){
	long t = 0;

	#ifndef SSIM
	__hardware_setup();
	#else
	__sim_setup();
	#endif
	__controller_setup();

	while (1){
		t = __get_time();
		#ifdef CONTROL_DELAY
			#ifndef SSIM
				__controls();
			#else
				__sim_controls();
			#endif
		#endif
		#ifndef SSIM
			__measurements();
		#endif
		#ifdef SIMULATION
			__sim_measurements();
		#endif
			__loop();
		#ifndef CONTROL_DELAY
			#ifndef SSIM
				__controls();
			#else
				__sim_controls();
			#endif
		#endif

		__idle();
		if((MAX_TIME >= 0) && __get_time() > MAX_TIME) break;
		#ifndef TAKE_YOUR_TIME
			if((__get_time()-t) > ITERATION_TIME) __timeout();
		#endif
		#ifndef NO_DELAY
			while((__get_time()-t)<=ITERATION_TIME);
		#endif
	}
	__timeout();
	return 0;
}

int main(){
#ifdef MPC_INTERRUPT
	return main_interrupt();
#else
	return main_counter();
#endif
}