

/********************************************************************************************************
DeAngelo Wilson
January 3 2020

					MrBayesDistanceMeasureCalculator -- derived dmc -- njp
********************************************************************************************************/


#include "MrBayesDistanceCalculator.h"

#include "SystemParameters.h"

#include <fstream>

namespace distanceMeasure
{
	//move to SystemParameters??? -- add GetCalcName() (switch statement on calculator.type)
	std::string MrBayesDistanceCalculator::GetCalculatorName() const
	{
		return "MrBayes";
	}

	//main driver
	void distanceMeasure::MrBayesDistanceCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
			//loop for all combos of 4
				//create aligned --> nexus --> execute nexus (repeat)
				// + extracts trees from .t file --> write/append to [...]Tree(s)MrBayes.newick
		this->calculate_large_list_tree(fileObjectManager, sequence_set_names, batch_id);
		this->calculate_quartet_trees(fileObjectManager, sequence_set_names, batch_id);
	}
	void distanceMeasure::MrBayesDistanceCalculator::calculate_large_list_tree(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
					//CREATE TREE FOR LARGE LIST
		//create .afa (aligned) sequence file --> NEXUS FILE
		const std::string nexus_file_path = CalculatorNexusFormatter::create_sequence_set_nexus_file(fileObjectManager, sequence_set_names);
		//create batch MRBAYES BLOCK file
		const std::string mrbayes_block_file_path = create_mrbayes_default_command_block_file(nexus_file_path);

		//WiNDOWS DEPENDENCE 
			//UNIX MRBayes Command -> input redirection
			//mb < batch.txt > log.txt & <-- (run task in background - do not wait)
		char mrbayes_command[200];
		//system call to extra-tools\\MrBayes... on MRBAYES command block file
			//NOTE:: call is relative to current_code (sln_folder/ForestTools/) execution
		sprintf_s(mrbayes_command, SystemParameters::GetMrBayesCommandString().c_str(), mrbayes_block_file_path.c_str());
		//
		system(mrbayes_command);
		//extarct tree
		char largelist_filename[100];
		sprintf_s(largelist_filename, SystemParameters::GetLargeListMatrixFileFormatString().c_str(), this->GetCalculatorName().c_str(), sequence_set_names.size(), batch_id);

		//open file for quartetTrees
		FILE* largeTreeFile;
		fopen_s(&largeTreeFile, largelist_filename, "w");
		//FILE* fastaFile = fopen(fasta_filename, "w");

		if (largeTreeFile)
		{
			//TODO
			//extract tree from created file (nexusfilepath.run_nRun.t)
			std::string newick = ExtractMrBayesNewick(nexus_file_path);
			size_t numBytesWritten = fwrite(newick.c_str(), newick.length(), 1, largeTreeFile);
			fclose(largeTreeFile);
		}
		else
		{
			printf("Could not create mrbayes large tree file...\n");
			exit(0);
		}

	}
	void MrBayesDistanceCalculator::calculate_quartet_trees(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
		const int fileCount = static_cast<int>(sequence_set_names.size());

		if (fileCount == 4)
		{
			//return distanceMatrix -- tree
		}
		if (fileCount < 4)
		{
			//ERROR
		}

		char quartets_filename[100];
		sprintf_s(quartets_filename, SystemParameters::GetQuartetTreesFileFormatString().c_str(), this->GetCalculatorName().c_str(), fileCount, batch_id);

		//open file for quartetTrees
		FILE* quartetsFile;
		fopen_s(&quartetsFile, quartets_filename, "w");
		//FILE* fastaFile = fopen(fasta_filename, "w");

		if (!quartetsFile)
		{
			printf("Could not create mrbayes quartet trees file...\n");
			exit(0);
		}

		for (int i = 0; i < fileCount; i++)
		{
			//while "atleast" 3 OTHER fileObjects exist
			for (int j = i + 1; j < fileCount; j++)
			{
				for (int k = j + 1; k < fileCount; k++)
				{
					for (int l = k + 1; l < fileCount; l++)
					{
						//create .afa (aligned) sequence file --> NEXUS FILE
						const std::string nexus_file_path = CalculatorNexusFormatter::create_sequence_set_nexus_file(fileObjectManager, CreateSubSequenceSet(sequence_set_names, i, j, k, l));
						//create batch MRBAYES BLOCK file
						std::string mrbayes_block_file_path = create_mrbayes_default_command_block_file(nexus_file_path);

						//WiNDOWS DEPENDENCE 
							//UNIX MRBayes Command -> input redirection
							//mb < batch.txt > log.txt & <-- (run task in background - do not wait)
						char mrbayes_command[200];
						//system call to extra-tools\\MrBayes... on MRBAYES command block file
							//NOTE:: call is relative to current_code (sln_folder/ForestTools/) execution
						sprintf_s(mrbayes_command, SystemParameters::GetMrBayesCommandString().c_str(), mrbayes_block_file_path.c_str());
						//run mrbayes on quartet
						system(mrbayes_command);

						//TODO
						//extract tree from created file (nexusfilepath.run_nRun.t)
						std::string newick = ExtractMrBayesNewick(nexus_file_path);

						//CREATE TREES FOR QUARTETS.. append all quartet newicks to same file...
						size_t numBytesWritten = fwrite(newick.c_str(), newick.length(), 1, quartetsFile);

					}
				}
			}
		}
		fclose(quartetsFile);

	}
	//TODO
	std::string distanceMeasure::MrBayesDistanceCalculator::ExtractMrBayesNewick(const std::string& relative_nxs_path)
	{
		//latest Mrbayes--run -- file_path
		std::string tFilename = relative_nxs_path + ".run" + std::to_string(SystemParameters::GetMrBayesNRuns()) + ".t";

		//open file .t mrbayes files
		//std::string filename = SystemParameters::GetNexusFileFormatString() + SystemParameters::GetMrBayesNRuns() + ".t"
		//std::ifstream tFile(filename);

		//if (!fastaInput.is_open())
		//{
		//	printf("File at path: %s - could not be opened\nFile Objects not to be created\n", pFOM->get_sequence_set_path().c_str());
		//}
		//else
		//{
		//	int count = 0;
		//	//read file
			//find 'end;' line
				//get previous line --> last newick_tree line

		//	std::string line;
		//	std::getline(fastaInput, line);

		//	printf("finished processing file\n");
		//	fastaInput.close();
		//}
		//seek to end

		//get species_names key
			//replace int with corresponding species name
		
		//add new line -- for next newick to start on
		return std::string();
	}

	
	std::vector<std::string> MrBayesDistanceCalculator::CreateSubSequenceSet(const std::vector<std::string>& sequence_set_names, int i, int j, int k, int l)
	{
		std::vector<std::string> subSequenceSet;
		std::vector<int> subSequenceSetIndexes{ i,j,k,l };
		for(unsigned int index = 0u; index < subSequenceSetIndexes.size(); index++)
		{
			//for each given index (i,j,k,l) add name to result_set
			subSequenceSet.push_back( sequence_set_names.at(subSequenceSetIndexes.at(index)) );
		}
		return subSequenceSet;
	}
	
	std::string distanceMeasure::MrBayesDistanceCalculator::create_mrbayes_default_command_block_file(const std::string& relative_nxs_path) const
	{
		//create mrbayes batch block file
			//use default mrbayes parameters
		std::string mrbayes_block_file_path = SystemParameters::GetMrBayesBlockString();
		char mrbayes_block[400];
		sprintf_s(mrbayes_block, 
			"begin mrbayes;\n"
			"set autoclose = yes nowarn = yes;\n"
			"execute %s;\n"
			"lset nst = 6 rates = gamma;\n"
			"mcmcp filename=%s;\n"
			"mcmc nruns = %d ngen = 10000 samplefreq = 10;\n"
			"end;",
			relative_nxs_path.c_str(),
			relative_nxs_path.c_str(),
			SystemParameters::GetMrBayesNRuns()
			);
		//TODO:: SEPARATE INTO FUNCTION... done in multiple .cpp's
		std::string mrbayes_block_string;
		mrbayes_block_string.reserve(200);
		mrbayes_block_string.append(mrbayes_block);
		
		//WiNDOWS DEPENDENCE
		FILE* nexusFile;
		fopen_s(&nexusFile, mrbayes_block_file_path.c_str(), "w");
		if (nexusFile)
		{
			size_t numBytesWritten = fwrite(mrbayes_block_string.c_str(), mrbayes_block_string.length(), 1, nexusFile);
			fclose(nexusFile);
		}
		else
		{
			printf("Could not create temp nexus file...\n");
			exit(0);
		}

		
		return mrbayes_block_file_path;
	}

}


