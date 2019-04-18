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
/*lab4-1(modified)*/

void sched_yield(void)
{
    static int times = 0, pos = 0;
    static struct Env *e = NULL;
    
    printf("\n\n*********** sched_yield %d begin ***********\n", times);
    if (times > 0) {
        --times;
        printf("-- times, running %x times:%d\n", e->env_id, times);
        env_run(e);
        return;
 
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


