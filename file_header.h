#ifdef __WAND__
target[name[file_header.h] type[include]]
#endif

#ifndef MIDISEQ_FILEHEADER_H
#define MIDISEQ_FILEHEADER_H

#include <cstdint>

namespace MIDISeq
	{
	struct FileHeader
		{
		uint16_t format;
		uint16_t n_tracks;
		uint16_t time_division;
		};
	}

#endif

