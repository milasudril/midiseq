#ifdef __WAND__
target[name[midi_sequence.o] type[object]]
#endif

#include "midi_sequence.h"
#include "midi_event.h"
#include "midi_file_header.h"
#include "midi_track_reader.h"
#include "midi_chunk_reader.h"
#include <herbs/stringbase/stringbase.h>

MuStudio::MIDI::Sequence::Sequence(){}

void MuStudio::MIDI::Sequence::load(Herbs::StreamIn& source)
	{
	MuStudio::MIDI::FileHeader header;
	MuStudio::MIDI::ChunkReader chunk_reader(source,header);
	
	time_division=header.time_division;
	tracks.lengthValidSet(header.n_tracks);
	tracks.clear();
	length=0;
	Herbs::Stringbase<char> chunk_name;
	while(chunk_reader.headerRead(chunk_name))
		{
		MuStudio::MIDI::TrackReader track_reader(chunk_reader);
		tracks.append(Herbs::Array<Event>());
		auto ptr_begin=tracks.end() - 1;
		MuStudio::MIDI::Event e;
		while(track_reader.eventNextGet(e))
			{
			ptr_begin->append(e);
			}
		length=std::max(length,e.time);
		chunk_reader.skip();	
		}
	}
	
namespace
	{
	bool time_compare(const MuStudio::MIDI::Event& a
		,const MuStudio::MIDI::Event& b)
		{
		return a.time<b.time;
		}
	}

MuStudio::MIDI::Sequence& MuStudio::MIDI::Sequence::tracksMerge()
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
