/**************************************************************************************************************************
DeAngelo Wilson
February 26 2020

					TreeAnalyzer
**************************************************************************************************************************/

#include "TreeAnalyzer.h"

#include "DistanceMeasureCalculator.h"

namespace phylo
{
	phylo::TreeAnalyzer::TreeAnalyzer(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count):
	pCalculators(pCalculators),
	calculator_count(calculator_count),
	results()
	{
	}

		//STAY IN BATCHCALCULATOR ==> REQUIRES KNOWING SEQUENCE SET count...
	void TreeAnalyzer::WriteAnalysisTable(const std::vector<float>& table_vector, const std::string& table_description, const int ss_size_count)
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
				const float avg_val = table_vector.at(distanceMeasure::DistanceMeasureCalculator::getArrayIndex(i, j, this->calculator_count)) / static_cast<float>(ss_size_count);
				this->results.append(std::to_string(avg_val));
				this->results.append(" ");

			}
			this->results.append("\n");
		}
		this->results.append("\n\n");
	}

		//TODO -- clean up to allow for better standardizing of label sizes + offsets...
	std::string TreeAnalyzer::GetStandardizedCalculatorLabels() const
	{
		std::string label;
		//const int columnOffset = this->standard_label_size + 2 + 1;
		//AZUL_UNUSED_VAR(columnOffset);

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

	std::string TreeAnalyzer::StandardizeCalculatorLabel(const int index) const
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
