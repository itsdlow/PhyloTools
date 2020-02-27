/**************************************************************************************************************************
DeAngelo Wilson
February 26 2020

					BatchCalculatorsAnalyzer
**************************************************************************************************************************/

#include "BatchCalculatorsAnalyzer.h"

#include "DistanceMeasureCalculator.h"
#include "SystemParameters.h"
#include "PhyloAnalysis.h"


namespace distanceMeasure
{
	BatchCalculatorsAnalyzer::BatchCalculatorsAnalyzer(const int calculator_count):
	calculator_count(calculator_count),
	pCalculators(nullptr)
	{
	}








	void BatchCalculatorsAnalyzer::batch_analyze_sequence_set(const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
		//analyze current sequence set trees
			//SequenceLists.txt --> must be ordered Largest sequence_set_size -> smallest (quartets) + GROUPED BY sequence_set_size
		const int sequence_set_size = static_cast<int>(sequence_set_names.size());
		//check if this sequence_set defines new size
		if (sequence_set_size != this->previous_ss_size)
		{
			//if first sequence_Set (full sequence_Set)
			if (this->previous_ss_size == 0)
			{
				this->InitializeCompatibilityVectors(sequence_set_size);
			}
			else
			{
				//write table for previous ss_size
				this->WriteAnalysisTables();

				//initialize vectors...
				//push sequence_set_size 0's to vector...
				this->InitializeCompatibilityVectors(sequence_set_size);
			}
			this->previous_ss_size = sequence_set_size;
			this->ss_size_count = 1;
		}
		else
		{
			//use this number to divide sums of compatability measures
			this->ss_size_count++;

		}
		//compare all permutations of tree_methods (calculators)
		for (int i = 0; i < this->calculator_count; i++)
		{
			//symmetric -- ignoring self
			//for(int j = i + 1; j < BatchDistanceCalculators::calculator_count; j++)
			//symmetric -- includes self
			//for(int j = i; j < BatchDistanceCalculators::calculator_count; j++)

			//calculate for all comparisons (include self -- NOT symmetric)
			for (int j = 0; j < this->calculator_count; j++)
			{
				this->CalculateAnalysisMeasures(i, j, sequence_set_size, batch_id);
			}
		}
		//NOTE::last sequence_sets, of sequence_set_size --> written in LogTotalCalc..()
	}
	/*****************************************************************************************
	 *				//TODO::MOVE TO ANALYSIS OBJECT... this privately inherits
	******************************************************************************************/
	void BatchCalculatorsAnalyzer::CalculateAnalysisMeasures(const int calculator_index_i, const int calculator_index_j, const int sequence_set_size, const int batch_id)
	{
		//get tree file names for current calcs ( i - j )
		char largelistTreeFileName_1[100];
		this->pCalculators[calculator_index_i]->GetLargeListTreeFileName(largelistTreeFileName_1, 100, batch_id, sequence_set_size);
		char largelistTreeFileName_2[100];
		this->pCalculators[calculator_index_j]->GetLargeListTreeFileName(largelistTreeFileName_2, 100, batch_id, sequence_set_size);

		char quartetsTreeFileName_1[100];
		this->pCalculators[calculator_index_i]->GetQuartetsTreeFileName(quartetsTreeFileName_1, 100, batch_id, sequence_set_size);
		char quartetsTreeFileName_2[100];
		this->pCalculators[calculator_index_j]->GetQuartetsTreeFileName(quartetsTreeFileName_2, 100, batch_id, sequence_set_size);


		//calculate all 4 compatibilities -- push to vectors
	//SYMMETRIC
		this->compatibilityTable_LtoL.at(DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, this->calculator_count)) += PhyloAnalysis::
			computeSymmetricDifference(std::string(largelistTreeFileName_1), std::string(largelistTreeFileName_2));

		this->compatibilityTable_QtoQ.at(DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, this->calculator_count)) += PhyloAnalysis::
			computeQuartetAgreement(std::string(quartetsTreeFileName_1), std::string(quartetsTreeFileName_2));
		this->compatibilityTable_IQtoIQ.at(DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, this->calculator_count)) += PhyloAnalysis::
			computeInducedQuartetAgreement(std::string(largelistTreeFileName_1), std::string(largelistTreeFileName_2));
		//NOT SYMMETRIC
		this->compatibilityTable_IQtoQ.at(DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, this->calculator_count)) += PhyloAnalysis::
			computeQuartetCompatibility(std::string(largelistTreeFileName_1), std::string(quartetsTreeFileName_2));
	}
	void BatchCalculatorsAnalyzer::InitializeCompatibilityVectors(const int sequence_count)
	{
		for (int i = 0; i < this->calculator_count; i++)
		{
			this->compatibilityTable_LtoL.push_back(0);
			this->compatibilityTable_IQtoQ.push_back(0);
			this->compatibilityTable_IQtoIQ.push_back(0);
			this->compatibilityTable_QtoQ.push_back(0);
		}
	}
	void BatchCalculatorsAnalyzer::WriteAnalysisTables()
	{
		//write each different anlysis measure table, to same file
		this->WriteAnalysisTable(this->compatibilityTable_LtoL, std::string("compatibilityTable_LtoL\n"));
		this->WriteAnalysisTable(this->compatibilityTable_QtoQ, std::string("compatibilityTable_QtoQ\n"));
		this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ, std::string("compatibilityTable_IQtoIQ\n"));
		this->WriteAnalysisTable(this->compatibilityTable_IQtoQ, std::string("compatibilityTable_IQtoQ\n"));

		//open + write to file
		char analysis_table_file_path[100];
		this->GetAnalysisTableFilePath(analysis_table_file_path, 100, this->previous_ss_size);

		FILE* pResults;
		//WINDOWS DEPENDENCE -- "_s" functions
		fopen_s(&pResults, analysis_table_file_path, "w");
		//this->pResults = fopen(largetree_filename, "w");
		//this->pQuartetResults = fopen(quartettrees_filename, "w");
		if (pResults != nullptr)
		{
			size_t numBytesWritten = fwrite(this->results.c_str(), this->results.length(), 1, pResults);
			printf("%s written...\n", analysis_table_file_path);
			fclose(pResults);
			//reset string for next batch
			this->results.clear();
		}
	}
	void BatchCalculatorsAnalyzer::WriteAnalysisTable(const std::vector<float>& table_vector, const std::string& table_description)
	{
		this->results.append(table_description);

		//TODO add row name... column names? --> standardized to avg_val lengths...
		//int standardized_label_length = 8;//8 == size (# of chars) of float
		//this->results.append(standardized_column_labels_string);

		//get average of compatibility tables, using this->previous_ss_size
			//write to results buffer
		for (int i = 0; i < this->calculator_count; i++)
		{
			//this->results.append(StandardizeLabel(calculators[i].GetCalculatorName(), standardized_label_length);
			for (int j = 0; j < this->calculator_count; j++)
			{
				const float avg_val = table_vector.at(DistanceMeasureCalculator::getArrayIndex(i, j, this->calculator_count)) / static_cast<float>(this->ss_size_count);
				this->results.append(std::to_string(avg_val));
				this->results.append(" ");

			}
			this->results.append("\n");
		}
		this->results.append("\n\n");
	}
	void BatchCalculatorsAnalyzer::GetAnalysisTableFilePath(char* buffer, const size_t buffer_size, const int sequence_count) const
	{
		sprintf_s(buffer, buffer_size, SystemParameters::GetAnalysisTableFileFormatString().c_str(), sequence_count);
	}


	
}
