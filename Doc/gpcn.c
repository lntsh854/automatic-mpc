#include "C:\Users\Admin\Documents\GitHub\AutoMATiC\Libs\C\defines.h"
#include "C:\Users\Admin\Documents\GitHub\AutoMATiC\Libs\C\profiler.h"
#include "C:\Users\Admin\Documents\GitHub\AutoMATiC\Libs\C\mpctools.h"
#include "C:\Users\Admin\Documents\GitHub\AutoMATiC\Libs\C\simulated_signals.h"
#include "C:\Users\Admin\Documents\GitHub\AutoMATiC\Libs\C\matrix_cal.h"
#include "C:\Users\Admin\Documents\GitHub\AutoMATiC\Libs\C\mat_lib.h"
#include "C:\Users\Admin\Documents\GitHub\AutoMATiC\Libs\C\allocation_nr.h"
#include "C:\Users\Admin\Documents\GitHub\AutoMATiC\Libs\C\qp.h"
#include "stm32f7xx_hal.h"
#include <string.h>
#include "main.h"
#include "mat_lib.h"

ArchiveData ad;
CurrentControl cc;

long get_time(){ return HAL_GetTick(); }

extern void timer_loop(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim->Instance == TIM2) {
        timer_loop();
    }
}
void measurements(){
profiler_start(4);
    new_output(&ad,__measureOutput());
profiler_end(4);
}
void controls(){
profiler_start(3);
    __setControlValue(last_control(&ad));
profiler_end(3);
}
void hardware_setup(){
profiler_start(1);
    low_lvl_main();
profiler_end(1);
}

