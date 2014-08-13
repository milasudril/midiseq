#ifdef __WAND__
target[name[event.h] type[include]]
#endif

#ifndef MIDISEQ_EVENT_H
#define MIDISEQ_EVENT_H

#include <cstddef>
#include <cstdint>

namespace MIDISeq
	{
	struct alignas(32) Event
		{
		uint64_t time; /**<This time is absolute when event is read from SMF*/
		uint64_t type;
		union
			{
			uint8_t  bytes[16];
			uint16_t words[8];
			uint32_t dwords[4];
			uint64_t qwords[2];
			} data;
		};
	}

#endif

