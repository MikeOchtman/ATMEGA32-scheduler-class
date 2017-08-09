/*
* scheduler.cpp
*
*  Created on: 12 Jul 2017
*      Author: Mike
*/

/*
* scheduler.cpp
*
* Created: 09/07/2017 17:39:27
* Author : Mike
*/

#include <avr/io.h>
#include "scheduler.h"



/*!
* @brief constructor for creating an instance of Scheduler
*
*@details Instantiates a Scheduler, presets the counter, and sets the state to HOLD<br>
*
*
* @param[in] pre the number of counts to count down before TIMEOUT
* @param[in] p_on_timeout name of function to be executed when timer times out
* @param[in] p_on_restart (*optional*) name of function to be executed when timer timer is restarted.
* defaults to nullptr if not defined
*
*
*
*/
Scheduler::Scheduler(unsigned long  pre, void (*p_on_timeout)(void), void (*p_on_restart)(void) = nullptr)
{
	preset = pre;
	remaining = pre;
	on_timeout = p_on_timeout;
	on_restart = p_on_restart;
	state = Scheduler_state::HOLD;

}



/*!
@brief constructor for building an array of schedulers

This one sets preset and on_timeout to default zero so that each has to be filled in later
with a call to set_preset and set_on_timeout


@return
*/
Scheduler::Scheduler()
{
	preset = 0;
	remaining = 0;
	on_timeout = nullptr;
	on_restart = nullptr;
	state = Scheduler_state::HOLD;
}

/*!
* @brief sets the preset value
*
*@details changes the preset value without affecting the state or the remaining count
*
* @param[in] pre the new preset value
*
*/
void Scheduler::set_preset(unsigned long  pre)
{
	preset = pre;

}

/*!
* @brief returns the current preset value
*
*
* @return unsigned long  the current preset value
*/
unsigned long  Scheduler::get_preset(void)
{
	return preset;
}

/*!
@brief allows changing the called function when timeout occurs

This has to be called after the default constructor has been called,
typically as part of an array of schedulers.


@param[in] f the name of a function that takes no parameters and returns none,
and handles the event when a timer times out.

*/
void Scheduler::set_on_timeout(void (*f)(void))
{
	on_timeout = f;
}

/*!
@brief allows changing the called function when restart occurs

This has to be called after the default constructor has been called,
typically as part of an array of schedulers.


@param[in] f the name of a function that takes no parameters and returns none,
and handles the event when a timer restarts.

*/
void Scheduler::set_on_restart(void (*f)(void))
{
	on_restart = f;
}

/*!
* @brief returns the current state of the Scheduler
*
*@details
*
* @return Scheduler_state state
*/
Scheduler::Scheduler_state Scheduler::get_state(void)
{
	return state;

}

/*!
* @brief returns the remaining counts required to TIMEOUT
*
*@details
*@
* @return unsigned long  remaining counts
*/
unsigned long  Scheduler::get_remaining(void)
{
	return	remaining;
}

/*!
* @brief Restarts the timer.
*
*@details this will set the state to RUNNING without affecting the preset or remaining counts.<br>
*if you need to restart the timer from full preset, use
*@code
*	[scheduler].reset();
*	[scheduler].restart();
*@endcode
*
*/
void Scheduler::restart(void)
{
	state = Scheduler_state::RUNNING;
	if (on_restart != nullptr) on_restart();
}

/*!
* @brief resets the counter, and puts it in HOLD
*
*@details the counter remaining counts are returned to the preset value,
*and the counter state returned to HOLD.<br>
*
*/
void Scheduler::reset(void)
{
	remaining = preset;
	state = Scheduler_state::HOLD;

}

/*!
* @brief puts the counter in HOLD.
*
*@details halts the countdown. To continue the countdown, use <code>[scheduler].restart()</code>.
*
*/
void Scheduler::hold(void) {
	state = Scheduler_state::HOLD;
}

/*!
* @brief should be called at every pass of the main loop. Tests whether the counter has reached TIMEOUT
*
* @details If it has timed out the remaining time is put back to preset.<br>
* If action is RESTART (or omitted) the counter is set back to RUNNING.
* If action is not RESTART, the counter is put on HOLD
*
* @param[in] action defaults to SchedulerState::RESTART if omitted. <br>Any other state will put the counter on hold.
*
*
* @return void
*/
void Scheduler::process(Scheduler_state action /*= SchedulerState::RESTART*/) {

	if (Scheduler_state::TIMEOUT == state) {
		remaining = preset;
		if (Scheduler_state::RESTART == action) {
			state = Scheduler_state::RUNNING;
		}
		else {
			state = Scheduler_state::HOLD;
		}

		if (on_timeout != nullptr) on_timeout();

	}

}

/*!
* @brief counts down if the counter is RUNNING
*
*@details checks counter state and decrements remaining counts if greater than 0<br>
* if counter remaining is 0, sets state to TIMEOUT
*
* @return unsigned long  the current counter remaining after the decrement
*/
unsigned long  Scheduler::count_down(void)
{
	if (Scheduler_state::RUNNING == state) {
		if (--remaining == 0) {
			state = Scheduler_state::TIMEOUT;

		}
	}
	return remaining;

}




