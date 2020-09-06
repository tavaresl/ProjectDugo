#pragma once


class Countdown
{
private:
	unsigned int timelimit;
	unsigned int tickrate;
	unsigned int counter;
	unsigned int tick;
	unsigned int beepInterval;

public:
	Countdown(unsigned int _timelimit, unsigned int _tickrate) {
		counter = _timelimit;
		timelimit = _timelimit;
		tickrate = _tickrate;
		tick = 0;
	}

	void Decrement() {
		if (tick < tickrate)
		{
			tick++;
			return;
		}

		if (counter > 0)
			counter--;

		tick = 0;
	}

	void Reset()
	{
		counter = timelimit;
	}

	unsigned int Counter() { return counter; }
};
