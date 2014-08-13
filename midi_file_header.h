#ifdef __WAND__
target[name[midi_file_header.h] type[include]]
#endif

#ifndef MUSTUDIO_MIDIFILEHEADER_H
#define MUSTUDIO_MIDIFILEHEADER_H

#include <cstdint>

namespace MuStudio
	{
	namespace MIDI
		{
		struct FileHeader
			{
			uint16_t format;
			uint16_t n_tracks;
			uint16_t time_division;
			};
		}
	}

#endif

