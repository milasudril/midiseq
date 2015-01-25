#ifdef __WAND__
target[name[sequence.o] type[object]]
#endif

#include "sequence.h"
#include "event.h"
#include "file_header.h"
#include "track_reader.h"
#include "chunk_reader.h"

#include "chunk_writer.h"
#include "track_writer.h"

#include <herbs/stringbase/stringbase.h>

MIDISeq::Sequence::Sequence(){}

void MIDISeq::Sequence::load(Herbs::StreamIn& source)
	{
	MIDISeq::FileHeader header;
	MIDISeq::ChunkReader chunk_reader(source,header);

	time_division=header.time_division;
	tracks.lengthValidSet(header.n_tracks);
	tracks.clear();
	length=0;
	Herbs::Stringbase<char> chunk_name;
	while(chunk_reader.headerRead(chunk_name))
		{
		MIDISeq::TrackReader track_reader(chunk_reader);
		tracks.append(Herbs::Array<Event>());
		auto ptr_begin=tracks.end() - 1;
		MIDISeq::Event e;
		while(track_reader.eventNextGet(e))
			{
			ptr_begin->append(e);
			}
		length=std::max(length,e.time);
		chunk_reader.skip();
		}
	}
	
void MIDISeq::Sequence::store(Herbs::FileOut& dest)
	{
	MIDISeq::FileHeader header;
	header.n_tracks=tracks.length();
	header.time_division=time_division;
	header.format=1;
	
	MIDISeq::ChunkWriter writer(dest,header);
	auto ptr=tracks.begin();
	while(ptr!=tracks.end())
		{
		writer.headerWrite(Herbs::Stringbase<char>("MTrk"));
		MIDISeq::TrackWriter track_writer(writer);
		auto event_current=ptr->begin();
		while(event_current!=ptr->end())
			{
			track_writer.eventWrite(*event_current);
			++event_current;
			}
		writer.chunkEnd();
		++ptr;
		}
	}
	

namespace
	{
	bool time_compare(const MIDISeq::Event& a
		,const MIDISeq::Event& b)
		{
		return a.time<b.time;
		}
	}

MIDISeq::Sequence& MIDISeq::Sequence::tracksMerge()
	{
	Herbs::Array<Herbs::Array<Event> > tracks_new(1);
	tracks_new.lengthValidSet(1);
	auto track_new=tracks_new.begin();

	auto track=tracksBegin();
	while(track!=tracksEnd())
		{
		auto event=track->begin();
		while(event!=track->end())
			{
			track_new->append(*event);
			++event;
			}
		++track;
		}

	std::sort(track_new->begin(),track_new->end(),time_compare);

	tracks=tracks_new;

	return *this;
	}