void controller(ArchiveData * ad, CurrentControl * c){
	static long Iuf;
	static long Iuftmp;
	static long Iyf;
	static float** Y0;
	static float** Yzad;
	static float** btmp1;
	static float** btmp2;
	static float** control_value;
	static float** disturbance;
	static float** du;
	static float** ftmp1;
	static float** ftmp4;
	static long i;
	static long itmp;
	static long j;
	static long m;
	static long n;
	static long p;
	static long r;
	static float** tmpu;
	static float** uk;
	static float** y0;
	static long k;
	static float*** GPC_b;
	static float** GPC_a;
	static float** fconst;
	static float** bvar;
	static float** b;
	static float** dumax;
	static float** dumin;
	static float** umax;
	static float** umin;
	static float ** H;
	static float ** A;
	static float * ftmp;
	static float * btmp;
	static float * qpx;
	if(ad == NULL){
		defineconnum(40);
		definevarnum(10);
		initializeqp();
		A=obtainA();
		H=obtainG();
		btmp=obtainb();
		ftmp=obtaint();
		qpx=obtainx();
		A[1][1] = 1.000000e+00f;
		A[1][2] = 0.000000e+00f;
		A[1][3] = 0.000000e+00f;
		A[1][4] = 0.000000e+00f;
		A[1][5] = 0.000000e+00f;
		A[1][6] = 0.000000e+00f;
		A[1][7] = 0.000000e+00f;
		A[1][8] = 0.000000e+00f;
		A[1][9] = 0.000000e+00f;
		A[1][10] = 0.000000e+00f;
		A[2][1] = 0.000000e+00f;
		A[2][2] = 1.000000e+00f;
		A[2][3] = 0.000000e+00f;
		A[2][4] = 0.000000e+00f;
		A[2][5] = 0.000000e+00f;
		A[2][6] = 0.000000e+00f;
		A[2][7] = 0.000000e+00f;
		A[2][8] = 0.000000e+00f;
		A[2][9] = 0.000000e+00f;
		A[2][10] = 0.000000e+00f;
		A[3][1] = 0.000000e+00f;
		A[3][2] = 0.000000e+00f;
		A[3][3] = 1.000000e+00f;
		A[3][4] = 0.000000e+00f;
		A[3][5] = 0.000000e+00f;
		A[3][6] = 0.000000e+00f;
		A[3][7] = 0.000000e+00f;
		A[3][8] = 0.000000e+00f;
		A[3][9] = 0.000000e+00f;
		A[3][10] = 0.000000e+00f;
		A[4][1] = 0.000000e+00f;
		A[4][2] = 0.000000e+00f;
		A[4][3] = 0.000000e+00f;
		A[4][4] = 1.000000e+00f;
		A[4][5] = 0.000000e+00f;
		A[4][6] = 0.000000e+00f;
		A[4][7] = 0.000000e+00f;
		A[4][8] = 0.000000e+00f;
		A[4][9] = 0.000000e+00f;
		A[4][10] = 0.000000e+00f;
		A[5][1] = 0.000000e+00f;
		A[5][2] = 0.000000e+00f;
		A[5][3] = 0.000000e+00f;
		A[5][4] = 0.000000e+00f;
		A[5][5] = 1.000000e+00f;
		A[5][6] = 0.000000e+00f;
		A[5][7] = 0.000000e+00f;
		A[5][8] = 0.000000e+00f;
		A[5][9] = 0.000000e+00f;
		A[5][10] = 0.000000e+00f;
		A[6][1] = 0.000000e+00f;
		A[6][2] = 0.000000e+00f;
		A[6][3] = 0.000000e+00f;
		A[6][4] = 0.000000e+00f;
		A[6][5] = 0.000000e+00f;
		A[6][6] = 1.000000e+00f;
		A[6][7] = 0.000000e+00f;
		A[6][8] = 0.000000e+00f;
		A[6][9] = 0.000000e+00f;
		A[6][10] = 0.000000e+00f;
		A[7][1] = 0.000000e+00f;
		A[7][2] = 0.000000e+00f;
		A[7][3] = 0.000000e+00f;
		A[7][4] = 0.000000e+00f;
		A[7][5] = 0.000000e+00f;
		A[7][6] = 0.000000e+00f;
		A[7][7] = 1.000000e+00f;
		A[7][8] = 0.000000e+00f;
		A[7][9] = 0.000000e+00f;
		A[7][10] = 0.000000e+00f;
		A[8][1] = 0.000000e+00f;
		A[8][2] = 0.000000e+00f;
		A[8][3] = 0.000000e+00f;
		A[8][4] = 0.000000e+00f;
		A[8][5] = 0.000000e+00f;
		A[8][6] = 0.000000e+00f;
		A[8][7] = 0.000000e+00f;
		A[8][8] = 1.000000e+00f;
		A[8][9] = 0.000000e+00f;
		A[8][10] = 0.000000e+00f;
		A[9][1] = 0.000000e+00f;
		A[9][2] = 0.000000e+00f;
		A[9][3] = 0.000000e+00f;
		A[9][4] = 0.000000e+00f;
		A[9][5] = 0.000000e+00f;
		A[9][6] = 0.000000e+00f;
		A[9][7] = 0.000000e+00f;
		A[9][8] = 0.000000e+00f;
		A[9][9] = 1.000000e+00f;
		A[9][10] = 0.000000e+00f;
		A[10][1] = 0.000000e+00f;
		A[10][2] = 0.000000e+00f;
		A[10][3] = 0.000000e+00f;
		A[10][4] = 0.000000e+00f;
		A[10][5] = 0.000000e+00f;
		A[10][6] = 0.000000e+00f;
		A[10][7] = 0.000000e+00f;
		A[10][8] = 0.000000e+00f;
		A[10][9] = 0.000000e+00f;
		A[10][10] = 1.000000e+00f;
		A[11][1] = -1.000000e+00f;
		A[11][2] = -0.000000e+00f;
		A[11][3] = -0.000000e+00f;
		A[11][4] = -0.000000e+00f;
		A[11][5] = -0.000000e+00f;
		A[11][6] = -0.000000e+00f;
		A[11][7] = -0.000000e+00f;
		A[11][8] = -0.000000e+00f;
		A[11][9] = -0.000000e+00f;
		A[11][10] = -0.000000e+00f;
		A[12][1] = -0.000000e+00f;
		A[12][2] = -1.000000e+00f;
		A[12][3] = -0.000000e+00f;
		A[12][4] = -0.000000e+00f;
		A[12][5] = -0.000000e+00f;
		A[12][6] = -0.000000e+00f;
		A[12][7] = -0.000000e+00f;
		A[12][8] = -0.000000e+00f;
		A[12][9] = -0.000000e+00f;
		A[12][10] = -0.000000e+00f;
		A[13][1] = -0.000000e+00f;
		A[13][2] = -0.000000e+00f;
		A[13][3] = -1.000000e+00f;
		A[13][4] = -0.000000e+00f;
		A[13][5] = -0.000000e+00f;
		A[13][6] = -0.000000e+00f;
		A[13][7] = -0.000000e+00f;
		A[13][8] = -0.000000e+00f;
		A[13][9] = -0.000000e+00f;
		A[13][10] = -0.000000e+00f;
		A[14][1] = -0.000000e+00f;
		A[14][2] = -0.000000e+00f;
		A[14][3] = -0.000000e+00f;
		A[14][4] = -1.000000e+00f;
		A[14][5] = -0.000000e+00f;
		A[14][6] = -0.000000e+00f;
		A[14][7] = -0.000000e+00f;
		A[14][8] = -0.000000e+00f;
		A[14][9] = -0.000000e+00f;
		A[14][10] = -0.000000e+00f;
		A[15][1] = -0.000000e+00f;
		A[15][2] = -0.000000e+00f;
		A[15][3] = -0.000000e+00f;
		A[15][4] = -0.000000e+00f;
		A[15][5] = -1.000000e+00f;
		A[15][6] = -0.000000e+00f;
		A[15][7] = -0.000000e+00f;
		A[15][8] = -0.000000e+00f;
		A[15][9] = -0.000000e+00f;
		A[15][10] = -0.000000e+00f;
		A[16][1] = -0.000000e+00f;
		A[16][2] = -0.000000e+00f;
		A[16][3] = -0.000000e+00f;
		A[16][4] = -0.000000e+00f;
		A[16][5] = -0.000000e+00f;
		A[16][6] = -1.000000e+00f;
		A[16][7] = -0.000000e+00f;
		A[16][8] = -0.000000e+00f;
		A[16][9] = -0.000000e+00f;
		A[16][10] = -0.000000e+00f;
		A[17][1] = -0.000000e+00f;
		A[17][2] = -0.000000e+00f;
		A[17][3] = -0.000000e+00f;
		A[17][4] = -0.000000e+00f;
		A[17][5] = -0.000000e+00f;
		A[17][6] = -0.000000e+00f;
		A[17][7] = -1.000000e+00f;
		A[17][8] = -0.000000e+00f;
		A[17][9] = -0.000000e+00f;
		A[17][10] = -0.000000e+00f;
		A[18][1] = -0.000000e+00f;
		A[18][2] = -0.000000e+00f;
		A[18][3] = -0.000000e+00f;
		A[18][4] = -0.000000e+00f;
		A[18][5] = -0.000000e+00f;
		A[18][6] = -0.000000e+00f;
		A[18][7] = -0.000000e+00f;
		A[18][8] = -1.000000e+00f;
		A[18][9] = -0.000000e+00f;
		A[18][10] = -0.000000e+00f;
		A[19][1] = -0.000000e+00f;
		A[19][2] = -0.000000e+00f;
		A[19][3] = -0.000000e+00f;
		A[19][4] = -0.000000e+00f;
		A[19][5] = -0.000000e+00f;
		A[19][6] = -0.000000e+00f;
		A[19][7] = -0.000000e+00f;
		A[19][8] = -0.000000e+00f;
		A[19][9] = -1.000000e+00f;
		A[19][10] = -0.000000e+00f;
		A[20][1] = -0.000000e+00f;
		A[20][2] = -0.000000e+00f;
		A[20][3] = -0.000000e+00f;
		A[20][4] = -0.000000e+00f;
		A[20][5] = -0.000000e+00f;
		A[20][6] = -0.000000e+00f;
		A[20][7] = -0.000000e+00f;
		A[20][8] = -0.000000e+00f;
		A[20][9] = -0.000000e+00f;
		A[20][10] = -1.000000e+00f;
		A[21][1] = 1.000000e+00f;
		A[21][2] = 0.000000e+00f;
		A[21][3] = 0.000000e+00f;
		A[21][4] = 0.000000e+00f;
		A[21][5] = 0.000000e+00f;
		A[21][6] = 0.000000e+00f;
		A[21][7] = 0.000000e+00f;
		A[21][8] = 0.000000e+00f;
		A[21][9] = 0.000000e+00f;
		A[21][10] = 0.000000e+00f;
		A[22][1] = 0.000000e+00f;
		A[22][2] = 1.000000e+00f;
		A[22][3] = 0.000000e+00f;
		A[22][4] = 0.000000e+00f;
		A[22][5] = 0.000000e+00f;
		A[22][6] = 0.000000e+00f;
		A[22][7] = 0.000000e+00f;
		A[22][8] = 0.000000e+00f;
		A[22][9] = 0.000000e+00f;
		A[22][10] = 0.000000e+00f;
		A[23][1] = 1.000000e+00f;
		A[23][2] = 0.000000e+00f;
		A[23][3] = 1.000000e+00f;
		A[23][4] = 0.000000e+00f;
		A[23][5] = 0.000000e+00f;
		A[23][6] = 0.000000e+00f;
		A[23][7] = 0.000000e+00f;
		A[23][8] = 0.000000e+00f;
		A[23][9] = 0.000000e+00f;
		A[23][10] = 0.000000e+00f;
		A[24][1] = 0.000000e+00f;
		A[24][2] = 1.000000e+00f;
		A[24][3] = 0.000000e+00f;
		A[24][4] = 1.000000e+00f;
		A[24][5] = 0.000000e+00f;
		A[24][6] = 0.000000e+00f;
		A[24][7] = 0.000000e+00f;
		A[24][8] = 0.000000e+00f;
		A[24][9] = 0.000000e+00f;
		A[24][10] = 0.000000e+00f;
		A[25][1] = 1.000000e+00f;
		A[25][2] = 0.000000e+00f;
		A[25][3] = 1.000000e+00f;
		A[25][4] = 0.000000e+00f;
		A[25][5] = 1.000000e+00f;
		A[25][6] = 0.000000e+00f;
		A[25][7] = 0.000000e+00f;
		A[25][8] = 0.000000e+00f;
		A[25][9] = 0.000000e+00f;
		A[25][10] = 0.000000e+00f;
		A[26][1] = 0.000000e+00f;
		A[26][2] = 1.000000e+00f;
		A[26][3] = 0.000000e+00f;
		A[26][4] = 1.000000e+00f;
		A[26][5] = 0.000000e+00f;
		A[26][6] = 1.000000e+00f;
		A[26][7] = 0.000000e+00f;
		A[26][8] = 0.000000e+00f;
		A[26][9] = 0.000000e+00f;
		A[26][10] = 0.000000e+00f;
		A[27][1] = 1.000000e+00f;
		A[27][2] = 0.000000e+00f;
		A[27][3] = 1.000000e+00f;
		A[27][4] = 0.000000e+00f;
		A[27][5] = 1.000000e+00f;
		A[27][6] = 0.000000e+00f;
		A[27][7] = 1.000000e+00f;
		A[27][8] = 0.000000e+00f;
		A[27][9] = 0.000000e+00f;
		A[27][10] = 0.000000e+00f;
		A[28][1] = 0.000000e+00f;
		A[28][2] = 1.000000e+00f;
		A[28][3] = 0.000000e+00f;
		A[28][4] = 1.000000e+00f;
		A[28][5] = 0.000000e+00f;
		A[28][6] = 1.000000e+00f;
		A[28][7] = 0.000000e+00f;
		A[28][8] = 1.000000e+00f;
		A[28][9] = 0.000000e+00f;
		A[28][10] = 0.000000e+00f;
		A[29][1] = 1.000000e+00f;
		A[29][2] = 0.000000e+00f;
		A[29][3] = 1.000000e+00f;
		A[29][4] = 0.000000e+00f;
		A[29][5] = 1.000000e+00f;
		A[29][6] = 0.000000e+00f;
		A[29][7] = 1.000000e+00f;
		A[29][8] = 0.000000e+00f;
		A[29][9] = 1.000000e+00f;
		A[29][10] = 0.000000e+00f;
		A[30][1] = 0.000000e+00f;
		A[30][2] = 1.000000e+00f;
		A[30][3] = 0.000000e+00f;
		A[30][4] = 1.000000e+00f;
		A[30][5] = 0.000000e+00f;
		A[30][6] = 1.000000e+00f;
		A[30][7] = 0.000000e+00f;
		A[30][8] = 1.000000e+00f;
		A[30][9] = 0.000000e+00f;
		A[30][10] = 1.000000e+00f;
		A[31][1] = -1.000000e+00f;
		A[31][2] = -0.000000e+00f;
		A[31][3] = -0.000000e+00f;
		A[31][4] = -0.000000e+00f;
		A[31][5] = -0.000000e+00f;
		A[31][6] = -0.000000e+00f;
		A[31][7] = -0.000000e+00f;
		A[31][8] = -0.000000e+00f;
		A[31][9] = -0.000000e+00f;
		A[31][10] = -0.000000e+00f;
		A[32][1] = -0.000000e+00f;
		A[32][2] = -1.000000e+00f;
		A[32][3] = -0.000000e+00f;
		A[32][4] = -0.000000e+00f;
		A[32][5] = -0.000000e+00f;
		A[32][6] = -0.000000e+00f;
		A[32][7] = -0.000000e+00f;
		A[32][8] = -0.000000e+00f;
		A[32][9] = -0.000000e+00f;
		A[32][10] = -0.000000e+00f;
		A[33][1] = -1.000000e+00f;
		A[33][2] = -0.000000e+00f;
		A[33][3] = -1.000000e+00f;
		A[33][4] = -0.000000e+00f;
		A[33][5] = -0.000000e+00f;
		A[33][6] = -0.000000e+00f;
		A[33][7] = -0.000000e+00f;
		A[33][8] = -0.000000e+00f;
		A[33][9] = -0.000000e+00f;
		A[33][10] = -0.000000e+00f;
		A[34][1] = -0.000000e+00f;
		A[34][2] = -1.000000e+00f;
		A[34][3] = -0.000000e+00f;
		A[34][4] = -1.000000e+00f;
		A[34][5] = -0.000000e+00f;
		A[34][6] = -0.000000e+00f;
		A[34][7] = -0.000000e+00f;
		A[34][8] = -0.000000e+00f;
		A[34][9] = -0.000000e+00f;
		A[34][10] = -0.000000e+00f;
		A[35][1] = -1.000000e+00f;
		A[35][2] = -0.000000e+00f;
		A[35][3] = -1.000000e+00f;
		A[35][4] = -0.000000e+00f;
		A[35][5] = -1.000000e+00f;
		A[35][6] = -0.000000e+00f;
		A[35][7] = -0.000000e+00f;
		A[35][8] = -0.000000e+00f;
		A[35][9] = -0.000000e+00f;
		A[35][10] = -0.000000e+00f;
		A[36][1] = -0.000000e+00f;
		A[36][2] = -1.000000e+00f;
		A[36][3] = -0.000000e+00f;
		A[36][4] = -1.000000e+00f;
		A[36][5] = -0.000000e+00f;
		A[36][6] = -1.000000e+00f;
		A[36][7] = -0.000000e+00f;
		A[36][8] = -0.000000e+00f;
		A[36][9] = -0.000000e+00f;
		A[36][10] = -0.000000e+00f;
		A[37][1] = -1.000000e+00f;
		A[37][2] = -0.000000e+00f;
		A[37][3] = -1.000000e+00f;
		A[37][4] = -0.000000e+00f;
		A[37][5] = -1.000000e+00f;
		A[37][6] = -0.000000e+00f;
		A[37][7] = -1.000000e+00f;
		A[37][8] = -0.000000e+00f;
		A[37][9] = -0.000000e+00f;
		A[37][10] = -0.000000e+00f;
		A[38][1] = -0.000000e+00f;
		A[38][2] = -1.000000e+00f;
		A[38][3] = -0.000000e+00f;
		A[38][4] = -1.000000e+00f;
		A[38][5] = -0.000000e+00f;
		A[38][6] = -1.000000e+00f;
		A[38][7] = -0.000000e+00f;
		A[38][8] = -1.000000e+00f;
		A[38][9] = -0.000000e+00f;
		A[38][10] = -0.000000e+00f;
		A[39][1] = -1.000000e+00f;
		A[39][2] = -0.000000e+00f;
		A[39][3] = -1.000000e+00f;
		A[39][4] = -0.000000e+00f;
		A[39][5] = -1.000000e+00f;
		A[39][6] = -0.000000e+00f;
		A[39][7] = -1.000000e+00f;
		A[39][8] = -0.000000e+00f;
		A[39][9] = -1.000000e+00f;
		A[39][10] = -0.000000e+00f;
		A[40][1] = -0.000000e+00f;
		A[40][2] = -1.000000e+00f;
		A[40][3] = -0.000000e+00f;
		A[40][4] = -1.000000e+00f;
		A[40][5] = -0.000000e+00f;
		A[40][6] = -1.000000e+00f;
		A[40][7] = -0.000000e+00f;
		A[40][8] = -1.000000e+00f;
		A[40][9] = -0.000000e+00f;
		A[40][10] = -1.000000e+00f;
		H[1][1] = 1.344566e+02f;
		H[1][2] = 4.114478e+01f;
		H[1][3] = 1.000269e+02f;
		H[1][4] = 3.169500e+01f;
		H[1][5] = 6.190011e+01f;
		H[1][6] = 2.023139e+01f;
		H[1][7] = 2.567725e+01f;
		H[1][8] = 8.808511e+00f;
		H[1][9] = 0.000000e+00f;
		H[1][10] = 0.000000e+00f;
		H[2][1] = 4.114478e+01f;
		H[2][2] = 3.436150e+01f;
		H[2][3] = 3.099487e+01f;
		H[2][4] = 2.477126e+01f;
		H[2][5] = 1.931418e+01f;
		H[2][6] = 1.576652e+01f;
		H[2][7] = 8.233726e+00f;
		H[2][8] = 6.910587e+00f;
		H[2][9] = 0.000000e+00f;
		H[2][10] = 0.000000e+00f;
		H[3][1] = 1.000269e+02f;
		H[3][2] = 3.099487e+01f;
		H[3][3] = 8.804002e+01f;
		H[3][4] = 2.792375e+01f;
		H[3][5] = 5.644363e+01f;
		H[3][6] = 1.883764e+01f;
		H[3][7] = 2.413991e+01f;
		H[3][8] = 8.414358e+00f;
		H[3][9] = 0.000000e+00f;
		H[3][10] = 0.000000e+00f;
		H[4][1] = 3.169500e+01f;
		H[4][2] = 2.477126e+01f;
		H[4][3] = 2.792375e+01f;
		H[4][4] = 2.454154e+01f;
		H[4][5] = 1.838345e+01f;
		H[4][6] = 1.514642e+01f;
		H[4][7] = 8.018635e+00f;
		H[4][8] = 6.775021e+00f;
		H[4][9] = 0.000000e+00f;
		H[4][10] = 0.000000e+00f;
		H[5][1] = 6.190011e+01f;
		H[5][2] = 1.931418e+01f;
		H[5][3] = 5.644363e+01f;
		H[5][4] = 1.838345e+01f;
		H[5][5] = 4.711061e+01f;
		H[5][6] = 1.565679e+01f;
		H[5][7] = 2.097070e+01f;
		H[5][8] = 7.558490e+00f;
		H[5][9] = 0.000000e+00f;
		H[5][10] = 0.000000e+00f;
		H[6][1] = 2.023139e+01f;
		H[6][2] = 1.576652e+01f;
		H[6][3] = 1.883764e+01f;
		H[6][4] = 1.514642e+01f;
		H[6][5] = 1.565679e+01f;
		H[6][6] = 1.510759e+01f;
		H[6][7] = 7.391926e+00f;
		H[6][8] = 6.332568e+00f;
		H[6][9] = 0.000000e+00f;
		H[6][10] = 0.000000e+00f;
		H[7][1] = 2.567725e+01f;
		H[7][2] = 8.233726e+00f;
		H[7][3] = 2.413991e+01f;
		H[7][4] = 8.018635e+00f;
		H[7][5] = 2.097070e+01f;
		H[7][6] = 7.391926e+00f;
		H[7][7] = 1.634423e+01f;
		H[7][8] = 5.541646e+00f;
		H[7][9] = 0.000000e+00f;
		H[7][10] = 0.000000e+00f;
		H[8][1] = 8.808511e+00f;
		H[8][2] = 6.910587e+00f;
		H[8][3] = 8.414358e+00f;
		H[8][4] = 6.775021e+00f;
		H[8][5] = 7.558490e+00f;
		H[8][6] = 6.332568e+00f;
		H[8][7] = 5.541646e+00f;
		H[8][8] = 6.871756e+00f;
		H[8][9] = 0.000000e+00f;
		H[8][10] = 0.000000e+00f;
		H[9][1] = 0.000000e+00f;
		H[9][2] = 0.000000e+00f;
		H[9][3] = 0.000000e+00f;
		H[9][4] = 0.000000e+00f;
		H[9][5] = 0.000000e+00f;
		H[9][6] = 0.000000e+00f;
		H[9][7] = 0.000000e+00f;
		H[9][8] = 0.000000e+00f;
		H[9][9] = 2.000000e+00f;
		H[9][10] = 0.000000e+00f;
		H[10][1] = 0.000000e+00f;
		H[10][2] = 0.000000e+00f;
		H[10][3] = 0.000000e+00f;
		H[10][4] = 0.000000e+00f;
		H[10][5] = 0.000000e+00f;
		H[10][6] = 0.000000e+00f;
		H[10][7] = 0.000000e+00f;
		H[10][8] = 0.000000e+00f;
		H[10][9] = 0.000000e+00f;
		H[10][10] = 2.000000e+00f;
		tmpu = darray(1,2,1,1);
		du = darray(1,2,1,1);
		Yzad = darray(1,10,1,1);
		y0 = darray(1,2,1,5);
		Y0 = darray(1,10,1,1);
		uk = darray(1,2,1,1);
		disturbance = darray(1,2,1,1);
		control_value = darray(1,1,1,2);
		ftmp1 = darray(1,10,1,1);
		ftmp4 = darray(1,10,1,1);
		fconst = darray(1,10,1,10);
		fconst[1][1] = 0.000000e+00f;
		fconst[1][2] = 0.000000e+00f;
		fconst[1][3] = -5.104583e+00f;
		fconst[1][4] = -1.622249e+00f;
		fconst[1][5] = -7.603490e+00f;
		fconst[1][6] = -1.928652e+00f;
		fconst[1][7] = -8.826808e+00f;
		fconst[1][8] = -1.986524e+00f;
		fconst[1][9] = -9.425674e+00f;
		fconst[1][10] = -1.997455e+00f;
		fconst[2][1] = 0.000000e+00f;
		fconst[2][2] = 0.000000e+00f;
		fconst[2][3] = -1.264241e+00f;
		fconst[2][4] = -2.853981e+00f;
		fconst[2][5] = -1.729329e+00f;
		fconst[2][6] = -3.671660e+00f;
		fconst[2][7] = -1.900426e+00f;
		fconst[2][8] = -3.905929e+00f;
		fconst[2][9] = -1.963369e+00f;
		fconst[2][10] = -3.973048e+00f;
		fconst[3][1] = 0.000000e+00f;
		fconst[3][2] = 0.000000e+00f;
		fconst[3][3] = 0.000000e+00f;
		fconst[3][4] = 0.000000e+00f;
		fconst[3][5] = -5.104583e+00f;
		fconst[3][6] = -1.622249e+00f;
		fconst[3][7] = -7.603490e+00f;
		fconst[3][8] = -1.928652e+00f;
		fconst[3][9] = -8.826808e+00f;
		fconst[3][10] = -1.986524e+00f;
		fconst[4][1] = 0.000000e+00f;
		fconst[4][2] = 0.000000e+00f;
		fconst[4][3] = 0.000000e+00f;
		fconst[4][4] = 0.000000e+00f;
		fconst[4][5] = -1.264241e+00f;
		fconst[4][6] = -2.853981e+00f;
		fconst[4][7] = -1.729329e+00f;
		fconst[4][8] = -3.671660e+00f;
		fconst[4][9] = -1.900426e+00f;
		fconst[4][10] = -3.905929e+00f;
		fconst[5][1] = 0.000000e+00f;
		fconst[5][2] = 0.000000e+00f;
		fconst[5][3] = 0.000000e+00f;
		fconst[5][4] = 0.000000e+00f;
		fconst[5][5] = 0.000000e+00f;
		fconst[5][6] = 0.000000e+00f;
		fconst[5][7] = -5.104583e+00f;
		fconst[5][8] = -1.622249e+00f;
		fconst[5][9] = -7.603490e+00f;
		fconst[5][10] = -1.928652e+00f;
		fconst[6][1] = 0.000000e+00f;
		fconst[6][2] = 0.000000e+00f;
		fconst[6][3] = 0.000000e+00f;
		fconst[6][4] = 0.000000e+00f;
		fconst[6][5] = 0.000000e+00f;
		fconst[6][6] = 0.000000e+00f;
		fconst[6][7] = -1.264241e+00f;
		fconst[6][8] = -2.853981e+00f;
		fconst[6][9] = -1.729329e+00f;
		fconst[6][10] = -3.671660e+00f;
		fconst[7][1] = 0.000000e+00f;
		fconst[7][2] = 0.000000e+00f;
		fconst[7][3] = 0.000000e+00f;
		fconst[7][4] = 0.000000e+00f;
		fconst[7][5] = 0.000000e+00f;
		fconst[7][6] = 0.000000e+00f;
		fconst[7][7] = 0.000000e+00f;
		fconst[7][8] = 0.000000e+00f;
		fconst[7][9] = -5.104583e+00f;
		fconst[7][10] = -1.622249e+00f;
		fconst[8][1] = 0.000000e+00f;
		fconst[8][2] = 0.000000e+00f;
		fconst[8][3] = 0.000000e+00f;
		fconst[8][4] = 0.000000e+00f;
		fconst[8][5] = 0.000000e+00f;
		fconst[8][6] = 0.000000e+00f;
		fconst[8][7] = 0.000000e+00f;
		fconst[8][8] = 0.000000e+00f;
		fconst[8][9] = -1.264241e+00f;
		fconst[8][10] = -2.853981e+00f;
		fconst[9][1] = 0.000000e+00f;
		fconst[9][2] = 0.000000e+00f;
		fconst[9][3] = 0.000000e+00f;
		fconst[9][4] = 0.000000e+00f;
		fconst[9][5] = 0.000000e+00f;
		fconst[9][6] = 0.000000e+00f;
		fconst[9][7] = 0.000000e+00f;
		fconst[9][8] = 0.000000e+00f;
		fconst[9][9] = 0.000000e+00f;
		fconst[9][10] = 0.000000e+00f;
		fconst[10][1] = 0.000000e+00f;
		fconst[10][2] = 0.000000e+00f;
		fconst[10][3] = 0.000000e+00f;
		fconst[10][4] = 0.000000e+00f;
		fconst[10][5] = 0.000000e+00f;
		fconst[10][6] = 0.000000e+00f;
		fconst[10][7] = 0.000000e+00f;
		fconst[10][8] = 0.000000e+00f;
		fconst[10][9] = 0.000000e+00f;
		fconst[10][10] = 0.000000e+00f;
		btmp1 = darray(1,40,1,1);
		bvar = darray(1,40,1,2);
		bvar[1][1] = 0.000000e+00f;
		bvar[1][2] = 0.000000e+00f;
		bvar[2][1] = 0.000000e+00f;
		bvar[2][2] = 0.000000e+00f;
		bvar[3][1] = 0.000000e+00f;
		bvar[3][2] = 0.000000e+00f;
		bvar[4][1] = 0.000000e+00f;
		bvar[4][2] = 0.000000e+00f;
		bvar[5][1] = 0.000000e+00f;
		bvar[5][2] = 0.000000e+00f;
		bvar[6][1] = 0.000000e+00f;
		bvar[6][2] = 0.000000e+00f;
		bvar[7][1] = 0.000000e+00f;
		bvar[7][2] = 0.000000e+00f;
		bvar[8][1] = 0.000000e+00f;
		bvar[8][2] = 0.000000e+00f;
		bvar[9][1] = 0.000000e+00f;
		bvar[9][2] = 0.000000e+00f;
		bvar[10][1] = 0.000000e+00f;
		bvar[10][2] = 0.000000e+00f;
		bvar[11][1] = 0.000000e+00f;
		bvar[11][2] = 0.000000e+00f;
		bvar[12][1] = 0.000000e+00f;
		bvar[12][2] = 0.000000e+00f;
		bvar[13][1] = 0.000000e+00f;
		bvar[13][2] = 0.000000e+00f;
		bvar[14][1] = 0.000000e+00f;
		bvar[14][2] = 0.000000e+00f;
		bvar[15][1] = 0.000000e+00f;
		bvar[15][2] = 0.000000e+00f;
		bvar[16][1] = 0.000000e+00f;
		bvar[16][2] = 0.000000e+00f;
		bvar[17][1] = 0.000000e+00f;
		bvar[17][2] = 0.000000e+00f;
		bvar[18][1] = 0.000000e+00f;
		bvar[18][2] = 0.000000e+00f;
		bvar[19][1] = 0.000000e+00f;
		bvar[19][2] = 0.000000e+00f;
		bvar[20][1] = 0.000000e+00f;
		bvar[20][2] = 0.000000e+00f;
		bvar[21][1] = 1.000000e+00f;
		bvar[21][2] = 0.000000e+00f;
		bvar[22][1] = 0.000000e+00f;
		bvar[22][2] = 1.000000e+00f;
		bvar[23][1] = 1.000000e+00f;
		bvar[23][2] = 0.000000e+00f;
		bvar[24][1] = 0.000000e+00f;
		bvar[24][2] = 1.000000e+00f;
		bvar[25][1] = 1.000000e+00f;
		bvar[25][2] = 0.000000e+00f;
		bvar[26][1] = 0.000000e+00f;
		bvar[26][2] = 1.000000e+00f;
		bvar[27][1] = 1.000000e+00f;
		bvar[27][2] = 0.000000e+00f;
		bvar[28][1] = 0.000000e+00f;
		bvar[28][2] = 1.000000e+00f;
		bvar[29][1] = 1.000000e+00f;
		bvar[29][2] = 0.000000e+00f;
		bvar[30][1] = 0.000000e+00f;
		bvar[30][2] = 1.000000e+00f;
		bvar[31][1] = -1.000000e+00f;
		bvar[31][2] = -0.000000e+00f;
		bvar[32][1] = -0.000000e+00f;
		bvar[32][2] = -1.000000e+00f;
		bvar[33][1] = -1.000000e+00f;
		bvar[33][2] = -0.000000e+00f;
		bvar[34][1] = -0.000000e+00f;
		bvar[34][2] = -1.000000e+00f;
		bvar[35][1] = -1.000000e+00f;
		bvar[35][2] = -0.000000e+00f;
		bvar[36][1] = -0.000000e+00f;
		bvar[36][2] = -1.000000e+00f;
		bvar[37][1] = -1.000000e+00f;
		bvar[37][2] = -0.000000e+00f;
		bvar[38][1] = -0.000000e+00f;
		bvar[38][2] = -1.000000e+00f;
		bvar[39][1] = -1.000000e+00f;
		bvar[39][2] = -0.000000e+00f;
		bvar[40][1] = -0.000000e+00f;
		bvar[40][2] = -1.000000e+00f;
		btmp2 = darray(1,40,1,1);
		b = darray(1,40,1,1);
		b[1][1] = 1.000000e-01f;
		b[2][1] = 1.000000e-01f;
		b[3][1] = 1.000000e-01f;
		b[4][1] = 1.000000e-01f;
		b[5][1] = 1.000000e-01f;
		b[6][1] = 1.000000e-01f;
		b[7][1] = 1.000000e-01f;
		b[8][1] = 1.000000e-01f;
		b[9][1] = 1.000000e-01f;
		b[10][1] = 1.000000e-01f;
		b[11][1] = 1.000000e-01f;
		b[12][1] = 1.000000e-01f;
		b[13][1] = 1.000000e-01f;
		b[14][1] = 1.000000e-01f;
		b[15][1] = 1.000000e-01f;
		b[16][1] = 1.000000e-01f;
		b[17][1] = 1.000000e-01f;
		b[18][1] = 1.000000e-01f;
		b[19][1] = 1.000000e-01f;
		b[20][1] = 1.000000e-01f;
		b[21][1] = 1.000000e+00f;
		b[22][1] = 1.000000e+00f;
		b[23][1] = 1.000000e+00f;
		b[24][1] = 1.000000e+00f;
		b[25][1] = 1.000000e+00f;
		b[26][1] = 1.000000e+00f;
		b[27][1] = 1.000000e+00f;
		b[28][1] = 1.000000e+00f;
		b[29][1] = 1.000000e+00f;
		b[30][1] = 1.000000e+00f;
		b[31][1] = 1.000000e+00f;
		b[32][1] = 1.000000e+00f;
		b[33][1] = 1.000000e+00f;
		b[34][1] = 1.000000e+00f;
		b[35][1] = 1.000000e+00f;
		b[36][1] = 1.000000e+00f;
		b[37][1] = 1.000000e+00f;
		b[38][1] = 1.000000e+00f;
		b[39][1] = 1.000000e+00f;
		b[40][1] = 1.000000e+00f;
		GPC_b = darray3(1,2,1,2,1,4);
		GPC_b[1][1][1] = 0.000000e+00f;
		GPC_b[1][1][2] = 2.552292e+00f;
		GPC_b[1][1][3] = -9.389356e-01f;
		GPC_b[1][1][4] = 0.000000e+00f;
		GPC_b[1][2][1] = 0.000000e+00f;
		GPC_b[1][2][2] = 6.321206e-01f;
		GPC_b[1][2][3] = -3.094493e-01f;
		GPC_b[1][2][4] = 0.000000e+00f;
		GPC_b[2][1][1] = 0.000000e+00f;
		GPC_b[2][1][2] = 8.111244e-01f;
		GPC_b[2][1][3] = -2.323910e-01f;
		GPC_b[2][1][4] = 0.000000e+00f;
		GPC_b[2][2][1] = 0.000000e+00f;
		GPC_b[2][2][2] = 1.426990e+00f;
		GPC_b[2][2][3] = -2.695237e-01f;
		GPC_b[2][2][4] = 0.000000e+00f;
		GPC_a = darray(1,2,1,2);
		GPC_a[1][1] = -8.574211e-01f;
		GPC_a[1][2] = 1.800923e-01f;
		GPC_a[2][1] = -4.753804e-01f;
		GPC_a[2][2] = 5.411377e-02f;
		dumax = darray(1,1,1,2);
		dumax[1][1] = 1.000000e-01f;
		dumax[1][2] = 1.000000e-01f;
		dumin = darray(1,1,1,2);
		dumin[1][1] = -1.000000e-01f;
		dumin[1][2] = -1.000000e-01f;
		umax = darray(1,1,1,2);
		umax[1][1] = 1.000000e+00f;
		umax[1][2] = 1.000000e+00f;
		umin = darray(1,1,1,2);
		umin[1][1] = -1.000000e+00f;
		umin[1][2] = -1.000000e+00f;
		return;
	}
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) tmpu[j][k] = 0;
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) du[j][k] = 0;
	for(j=1;j<=10;++j) for(k=1;k<=1;++k) Yzad[j][k] = 0;
	for(j=1;j<=2;++j) for(k=1;k<=5;++k) y0[j][k] = 0;
	for(j=1;j<=10;++j) for(k=1;k<=1;++k) Y0[j][k] = 0;
	itmp=1;
	for(i=1;i<=5;++i){
		for(j=1;j<=2;++j){
			Yzad[itmp][1]=ad->z[j-1];
			itmp=itmp+1;
		}
	}
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) uk[j][k] = 0;
	for(i=1;i<=2;++i){
		uk[i][1]=ad->u[ad->k-1][i-1];
	}
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) disturbance[j][k] = 0;
	for(m=1;m<=2;++m){
		disturbance[m][1]=0;
		for(n=1;n<=2;++n){
			for(i=1;i<=4;++i){
				disturbance[m][1]=disturbance[m][1]+GPC_b[m][n][i]*ad->u[ad->k-i][n-1];
			}
		}
		for(i=1;i<=2;++i){
			disturbance[m][1]=disturbance[m][1]-GPC_a[m][i]*ad->y[ad->k-i][m-1];
		}
		disturbance[m][1]=ad->y[ad->k][m-1]-disturbance[m][1];
		for(p=1;p<=5;++p){
			Iyf=p-1;
			if(2<Iyf){
				Iyf=2;
			}
			Iuftmp=4;
			if(p<Iuftmp){
				Iuftmp=p;
			}
			Iuf=0;
			if(Iuftmp>Iuf){
				Iuf=Iuftmp;
			}
			y0[m][p]=disturbance[m][1];
			for(n=1;n<=2;++n){
				for(r=1;r<=Iuf;++r){
					y0[m][p]=y0[m][p]+GPC_b[m][n][r]*ad->u[ad->k-1][n-1];
				}
			}
			for(n=1;n<=2;++n){
				for(r=(Iuf+1);r<=4;++r){
					y0[m][p]=y0[m][p]+GPC_b[m][n][r]*ad->u[ad->k-r+p][n-1];
				}
			}
			for(r=1;r<=Iyf;++r){
				y0[m][p]=y0[m][p]-GPC_a[m][r]*y0[m][p-r];
			}
			for(r=(Iyf+1);r<=2;++r){
				y0[m][p]=y0[m][p]-GPC_a[m][r]*ad->y[ad->k-r+p][m-1];
			}
		}
	}
	itmp=1;
	for(i=1;i<=5;++i){
		for(j=1;j<=2;++j){
			Y0[itmp][1]=y0[j][i];
			itmp=itmp+1;
		}
	}
	sumaa(Yzad,Y0,ftmp1,10,1,-1);
	productab(fconst,ftmp1,ftmp4,10,10,10,1);
	for(i=1;i<=10;++i){
		ftmp[i]=ftmp4[i][1];
	}
	productab(bvar,uk,btmp1,40,2,2,1);
	sumaa(b,btmp1,btmp2,40,1,1);
	for(i=1;i<=40;++i){
		btmp[i]=btmp2[i][1];
	}
	for(i=1;i<=2;++i){
		qpx[i]=0;
	}
	productv(btmp,-1,btmp,40);qp();
	for(i=1;i<=2;++i){
		du[i][1]=qpx[i];
	}
	for(n=1;n<=2;++n){
		if(du[n][1]>dumax[1][n]){
			du[n][1]=dumax[1][n];
		}
		if(du[n][1]<dumin[1][n]){
			du[n][1]=dumin[1][n];
		}
		tmpu[n][1]=ad->u[ad->k-1][n-1]+du[n][1];
		if(tmpu[n][1]>umax[1][n]){
			tmpu[n][1]=umax[1][n];
		}
		if(tmpu[n][1]<umin[1][n]){
			tmpu[n][1]=umin[1][n];
		}
		du[n][1]=tmpu[n][1]-ad->u[ad->k-1][n-1];
	}
	for(j=1;j<=1;++j) for(k=1;k<=2;++k) control_value[j][k] = 0;
	for(n=1;n<=2;++n){
		control_value[1][n]=du[n][1];
	}

	set_current_control_increment(c,&(control_value[1][1])); // du is indexed starting with 1, therefore to maintain compatibility it is required to refer to first element of an actual array
}

