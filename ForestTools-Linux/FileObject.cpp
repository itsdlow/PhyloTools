/******************************************************************************
DeAngelo Wilson
January 3* 2020

						FileObject -- NCBI Fasta Sequences, Object
******************************************************************************/
#include "FileObject.h"

#include <fstream>

namespace distanceMeasure
{

	FileObject::FileObject()
	{
		//do nothing
	}

	/*
	FileObject::FileObject(const std::string& path, const std::string& filename):
	filePath(path),
	fileName(filename)
	{
		//this->fileName = new char[120];
		//this->filePath = new char[120];
		
		//memcpy(this->fileName, filename.c_str(), filename.size()+1);
		//memcpy(this->filePath, path.c_str(), path.size()+1);

		//set reminaing members of FileObject --> read file
		readArgumentFile();


		
		//std::string filePath;
		//std::string fileName;
		//
		////set in readArgFile
		//	long sequencesize;

		//	std::string line;

		//	//pointing to same memory block
		//	char* fileBufferAddr;
		//	char* sequenceBuffer;


		
	}
	*/

	FileObject::FileObject(const std::string& description, const std::string & sequence, const std::string & filename):
		sequencesize(static_cast<int>(sequence.size())),
		fileName(filename),
		fastaDescription(description),
		sequenceString(sequence)
	{
		//this->sequencesize = sequence.size();
		//set sequence buffer
		//this->sequenceBuffer = new char[this->sequencesize];
		//memcpy(this->sequenceBuffer, sequence.c_str(), this->sequencesize);
		//printf("sequenceBuffer::%s\n", this->sequenceBuffer);

		//printf("sequenceString::%s\n", this->sequenceString.c_str());
	}

	//accesors
	const std::string distanceMeasure::FileObject::GetSequenceName() const
	{
		return this->fileName;
	}
	const int distanceMeasure::FileObject::GetSequenceSize() const
	{
		return this->sequencesize;
	}
	const std::string distanceMeasure::FileObject::GetFastaDescriptionString() const
	{
		return this->fastaDescription;
	}
	const std::string distanceMeasure::FileObject::GetSequenceString() const
	{
		return this->sequenceString;
	}
	const char distanceMeasure::FileObject::GetSequenceCharAt(const int pos) const
	{
		return this->sequenceString.at(pos);
	}





	FileObject::~FileObject()
	{
		//delete dynamically allocated charsequence Buffer
		//delete[] this->fileBufferAddr;
		//delete[] this->line;
	}

	//void FileObject::readSequencesFile()
	//{

	//}


	//void FileObject::readArgumentFile()
	//{
	//	const int MAX_LINE_SIZE = 120;

	//	//open + read argument files
	//	FILE * fastaInput;
	//	fopen_s(&fastaInput, this->filePath.c_str(), "rb");
	//	//FILE* fastaInput = fopen(this->filePath.c_str(), "rb");

	//	//get size of file
	//	fseek(fastaInput, 0, SEEK_END);
	//	long filesize = ftell(fastaInput);
	//	rewind(fastaInput);

	//	//create Sequence Buffer
	//	this->fileBufferAddr = new char[filesize];
	//	this->sequenceBuffer = this->fileBufferAddr;
	//	this->line = new char[MAX_LINE_SIZE];

	//	fread(fileBufferAddr, filesize, 1, fastaInput);
	//	memset(line, 0, MAX_LINE_SIZE);//clear buffer --> prevents random data from printing on printf

	//	int i = 0;//i == line-size

	//	//getline(fastaInput2,line) -- get first line of input file --> into "line"
	//	while (sequenceBuffer) {
	//		if (*sequenceBuffer == '\n')
	//		{
	//			i++;
	//			sequenceBuffer++;
	//			break;
	//		}
	//		else
	//		{
	//			line[i] = *sequenceBuffer;
	//			i++;
	//			sequenceBuffer++;
	//		}
	//	}
	//	fclose(fastaInput);
	//	this->sequencesize = filesize - i;

	//}












}
