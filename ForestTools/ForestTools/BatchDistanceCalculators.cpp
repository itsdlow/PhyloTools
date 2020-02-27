/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/

#include "BatchDistanceCalculators.h"

#include "MrBayesDistanceCalculator.h"
#include "PValueDistanceCalculator.h"
#include "LcsDistanceCalculator.h"

//#include "PhyloAnalysis.h"

namespace distanceMeasure
{

	distanceMeasure::BatchDistanceCalculators::BatchDistanceCalculators(const int calculator_count):
	BatchCalculatorsAnalyzer(calculator_count),
	calculator_count(calculator_count),
	calculators(new DistanceMeasureCalculator* [calculator_count])
	{
		//give Calculator analyzer ref to calculators
		BatchCalculatorsAnalyzer::SetCalculatorsArray(calculators);
		//NOT IMPLEMENTED::
		//take as input parameter --> calc_count + bit_mask?
			//defines what calculator methods to include in batch_calculation
			//
		//intiialzie calculators array -- WITH ALL CALCULATOR METHODS
		//for(int i = 0; i < BatchDistanceCalculators::calculator_count; i++)
		//{
		//	this->calculators[i] = GetCalculator(i);
		//}
		this->calculators[0] = new PValueDistanceCalculator();
		this->calculators[1] = new LcsDistanceCalculator();
	}

	std::string distanceMeasure::BatchDistanceCalculators::GetCalculatorName() const
	{
		return "Batch";
	}
	
	void BatchDistanceCalculators::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{
		//double totalCalculationTime = 0.0;
		this->StartCalculationTimer();
		for (int i = 0; i < this->calculator_count; i++)
		{
			//this->calculators[i]->InitializeSequenceSetTimingsLog();
			this->calculators[i]->calculate_and_output_matrix(fileObjectManager, sequence_set_names, sequence_set, batch_id);
			//totalCalculationTime += this->calculators[i]->LogTotalCalculationTime();
		}
		this->StopCalculationTimer(batch_id, sequence_set);
		////printf("\nBatch Calculation Time For Sequence Set Lists: %f minutes\n", totalCalculationTime/60.0);

		//analyze trees...
			//if called here...
			//DOES NOT NEED TO BE VIRTUAL
		BatchCalculatorsAnalyzer::batch_analyze_sequence_set(sequence_set_names, batch_id);
	}

	//void distanceMeasure::BatchDistanceCalculators::batch_analyze_sequence_set(const std::vector<std::string>& sequence_set_names, const int batch_id)
	//{
	//	//analyze current sequence set trees
	//		//SequenceLists.txt --> must be ordered Largest sequence_set_size -> smallest (quartets) + GROUPED BY sequence_set_size
	//	const int sequence_set_size = static_cast<int>(sequence_set_names.size());
	//	//check if this sequence_set defines new size
	//	if(sequence_set_size != this->previous_ss_size)
	//	{
	//		//if first sequence_Set (full sequence_Set)
	//		if(this->previous_ss_size == 0)
	//		{
	//			this->InitializeCompatibilityVectors(sequence_set_size);
	//		}
	//		else
	//		{
	//			//write table for previous ss_size
	//			this->WriteAnalysisTables(this->previous_ss_size);
	//			
	//			//initialize vectors...
	//			//push sequence_set_size 0's to vector...
	//			this->InitializeCompatibilityVectors(sequence_set_size);
	//		}
	//		this->previous_ss_size = sequence_set_size;
	//		this->ss_size_count = 1;
	//	}
	//	else
	//	{
	//		//use this number to divide sums of compatability measures
	//		this->ss_size_count++;
	//		
	//	}
	//	//compare all permutations of tree_methods (calculators)
	//	for(int i = 0; i < BatchDistanceCalculators::calculator_count; i++)
	//	{
	//		//symmetric -- ignoring self
	//		//for(int j = i + 1; j < BatchDistanceCalculators::calculator_count; j++)
	//		//symmetric -- includes self
	//		//for(int j = i; j < BatchDistanceCalculators::calculator_count; j++)

