#ifdef __WAND__
target[name[chunk_writer.h] type[include]]
dependency[chunk_writer.o]
#endif

#ifndef MIDISEQ_CHUNKWRITER_H
#define MIDISEQ_CHUNKWRITER_H

#include <cstdint>
#include <cstddef>

namespace Herbs
	{
	class FileOut;
	
	template<class T>
	class Stringbase;
	}

namespace MIDISeq
	{
	class FileHeader;

	class ChunkWriter
		{
		public:
			ChunkWriter(Herbs::FileOut& dest,const FileHeader& header);
			~ChunkWriter();

			bool headerWrite(const Herbs::Stringbase<char>& name);
			bool chunkEnd();
			size_t dataWrite(const void* buffer,size_t buffer_size);

			bool bytePut(uint8_t x);
			bool varfieldPut(size_t x);

		private:
			Herbs::FileOut& m_dest;
			uint64_t position;
			uint64_t offset_chunk;
			uint32_t size_chunk;
			
		};
	}

#endif
