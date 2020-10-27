/**************************************************************************************************************************
DeAngelo Wilson
August 5 2020

					ForestPlug
					-> the start plug for forestTools (main)
					---- resource managing object for ForestTools
**************************************************************************************************************************/

#ifndef _ForestPlug
#define _ForestPlug

#include <string>
#include <vector>

namespace distanceMeasure
{
	class DistanceMatrixObject;
	class DistanceMeasureCalculator;
	class SequenceNamesStrategy;
	struct RunFlags;
}

class ForestPlug
{
private:
	const int ASCII_INTEGER_DIFFERENCE = 48;

	// Run flags (global blackboard) between dmc's... owned by DMO (this)
	distanceMeasure::RunFlags* poRunFlags;
public:
	ForestPlug();
	ForestPlug(const ForestPlug&) = delete;
	ForestPlug& operator=(const ForestPlug&) = delete;
	~ForestPlug();


	void run();

	//use by DMO -- NOTE:: so ugly
	static void SetSequenceListsFile(int batch_flag, const distanceMeasure::DistanceMatrixObject& dmo, std::string& sequence_lists_file);

private:
	//Sets (this) run flags...
	distanceMeasure::DistanceMeasureCalculator* CreateDistanceCalculator() const;

	void InitializeBatchCalculatorFlags(distanceMeasure::RunFlags* flags, const int batch_calculator_index) const;

	//static helpers
	static void split(const std::string& s, char delim, std::vector<std::string>& result);
	static void TryClearingTempFiles();
	static void SetRunFlags(distanceMeasure::RunFlags* flags);
	static distanceMeasure::SequenceNamesStrategy* GetNamingStrategy();
	//returns path to initial-full FASTA file
	std::string GetOriginalFastaInputPath();

	//private function -- helpers functions
	std::string GetSingleFastaFilePath();
	std::string CombineMultipleFastaFileInputs();
	std::string CombineMultipleChromosomalFastaInputs();



};


#endif