	//		//calculate for all comparisons (include self -- NOT symmetric)
	//		for(int j = 0; j < BatchDistanceCalculators::calculator_count; j++)
	//		{
	//			this->CalculateAnalysisMeasures(i, j, sequence_set_size, batch_id);
	//		}
	//	}
	//	//NOTE::last sequence_sets, of sequence_set_size --> written in LogTotalCalc..()
	//}
	///*****************************************************************************************
	// *				//TODO::MOVE TO ANALYSIS OBJECT... this privately inherits
	//******************************************************************************************/
	//void BatchDistanceCalculators::CalculateAnalysisMeasures(const int calculator_index_i, const int calculator_index_j, const int sequence_set_size, const int batch_id)
	//{
	//	//get tree file names for current calcs ( i - j )
	//	char largelistTreeFileName_1[100];
	//	this->calculators[calculator_index_i]->GetLargeListTreeFileName(largelistTreeFileName_1, 100, batch_id, sequence_set_size);
	//	char largelistTreeFileName_2[100];
	//	this->calculators[calculator_index_j]->GetLargeListTreeFileName(largelistTreeFileName_2, 100, batch_id, sequence_set_size);

	//	char quartetsTreeFileName_1[100];
	//	this->calculators[calculator_index_i]->GetQuartetsTreeFileName(quartetsTreeFileName_1, 100, batch_id, sequence_set_size);
	//	char quartetsTreeFileName_2[100];
	//	this->calculators[calculator_index_j]->GetQuartetsTreeFileName(quartetsTreeFileName_2, 100, batch_id, sequence_set_size);

	//	
	//		//calculate all 4 compatibilities -- push to vectors
	//	//SYMMETRIC
	//	this->compatibilityTable_LtoL.at(DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, sequence_set_size)) += PhyloAnalysis::
	//		computeSymmetricDifference(std::string(largelistTreeFileName_1), std::string(largelistTreeFileName_2));
	//	
	//	this->compatibilityTable_QtoQ.at(DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, sequence_set_size)) += PhyloAnalysis::
	//		computeQuartetAgreement(std::string(quartetsTreeFileName_1), std::string(quartetsTreeFileName_2));
	//	this->compatibilityTable_IQtoIQ.at(DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, sequence_set_size)) += PhyloAnalysis::
	//		computeInducedQuartetAgreement(std::string(largelistTreeFileName_1), std::string(largelistTreeFileName_2));
	//	//NOT SYMMETRIC
	//	this->compatibilityTable_IQtoQ.at(DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, sequence_set_size)) += PhyloAnalysis::
	//		computeQuartetCompatibility(std::string(largelistTreeFileName_1), std::string(quartetsTreeFileName_2));
	//}
	//void BatchDistanceCalculators::InitializeCompatibilityVectors(const int sequence_count)
	//{
	//	for(int i = 0; i < BatchDistanceCalculators::calculator_count; i++)
	//	{
	//		this->compatibilityTable_LtoL.push_back(0);
	//		this->compatibilityTable_IQtoQ.push_back(0);
	//		this->compatibilityTable_IQtoIQ.push_back(0);
	//		this->compatibilityTable_QtoQ.push_back(0);
	//	}
	//}
	//void distanceMeasure::BatchDistanceCalculators::WriteAnalysisTables(const int sequence_count)
	//{
	//	//write each different anlysis measure table, to same file
	//	this->WriteAnalysisTable(this->compatibilityTable_LtoL, std::string("compatibilityTable_LtoL\n"));
	//	this->WriteAnalysisTable(this->compatibilityTable_QtoQ, std::string("compatibilityTable_QtoQ\n"));
	//	this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ, std::string("compatibilityTable_IQtoIQ\n"));
	//	this->WriteAnalysisTable(this->compatibilityTable_IQtoQ, std::string("compatibilityTable_IQtoQ\n"));

	//	//open + write to file
	//	char analysis_table_file_path[100];
	//	this->GetAnalysisTableFilePath(analysis_table_file_path, 100, sequence_count);

