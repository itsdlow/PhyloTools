/**************************************************************************************************************************
DeAngelo Wilson
August 5 2020

					ForestPlug
					-> the start plug for forestTools (main)
					---- resource managing object for ForestTools
**************************************************************************************************************************/

#ifndef _ForestPlug
#define _ForestPlug

namespace distanceMeasure
{
	class DistanceMatrixObject;
	class DistanceMeasureCalculator;
	class SequenceNamesStrategy;
	class RunFlags;
}

class ForestPlug
{
private:
	const int ASCII_INTEGER_DIFFERENCE = 48;

public:
	ForestPlug();
	ForestPlug(const ForestPlug&) = delete;
	ForestPlug& operator=(const ForestPlug&) = delete;
	~ForestPlug();


	void run();

private:
	void TryClearingTempFiles();
	distanceMeasure::DistanceMeasureCalculator* CreateDistanceCalculator();
	void InitializeBatchCalculatorFlags(distanceMeasure::RunFlags* flags, const int batch_calculator_index);
	void SetRunFlags(distanceMeasure::RunFlags* flags);
	std::string SetSequenceListsFile(int batch_flag, const distanceMeasure::DistanceMatrixObject& dmo);
	distanceMeasure::SequenceNamesStrategy* GetNamingStrategy();

	//returns path to initial-full FASTA file
	std::string GetOriginalFastaInputPath();
};


#endif
