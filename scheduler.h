/*
 *
 *
 *
 */


/*!
* @brief class to operate as scheduler for several tasks to run concurrently, but asynchronously
*
* How to use: <br>
* Create one instance of Scheduler for each process to be run, with the name of a function <br>
* to be called on timeout, and optionally a function to be called on restart.<br>
* Both functions nust be of the type 'void function(void) { ... }'
*
* There are two constructors, one taking no parameters and designed to be used when declaring an array of schedulers.
* The other constructor takes the number of count_down cycles and optionally, the names
* of the event-handling functions.
*
* Every instance of Scheduler will be on HOLD after creation. To get the counter RUNNING,
* call [scheduler].restart(). If the preset time has been altered either as part of initialising an array, or due to altering the timout period
during run-time, .reset() must be called before .restart().
*
* [scheduler].count_down() must be called regularly, ideally inside a 1ms timer interrupt routine. <br>
* In the main while(1) loop, call process() every scan cycle. If the count_down has timed out, the
* associated 'on_timeout' function will be called.
*
* if .process() is called without parameters, the scheduler will reset and restart itself to time out another cycle.
* if .process() is called with Scheduler::Scheduler_state::HOLD then the scheduler will halt, waiting for a restart.
* Thus schedulers can operate as free-running repetitive devices or as a one-shot that will carry out an optional
* function when it restarts, and an optional function when it stops.
*
* It is useful to declare an array of schedulers, and then to alias each scheduler with an appropriate name, viz
* @code
Scheduler schedule[6];
Scheduler *lcd_scheduler = &schedule[0];
Scheduler *key_scheduler = &schedule[1];
Scheduler *exposure_scheduler = &schedule[2];
Scheduler *idiotlight_scheduler = &schedule[3];
Scheduler *menu_scheduler = &schedule[4];
Scheduler *buzzer_scheduler = &schedule[5];
* @endcode

This way, each scheduler can easily be handled individually as well as a group, as required.

For example, in a 1ms interrupt handler: (note C++11 notation...
@code
ISR(TIMER1_OVF_vect) {
	TCNT1 = 65535-8000;
	for (Scheduler& s : schedule) {
		s.count_down();
	}
}
@endcode

while individually treated in the main loop thus:
@code
while (1)
{
	key_scheduler->process();
	lcd_scheduler->process();
	exposure_scheduler->process(Scheduler::Scheduler_state::HOLD);
	idiotlight_scheduler->process(Scheduler::Scheduler_state::HOLD);
	buzzer_scheduler->process(Scheduler::Scheduler_state::HOLD);
	menu_scheduler->process();
}
@endcode

*/
class Scheduler {
public:

	/*!
	\brief possible states of a scheduler/timer

	scheduler will behave according to its state.

	-# TIMEOUT counter remaining time is 0, will not count down further.
	-# RUNNING counter remaining time will be decremented every call until TIMEOUT
	-# HOLD counter remaining time will not be decremented, will never time out
	-# RESTART Timer continues the countdown
	-# RESET remaining time is set back to preset maximum duration and put in HOLD
	*/
	enum class Scheduler_state {
							TIMEOUT, /*!< TIMEOUT counter remaining time is 0, will not count down further. */
							RUNNING,  /*!< RUNNING counter remaining time will be decremented every call until TIMEOUT */
							HOLD,  /*!< HOLD counter remaining time will not be decremented, will never time out */
							RESTART,  /*!< RESTART Timer continues the countdown */
							RESET /*!< RESET remaining time is set back to preset maximum duration and put in HOLD */
							};

	Scheduler();
	Scheduler(unsigned long  pre, void (*on_timeout)(void), void (*on_restart)(void));
	void set_preset(unsigned long  pre);
	unsigned long  get_preset(void);
	void set_on_timeout(void (*f)(void));
	void set_on_restart(void (*f)(void));
	Scheduler_state get_state(void);
	unsigned long  get_remaining(void);
	void restart(void);
	void reset(void);
	void hold(void);
	void process(Scheduler_state action = Scheduler_state::RESTART);
	unsigned long  count_down(void);
private:

	unsigned long  preset;
	unsigned long  remaining;
	Scheduler_state state;
	void (*on_timeout)(void);
	void (*on_restart)(void);

};
