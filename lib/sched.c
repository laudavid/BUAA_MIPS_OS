#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *  Search through 'envs' for a runnable environment ,
 *  in circular fashion starting after the previously running env,
 *  and switch to the first such environment found.
 *
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */
//when occur next exception break,carry out this function again. 
void sched_yield(void)
{
    static int times = 0, pos = 0;
    static struct Env *e = NULL;
    
    printf("\n\n*********** sched_yield %d begin ***********\n", times);
    if (times > 0 && e != NULL && e->env_status == ENV_RUNNABLE) {
        --times;
        printf("-- times, running %x times:%d\n", e->env_id, times);
        env_run(e);
        return;
    }   

    //if (e == NULL)
    //    e = LIST_FIRST(&env_sched_list[pos]);
    //LIST_REMOVE(e, env_sched_link); 
    if (e != NULL && e->env_status == ENV_RUNNABLE)
      LIST_INSERT_HEAD(&env_sched_list[pos ^ 1], e, env_sched_link);
    e = NULL;
    
    /* check if current list is empty, switch list on empty */
    if (LIST_EMPTY(&env_sched_list[pos])) {
        pos ^= 1;
        printf("switch to %d\n", pos);
    }
    if(!LIST_EMPTY(&env_sched_list[pos])){
        e = LIST_FIRST(&env_sched_list[pos]);
        LIST_REMOVE(e, env_sched_link);
    }
    if (e == NULL) {
        printf("env_sched_list[%d] is empty!\n", pos);
        while(1);   
    }   

    times = e->env_pri;
    --times;
    //printf("test\n");
    
    //peek_sched_list(env_sched_list, pos);
    env_run(e);
    
}
/*void sched_yield(void){
	static int i = -1;
	int n=0;
	for(;n<NENV-1;n++){
		i++;
		if(i==NENV)
			i=0;
		if(envs[i].env_status == ENV_RUNNABLE)
			env_run(&envs[i]);
	}
}*/


