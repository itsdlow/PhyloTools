/**************************************************************************************************************************
DeAngelo Wilson
July 22 2020

					LargeTreeAnalyzer
**************************************************************************************************************************/

#include "LargeTreeAnalyzer.h"

#include "DistanceMeasureCalculator.h"
#include "SystemParameters.h"
#include "PhyloAnalysis.h"


namespace phylo
{
	LargeTreeAnalyzer::LargeTreeAnalyzer(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count) :
		TreeAnalyzer(pCalculators, calculator_count)
	{
	}

	void LargeTreeAnalyzer::CalculateAnalysisMeasures(const int calculator_index_i, const int calculator_index_j, const int sequence_set_size, const int batch_id)
	{
		//get tree file names for current calcs ( i - j )
		char largelistTreeFileName_1[100];
		this->pCalculators[calculator_index_i]->GetLargeListTreeFileName(largelistTreeFileName_1, batch_id, sequence_set_size);
		char largelistTreeFileName_2[100];
		this->pCalculators[calculator_index_j]->GetLargeListTreeFileName(largelistTreeFileName_2, batch_id, sequence_set_size);

		const std::string largeListTreeFilename1(largelistTreeFileName_1);
		const std::string largeListTreeFilename2(largelistTreeFileName_2);


		const size_t pos = static_cast<size_t>(distanceMeasure::DistanceMeasureCalculator::getArrayIndex(calculator_index_i, calculator_index_j, this->calculator_count));
		//calculate all 4 compatibilities -- push to vectors
	//SYMMETRIC
		this->compatibilityTable_LtoL.at(pos) += PhyloAnalysis::
			computeSymmetricDifference(largeListTreeFilename1, largeListTreeFilename2);

		//this->compatibilityTable_IQtoIQ.at(pos) += PhyloAnalysis::computeInducedQuartetAgreement(largeListTreeFilename1, largeListTreeFilename2);

		//calculate all 4_NEW_compatibilities -- push to vectors
		//SYMMETRIC
		this->compatibilityTable_LtoL_.at(pos) += PhyloAnalysis::
			computeCompatibilityMetric_LtoL(largeListTreeFilename1, largeListTreeFilename2);

		//his->compatibilityTable_IQtoIQ_.at(pos) += PhyloAnalysis::computeCompatibilityMetric_IQtoIQ(largeListTreeFilename1, largeListTreeFilename2);
	}
	void LargeTreeAnalyzer::InitializeCompatibilityVectors(const int table_dimensions)
	{
		for (int i = 0; i < table_dimensions; i++)
		{
			for (int j = 0; j < table_dimensions; j++)
			{
				this->compatibilityTable_LtoL.push_back(0);
				//this->compatibilityTable_IQtoIQ.push_back(0);
				this->compatibilityTable_LtoL_.push_back(0);
				//this->compatibilityTable_IQtoIQ_.push_back(0);
			}

		}
	}

	void LargeTreeAnalyzer::WriteAnalysisTables(const int ss_size_count, const int ss_size)
	{
		//write each different anlysis measure table, to same file
		this->WriteAnalysisTable(this->compatibilityTable_LtoL, std::string("compatibilityTable_LtoL\n"), ss_size_count);
		//this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ, std::string("compatibilityTable_IQtoIQ\n"), ss_size_count);
		//new measures
		this->WriteAnalysisTable(this->compatibilityTable_LtoL_, std::string("new_compatibilityTable_LtoL\n"), ss_size_count);
		//this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ_, std::string("new_compatibilityTable_IQtoIQ\n"), ss_size_count);

		//open + write to file
		char analysis_table_file_path[100];
		//this->GetAnalysisTableFilePath(analysis_table_file_path, 100, this->previous_ss_size);
		SystemParameters::GetAnalysisTableFilePath(analysis_table_file_path, ss_size);

		FILE* pResults = fopen(analysis_table_file_path, "w");
		if (pResults != nullptr)
		{
			size_t numBytesWritten = fwrite(this->results.c_str(), this->results.length(), 1, pResults);
			AZUL_UNUSED_VAR(numBytesWritten);

			printf("%s written...\n", analysis_table_file_path);
			fclose(pResults);
			//reset string for next batch
			this->results.clear();
		}
	}
}
