#ifdef __WAND__
target[name[track_writer.h] type[include]]
dependency [track_writer.o]
#endif

#ifndef MIDISEQ_TRACKWRITER_H
#define MIDISEQ_TRACKWRITER_H

#include <cstdint>

namespace MIDISeq
	{
	class ChunkWriter;
	class Event;
	class TrackWriter
		{
		public:
			explicit TrackWriter(ChunkWriter& writer):m_writer(writer)
				,time_prev(0)
				{}

			bool eventWrite(const Event& event);

		private:
			ChunkWriter& m_writer;
			uint64_t time_prev;
			uint8_t status_prev;
		};

	}

#endif