	//	FILE* pResults;
	//	//WINDOWS DEPENDENCE -- "_s" functions
	//	fopen_s(&pResults, analysis_table_file_path, "w");
	//	//this->pResults = fopen(largetree_filename, "w");
	//	//this->pQuartetResults = fopen(quartettrees_filename, "w");
	//	if (pResults != nullptr)
	//	{
	//		size_t numBytesWritten = fwrite(this->results.c_str(), this->results.length(), 1, pResults);
	//		printf("%s written...\n", analysis_table_file_path);
	//		fclose(pResults);
	//		//reset string for next batch
	//		this->results.clear();
	//	}
	//}
	//void BatchDistanceCalculators::WriteAnalysisTable(const std::vector<float>& table_vector, const std::string& table_description)
	//{
	//	this->results.append(table_description);
	//	
	//	//TODO add row name... column names? --> standardized to avg_val lengths...
	//	//int standardized_label_length = 8;//8 == size (# of chars) of float
	//	//this->results.append(standardized_column_labels_string);

	//	//get average of compatibility tables, using this->previous_ss_size
	//		//write to results buffer
	//	for(int i = 0; i < BatchDistanceCalculators::calculator_count; i++)
	//	{
	//		//this->results.append(StandardizeLabel(calculators[i].GetCalculatorName(), standardized_label_length);
	//		for(int j = 0; j < BatchDistanceCalculators::calculator_count; j++)
	//		{
	//			const float avg_val = table_vector.at(DistanceMeasureCalculator::getArrayIndex(i, j, BatchDistanceCalculators::calculator_count)) /  static_cast<float>(this->ss_size_count);
	//			this->results.append(std::to_string(avg_val));
	//			this->results.append(" ");
	//			
	//		}
	//		this->results.append("\n");
	//	}
	//	this->results.append("\n\n");
	//}
	//void BatchDistanceCalculators::GetAnalysisTableFilePath(char* buffer, const size_t buffer_size, const int sequence_count) const
	//{
	//	sprintf_s(buffer, buffer_size, SystemParameters::GetAnalysisTableFileFormatString().c_str(), sequence_count);
	//}
	//
	void BatchDistanceCalculators::InitializeSequenceSetTimingsLog()
	{
		DistanceMeasureCalculator::InitializeSequenceSetTimingsLog();
		//COMPOSITE... initialize all calc timingsLogFILE
		for (int i = 0; i < this->calculator_count; i++)
		{
			//this->calculators[i]->InitializeSequenceSetTimingsLog();
			this->calculators[i]->InitializeSequenceSetTimingsLog();
			//totalCalculationTime += this->calculators[i]->LogTotalCalculationTime();
		}
	}
	//composite pattern --> tell all derived calcs...
	void BatchDistanceCalculators::LogTotalCalculationTime()
	{
		//TODO
		//last sequence_set must be written to file + file close
		BatchCalculatorsAnalyzer::WriteAnalysisTables();
		
		for (int i = 0; i < this->calculator_count; i++)
		{
			this->calculators[i]->LogTotalCalculationTime();
		}

		DistanceMeasureCalculator::LogTotalCalculationTime();
	}


	distanceMeasure::BatchDistanceCalculators::~BatchDistanceCalculators()
	{
		for (int i = 0; i < this->calculator_count; i++)
		{
			delete this->calculators[i];
		}
	}

	DistanceMeasureCalculator* distanceMeasure::BatchDistanceCalculators::GetCalculator(int i)
	{
		DistanceMeasureCalculator* dmc = nullptr;

		switch (i)
		{
		case 0:
			dmc = new LcsDistanceCalculator();
			break;
		case 1:
			dmc = new PValueDistanceCalculator();
			break;
		case 2:
			dmc = new MrBayesDistanceCalculator();
			break;
			//case 3:
			//	dmc = new NcdDistanceCalculator();
		default:
			break;
		}
		return dmc;
	}
}

