


#ifndef _FileObject
#define _FileObject


#include <string>
//#include <cstring>

namespace distanceMeasure
{

	class FileObject
	{
	public:
		FileObject();

		//FileObject(const std::string& path, const std::string& filename);
		FileObject(const std::string& description, const std::string& sequence, const std::string& filename);

		FileObject(const FileObject&) = delete;
		FileObject& operator=(const FileObject&) = delete;

		~FileObject();

		const std::string GetSequenceName() const;
		const int GetSequenceSize() const;
		const std::string GetFastaDescriptionString() const;
		const std::string GetSequenceString() const;
		const char GetSequenceCharAt(const int pos) const;

		//void readArgumentFile();
		//void readSequencesFile();
	private:
		int sequencesize;

		//std::string filePath;
		std::string fileName;
		std::string fastaDescription;
		std::string sequenceString;

		//char* line;

		//pointing to same memory block
		//char* fileBufferAddr;
		//char* sequenceBuffer;
		


	};

}
#endif // !_FileObject
