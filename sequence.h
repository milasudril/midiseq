#ifdef __WAND__
target[name[sequence.h] type[include]]
dependency[sequence.o]
#endif

#ifndef MIDISEQ_SEQUENCE_H
#define MIDISEQ_SEQUENCE_H

#include <herbs/array/array.h>

namespace Herbs
	{
	class StreamIn;
	}

namespace MIDISeq
	{
	class Event;
	class Sequence
		{
		public:
			Sequence();
			explicit Sequence(Herbs::StreamIn& source)
				{load(source);}

			void load(Herbs::StreamIn& source);

			Herbs::Array<Event>* tracksBegin()
				{return tracks.begin();}
			const Herbs::Array<Event>* tracksBegin() const
				{return tracks.begin();}
			Herbs::Array<Event>* tracksEnd()
				{return tracks.end();}
			const Herbs::Array<Event>* tracksEnd() const
				{return tracks.end();}

			size_t timeDivisionGet() const
				{return time_division;}

			size_t lengthGet() const
				{return length;}

			Sequence& tracksMerge();

		private:
			Herbs::Array<Herbs::Array<Event> > tracks;
			uint64_t time_division;
			uint64_t length;
		};
	}

#endif
