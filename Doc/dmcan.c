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

int algorithm_used = 0;

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

void controllerDMCA(ArchiveData * ad, CurrentControl * c){
	static float** AutoMATiC_DMC_Y;
	static float** AutoMATiC_DMC_Yzad;
	static float** AutoMATiC_DMC_dUp;
	static float** AutoMATiC_DMC_du;
	static float** AutoMATiC_DMC_dutmp1;
	static float** AutoMATiC_DMC_dutmp2;
	static float** AutoMATiC_DMC_e;
	static long AutoMATiC_DMC_i;
	static long AutoMATiC_DMC_itmp;
	static long AutoMATiC_DMC_j;
	static long AutoMATiC_DMC_n;
	static float** AutoMATiC_DMC_tmpu;
	static float** control_value;
	static long j;
	static long k;
	static float** AutoMATiC_DMC_Ke;
	static float** AutoMATiC_DMC_Ku;
	static float** dumax;
	static float** dumin;
	static float** umax;
	static float** umin;
	if(ad == NULL){
		AutoMATiC_DMC_dUp = darray(1,8,1,1);
		AutoMATiC_DMC_Yzad = darray(1,10,1,1);
		AutoMATiC_DMC_Y = darray(1,10,1,1);
		AutoMATiC_DMC_tmpu = darray(1,2,1,1);
		AutoMATiC_DMC_e = darray(1,2,1,1);
		control_value = darray(1,1,1,2);
		AutoMATiC_DMC_dutmp1 = darray(1,2,1,1);
		AutoMATiC_DMC_Ke = darray(1,2,1,2);
		AutoMATiC_DMC_Ke[1][1] = 3.373006e-01f;
		AutoMATiC_DMC_Ke[1][2] = -8.539666e-02f;
		AutoMATiC_DMC_Ke[2][1] = -1.016548e-01f;
		AutoMATiC_DMC_Ke[2][2] = 5.115693e-01f;
		AutoMATiC_DMC_dutmp2 = darray(1,2,1,1);
		AutoMATiC_DMC_Ku = darray(1,2,1,8);
		AutoMATiC_DMC_Ku[1][1] = 1.246690e+00f;
		AutoMATiC_DMC_Ku[1][2] = 1.325999e-01f;
		AutoMATiC_DMC_Ku[1][3] = 6.350099e-01f;
		AutoMATiC_DMC_Ku[1][4] = 6.239113e-02f;
		AutoMATiC_DMC_Ku[1][5] = 3.043741e-01f;
		AutoMATiC_DMC_Ku[1][6] = 2.660202e-02f;
		AutoMATiC_DMC_Ku[1][7] = 1.005321e-01f;
		AutoMATiC_DMC_Ku[1][8] = 7.749453e-03f;
		AutoMATiC_DMC_Ku[2][1] = 6.013931e-02f;
		AutoMATiC_DMC_Ku[2][2] = 8.674500e-01f;
		AutoMATiC_DMC_Ku[2][3] = -1.172849e-01f;
		AutoMATiC_DMC_Ku[2][4] = 2.406519e-01f;
		AutoMATiC_DMC_Ku[2][5] = -7.501822e-02f;
		AutoMATiC_DMC_Ku[2][6] = 6.519486e-02f;
		AutoMATiC_DMC_Ku[2][7] = -2.764289e-02f;
		AutoMATiC_DMC_Ku[2][8] = 1.396884e-02f;
		AutoMATiC_DMC_du = darray(1,2,1,1);
		dumax = darray(1,1,1,2);
		dumax[1][1] = 1.000000e-02f;
		dumax[1][2] = 1.000000e-02f;
		dumin = darray(1,1,1,2);
		dumin[1][1] = -1.000000e-02f;
		dumin[1][2] = -1.000000e-02f;
		umax = darray(1,1,1,2);
		umax[1][1] = 1.000000e+00f;
		umax[1][2] = 1.000000e+00f;
		umin = darray(1,1,1,2);
		umin[1][1] = -1.000000e+00f;
		umin[1][2] = -1.000000e+00f;
		return;
	}
	for(j=1;j<=8;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_dUp[j][k] = 0;
	for(j=1;j<=10;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_Yzad[j][k] = 0;
	for(j=1;j<=10;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_Y[j][k] = 0;
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_tmpu[j][k] = 0;
	AutoMATiC_DMC_itmp=1;
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=4;++AutoMATiC_DMC_i){
		for(AutoMATiC_DMC_j=1;AutoMATiC_DMC_j<=2;++AutoMATiC_DMC_j){
			AutoMATiC_DMC_dUp[AutoMATiC_DMC_itmp][1]=ad->du[ad->k-AutoMATiC_DMC_i][AutoMATiC_DMC_j-1];
			AutoMATiC_DMC_itmp=AutoMATiC_DMC_itmp+1;
		}
	}
	AutoMATiC_DMC_itmp=1;
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=5;++AutoMATiC_DMC_i){
		for(AutoMATiC_DMC_j=1;AutoMATiC_DMC_j<=2;++AutoMATiC_DMC_j){
			AutoMATiC_DMC_Yzad[AutoMATiC_DMC_itmp][1]=ad->z[AutoMATiC_DMC_j-1];
			AutoMATiC_DMC_itmp=AutoMATiC_DMC_itmp+1;
		}
	}
	AutoMATiC_DMC_itmp=1;
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=5;++AutoMATiC_DMC_i){
		for(AutoMATiC_DMC_j=1;AutoMATiC_DMC_j<=2;++AutoMATiC_DMC_j){
			AutoMATiC_DMC_Y[AutoMATiC_DMC_itmp][1]=ad->y[ad->k][AutoMATiC_DMC_j-1];
			AutoMATiC_DMC_itmp=AutoMATiC_DMC_itmp+1;
		}
	}
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_e[j][k] = 0;
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=2;++AutoMATiC_DMC_i){
		AutoMATiC_DMC_e[AutoMATiC_DMC_i][1]=ad->z[AutoMATiC_DMC_i-1]-ad->y[ad->k][AutoMATiC_DMC_i-1];
	}
	productab(AutoMATiC_DMC_Ke,AutoMATiC_DMC_e,AutoMATiC_DMC_dutmp1,2,2,2,1);
	productab(AutoMATiC_DMC_Ku,AutoMATiC_DMC_dUp,AutoMATiC_DMC_dutmp2,2,8,8,1);
	sumaa(AutoMATiC_DMC_dutmp1,AutoMATiC_DMC_dutmp2,AutoMATiC_DMC_du,2,1,-1);
	for(AutoMATiC_DMC_n=1;AutoMATiC_DMC_n<=2;++AutoMATiC_DMC_n){
		if(AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]>dumax[1][AutoMATiC_DMC_n]){
			AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]=dumax[1][AutoMATiC_DMC_n];
		}
		if(AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]<dumin[1][AutoMATiC_DMC_n]){
			AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]=dumin[1][AutoMATiC_DMC_n];
		}
		AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]=ad->u[ad->k-1][AutoMATiC_DMC_n-1]+AutoMATiC_DMC_du[AutoMATiC_DMC_n][1];
		if(AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]>umax[1][AutoMATiC_DMC_n]){
			AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]=umax[1][AutoMATiC_DMC_n];
		}
		if(AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]<umin[1][AutoMATiC_DMC_n]){
			AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]=umin[1][AutoMATiC_DMC_n];
		}
		AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]=AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]-ad->u[ad->k-1][AutoMATiC_DMC_n-1];
	}
	for(j=1;j<=1;++j) for(k=1;k<=2;++k) control_value[j][k] = 0;
	for(AutoMATiC_DMC_n=1;AutoMATiC_DMC_n<=2;++AutoMATiC_DMC_n){
		control_value[1][AutoMATiC_DMC_n]=AutoMATiC_DMC_du[AutoMATiC_DMC_n][1];
	}

	set_current_control_increment(c,&(control_value[1][1])); // du is indexed starting with 1, therefore to maintain compatibility it is required to refer to first element of an actual array
}
void controllerDMCN(ArchiveData * ad, CurrentControl * c){
	static float** AutoMATiC_DMC_Y;
	static float** AutoMATiC_DMC_Yzad;
	static float** AutoMATiC_DMC_btmp1;
	static float** AutoMATiC_DMC_btmp2;
	static float** AutoMATiC_DMC_dUp;
	static float** AutoMATiC_DMC_du;
	static float** AutoMATiC_DMC_dutmp1;
	static float** AutoMATiC_DMC_dutmp2;
	static float** AutoMATiC_DMC_e;
	static float** AutoMATiC_DMC_ftmp1;
	static float** AutoMATiC_DMC_ftmp2;
	static float** AutoMATiC_DMC_ftmp3;
	static float** AutoMATiC_DMC_ftmp4;
	static long AutoMATiC_DMC_i;
	static long AutoMATiC_DMC_itmp;
	static long AutoMATiC_DMC_j;
	static long AutoMATiC_DMC_n;
	static float** AutoMATiC_DMC_tmpu;
	static float** AutoMATiC_DMC_uk;
	static float** control_value;
	static long j;
	static long k;
	static float** AutoMATiC_DMC_Ke;
	static float** AutoMATiC_DMC_Ku;
	static float** AutoMATiC_DMC_Mp;
	static float** AutoMATiC_DMC_fconst;
	static float** AutoMATiC_DMC_bvar;
	static float** AutoMATiC_DMC_b;
	static float** dumax;
	static float** dumin;
	static float** umax;
	static float** umin;
	static float ** AutoMATiC_DMC_H;
	static float ** AutoMATiC_DMC_A;
	static float * AutoMATiC_DMC_ftmp;
	static float * AutoMATiC_DMC_btmp;
	static float * AutoMATiC_DMC_qpx;
	if(ad == NULL){
		defineconnum(40);
		definevarnum(10);
		initializeqp();
		AutoMATiC_DMC_A=obtainA();
		AutoMATiC_DMC_H=obtainG();
		AutoMATiC_DMC_btmp=obtainb();
		AutoMATiC_DMC_ftmp=obtaint();
		AutoMATiC_DMC_qpx=obtainx();
		AutoMATiC_DMC_A[1][1] = 1.000000e+00f;
		AutoMATiC_DMC_A[1][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[1][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[1][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[1][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[1][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[1][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[1][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[1][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[1][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[2][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[2][2] = 1.000000e+00f;
		AutoMATiC_DMC_A[2][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[2][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[2][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[2][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[2][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[2][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[2][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[2][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[3][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[3][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[3][3] = 1.000000e+00f;
		AutoMATiC_DMC_A[3][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[3][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[3][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[3][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[3][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[3][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[3][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[4][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[4][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[4][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[4][4] = 1.000000e+00f;
		AutoMATiC_DMC_A[4][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[4][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[4][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[4][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[4][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[4][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[5][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[5][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[5][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[5][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[5][5] = 1.000000e+00f;
		AutoMATiC_DMC_A[5][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[5][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[5][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[5][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[5][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[6][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[6][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[6][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[6][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[6][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[6][6] = 1.000000e+00f;
		AutoMATiC_DMC_A[6][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[6][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[6][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[6][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[7][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[7][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[7][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[7][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[7][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[7][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[7][7] = 1.000000e+00f;
		AutoMATiC_DMC_A[7][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[7][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[7][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[8][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[8][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[8][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[8][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[8][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[8][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[8][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[8][8] = 1.000000e+00f;
		AutoMATiC_DMC_A[8][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[8][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[9][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[9][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[9][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[9][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[9][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[9][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[9][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[9][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[9][9] = 1.000000e+00f;
		AutoMATiC_DMC_A[9][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[10][10] = 1.000000e+00f;
		AutoMATiC_DMC_A[11][1] = -1.000000e+00f;
		AutoMATiC_DMC_A[11][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[11][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[11][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[11][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[11][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[11][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[11][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[11][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[11][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[12][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[12][2] = -1.000000e+00f;
		AutoMATiC_DMC_A[12][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[12][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[12][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[12][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[12][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[12][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[12][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[12][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[13][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[13][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[13][3] = -1.000000e+00f;
		AutoMATiC_DMC_A[13][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[13][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[13][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[13][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[13][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[13][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[13][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[14][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[14][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[14][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[14][4] = -1.000000e+00f;
		AutoMATiC_DMC_A[14][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[14][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[14][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[14][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[14][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[14][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[15][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[15][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[15][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[15][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[15][5] = -1.000000e+00f;
		AutoMATiC_DMC_A[15][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[15][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[15][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[15][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[15][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[16][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[16][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[16][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[16][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[16][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[16][6] = -1.000000e+00f;
		AutoMATiC_DMC_A[16][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[16][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[16][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[16][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[17][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[17][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[17][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[17][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[17][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[17][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[17][7] = -1.000000e+00f;
		AutoMATiC_DMC_A[17][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[17][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[17][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[18][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[18][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[18][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[18][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[18][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[18][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[18][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[18][8] = -1.000000e+00f;
		AutoMATiC_DMC_A[18][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[18][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[19][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[19][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[19][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[19][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[19][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[19][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[19][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[19][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[19][9] = -1.000000e+00f;
		AutoMATiC_DMC_A[19][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[20][10] = -1.000000e+00f;
		AutoMATiC_DMC_A[21][1] = 1.000000e+00f;
		AutoMATiC_DMC_A[21][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[21][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[21][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[21][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[21][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[21][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[21][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[21][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[21][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[22][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[22][2] = 1.000000e+00f;
		AutoMATiC_DMC_A[22][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[22][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[22][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[22][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[22][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[22][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[22][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[22][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[23][1] = 1.000000e+00f;
		AutoMATiC_DMC_A[23][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[23][3] = 1.000000e+00f;
		AutoMATiC_DMC_A[23][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[23][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[23][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[23][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[23][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[23][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[23][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[24][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[24][2] = 1.000000e+00f;
		AutoMATiC_DMC_A[24][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[24][4] = 1.000000e+00f;
		AutoMATiC_DMC_A[24][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[24][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[24][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[24][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[24][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[24][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[25][1] = 1.000000e+00f;
		AutoMATiC_DMC_A[25][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[25][3] = 1.000000e+00f;
		AutoMATiC_DMC_A[25][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[25][5] = 1.000000e+00f;
		AutoMATiC_DMC_A[25][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[25][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[25][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[25][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[25][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[26][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[26][2] = 1.000000e+00f;
		AutoMATiC_DMC_A[26][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[26][4] = 1.000000e+00f;
		AutoMATiC_DMC_A[26][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[26][6] = 1.000000e+00f;
		AutoMATiC_DMC_A[26][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[26][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[26][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[26][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[27][1] = 1.000000e+00f;
		AutoMATiC_DMC_A[27][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[27][3] = 1.000000e+00f;
		AutoMATiC_DMC_A[27][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[27][5] = 1.000000e+00f;
		AutoMATiC_DMC_A[27][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[27][7] = 1.000000e+00f;
		AutoMATiC_DMC_A[27][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[27][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[27][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[28][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[28][2] = 1.000000e+00f;
		AutoMATiC_DMC_A[28][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[28][4] = 1.000000e+00f;
		AutoMATiC_DMC_A[28][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[28][6] = 1.000000e+00f;
		AutoMATiC_DMC_A[28][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[28][8] = 1.000000e+00f;
		AutoMATiC_DMC_A[28][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[28][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[29][1] = 1.000000e+00f;
		AutoMATiC_DMC_A[29][2] = 0.000000e+00f;
		AutoMATiC_DMC_A[29][3] = 1.000000e+00f;
		AutoMATiC_DMC_A[29][4] = 0.000000e+00f;
		AutoMATiC_DMC_A[29][5] = 1.000000e+00f;
		AutoMATiC_DMC_A[29][6] = 0.000000e+00f;
		AutoMATiC_DMC_A[29][7] = 1.000000e+00f;
		AutoMATiC_DMC_A[29][8] = 0.000000e+00f;
		AutoMATiC_DMC_A[29][9] = 1.000000e+00f;
		AutoMATiC_DMC_A[29][10] = 0.000000e+00f;
		AutoMATiC_DMC_A[30][1] = 0.000000e+00f;
		AutoMATiC_DMC_A[30][2] = 1.000000e+00f;
		AutoMATiC_DMC_A[30][3] = 0.000000e+00f;
		AutoMATiC_DMC_A[30][4] = 1.000000e+00f;
		AutoMATiC_DMC_A[30][5] = 0.000000e+00f;
		AutoMATiC_DMC_A[30][6] = 1.000000e+00f;
		AutoMATiC_DMC_A[30][7] = 0.000000e+00f;
		AutoMATiC_DMC_A[30][8] = 1.000000e+00f;
		AutoMATiC_DMC_A[30][9] = 0.000000e+00f;
		AutoMATiC_DMC_A[30][10] = 1.000000e+00f;
		AutoMATiC_DMC_A[31][1] = -1.000000e+00f;
		AutoMATiC_DMC_A[31][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[31][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[31][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[31][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[31][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[31][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[31][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[31][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[31][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[32][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[32][2] = -1.000000e+00f;
		AutoMATiC_DMC_A[32][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[32][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[32][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[32][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[32][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[32][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[32][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[32][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[33][1] = -1.000000e+00f;
		AutoMATiC_DMC_A[33][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[33][3] = -1.000000e+00f;
		AutoMATiC_DMC_A[33][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[33][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[33][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[33][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[33][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[33][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[33][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[34][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[34][2] = -1.000000e+00f;
		AutoMATiC_DMC_A[34][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[34][4] = -1.000000e+00f;
		AutoMATiC_DMC_A[34][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[34][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[34][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[34][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[34][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[34][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[35][1] = -1.000000e+00f;
		AutoMATiC_DMC_A[35][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[35][3] = -1.000000e+00f;
		AutoMATiC_DMC_A[35][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[35][5] = -1.000000e+00f;
		AutoMATiC_DMC_A[35][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[35][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[35][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[35][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[35][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[36][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[36][2] = -1.000000e+00f;
		AutoMATiC_DMC_A[36][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[36][4] = -1.000000e+00f;
		AutoMATiC_DMC_A[36][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[36][6] = -1.000000e+00f;
		AutoMATiC_DMC_A[36][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[36][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[36][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[36][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[37][1] = -1.000000e+00f;
		AutoMATiC_DMC_A[37][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[37][3] = -1.000000e+00f;
		AutoMATiC_DMC_A[37][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[37][5] = -1.000000e+00f;
		AutoMATiC_DMC_A[37][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[37][7] = -1.000000e+00f;
		AutoMATiC_DMC_A[37][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[37][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[37][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[38][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[38][2] = -1.000000e+00f;
		AutoMATiC_DMC_A[38][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[38][4] = -1.000000e+00f;
		AutoMATiC_DMC_A[38][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[38][6] = -1.000000e+00f;
		AutoMATiC_DMC_A[38][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[38][8] = -1.000000e+00f;
		AutoMATiC_DMC_A[38][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[38][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[39][1] = -1.000000e+00f;
		AutoMATiC_DMC_A[39][2] = -0.000000e+00f;
		AutoMATiC_DMC_A[39][3] = -1.000000e+00f;
		AutoMATiC_DMC_A[39][4] = -0.000000e+00f;
		AutoMATiC_DMC_A[39][5] = -1.000000e+00f;
		AutoMATiC_DMC_A[39][6] = -0.000000e+00f;
		AutoMATiC_DMC_A[39][7] = -1.000000e+00f;
		AutoMATiC_DMC_A[39][8] = -0.000000e+00f;
		AutoMATiC_DMC_A[39][9] = -1.000000e+00f;
		AutoMATiC_DMC_A[39][10] = -0.000000e+00f;
		AutoMATiC_DMC_A[40][1] = -0.000000e+00f;
		AutoMATiC_DMC_A[40][2] = -1.000000e+00f;
		AutoMATiC_DMC_A[40][3] = -0.000000e+00f;
		AutoMATiC_DMC_A[40][4] = -1.000000e+00f;
		AutoMATiC_DMC_A[40][5] = -0.000000e+00f;
		AutoMATiC_DMC_A[40][6] = -1.000000e+00f;
		AutoMATiC_DMC_A[40][7] = -0.000000e+00f;
		AutoMATiC_DMC_A[40][8] = -1.000000e+00f;
		AutoMATiC_DMC_A[40][9] = -0.000000e+00f;
		AutoMATiC_DMC_A[40][10] = -1.000000e+00f;
		AutoMATiC_DMC_H[1][1] = 1.326566e+02f;
		AutoMATiC_DMC_H[1][2] = 4.114478e+01f;
		AutoMATiC_DMC_H[1][3] = 1.000269e+02f;
		AutoMATiC_DMC_H[1][4] = 3.169500e+01f;
		AutoMATiC_DMC_H[1][5] = 6.190011e+01f;
		AutoMATiC_DMC_H[1][6] = 2.023139e+01f;
		AutoMATiC_DMC_H[1][7] = 2.567725e+01f;
		AutoMATiC_DMC_H[1][8] = 8.808511e+00f;
		AutoMATiC_DMC_H[1][9] = 0.000000e+00f;
		AutoMATiC_DMC_H[1][10] = 0.000000e+00f;
		AutoMATiC_DMC_H[2][1] = 4.114478e+01f;
		AutoMATiC_DMC_H[2][2] = 3.256150e+01f;
		AutoMATiC_DMC_H[2][3] = 3.099487e+01f;
		AutoMATiC_DMC_H[2][4] = 2.477126e+01f;
		AutoMATiC_DMC_H[2][5] = 1.931418e+01f;
		AutoMATiC_DMC_H[2][6] = 1.576652e+01f;
		AutoMATiC_DMC_H[2][7] = 8.233726e+00f;
		AutoMATiC_DMC_H[2][8] = 6.910587e+00f;
		AutoMATiC_DMC_H[2][9] = 0.000000e+00f;
		AutoMATiC_DMC_H[2][10] = 0.000000e+00f;
		AutoMATiC_DMC_H[3][1] = 1.000269e+02f;
		AutoMATiC_DMC_H[3][2] = 3.099487e+01f;
		AutoMATiC_DMC_H[3][3] = 8.624002e+01f;
		AutoMATiC_DMC_H[3][4] = 2.792375e+01f;
		AutoMATiC_DMC_H[3][5] = 5.644363e+01f;
		AutoMATiC_DMC_H[3][6] = 1.883764e+01f;
		AutoMATiC_DMC_H[3][7] = 2.413991e+01f;
		AutoMATiC_DMC_H[3][8] = 8.414358e+00f;
		AutoMATiC_DMC_H[3][9] = 0.000000e+00f;
		AutoMATiC_DMC_H[3][10] = 0.000000e+00f;
		AutoMATiC_DMC_H[4][1] = 3.169500e+01f;
		AutoMATiC_DMC_H[4][2] = 2.477126e+01f;
		AutoMATiC_DMC_H[4][3] = 2.792375e+01f;
		AutoMATiC_DMC_H[4][4] = 2.274154e+01f;
		AutoMATiC_DMC_H[4][5] = 1.838345e+01f;
		AutoMATiC_DMC_H[4][6] = 1.514642e+01f;
		AutoMATiC_DMC_H[4][7] = 8.018635e+00f;
		AutoMATiC_DMC_H[4][8] = 6.775021e+00f;
		AutoMATiC_DMC_H[4][9] = 0.000000e+00f;
		AutoMATiC_DMC_H[4][10] = 0.000000e+00f;
		AutoMATiC_DMC_H[5][1] = 6.190011e+01f;
		AutoMATiC_DMC_H[5][2] = 1.931418e+01f;
		AutoMATiC_DMC_H[5][3] = 5.644363e+01f;
		AutoMATiC_DMC_H[5][4] = 1.838345e+01f;
		AutoMATiC_DMC_H[5][5] = 4.531061e+01f;
		AutoMATiC_DMC_H[5][6] = 1.565679e+01f;
		AutoMATiC_DMC_H[5][7] = 2.097070e+01f;
		AutoMATiC_DMC_H[5][8] = 7.558490e+00f;
		AutoMATiC_DMC_H[5][9] = 0.000000e+00f;
		AutoMATiC_DMC_H[5][10] = 0.000000e+00f;
		AutoMATiC_DMC_H[6][1] = 2.023139e+01f;
		AutoMATiC_DMC_H[6][2] = 1.576652e+01f;
		AutoMATiC_DMC_H[6][3] = 1.883764e+01f;
		AutoMATiC_DMC_H[6][4] = 1.514642e+01f;
		AutoMATiC_DMC_H[6][5] = 1.565679e+01f;
		AutoMATiC_DMC_H[6][6] = 1.330759e+01f;
		AutoMATiC_DMC_H[6][7] = 7.391926e+00f;
		AutoMATiC_DMC_H[6][8] = 6.332568e+00f;
		AutoMATiC_DMC_H[6][9] = 0.000000e+00f;
		AutoMATiC_DMC_H[6][10] = 0.000000e+00f;
		AutoMATiC_DMC_H[7][1] = 2.567725e+01f;
		AutoMATiC_DMC_H[7][2] = 8.233726e+00f;
		AutoMATiC_DMC_H[7][3] = 2.413991e+01f;
		AutoMATiC_DMC_H[7][4] = 8.018635e+00f;
		AutoMATiC_DMC_H[7][5] = 2.097070e+01f;
		AutoMATiC_DMC_H[7][6] = 7.391926e+00f;
		AutoMATiC_DMC_H[7][7] = 1.454423e+01f;
		AutoMATiC_DMC_H[7][8] = 5.541646e+00f;
		AutoMATiC_DMC_H[7][9] = 0.000000e+00f;
		AutoMATiC_DMC_H[7][10] = 0.000000e+00f;
		AutoMATiC_DMC_H[8][1] = 8.808511e+00f;
		AutoMATiC_DMC_H[8][2] = 6.910587e+00f;
		AutoMATiC_DMC_H[8][3] = 8.414358e+00f;
		AutoMATiC_DMC_H[8][4] = 6.775021e+00f;
		AutoMATiC_DMC_H[8][5] = 7.558490e+00f;
		AutoMATiC_DMC_H[8][6] = 6.332568e+00f;
		AutoMATiC_DMC_H[8][7] = 5.541646e+00f;
		AutoMATiC_DMC_H[8][8] = 5.071756e+00f;
		AutoMATiC_DMC_H[8][9] = 0.000000e+00f;
		AutoMATiC_DMC_H[8][10] = 0.000000e+00f;
		AutoMATiC_DMC_H[9][1] = 0.000000e+00f;
		AutoMATiC_DMC_H[9][2] = 0.000000e+00f;
		AutoMATiC_DMC_H[9][3] = 0.000000e+00f;
		AutoMATiC_DMC_H[9][4] = 0.000000e+00f;
		AutoMATiC_DMC_H[9][5] = 0.000000e+00f;
		AutoMATiC_DMC_H[9][6] = 0.000000e+00f;
		AutoMATiC_DMC_H[9][7] = 0.000000e+00f;
		AutoMATiC_DMC_H[9][8] = 0.000000e+00f;
		AutoMATiC_DMC_H[9][9] = 2.000000e-01f;
		AutoMATiC_DMC_H[9][10] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][1] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][2] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][3] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][4] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][5] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][6] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][7] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][8] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][9] = 0.000000e+00f;
		AutoMATiC_DMC_H[10][10] = 2.000000e-01f;
		AutoMATiC_DMC_dUp = darray(1,8,1,1);
		AutoMATiC_DMC_Yzad = darray(1,10,1,1);
		AutoMATiC_DMC_Y = darray(1,10,1,1);
		AutoMATiC_DMC_tmpu = darray(1,2,1,1);
		AutoMATiC_DMC_e = darray(1,2,1,1);
		AutoMATiC_DMC_du = darray(1,2,1,1);
		AutoMATiC_DMC_uk = darray(1,2,1,1);
		control_value = darray(1,1,1,2);
		AutoMATiC_DMC_dutmp1 = darray(1,2,1,1);
		AutoMATiC_DMC_Ke = darray(1,2,1,2);
		AutoMATiC_DMC_Ke[1][1] = 4.287541e-01f;
		AutoMATiC_DMC_Ke[1][2] = -1.697703e-01f;
		AutoMATiC_DMC_Ke[2][1] = -2.150208e-01f;
		AutoMATiC_DMC_Ke[2][2] = 7.408936e-01f;
		AutoMATiC_DMC_dutmp2 = darray(1,2,1,1);
		AutoMATiC_DMC_Ku = darray(1,2,1,8);
		AutoMATiC_DMC_Ku[1][1] = 1.478655e+00f;
		AutoMATiC_DMC_Ku[1][2] = 5.747707e-02f;
		AutoMATiC_DMC_Ku[1][3] = 7.739905e-01f;
		AutoMATiC_DMC_Ku[1][4] = 4.678980e-02f;
		AutoMATiC_DMC_Ku[1][5] = 3.847941e-01f;
		AutoMATiC_DMC_Ku[1][6] = 2.458927e-02f;
		AutoMATiC_DMC_Ku[1][7] = 1.274552e-01f;
		AutoMATiC_DMC_Ku[1][8] = 7.796081e-03f;
		AutoMATiC_DMC_Ku[2][1] = -1.211545e-01f;
		AutoMATiC_DMC_Ku[2][2] = 1.179103e+00f;
		AutoMATiC_DMC_Ku[2][3] = -2.753100e-01f;
		AutoMATiC_DMC_Ku[2][4] = 3.224919e-01f;
		AutoMATiC_DMC_Ku[2][5] = -1.704160e-01f;
		AutoMATiC_DMC_Ku[2][6] = 8.648664e-02f;
		AutoMATiC_DMC_Ku[2][7] = -6.033504e-02f;
		AutoMATiC_DMC_Ku[2][8] = 1.809708e-02f;
		AutoMATiC_DMC_ftmp1 = darray(1,10,1,1);
		AutoMATiC_DMC_ftmp2 = darray(1,10,1,1);
		AutoMATiC_DMC_Mp = darray(1,10,1,8);
		AutoMATiC_DMC_Mp[1][1] = 2.552292e+00f;
		AutoMATiC_DMC_Mp[1][2] = 6.321206e-01f;
		AutoMATiC_DMC_Mp[1][3] = 1.249453e+00f;
		AutoMATiC_DMC_Mp[1][4] = 2.325442e-01f;
		AutoMATiC_DMC_Mp[1][5] = 6.116594e-01f;
		AutoMATiC_DMC_Mp[1][6] = 8.554821e-02f;
		AutoMATiC_DMC_Mp[1][7] = 2.994327e-01f;
		AutoMATiC_DMC_Mp[1][8] = 3.147143e-02f;
		AutoMATiC_DMC_Mp[2][1] = 8.111244e-01f;
		AutoMATiC_DMC_Mp[2][2] = 1.426990e+00f;
		AutoMATiC_DMC_Mp[2][3] = 1.532016e-01f;
		AutoMATiC_DMC_Mp[2][4] = 4.088396e-01f;
		AutoMATiC_DMC_Mp[2][5] = 2.893605e-02f;
		AutoMATiC_DMC_Mp[2][6] = 1.171345e-01f;
		AutoMATiC_DMC_Mp[2][7] = 5.465313e-03f;
		AutoMATiC_DMC_Mp[2][8] = 3.355960e-02f;
		AutoMATiC_DMC_Mp[3][1] = 3.801745e+00f;
		AutoMATiC_DMC_Mp[3][2] = 8.646647e-01f;
		AutoMATiC_DMC_Mp[3][3] = 1.861112e+00f;
		AutoMATiC_DMC_Mp[3][4] = 3.180924e-01f;
		AutoMATiC_DMC_Mp[3][5] = 9.110921e-01f;
		AutoMATiC_DMC_Mp[3][6] = 1.170196e-01f;
		AutoMATiC_DMC_Mp[3][7] = 2.994327e-01f;
		AutoMATiC_DMC_Mp[3][8] = 3.147143e-02f;
		AutoMATiC_DMC_Mp[4][1] = 9.643260e-01f;
		AutoMATiC_DMC_Mp[4][2] = 1.835830e+00f;
		AutoMATiC_DMC_Mp[4][3] = 1.821377e-01f;
		AutoMATiC_DMC_Mp[4][4] = 5.259741e-01f;
		AutoMATiC_DMC_Mp[4][5] = 3.440136e-02f;
		AutoMATiC_DMC_Mp[4][6] = 1.506941e-01f;
		AutoMATiC_DMC_Mp[4][7] = 5.465313e-03f;
		AutoMATiC_DMC_Mp[4][8] = 3.355960e-02f;
		AutoMATiC_DMC_Mp[5][1] = 4.413404e+00f;
		AutoMATiC_DMC_Mp[5][2] = 9.502129e-01f;
		AutoMATiC_DMC_Mp[5][3] = 2.160545e+00f;
		AutoMATiC_DMC_Mp[5][4] = 3.495638e-01f;
		AutoMATiC_DMC_Mp[5][5] = 9.110921e-01f;
		AutoMATiC_DMC_Mp[5][6] = 1.170196e-01f;
		AutoMATiC_DMC_Mp[5][7] = 2.994327e-01f;
		AutoMATiC_DMC_Mp[5][8] = 3.147143e-02f;
		AutoMATiC_DMC_Mp[6][1] = 9.932621e-01f;
		AutoMATiC_DMC_Mp[6][2] = 1.952965e+00f;
		AutoMATiC_DMC_Mp[6][3] = 1.876030e-01f;
		AutoMATiC_DMC_Mp[6][4] = 5.595337e-01f;
		AutoMATiC_DMC_Mp[6][5] = 3.440136e-02f;
		AutoMATiC_DMC_Mp[6][6] = 1.506941e-01f;
		AutoMATiC_DMC_Mp[6][7] = 5.465313e-03f;
		AutoMATiC_DMC_Mp[6][8] = 3.355960e-02f;
		AutoMATiC_DMC_Mp[7][1] = 4.712837e+00f;
		AutoMATiC_DMC_Mp[7][2] = 9.816844e-01f;
		AutoMATiC_DMC_Mp[7][3] = 2.160545e+00f;
		AutoMATiC_DMC_Mp[7][4] = 3.495638e-01f;
		AutoMATiC_DMC_Mp[7][5] = 9.110921e-01f;
		AutoMATiC_DMC_Mp[7][6] = 1.170196e-01f;
		AutoMATiC_DMC_Mp[7][7] = 2.994327e-01f;
		AutoMATiC_DMC_Mp[7][8] = 3.147143e-02f;
		AutoMATiC_DMC_Mp[8][1] = 9.987274e-01f;
		AutoMATiC_DMC_Mp[8][2] = 1.986524e+00f;
		AutoMATiC_DMC_Mp[8][3] = 1.876030e-01f;
		AutoMATiC_DMC_Mp[8][4] = 5.595337e-01f;
		AutoMATiC_DMC_Mp[8][5] = 3.440136e-02f;
		AutoMATiC_DMC_Mp[8][6] = 1.506941e-01f;
		AutoMATiC_DMC_Mp[8][7] = 5.465313e-03f;
		AutoMATiC_DMC_Mp[8][8] = 3.355960e-02f;
		AutoMATiC_DMC_Mp[9][1] = 4.712837e+00f;
		AutoMATiC_DMC_Mp[9][2] = 9.816844e-01f;
		AutoMATiC_DMC_Mp[9][3] = 2.160545e+00f;
		AutoMATiC_DMC_Mp[9][4] = 3.495638e-01f;
		AutoMATiC_DMC_Mp[9][5] = 9.110921e-01f;
		AutoMATiC_DMC_Mp[9][6] = 1.170196e-01f;
		AutoMATiC_DMC_Mp[9][7] = 2.994327e-01f;
		AutoMATiC_DMC_Mp[9][8] = 3.147143e-02f;
		AutoMATiC_DMC_Mp[10][1] = 9.987274e-01f;
		AutoMATiC_DMC_Mp[10][2] = 1.986524e+00f;
		AutoMATiC_DMC_Mp[10][3] = 1.876030e-01f;
		AutoMATiC_DMC_Mp[10][4] = 5.595337e-01f;
		AutoMATiC_DMC_Mp[10][5] = 3.440136e-02f;
		AutoMATiC_DMC_Mp[10][6] = 1.506941e-01f;
		AutoMATiC_DMC_Mp[10][7] = 5.465313e-03f;
		AutoMATiC_DMC_Mp[10][8] = 3.355960e-02f;
		AutoMATiC_DMC_ftmp3 = darray(1,10,1,1);
		AutoMATiC_DMC_ftmp4 = darray(1,10,1,1);
		AutoMATiC_DMC_fconst = darray(1,10,1,10);
		AutoMATiC_DMC_fconst[1][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[1][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[1][3] = -5.104583e+00f;
		AutoMATiC_DMC_fconst[1][4] = -1.622249e+00f;
		AutoMATiC_DMC_fconst[1][5] = -7.603490e+00f;
		AutoMATiC_DMC_fconst[1][6] = -1.928652e+00f;
		AutoMATiC_DMC_fconst[1][7] = -8.826808e+00f;
		AutoMATiC_DMC_fconst[1][8] = -1.986524e+00f;
		AutoMATiC_DMC_fconst[1][9] = -9.425674e+00f;
		AutoMATiC_DMC_fconst[1][10] = -1.997455e+00f;
		AutoMATiC_DMC_fconst[2][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[2][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[2][3] = -1.264241e+00f;
		AutoMATiC_DMC_fconst[2][4] = -2.853981e+00f;
		AutoMATiC_DMC_fconst[2][5] = -1.729329e+00f;
		AutoMATiC_DMC_fconst[2][6] = -3.671660e+00f;
		AutoMATiC_DMC_fconst[2][7] = -1.900426e+00f;
		AutoMATiC_DMC_fconst[2][8] = -3.905929e+00f;
		AutoMATiC_DMC_fconst[2][9] = -1.963369e+00f;
		AutoMATiC_DMC_fconst[2][10] = -3.973048e+00f;
		AutoMATiC_DMC_fconst[3][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[3][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[3][3] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[3][4] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[3][5] = -5.104583e+00f;
		AutoMATiC_DMC_fconst[3][6] = -1.622249e+00f;
		AutoMATiC_DMC_fconst[3][7] = -7.603490e+00f;
		AutoMATiC_DMC_fconst[3][8] = -1.928652e+00f;
		AutoMATiC_DMC_fconst[3][9] = -8.826808e+00f;
		AutoMATiC_DMC_fconst[3][10] = -1.986524e+00f;
		AutoMATiC_DMC_fconst[4][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[4][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[4][3] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[4][4] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[4][5] = -1.264241e+00f;
		AutoMATiC_DMC_fconst[4][6] = -2.853981e+00f;
		AutoMATiC_DMC_fconst[4][7] = -1.729329e+00f;
		AutoMATiC_DMC_fconst[4][8] = -3.671660e+00f;
		AutoMATiC_DMC_fconst[4][9] = -1.900426e+00f;
		AutoMATiC_DMC_fconst[4][10] = -3.905929e+00f;
		AutoMATiC_DMC_fconst[5][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[5][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[5][3] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[5][4] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[5][5] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[5][6] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[5][7] = -5.104583e+00f;
		AutoMATiC_DMC_fconst[5][8] = -1.622249e+00f;
		AutoMATiC_DMC_fconst[5][9] = -7.603490e+00f;
		AutoMATiC_DMC_fconst[5][10] = -1.928652e+00f;
		AutoMATiC_DMC_fconst[6][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[6][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[6][3] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[6][4] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[6][5] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[6][6] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[6][7] = -1.264241e+00f;
		AutoMATiC_DMC_fconst[6][8] = -2.853981e+00f;
		AutoMATiC_DMC_fconst[6][9] = -1.729329e+00f;
		AutoMATiC_DMC_fconst[6][10] = -3.671660e+00f;
		AutoMATiC_DMC_fconst[7][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[7][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[7][3] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[7][4] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[7][5] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[7][6] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[7][7] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[7][8] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[7][9] = -5.104583e+00f;
		AutoMATiC_DMC_fconst[7][10] = -1.622249e+00f;
		AutoMATiC_DMC_fconst[8][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[8][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[8][3] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[8][4] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[8][5] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[8][6] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[8][7] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[8][8] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[8][9] = -1.264241e+00f;
		AutoMATiC_DMC_fconst[8][10] = -2.853981e+00f;
		AutoMATiC_DMC_fconst[9][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[9][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[9][3] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[9][4] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[9][5] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[9][6] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[9][7] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[9][8] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[9][9] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[9][10] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][1] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][2] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][3] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][4] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][5] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][6] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][7] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][8] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][9] = 0.000000e+00f;
		AutoMATiC_DMC_fconst[10][10] = 0.000000e+00f;
		AutoMATiC_DMC_btmp1 = darray(1,40,1,1);
		AutoMATiC_DMC_bvar = darray(1,40,1,2);
		AutoMATiC_DMC_bvar[1][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[1][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[2][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[2][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[3][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[3][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[4][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[4][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[5][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[5][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[6][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[6][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[7][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[7][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[8][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[8][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[9][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[9][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[10][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[10][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[11][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[11][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[12][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[12][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[13][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[13][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[14][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[14][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[15][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[15][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[16][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[16][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[17][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[17][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[18][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[18][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[19][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[19][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[20][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[20][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[21][1] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[21][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[22][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[22][2] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[23][1] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[23][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[24][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[24][2] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[25][1] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[25][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[26][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[26][2] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[27][1] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[27][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[28][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[28][2] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[29][1] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[29][2] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[30][1] = 0.000000e+00f;
		AutoMATiC_DMC_bvar[30][2] = 1.000000e+00f;
		AutoMATiC_DMC_bvar[31][1] = -1.000000e+00f;
		AutoMATiC_DMC_bvar[31][2] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[32][1] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[32][2] = -1.000000e+00f;
		AutoMATiC_DMC_bvar[33][1] = -1.000000e+00f;
		AutoMATiC_DMC_bvar[33][2] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[34][1] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[34][2] = -1.000000e+00f;
		AutoMATiC_DMC_bvar[35][1] = -1.000000e+00f;
		AutoMATiC_DMC_bvar[35][2] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[36][1] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[36][2] = -1.000000e+00f;
		AutoMATiC_DMC_bvar[37][1] = -1.000000e+00f;
		AutoMATiC_DMC_bvar[37][2] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[38][1] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[38][2] = -1.000000e+00f;
		AutoMATiC_DMC_bvar[39][1] = -1.000000e+00f;
		AutoMATiC_DMC_bvar[39][2] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[40][1] = -0.000000e+00f;
		AutoMATiC_DMC_bvar[40][2] = -1.000000e+00f;
		AutoMATiC_DMC_btmp2 = darray(1,40,1,1);
		AutoMATiC_DMC_b = darray(1,40,1,1);
		AutoMATiC_DMC_b[1][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[2][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[3][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[4][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[5][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[6][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[7][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[8][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[9][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[10][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[11][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[12][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[13][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[14][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[15][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[16][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[17][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[18][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[19][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[20][1] = 1.000000e-01f;
		AutoMATiC_DMC_b[21][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[22][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[23][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[24][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[25][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[26][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[27][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[28][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[29][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[30][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[31][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[32][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[33][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[34][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[35][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[36][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[37][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[38][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[39][1] = 1.000000e+00f;
		AutoMATiC_DMC_b[40][1] = 1.000000e+00f;
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
	for(j=1;j<=8;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_dUp[j][k] = 0;
	for(j=1;j<=10;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_Yzad[j][k] = 0;
	for(j=1;j<=10;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_Y[j][k] = 0;
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_tmpu[j][k] = 0;
	AutoMATiC_DMC_itmp=1;
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=4;++AutoMATiC_DMC_i){
		for(AutoMATiC_DMC_j=1;AutoMATiC_DMC_j<=2;++AutoMATiC_DMC_j){
			AutoMATiC_DMC_dUp[AutoMATiC_DMC_itmp][1]=ad->du[ad->k-AutoMATiC_DMC_i][AutoMATiC_DMC_j-1];
			AutoMATiC_DMC_itmp=AutoMATiC_DMC_itmp+1;
		}
	}
	AutoMATiC_DMC_itmp=1;
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=5;++AutoMATiC_DMC_i){
		for(AutoMATiC_DMC_j=1;AutoMATiC_DMC_j<=2;++AutoMATiC_DMC_j){
			AutoMATiC_DMC_Yzad[AutoMATiC_DMC_itmp][1]=ad->z[AutoMATiC_DMC_j-1];
			AutoMATiC_DMC_itmp=AutoMATiC_DMC_itmp+1;
		}
	}
	AutoMATiC_DMC_itmp=1;
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=5;++AutoMATiC_DMC_i){
		for(AutoMATiC_DMC_j=1;AutoMATiC_DMC_j<=2;++AutoMATiC_DMC_j){
			AutoMATiC_DMC_Y[AutoMATiC_DMC_itmp][1]=ad->y[ad->k][AutoMATiC_DMC_j-1];
			AutoMATiC_DMC_itmp=AutoMATiC_DMC_itmp+1;
		}
	}
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_e[j][k] = 0;
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=2;++AutoMATiC_DMC_i){
		AutoMATiC_DMC_e[AutoMATiC_DMC_i][1]=ad->z[AutoMATiC_DMC_i-1]-ad->y[ad->k][AutoMATiC_DMC_i-1];
	}
	productab(AutoMATiC_DMC_Ke,AutoMATiC_DMC_e,AutoMATiC_DMC_dutmp1,2,2,2,1);
	productab(AutoMATiC_DMC_Ku,AutoMATiC_DMC_dUp,AutoMATiC_DMC_dutmp2,2,8,8,1);
	sumaa(AutoMATiC_DMC_dutmp1,AutoMATiC_DMC_dutmp2,AutoMATiC_DMC_du,2,1,-1);
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_du[j][k] = 0;
	for(j=1;j<=2;++j) for(k=1;k<=1;++k) AutoMATiC_DMC_uk[j][k] = 0;
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=2;++AutoMATiC_DMC_i){
		AutoMATiC_DMC_uk[AutoMATiC_DMC_i][1]=ad->u[ad->k-1][AutoMATiC_DMC_i-1];
	}
	sumaa(AutoMATiC_DMC_Yzad,AutoMATiC_DMC_Y,AutoMATiC_DMC_ftmp1,10,1,-1);
	productab(AutoMATiC_DMC_Mp,AutoMATiC_DMC_dUp,AutoMATiC_DMC_ftmp2,10,8,8,1);
	sumaa(AutoMATiC_DMC_ftmp1,AutoMATiC_DMC_ftmp2,AutoMATiC_DMC_ftmp3,10,1,-1);
	productab(AutoMATiC_DMC_fconst,AutoMATiC_DMC_ftmp3,AutoMATiC_DMC_ftmp4,10,10,10,1);
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=10;++AutoMATiC_DMC_i){
		AutoMATiC_DMC_ftmp[AutoMATiC_DMC_i]=AutoMATiC_DMC_ftmp4[AutoMATiC_DMC_i][1];
	}
	productab(AutoMATiC_DMC_bvar,AutoMATiC_DMC_uk,AutoMATiC_DMC_btmp1,40,2,2,1);
	sumaa(AutoMATiC_DMC_b,AutoMATiC_DMC_btmp1,AutoMATiC_DMC_btmp2,40,1,1);
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=40;++AutoMATiC_DMC_i){
		AutoMATiC_DMC_btmp[AutoMATiC_DMC_i]=AutoMATiC_DMC_btmp2[AutoMATiC_DMC_i][1];
	}
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=2;++AutoMATiC_DMC_i){
		AutoMATiC_DMC_qpx[AutoMATiC_DMC_i]=0;
	}
	productv(AutoMATiC_DMC_btmp,-1,AutoMATiC_DMC_btmp,40);qp();
	for(AutoMATiC_DMC_i=1;AutoMATiC_DMC_i<=2;++AutoMATiC_DMC_i){
		AutoMATiC_DMC_du[AutoMATiC_DMC_i][1]=AutoMATiC_DMC_qpx[AutoMATiC_DMC_i];
	}
	for(AutoMATiC_DMC_n=1;AutoMATiC_DMC_n<=2;++AutoMATiC_DMC_n){
		if(AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]>dumax[1][AutoMATiC_DMC_n]){
			AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]=dumax[1][AutoMATiC_DMC_n];
		}
		if(AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]<dumin[1][AutoMATiC_DMC_n]){
			AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]=dumin[1][AutoMATiC_DMC_n];
		}
		AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]=ad->u[ad->k-1][AutoMATiC_DMC_n-1]+AutoMATiC_DMC_du[AutoMATiC_DMC_n][1];
		if(AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]>umax[1][AutoMATiC_DMC_n]){
			AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]=umax[1][AutoMATiC_DMC_n];
		}
		if(AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]<umin[1][AutoMATiC_DMC_n]){
			AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]=umin[1][AutoMATiC_DMC_n];
		}
		AutoMATiC_DMC_du[AutoMATiC_DMC_n][1]=AutoMATiC_DMC_tmpu[AutoMATiC_DMC_n][1]-ad->u[ad->k-1][AutoMATiC_DMC_n-1];
	}
	for(j=1;j<=1;++j) for(k=1;k<=2;++k) control_value[j][k] = 0;
	for(AutoMATiC_DMC_n=1;AutoMATiC_DMC_n<=2;++AutoMATiC_DMC_n){
		control_value[1][AutoMATiC_DMC_n]=AutoMATiC_DMC_du[AutoMATiC_DMC_n][1];
	}

	set_current_control_increment(c,&(control_value[1][1])); // du is indexed starting with 1, therefore to maintain compatibility it is required to refer to first element of an actual array
}

void controller_setup(){
profiler_start(2);
    init_archive_data(&ad, 200, 200, 2, 2, 0, 0, 0.01);
    init_current_control(&cc,&ad);
    controllerDMCA(NULL,NULL);
    controllerDMCN(NULL,NULL);
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
    sprintf(str,      "%d,",algorithm_used);               write_string(str);
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
	if(++i > 300){
		i = 0;
	}
   	if((i%10==0) && ((random() % 100) < 10))
		algorithm_used = algorithm_used==0?1:0;

	
profiler_start(50);
	if(algorithm_used == 0) controllerDMCA(&ad,&cc);
	else                    controllerDMCN(&ad,&cc);
profiler_end(50);
	
	push_current_controls_to_archive_data(&cc,&ad);	
profiler_end(10);
}

void timeout(){
    while(1);
}
