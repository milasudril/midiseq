#ifdef __WAND__
target[name[midi_event.h] type[include]]
#endif

#ifndef MUSTUDIO_MIDI_H
#define MUSTUDIO_MIDI_H

#include <cstddef>
#include <cstdint>

namespace MuStudio
	{
	namespace MIDI
		{
		struct alignas(32) Event
			{
			uint64_t time; /*<This time is absolute*/
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
	}

#endif

