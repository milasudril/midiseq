#ifdef __WAND__
target[name[track_reader.h] type[include]]
dependency [track_reader.o]
#endif

#ifndef MIDISEQ_TRACKREADER_H
#define MIDISEQ_TRACKREADER_H

#include <cstdint>

namespace MIDISeq
	{
	class ChunkReader;
	class Event;
	class TrackReader
		{
		public:
			explicit TrackReader(ChunkReader& reader):m_reader(reader)
				,time_running(0),status_prev(0)
				{}

			bool eventNextGet(Event& event);

		private:
			ChunkReader& m_reader;
			uint64_t time_running;
			uint8_t status_prev;
		};

	}

#endif
