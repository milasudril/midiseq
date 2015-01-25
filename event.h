#ifdef __WAND__
target[name[event.h] type[include]]
#endif

#ifndef MIDISEQ_EVENT_H
#define MIDISEQ_EVENT_H

#include <cstddef>
#include <cstdint>

namespace MIDISeq
	{
	struct Event
		{
		uint64_t time; /**<This time is absolute*/
		uint8_t size;
		uint8_t data[23];
		};
	}

#endif

