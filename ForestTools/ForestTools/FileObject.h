


#ifndef _FileObject
#define _FileObject


#include <string>
#include <cstring>

namespace distanceMeasure
{

	class FileObject
	{
	public:
		FileObject();

		//FileObject(const std::string& path, const std::string& filename);
		FileObject(const std::string& sequence, const std::string& filename);

		FileObject(const FileObject&) = delete;
		FileObject& operator=(const FileObject&) = delete;

		~FileObject();

		void readArgumentFile();
		void readSequencesFile();
	//REDEFINE AS PRIVATE???
	//private:

		std::string filePath;
		std::string fileName;
		
		//std::string sequenceString;
		long sequencesize;

		char* line;

		//pointing to same memory block
		char* fileBufferAddr;
		char* sequenceBuffer;
		


	};

}
#endif // !_FileObject
