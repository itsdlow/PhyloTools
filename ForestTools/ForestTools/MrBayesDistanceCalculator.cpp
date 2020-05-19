

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
	distanceMeasure::MrBayesDistanceCalculator::MrBayesDistanceCalculator(RunFlags* flags):
	AlignedDistanceMeasureCalculator(flags)
	{
	}
	
	//move to SystemParameters??? -- add GetCalcName() (switch statement on calculator.type)
	std::string MrBayesDistanceCalculator::GetCalculatorName() const
	{
		return "MrBayes";
	}

	//main driver
	void distanceMeasure::MrBayesDistanceCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{
		this->StartCalculationTimer();
		//loop for all combos of 4
			//create aligned --> nexus --> execute nexus (repeat)
			// + extracts trees from .t file --> write/append to [...]Tree(s)MrBayes.newick
		this->calculate_large_list_tree(fileObjectManager, sequence_set_names, batch_id);

		if(this->GetCalculatorFlags()->generate_quartets)
		{
			this->calculate_quartet_trees(fileObjectManager, sequence_set_names, batch_id);
		}
		//adds to total time + sets calculationTime
		this->StopCalculationTimer(batch_id, sequence_set);
	}
	void distanceMeasure::MrBayesDistanceCalculator::calculate_large_list_tree(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
		const std::string nexus_file_path = CalculatorNexusFormatter::create_sequence_set_nexus_file(this, fileObjectManager, sequence_set_names, static_cast<int>(sequence_set_names.size()), batch_id);
		const std::string mrbayes_block_file_path = create_mrbayes_default_command_block_file(nexus_file_path);

		char mrbayes_command[200];
		//system call to extra-tools\\MrBayes... on MRBAYES command block file
		this->GetMrBayesBatchCommand(mrbayes_command, mrbayes_block_file_path);
		//execute command --> create LargeList MrBayes files (.t + .p)
		system(mrbayes_command);
		
		//extract tree ------------------
		//int size = SystemParameters::GetTreeFilePathSize();//Variable length arrays -- not working
		char largelist_filename[100];
		this->GetLargeListTreeFileName(largelist_filename, batch_id, sequence_set_names.size());
		
		//open file for quartetTrees
		FILE* largeTreeFile = fopen(largelist_filename, "w");
		//FILE* fastaFile = fopen(fasta_filename, "w");
		
		if (largeTreeFile)
		{
			//extract tree from created file (nexusfilepath.run[nRun].t)
			std::string newick = ExtractMrBayesNewick(GetMrBayesTFileName(nexus_file_path));
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
			printf("CalculateAllQuartetsDistanceMeasures:: Tree is quartet\n");
			return;
		}
		if (fileCount < 4)
		{
			//ERROR
			printf("Sequence set contains less leaves than minimum viable tree (quartet): %d\n", fileCount);
			exit(0);
		}

		char quartets_filename[100];
		this->GetQuartetsTreeFileName(quartets_filename, batch_id, fileCount);//,sequence_set_names.size());

		//open file for quartetTrees
		FILE* quartetsFile = fopen(quartets_filename, "w");

		if (!quartetsFile)
		{
			printf("Could not create mrbayes quartet trees file...\n");
			exit(0);
		}

		int count = 0;
		for (int i = 0; i < fileCount; i++)
		{
			//while "atleast" 3 OTHER fileObjects exist
			for (int j = i + 1; j < fileCount; j++)
			{
				for (int k = j + 1; k < fileCount; k++)
				{
					for (int l = k + 1; l < fileCount; l++)
					{
						//vector of quartet indicies
						std::vector<int> indexV{ i,j,k,l };//removed for MAC build
						//std::vector<int> indexV;
						//indexV.push_back(i);
						//indexV.push_back(j);
						//indexV.push_back(k);
						//indexV.push_back(l);
						const std::vector<std::string> subsequence_set_names = DistanceMeasureCalculator::CreateSubsequenceSet(sequence_set_names, indexV);
						const std::string subsequence_set = DistanceMeasureCalculator::CreateSubsequenceSetString(subsequence_set_names);

						//TODO:: CHANGE QUARTETS NAMING SCHEME -- include total sequence count + subsequence count
						//create .afa (aligned) sequence file --> NEXUS FILE
						const std::string nexus_file_path = CalculatorNexusFormatter::create_sequence_set_nexus_file(this, fileObjectManager, subsequence_set_names, fileCount, count++);
						//create batch MRBAYES BLOCK file
						std::string mrbayes_block_file_path = create_mrbayes_default_command_block_file(nexus_file_path);
						
						char mrbayes_command[200];
						//system call to extra-tools\\MrBayes... on MRBAYES command block file
						this->GetMrBayesBatchCommand(mrbayes_command, mrbayes_block_file_path);
						system(mrbayes_command);

						//extract tree from created file (nexusfilepath.run[nRun].t)
						std::string newick = ExtractMrBayesNewick(GetMrBayesTFileName(nexus_file_path));

						//CREATE TREES_FILE FOR QUARTETS.. append all quartet newicks to same file...
						size_t numBytesWritten = fwrite(newick.c_str(), newick.length(), 1, quartetsFile);
						//"writes" as quartets extarcted
						fflush(quartetsFile);
					}
				}
			}
		}
		fclose(quartetsFile);

	}

	/*****************************************************************************
	 *						tree Extraction 
	 ******************************************************************************/
	std::string MrBayesDistanceCalculator::ExtractMrBayesNewick(const std::string& t_file_name)
	{
		//given latest Mrbayes--run--file_path (t_file_name)
		//std::string tFilename = relative_nxs_path + ".run" + std::to_string(SystemParameters::GetMrBayesNRuns()) + ".t";

		//open file .t mrbayes files
		std::ifstream tFile(t_file_name);

		if (!tFile.is_open())
		{
			printf("MrBayes .t file not opened.\nCould not create mrBayes extracted Tree file\n");
			exit(0);
		}
		const std::vector<std::string> key = MrBayesDistanceCalculator::GetTFileKey(tFile);
		//safety checks for incomplete .nxs.run#.t file...
		if(key.empty())
		{
			printf("MrBayes .t file not formatted correctly -- key entries not found... exiting\n");
			exit(0);
		}
		//gets latest "tree gen" (newick) line from .t file -- cleans/parses newick
		std::string newick = MrBayesDistanceCalculator::ParseTreeGenLine(key, MrBayesDistanceCalculator::GetTreeGenLine(tFile));
		
		printf("finished processing tFile\n");
		tFile.close();

		return newick;
	}
	
		//Tree Extraction Helpers

	std::string MrBayesDistanceCalculator::ParseTreeGenLine(const std::vector<std::string>& species_names_key, const std::string& tree_gen_line)
	{
		const int chars_of_interest_count = 3;
		char chars_of_interest[chars_of_interest_count] = { ',', '(', ')' };
		
		//read tree gen line -- starting with first_of '(' -- ending with ';' (excluding)
		const size_t start = tree_gen_line.find_first_of('(');
		const size_t count = tree_gen_line.find(';') - start;
		std::string res = tree_gen_line.substr(start, count);
		
		//find ':' -- get key_number (find first parenthesis from ':' using reverse iterator
			//---> replace key_number with names_key species name
		size_t cur = std::string::npos;
		//while more key_numbers in line
		while( (cur = res.find_last_of(':', cur)) != std::string::npos )
		{
			//look for key_number (if present --> not all ':' preceded by key_number)
			//from cur position, loop (backward) thru string
				//until non-integer found ( ',' - '('- ')' )
			bool found = false;
			//get char left of ':' either char of interest || part of key_number
			const size_t key_end = cur - 1;
			size_t key_start = cur - 1;
			std::string key_number;
			while(!found)
			{
				char cur_char = res.at(key_start);
				//check if cur char is of interest -- else part of key_number
				for(int i = 0; i < chars_of_interest_count; i++)
				{
					if(cur_char == chars_of_interest[i])
					{
						//end of key_number
						found = true;
					}
				}
				if(!found)
				{
					key_start--;
					//cur pos, NOT char of interest
						//add cur pos to key_number
					key_number.insert(0,1, cur_char);
				}
			}
			//key_start == char_of_interest --> left of key_start (if key_number_length > 0)
			//replace if key_number_length > 0
			//set cur == key_start... ignores the index changes from replace?  (cur - key_number_length
			const size_t key_number_length = key_end - key_start;
			if(key_number_length > 0)
			{
				//replace key_number
				res.replace(key_start + 1, key_number_length, GetKeySpeciesName(species_names_key, std::stoi(key_number)));
			}

			cur = key_start;
		}
		//add new lines -- for next newick to start on
		res.append("\n\n");
		return res;
	}

	//	---> if key_entries not found before EOF -> returns empty key
	//Given MrBayes .t filestream -- get translate-key
	std::vector<std::string> distanceMeasure::MrBayesDistanceCalculator::GetTFileKey(std::ifstream& tFile)
	{
		std::vector<std::string> key;
		std::string line;
		
		//std::getline(tFile, line);
		//read file til "translate" found --> key definition next
			//OR --> end of file reached
		while(std::getline(tFile, line) && line.find("translate") == std::string::npos)
		{
		}
		
		
		//read first lines of file until "tree gen" found --> (no more species names to read)
		while (std::getline(tFile, line) && line.find("tree gen") == std::string::npos)
		{
			//add to key vector
			key.push_back(CleanKeyEntry(line));
			//std::getline(tFile, line);
		}
		
		return key;
	}
	std::string MrBayesDistanceCalculator::GetTreeGenLine(std::ifstream& tFile)
	{
		//read file
		//find first (from EOF) -- 'tree gen' line ( '=' )
			//seek to spot before EOF
		tFile.seekg(-1, std::ios_base::end);
		if(tFile.peek() == '\n')
		{
			//assumes file is accurate...
			tFile.seekg(-1, std::ios_base::cur);
			int i = static_cast<int>(tFile.tellg());
			for(; i > 0; i--)
			{
				// If the data was a '=' --> defining tree
					// Stop at the current position.
				if (tFile.peek() == '=') 
				{
					tFile.get();
					break;
				}
				//move back 1 char
				tFile.seekg(i, std::ios_base::beg); 
			}
		}
		else
		{
			printf("bad tFile?\n");
			exit(0);
		}

		std::string line;
		std::getline(tFile, line);

		return line;
	}

	
	std::string MrBayesDistanceCalculator::GetMrBayesTFileName(const std::string& relative_nxs_path)
	{
		int nruns = SystemParameters::GetMrBayesNRuns();
		//if only 1 gen-run (nruns == 1) --> do not append nruns to string
		if(nruns < 2)
		{
			return relative_nxs_path + ".t";
		}
		return relative_nxs_path + ".run" + std::to_string(nruns) + ".t";
	}
	
	std::string distanceMeasure::MrBayesDistanceCalculator::GetKeySpeciesName(const std::vector<std::string>& species_names_key, size_t i)
	{
		const size_t key_index_offset = 1;
		return species_names_key.at(i - key_index_offset);
	}
	//return cleaned substr of key entry line
	std::string distanceMeasure::MrBayesDistanceCalculator::CleanKeyEntry(const std::string& key_entry)
	{
		//FORMAT:: '# species_name,'
			//remove command, whitespace, and number (#)

		//const size_t start = key_entry.find_first_not_of(' ');
			//IMPROVE CLARITY --> store key_number, species name in dictionary...
		//find first space (ignoring leading whitespace)
		const size_t adjusted_start = key_entry.find(' ', key_entry.find_first_not_of(' ')) + 1;
		
		//get pos of end of string  ( ',' delimits end of string -- ';' delimits end of key )
			//COULD CHANGE "CleanKeyEntry" to run until ';' delimiter found
		const size_t end = key_entry.find_first_of(",;", adjusted_start);

		//size of substr
		const size_t count = end - adjusted_start;
		return key_entry.substr(adjusted_start, count);
	}
	/*****************************************************************************
	*						END:: tree Extraction
	******************************************************************************/

	
	std::string distanceMeasure::MrBayesDistanceCalculator::create_mrbayes_default_command_block_file(const std::string& relative_nxs_path) const
	{
		//create mrbayes batch block file
			//use default mrbayes parameters
		std::string mrbayes_block_file_path = SystemParameters::GetMrBayesBlockString();
		char mrbayes_block[400];
		sprintf(mrbayes_block, 
			"begin mrbayes;\n"
			"set autoclose = yes nowarn = yes;\n"
			"execute %s;\n"
			//"lset nst = 6 rates = gamma;\n"
			"mcmcp filename=%s;\n"
			"mcmc nchains = %d nruns = %d ngen = 10000 samplefreq = 10;\n"
			"end;",
			relative_nxs_path.c_str(),
			relative_nxs_path.c_str(),
			SystemParameters::GetMrBayesNChains(),
			SystemParameters::GetMrBayesNRuns()
			);

		//TODO:: SEPARATE INTO FUNCTION... done in multiple .cpp's
		std::string mrbayes_block_string;
		mrbayes_block_string.reserve(200);
		mrbayes_block_string.append(mrbayes_block);
		
		FILE* nexusFile = fopen(mrbayes_block_file_path.c_str(), "w");
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

	void distanceMeasure::MrBayesDistanceCalculator::GetMrBayesBatchCommand(char* buffer, const std::string batch_block_file_path) const
	{
		//WiNDOWS DEPENDENCE 
		//UNIX MRBayes Command -> input redirection
		//mb < batch.txt > log.txt & <-- (run task in background - do not wait)
		//NOTE:: call is relative to current_code (sln_folder/ForestTools/) execution
		//sprintf(buffer, SystemParameters::GetMrBayesCommandString().c_str(), batch_block_file_path.c_str());
		SystemParameters::GetMrBayesCommand(buffer, batch_block_file_path.c_str());
	}



	//internal calc specific function -- NOT NEEDED
	void distanceMeasure::MrBayesDistanceCalculator::write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount, const int quartet_count)
	{
		//by default DMC does not have method to write quartet matrix -- derived Internal CAlculators define...
	}
	float distanceMeasure::MrBayesDistanceCalculator::normalize(int differenceCount, int sequencesize) const
	{
		return 0.0f;
	}
	float distanceMeasure::MrBayesDistanceCalculator::calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const
	{
		return 0.0f;
	}

}


