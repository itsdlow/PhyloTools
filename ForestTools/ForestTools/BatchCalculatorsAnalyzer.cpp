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

	//NOTE:: BROKEN --> SEQUENCELISTS.TXT GIVEN NOT ORDERED (16,12,8,8,...,8,12,8,...,8,12,...4,..)***********************************************************************************
	//**********************************************************************************************************************************************************************
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
	 *				
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
		const std::string quartetsFilename1(quartetsTreeFileName_1);
		const std::string largeListTreeFilename1(largelistTreeFileName_1);
		const std::string quartetsFilename2(quartetsTreeFileName_2);
		const std::string largeListTreeFilename2(largelistTreeFileName_2);

		
		size_t pos = static_cast<size_t>(DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, this->calculator_count));
		//calculate all 4 compatibilities -- push to vectors
	//SYMMETRIC
		this->compatibilityTable_LtoL.at(pos) += PhyloAnalysis::
			computeSymmetricDifference(largeListTreeFilename1, largeListTreeFilename2);
		
		this->compatibilityTable_QtoQ.at(pos) += PhyloAnalysis::
			computeQuartetAgreement(quartetsFilename1, quartetsFilename2);
		
		this->compatibilityTable_IQtoIQ.at(pos) += PhyloAnalysis::
			computeInducedQuartetAgreement(largeListTreeFilename1, largeListTreeFilename2);
		
		//NOT SYMMETRIC
		this->compatibilityTable_IQtoQ.at(pos) += PhyloAnalysis::
			computeQuartetCompatibility(largeListTreeFilename1, quartetsFilename2);
		//calculate all 4_NEW_compatibilities -- push to vectors
		//SYMMETRIC
		this->compatibilityTable_LtoL_.at(pos) += PhyloAnalysis::
			computeCompatibilityMetric_LtoL(largeListTreeFilename1, largeListTreeFilename2);
		
		this->compatibilityTable_QtoQ_.at(pos) += PhyloAnalysis::
			computeCompatibilityMetric_QtoQ(quartetsFilename1, quartetsFilename2);
		
		this->compatibilityTable_IQtoIQ_.at(pos) += PhyloAnalysis::
			computeCompatibilityMetric_IQtoIQ(largeListTreeFilename1, largeListTreeFilename2);
		
		//NOT SYMMETRIC
		this->compatibilityTable_IQtoQ_.at(pos) += PhyloAnalysis::
			computeCompatibilityMetric_IQtoQ(largeListTreeFilename1, quartetsFilename2);
	}
	void BatchCalculatorsAnalyzer::InitializeCompatibilityVectors(const int sequence_count)
	{
		for (int i = 0; i < this->calculator_count; i++)
		{
			for(int j = 0; j < this->calculator_count; j++)
			{
				this->compatibilityTable_LtoL.push_back(0);
				this->compatibilityTable_IQtoQ.push_back(0);
				this->compatibilityTable_IQtoIQ.push_back(0);
				this->compatibilityTable_QtoQ.push_back(0);
				this->compatibilityTable_LtoL_.push_back(0);
				this->compatibilityTable_IQtoQ_.push_back(0);
				this->compatibilityTable_IQtoIQ_.push_back(0);
				this->compatibilityTable_QtoQ_.push_back(0);
			}

		}
	}
	//TODO:: change to a for loop???
	void BatchCalculatorsAnalyzer::WriteAnalysisTables()
	{
		//write each different anlysis measure table, to same file
		this->WriteAnalysisTable(this->compatibilityTable_LtoL, std::string("compatibilityTable_LtoL\n"));
		this->WriteAnalysisTable(this->compatibilityTable_QtoQ, std::string("compatibilityTable_QtoQ\n"));
		this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ, std::string("compatibilityTable_IQtoIQ\n"));
		this->WriteAnalysisTable(this->compatibilityTable_IQtoQ, std::string("compatibilityTable_IQtoQ\n"));
		//new measures
		this->WriteAnalysisTable(this->compatibilityTable_LtoL_, std::string("new_compatibilityTable_LtoL\n"));
		this->WriteAnalysisTable(this->compatibilityTable_QtoQ_, std::string("new_compatibilityTable_QtoQ\n"));
		this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ_, std::string("new_compatibilityTable_IQtoIQ\n"));
		this->WriteAnalysisTable(this->compatibilityTable_IQtoQ_, std::string("new_compatibilityTable_IQtoQ\n"));
		
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
		this->results.append(this->GetStandardizedCalculatorLabels());

		//get average of compatibility tables, using this->previous_ss_size
			//write to results buffer
		for (int i = 0; i < this->calculator_count; i++)
		{
			this->results.append(this->StandardizeCalculatorLabel(i));
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

	//TODO -- clean up to allow for better standardizing of label sizes + offsets...
	std::string distanceMeasure::BatchCalculatorsAnalyzer::GetStandardizedCalculatorLabels() const
	{
		std::string label;
		const int columnOffset = this->standard_label_size + 2 + 1;
		//add tab to skip 1st column
		//label.append(" ", columnOffset);
		//BAD -- static offset for first column (9 spaces)
		label.append("         ");
		
		for(int i = 0; i < this->calculator_count; i++)
		{
			
			label.append(StandardizeCalculatorLabel(i));
		}
		label.append("\n");
		return label;
	}

	std::string distanceMeasure::BatchCalculatorsAnalyzer::StandardizeCalculatorLabel(const int index) const
	{
		std::string label = this->pCalculators[index]->GetCalculatorName().substr(0, this->standard_label_size);
		int size = static_cast<int>(label.size() );
		if(size < this->standard_label_size)
		{
			const int diff = this->standard_label_size - size;
			for(int i = 0; i < diff; i++)
			{
				label.append(" ");
			}
		}
		
		return " " + label + "  ";
	}
}
