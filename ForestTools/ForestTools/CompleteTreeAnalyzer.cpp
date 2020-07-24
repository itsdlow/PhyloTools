/**************************************************************************************************************************
DeAngelo Wilson
July 22 2020

					CompleteTreeAnalyzer
**************************************************************************************************************************/

#include "CompleteTreeAnalyzer.h"

#include "DistanceMeasureCalculator.h"
#include "SystemParameters.h"
#include "PhyloAnalysis.h"


namespace phylo
{
	CompleteTreeAnalyzer::CompleteTreeAnalyzer(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count):
	TreeAnalyzer(pCalculators, calculator_count)
	{
	}
	
	void CompleteTreeAnalyzer::CalculateAnalysisMeasures(const int calculator_index_i, const int calculator_index_j, const int sequence_set_size, const int batch_id)
	{
		//get tree file names for current calcs ( i - j )
		char largelistTreeFileName_1[100];
		this->pCalculators[calculator_index_i]->GetLargeListTreeFileName(largelistTreeFileName_1, batch_id, sequence_set_size);
		char largelistTreeFileName_2[100];
		this->pCalculators[calculator_index_j]->GetLargeListTreeFileName(largelistTreeFileName_2, batch_id, sequence_set_size);

		char quartetsTreeFileName_1[100];
		this->pCalculators[calculator_index_i]->GetQuartetsTreeFileName(quartetsTreeFileName_1, batch_id, sequence_set_size);
		char quartetsTreeFileName_2[100];
		this->pCalculators[calculator_index_j]->GetQuartetsTreeFileName(quartetsTreeFileName_2, batch_id, sequence_set_size);
		const std::string quartetsFilename1(quartetsTreeFileName_1);
		const std::string largeListTreeFilename1(largelistTreeFileName_1);
		const std::string quartetsFilename2(quartetsTreeFileName_2);
		const std::string largeListTreeFilename2(largelistTreeFileName_2);


		const size_t pos = static_cast<size_t>(distanceMeasure::DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, this->calculator_count));
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
	void CompleteTreeAnalyzer::InitializeCompatibilityVectors(const int table_dimensions)
	{
		for (int i = 0; i < table_dimensions; i++)
		{
			for (int j = 0; j < table_dimensions; j++)
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
	void CompleteTreeAnalyzer::WriteAnalysisTables(const int ss_size_count, const int ss_size)
	{
		//write each different anlysis measure table, to same file
		this->WriteAnalysisTable(this->compatibilityTable_LtoL, std::string("compatibilityTable_LtoL\n"), ss_size_count);
		this->WriteAnalysisTable(this->compatibilityTable_QtoQ, std::string("compatibilityTable_QtoQ\n"), ss_size_count);
		this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ, std::string("compatibilityTable_IQtoIQ\n"), ss_size_count);
		this->WriteAnalysisTable(this->compatibilityTable_IQtoQ, std::string("compatibilityTable_IQtoQ\n"), ss_size_count);
		//new measures
		this->WriteAnalysisTable(this->compatibilityTable_LtoL_, std::string("new_compatibilityTable_LtoL\n"), ss_size_count);
		this->WriteAnalysisTable(this->compatibilityTable_QtoQ_, std::string("new_compatibilityTable_QtoQ\n"), ss_size_count);
		this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ_, std::string("new_compatibilityTable_IQtoIQ\n"), ss_size_count);
		this->WriteAnalysisTable(this->compatibilityTable_IQtoQ_, std::string("new_compatibilityTable_IQtoQ\n"), ss_size_count);

		//open + write to file
		char analysis_table_file_path[100];
		//this->GetAnalysisTableFilePath(analysis_table_file_path, 100, this->previous_ss_size);
		SystemParameters::GetAnalysisTableFilePath(analysis_table_file_path, ss_size);

		FILE* pResults = fopen(analysis_table_file_path, "w");
		if (pResults != nullptr)
		{
			size_t numBytesWritten = fwrite(this->results.c_str(), this->results.length(), 1, pResults);
			printf("%s written...\n", analysis_table_file_path);
			fclose(pResults);
			//reset string for next batch
			this->results.clear();
		}
	}
}
