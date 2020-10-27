/******************************************************************************
DeAngelo Wilson
August 11 2020

						MrBayesDataType
******************************************************************************/
#ifndef _MrBayesDataType
#define _MrBayesDataType

#include <string>

namespace distanceMeasure
{
	class MrBayesDataType
	{
	public:
		MrBayesDataType() = delete;
		//MrBayesDataType(std::string datatype, std::vector<std::string> alphabet);
		//MrBayesDataType(std::string datatype, const char** alphabet);
		MrBayesDataType(std::string datatype, std::string alphabet, char unknown_replacement);

		
		MrBayesDataType(const MrBayesDataType&) = delete;
		MrBayesDataType& operator=(const MrBayesDataType&) = delete;
		virtual ~MrBayesDataType();

		std::string GetDataType() const { return this->datatype; };

		std::string GetAlphabet() const { return this->alphabet; };
		//const char** const  GetAlphabet() { return this->alphabet; };

		char GetUnknownChar() const { return this->unknown_replacement; };

	private:
		std::string datatype;
		//std::vector<std::string> alphabet;
		//const char** alphabet;
		std::string alphabet;
		char unknown_replacement;
	};

	class MrBayesProteinDataType: public MrBayesDataType
	{
	public:
		MrBayesProteinDataType();

		MrBayesProteinDataType(const MrBayesProteinDataType&) = delete;
		MrBayesProteinDataType& operator=(const MrBayesProteinDataType&) = delete;
		virtual ~MrBayesProteinDataType() = default;
		
	private:

	};
	class MrBayesDNADataType : public MrBayesDataType
	{
	public:
		MrBayesDNADataType();

		MrBayesDNADataType(const MrBayesDNADataType&) = delete;
		MrBayesDNADataType& operator=(const MrBayesDNADataType&) = delete;
		virtual ~MrBayesDNADataType() = default;

	private:

	};
	class MrBayesRNADataType : public MrBayesDataType
	{
	public:
		MrBayesRNADataType();

		MrBayesRNADataType(const MrBayesRNADataType&) = delete;
		MrBayesRNADataType& operator=(const MrBayesRNADataType&) = delete;
		virtual ~MrBayesRNADataType() = default;

	private:

	};


	//struct MrBayesProteinDataType
	//{
	//	const char datatype[8] = "protein";
	//	const char* alphabet[21] = { "A", "R", "N", "D", "C", "Q", "E", "G", "H", "I", "L", "K", "M", "F", "P", "S", "T", "W", "Y", "V", "X" };
	//};

	//DNA
	////{A, C, G, T, R, Y, M, K, S, W, H, B, V, D, N}


	//RNA
	//{A, C, G, U, R, Y, M, K, S, W, H, B, V, D, N}
}

#endif