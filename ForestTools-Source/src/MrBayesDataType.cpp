/******************************************************************************
DeAngelo Wilson
August 11 2020

						MrBayesDataType
******************************************************************************/

#include "MrBayesDataType.h"

namespace distanceMeasure
{
	//MrBayesDataType::MrBayesDataType(std::string datatype, const char** alphabet):
	//datatype(std::move(datatype)),
	//alphabet(alphabet)
	//{
	//}

	MrBayesDataType::MrBayesDataType(std::string datatype, std::string alphabet, char unknown_replacement) :
		datatype(std::move(datatype)),
		alphabet(std::move(alphabet)),
		unknown_replacement(unknown_replacement)
	{
	}

	
	MrBayesDataType::~MrBayesDataType()
	{
		//delete[] this->alphabet;
	}

	//MrBayesProteinDataType::MrBayesProteinDataType()
	//	:MrBayesDataType("protein", new const char* [] {"A", "R", "N", "D", "C", "Q", "E", "G", "H", "I", "L", "K", "M", "F", "P", "S", "T", "W", "Y", "V", "X"})
	//{
	//}
	MrBayesProteinDataType::MrBayesProteinDataType()
		:MrBayesDataType("protein", "ARNDCQEGHILKMFPSTWYVX", 'X')
	{
	}

	distanceMeasure::MrBayesDNADataType::MrBayesDNADataType()
		: MrBayesDataType("DNA", "ACGTRYMKSWHBVDN", 'N')
	{
	}

	distanceMeasure::MrBayesRNADataType::MrBayesRNADataType()
		: MrBayesDataType("RNA", "ACGURYMKSWHBVDN", 'N')
	{
	}
}
