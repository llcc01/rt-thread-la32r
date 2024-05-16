#include <rtthread.h>
#include <rthw.h>

void rt_application_init(void);
void rt_hw_board_init(void);

#ifdef RT_USING_FINSH
int finsh_system_init(void);
#endif

#ifndef RT_USING_USER_MAIN

int rtthread_startup(void)
{
#ifdef RT_USING_SMP
    rt_hw_spin_lock_init(&_cpus_lock);
#endif
    rt_hw_local_irq_disable();

    /* board level initialization
     * NOTE: please initialize heap inside board initialization.
     */
    rt_hw_board_init();

    /* show RT-Thread version */
    rt_show_version();

    /* timer system initialization */
    rt_system_timer_init();

    /* scheduler system initialization */
    rt_system_scheduler_init();

#ifdef RT_USING_SIGNALS
    /* signal system initialization */
    rt_system_signal_init();
#endif /* RT_USING_SIGNALS */

    /* create init_thread */
    // rt_application_init();

#ifdef RT_USING_FINSH
	/* init the finsh input */
	// rt_hw_finsh_init();

	/* init finsh */
	finsh_system_init();
#endif

    /* timer thread initialization */
    rt_system_timer_thread_init();

    /* idle thread initialization */
    rt_thread_idle_init();

#ifdef RT_USING_SMP
    rt_hw_spin_lock(&_cpus_lock);
#endif /* RT_USING_SMP */

    /* start scheduler */
    rt_system_scheduler_start();

    /* never reach here */
    return 0;
}

#endif /* RT_USING_USER_MAIN */