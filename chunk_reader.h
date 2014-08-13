#ifdef __WAND__
target[name[chunk_reader.h] type[include]]
dependency[chunk_reader.o]
#endif

#ifndef MIDISEQ_CHUNKREADER_H
#define MIDISEQ_CHUNKREADER_H

#include <chunkio/reader.h>
#include <cstdint>
#include <cstddef>

namespace Herbs
	{
	class StreamIn;
	}

namespace MIDISeq
	{
	class FileHeader;

	class ChunkReader:public ChunkIO::Reader
		{
		public:
			explicit ChunkReader(Herbs::StreamIn& source,FileHeader& header);
			~ChunkReader();

			bool headerRead(Herbs::Stringbase<char>& name);
			size_t dataRead(void* buffer,size_t buffer_size);
			bool skip();
			bool skip(size_t n_bytes);
			bool eoc() const
				{return size_chunk==0;}

			uint8_t byteGet();
			size_t varfieldGet();

		private:
			Herbs::StreamIn& m_source;
			size_t size_chunk;
		};
	}

#endif
