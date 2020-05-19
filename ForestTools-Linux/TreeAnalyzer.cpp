///**************************************************************************************************************************
//DeAngelo Wilson
//February 26 2020
//
//					TreeAnalyzer
//**************************************************************************************************************************/
//
//#include "TreeAnalyzer.h"
//
//#include "DistanceMeasureCalculator.h"
//#include "SystemParameters.h"
//#include "PhyloAnalysis.h"
//
//
//namespace phylo
//{
//	//TreeAnalyzer::TreeAnalyzer(const int tree_count) :
//	//	tree_count(tree_count)//,
//	//	//pCalculators(nullptr)
//	//{
//	//}
//
//	void TreeAnalyzer::analyze_trees(const std::vector<std::string>& tree_paths)
//	{
//		//analyze current sequence set trees
//			//SequenceLists.txt --> must be ordered Largest sequence_set_size -> smallest (quartets) + GROUPED BY sequence_set_size
//		const int tree_count = static_cast<int>(tree_paths.size());
//
//		//set all table entries to 0...
//		this->InitializeCompatibilityVectors(tree_count);
//
//		//compare all permutations of tree_methods (calculators)
//		for (int i = 0; i < tree_count; i++)
//		{
//			//symmetric -- ignoring self
//			//for(int j = i + 1; j < BatchDistanceCalculators::tree_count; j++)
//			//symmetric -- includes self
//			//for(int j = i; j < BatchDistanceCalculators::tree_count; j++)
//
//			//calculate for all comparisons (include self -- NOT symmetric)
//			for (int j = 0; j < tree_count; j++)
//			{
//				//this->CalculateAnalysisMeasures(i, j, sequence_set_size, batch_id);
//				this->CalculateLargeTreeAnalysisMeasures(i, j, tree_paths.at(i), tree_paths.at(j), tree_count);
//
//			}
//		}
//		//
//		this->WriteLargeTreeAnalysisTables(tree_count);
//
//	}
//	
//	//TODO:: seperate BatchCalculatorAnalyzer & TreeAnalyzer
//		//batch calc analyzer --> deals with iterating through sequence sets
//		//TreeAnalyzer --> contains Compatibility Table Vectors, takes file names of newick trees
//			//calls Tree analysis functions + writes tables
//	/*****************************************************************************************
//	 *
//	******************************************************************************************/
//	//NOTE:: Could allow user to specify directory with all input files --> then jsut needs to give filename (not path)...
//	void TreeAnalyzer::CalculateLargeTreeAnalysisMeasures(const int index_i, const int index_j, const std::string& tree_path_i, const std::string& tree_path_j, const int tree_count)
//	{
//		//TREE FILENAMES GIVEN...
//		//
//		//NOTE:: COULD/SHOULD CHECK THAT TREES ARE ON SAME SEQUENCE SET??? ---> resulst will reflect either way...
//				//PhyloAnalysis --> shouldnt break if different sequence sets?
//
//		size_t pos = static_cast<size_t>(distanceMeasure::DistanceMeasureCalculator::getArrayIndex(index_i, index_j, tree_count));
//		//calculate all 4 compatibilities -- push to vectors
//
//		//SYMMETRIC
//		this->compatibilityTable_LtoL.at(pos) += PhyloAnalysis::
//			computeSymmetricDifference(tree_path_i, tree_path_j);
//
//		//this->compatibilityTable_QtoQ.at(pos) += PhyloAnalysis::
//		//	computeQuartetAgreement(quartetsFilename1, quartetsFilename2);
//
//		this->compatibilityTable_IQtoIQ.at(pos) += PhyloAnalysis::
//			computeInducedQuartetAgreement(tree_path_i, tree_path_j);
//
//		////NOT SYMMETRIC
//		//this->compatibilityTable_IQtoQ.at(pos) += PhyloAnalysis::
//		//	computeQuartetCompatibility(largeListTreeFilename1, quartetsFilename2);
//		//calculate all 4_NEW_compatibilities -- push to vectors
//		//SYMMETRIC
//		this->compatibilityTable_LtoL_.at(pos) += PhyloAnalysis::
//			computeCompatibilityMetric_LtoL(tree_path_i, tree_path_j);
//
//		//this->compatibilityTable_QtoQ_.at(pos) += PhyloAnalysis::
//		//	computeCompatibilityMetric_QtoQ(quartetsFilename1, quartetsFilename2);
//
//		this->compatibilityTable_IQtoIQ_.at(pos) += PhyloAnalysis::
//			computeCompatibilityMetric_IQtoIQ(tree_path_i, tree_path_j);
//
//		////NOT SYMMETRIC
//		//this->compatibilityTable_IQtoQ_.at(pos) += PhyloAnalysis::
//		//	computeCompatibilityMetric_IQtoQ(largeListTreeFilename1, quartetsFilename2);
//	}
//	void TreeAnalyzer::InitializeCompatibilityVectors(const int table_dimension)
//	{
//		for (int i = 0; i < table_dimension; i++)
//		{
//			for (int j = 0; j < table_dimension; j++)
//			{
//				this->compatibilityTable_LtoL.push_back(0);
//				this->compatibilityTable_IQtoQ.push_back(0);
//				this->compatibilityTable_IQtoIQ.push_back(0);
//				this->compatibilityTable_QtoQ.push_back(0);
//				this->compatibilityTable_LtoL_.push_back(0);
//				this->compatibilityTable_IQtoQ_.push_back(0);
//				this->compatibilityTable_IQtoIQ_.push_back(0);
//				this->compatibilityTable_QtoQ_.push_back(0);
//			}
//
//		}
//	}
//	//TODO:: change to a for loop???
//	void TreeAnalyzer::WriteLargeTreeAnalysisTables(const int tree_count)
//	{
//		//write each different anlysis measure table, to same file
//		this->WriteAnalysisTable(this->compatibilityTable_LtoL, std::string("compatibilityTable_LtoL\n"));
//		//this->WriteAnalysisTable(this->compatibilityTable_QtoQ, std::string("compatibilityTable_QtoQ\n"));
//		this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ, std::string("compatibilityTable_IQtoIQ\n"));
//		//this->WriteAnalysisTable(this->compatibilityTable_IQtoQ, std::string("compatibilityTable_IQtoQ\n"));
//		//new measures
//		this->WriteAnalysisTable(this->compatibilityTable_LtoL_, std::string("new_compatibilityTable_LtoL\n"));
//		//this->WriteAnalysisTable(this->compatibilityTable_QtoQ_, std::string("new_compatibilityTable_QtoQ\n"));
//		this->WriteAnalysisTable(this->compatibilityTable_IQtoIQ_, std::string("new_compatibilityTable_IQtoIQ\n"));
//		//this->WriteAnalysisTable(this->compatibilityTable_IQtoQ_, std::string("new_compatibilityTable_IQtoQ\n"));
//
//		//NOTE:: ANALYSIS FILE SHOULD BE NAMED .../AnalysisTables_tree-count_sequence_set_size
//		//open + write to file
//		char analysis_table_file_path[100];
//		this->GetAnalysisTableFilePath(analysis_table_file_path, 100, tree_count);
//
//		FILE* pResults;
//		//WINDOWS DEPENDENCE -- "_s" functions
//		fopen_s(&pResults, analysis_table_file_path, "w");
//		//this->pResults = fopen(largetree_filename, "w");
//		//this->pQuartetResults = fopen(quartettrees_filename, "w");
//		if (pResults != nullptr)
//		{
//			size_t numBytesWritten = fwrite(this->results.c_str(), this->results.length(), 1, pResults);
//			printf("%s written...\n", analysis_table_file_path);
//			fclose(pResults);
//			//reset string for next batch
//			this->results.clear();
//		}
//	}
//	void TreeAnalyzer::WriteAnalysisTable(const std::vector<float>& table_vector, const std::string& table_description, const int table_dimension, const int ss_size_count)
//	{
//		this->results.append(table_description);
//		this->results.append(this->GetStandardizedCalculatorLabels());
//
//		//get average of compatibility tables, using this->previous_ss_size
//			//write to results buffer
//		for (int i = 0; i < table_dimension; i++)
//		{
//			this->results.append(this->StandardizeCalculatorLabel(i));
//			for (int j = 0; j < table_dimension; j++)
//			{
//				const float avg_val = table_vector.at(distanceMeasure::DistanceMeasureCalculator::getArrayIndex(i, j, table_dimension)) / static_cast<float>(ss_size_count);
//				this->results.append(std::to_string(avg_val));
//				this->results.append(" ");
//
//			}
//			this->results.append("\n");
//		}
//		this->results.append("\n\n");
//	}
//	void TreeAnalyzer::GetAnalysisTableFilePath(char* buffer, const size_t buffer_size, const int tree_count) const
//	{
//		sprintf_s(buffer, buffer_size, SystemParameters::GetAnalysisTableFileFormatString().c_str(), tree_count);
//	}
//	void TreeAnalyzer::GetBatchAnalysisTableFilePath(char* buffer, const size_t buffer_size, const int sequence_count) const
//	{
//		sprintf_s(buffer, buffer_size, SystemParameters::GetBatchAnalysisTableFileFormatString().c_str(), sequence_count);
//	}
//	
//	//TODO -- clean up to allow for better standardizing of label sizes + offsets...
//	std::string phylo::TreeAnalyzer::GetStandardizedCalculatorLabels() const
//	{
//		std::string label;
//		const int columnOffset = this->standard_label_size + 2 + 1;
//		//add tab to skip 1st column
//		label.append(columnOffset, ' ');
//		//BAD -- static offset for first column (9 spaces)
//		//label.append("         ");
//
//		for (int i = 0; i < this->tree_count; i++)
//		{
//
//			label.append(this->StandardizeCalculatorLabel(i));
//		}
//		label.append("\n");
//		return label;
//	}
//
//	//std::string phylo::TreeAnalyzer::StandardizeCalculatorLabel(const int index) const
//	//{
//	//	std::string label = this->pCalculators[index]->GetCalculatorName().substr(0, this->standard_label_size);
//	//	int size = static_cast<int>(label.size());
//	//	if (size < this->standard_label_size)
//	//	{
//	//		const int diff = this->standard_label_size - size;
//	//		for (int i = 0; i < diff; i++)
//	//		{
//	//			label.append(" ");
//	//		}
//	//	}
//
//	//	return " " + label + "  ";
//	//}
//}
