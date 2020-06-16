#include "manager.h"

void manager_start(){
	OS_ERR err;
	
	OSTaskCreate((OS_TCB     *)&manager_tcb,                
    	(CPU_CHAR   *)"Magnetic task start",
         (OS_TASK_PTR )manager_task, 
         (void       *)0,
         (OS_PRIO     )MANAGER_TASK_PRIO,
         (CPU_STK    *)&manager_stack[0],
         (CPU_STK_SIZE)MANAGER_TASK_STK_SIZE / 10,
         (CPU_STK_SIZE)MANAGER_TASK_STK_SIZE,
         (OS_MSG_QTY  )0,
         (OS_TICK     )MANAGER_CLOCK_TICK,
         (void       *)0,
         (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
         (OS_ERR     *)&err);
}

void manager_stop(){
	OS_ERR err;
	
	running = 0;
	motor_release_mutex();
	OSTaskDel(&manager_tcb, &err);
}

void manager_task(){
	OS_ERR err;
	
	printf("MANAGER TASK ENTER\n");
	running = 1;
	
	while(DEF_TRUE){
		if(!get_magnetic_bit() && get_ir_bit()) {
			printf("ALL SAFE STOP MANAGER TASK\n");
			running = 0;
			manager_stop();
		}
		
		printf("MANAGER TASK STOP MOTOR\n");
		motor_get_mutex();
		motor_set_bit(MOTOR_STOP);
		motor_release_mutex();
	}
	
	OSTaskDel(&manager_tcb, &err);
}