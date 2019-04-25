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
        LIST_INSERT_HEAD(&env_sched_list[pos ^ 1], e, env_sched_link);
        //LIST_INSERT_TAIL(&env_sched_list[pos ^ 1], e, env_sched_link, tmp);
    }

    // check if current list is empty, switch list on empty
    if (LIST_EMPTY(&env_sched_list[pos])) {
        pos ^= 1;
    }

    if (!LIST_EMPTY(&env_sched_list[pos])) {
        e = LIST_FIRST(&env_sched_list[pos]);
        LIST_REMOVE(e, env_sched_link);
        times = e->env_pri;
        env_run(e);
        return;
    }

    printf("There is no runnable env in env_sched_list!\n");
    while (1);
}

/*lab4-1(modified)*/
/*
void sched_yield(void)
{
    static pos = 0;
    static times = 0;
    static struct Env *e;

    times++;
    while(1) {
        if(curenv == NULL || times == curenv->env_pri || curenv->env_status != ENV_RUNNABLE) {
            times = 0;
            if(curenv != NULL && curenv->env_status == ENV_RUNNABLE) {
                LIST_INSERT_HEAD(&env_sched_list[!pos], curenv, env_sched_link);
            }
            if(LIST_EMPTY(&env_sched_list[pos])) {
                pos = !pos;
            }
            if(!LIST_EMPTY(&env_sched_list[pos])) {
                e = LIST_FIRST(&env_sched_list[pos]);
                LIST_REMOVE(e, env_sched_link);
            } else {
                e = NULL;
            }
        } else {
            e = curenv;
        }

        if(e != NULL) {
            env_run(e);
            return;
        }
        else {
                curenv = NULL;//optional
        }
    }
}
*/

/*lab4-1(original)*/
/*
void sched_yield(void)
{
    static pos = 0;
    static times = 0;
    static struct Env *e;

    times++;

    if(curenv==NULL || times==curenv->env_pri || curenv->env_status != ENV_RUNNABLE){
        times = 0;
        if(curenv != NULL && curenv->env_status == ENV_RUNNABLE) {
            LIST_INSERT_HEAD(&env_sched_list[!pos],curenv,env_sched_link);
        }
        if(LIST_EMPTY(&env_sched_list[pos])){
            pos = !pos;
        }
        if(!LIST_EMPTY(&env_sched_list[pos])){
            e = LIST_FIRST(&env_sched_list[pos]);
            LIST_REMOVE(e, env_sched_link);
        }else{
            e = NULL;
        }
    } else{
        e = curenv;
    }

    if(e != NULL)
    {
        env_run(e);
    }
}
*/

/*lab4-1(copy)*/
/*
void sched_yield(void)
{
    static pos = 0;
    static times = 0;
    static struct Env *e;
    if(times == 0 || e->env_status != ENV_RUNNABLE) {
        times = 0;
        if(e != NULL) {
            LIST_INSERT_HEAD(&(env_sched_list[!pos]), e, env_sched_link);
        }
        if(LIST_EMPTY(&(env_sched_list[pos])))
            pos = !pos;
        if(!LIST_EMPTY(&(env_sched_list[pos]))) {
            e = LIST_FIRST(&(env_sched_list[pos]));
            LIST_REMOVE(e, env_sched_link);
            times = e->env_pri;
        } else {
            e = NULL;
        }
    }

    if(times != 0) {
        times--;
        env_run(e);
    }
}
*/

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
}
*/

/*lab3-2-exam:dynamic priority(final)*/
/*
void sched_yield(void)
{
        static pos = 0;
        static times = 0;
        static struct Env *e;

    times++;

    if(curenv==NULL || times==curenv->env_pri){
        times = 0;
        if(curenv != NULL){
            curenv->env_pri = curenv->env_pri - curenv->env_runs;
            if(curenv->env_pri==0){
                env_destroy(curenv);
                return;
            }else{
                LIST_INSERT_HEAD(&env_sched_list[!pos],curenv,env_sched_link);
            }
        }
        if(LIST_EMPTY(&env_sched_list[pos])){
            pos = !pos;
        }
        if(!LIST_EMPTY(&env_sched_list[pos])){
            e = LIST_FIRST(&env_sched_list[pos]);
            LIST_REMOVE(e, env_sched_link);
        } else{
            e = NULL;
        }
    } else{
        e = curenv;
    }

    if(e!=NULL){
        env_run(e);
    }
    else {
        while(1);
    }
}
*/

/*lab3-2-exam:dynamic priority(original)*/
/*
void sched_yield(void)
{
        static pos = 0;
        static times = 0;
        static struct Env *e;

    times++;

    if(curenv==NULL || times==curenv->env_pri){
        times = 0;
        if(curenv != NULL){
            curenv->env_pri = curenv->env_pri - curenv->env_runs;
            if(curenv->env_pri==0){
                env_destroy(curenv);
            }else{
                LIST_INSERT_HEAD(&env_sched_list[!pos],curenv,env_sched_link);
            }
        }
        if(LIST_EMPTY(&env_sched_list[pos])){
            pos = !pos;
        }
        if(!LIST_EMPTY(&env_sched_list[pos])){
            e = LIST_FIRST(&env_sched_list[pos]);
            LIST_REMOVE(e, env_sched_link);
        }
    } else{
        e = curenv;
    }

    if(e!=NULL){
        env_run(e);
    }
}
*/

/*lab3-1(modified1)*/
/*
void sched_yield(void)
{
        static pos = 0;
        static times = 0;
        static struct Env *e;
    if(times == 0){
        if(e != NULL){
        LIST_INSERT_HEAD(&env_sched_list[pos==0],e,env_sched_link);
        }
        if(LIST_EMPTY(&env_sched_list[pos])){
            pos = (pos==0) ? 1 : 0;
        }
        if(!LIST_EMPTY(&env_sched_list[pos])){
            e = LIST_FIRST(&env_sched_list[pos]);
            LIST_REMOVE(e, env_sched_link);
            times = e -> env_pri;
        } else{
            e = NULL;
        }
    }

    if(times != 0){
        times--;
        env_run(e);
    }
}
*/

/*lab3-1(modified1)*/
/*
void sched_yield(void)
{
        static pos = 0;
        static times = 0;
        static struct Env *e;

    times++;

    if(e==NULL || times==e->env_pri){
        times = 0;
        if(e != NULL){
            LIST_INSERT_HEAD(&env_sched_list[!pos],e,env_sched_link);
        }
        if(LIST_EMPTY(&env_sched_list[pos])){
            pos = !pos;
        }
        if(!LIST_EMPTY(&env_sched_list[pos])){
            e = LIST_FIRST(&env_sched_list[pos]);
            LIST_REMOVE(e, env_sched_link);
        }
    }

    if(e != NULL){
        env_run(e);
    }
}
*/

/*lab3-exam(last year)*/
/*
void sched_yield(void)
{
    static long position = -1;
    static int a_count = 0;
    while(1) {
        position++;
        if(position >= 2)
            position = 0;
        if(position == 0 && envs[position].env_status == ENV_RUNNABLE) {
            if(a_count >= 10)
                continue;
            else {
                a_count++;
                env_run(&envs[position]);
            }
        } else if(position == 1 && envs[position].env_status == ENV_RUNNABLE)
            env_run(&envs[position]);
    }
}
*/