void controller_setup(){
profiler_start(2);
    init_archive_data(&ad, 200, 200, 2, 2, 0, 0, 0.01);
    init_current_control(&cc,&ad);
    controller(NULL,NULL);
profiler_end(2);
}

void idle(){
profiler_start(13);
    const int k = 0;
	static int i = 0;
	static char str[1000] = {0};

    sprintf(str, "x = [%f,%f,",ad.y[k][0], ad.y[k][1]);    write_string(str);
    sprintf(str,      "%f,%f,",ad.z[0], ad.z[1]);          write_string(str);
    sprintf(str,      "%f,%f,",ad.u[k-1][0],ad.u[k-1][1]); write_string(str);
    write_string("];\n\r");
	if(++i > 510) profiler_print();
profiler_end(13);
}

void loop(){
profiler_start(10);
    static int i = 0;
	     if(i< 100){ ad.z[0] = -0.1f; ad.z[1] = +0.2f; }
	else if(i< 150){ ad.z[0] = -0.1f; ad.z[1] = -0.2f; }
	else if(i< 250){ ad.z[0] = +0.1f; ad.z[1] = -0.2f; }
	else           { ad.z[0] = +0.1f; ad.z[1] = +0.2f; }
	if(++i > 300) i = 0;
	
profiler_start(50);
	controller(&ad,&cc);
profiler_end(50);
	
	push_current_controls_to_archive_data(&cc,&ad);	
profiler_end(10);
}

void timeout(){
    while(1);
}
