#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Check whether current environment has used up its time. If yes, then
 *  check whether the list env_sched_list[pos] is empty. If yes, change the
 *  pointer to point the other list. Next, if current list is not empty,
 *  pop the first one and allocate running time to it.
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */
void sched_yield(void)
{
    static int times = 0, pos = 0;
    static struct Env *e = NULL;
    struct Env *tmp;

    if (--times > 0 && e != NULL && e->env_status == ENV_RUNNABLE) {
        env_run(e);
        return;
    }

    if (e != NULL && e->env_status == ENV_RUNNABLE) {
        LIST_INSERT_TAIL(&env_sched_list[pos ^ 1], e, env_sched_link, tmp);
        //LIST_INSERT_HEAD(&env_sched_list[pos ^ 1], e, env_sched_link);
    }

    /* check if current list is empty, switch list on empty */
    if (LIST_EMPTY(&env_sched_list[pos])) {
        pos ^= 1;
        //printf("switch to %d\n", pos);
    }

    if (!LIST_EMPTY(&env_sched_list[pos])) {
        e = LIST_FIRST(&env_sched_list[pos]);
        LIST_REMOVE(e, env_sched_link);
    } else {
        printf("env_sched_list is empty!\n");
        while (1);
    }

    times = e->env_pri;
    env_run(e);
}


/*lab4-1-Extra:Round Robin*/
/*
void sched_yield(void)
{

    static int i = 0;
    while (1) {
        i++;
        i = i % NENV;
        if (envs[i].env_status == ENV_RUNNABLE) {
            env_run(&envs[i]);
        return;
    }
}
*/



