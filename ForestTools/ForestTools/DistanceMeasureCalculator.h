/******************************************************************************
DeAngelo Wilson
January 3 2020

						DistanceMeasureCalculator (strategy -- base)
******************************************************************************/
#ifndef _DistanceMeasureCalculator
#define _DistanceMeasureCalculator

//#include "FileObject.h"

#include <string>
#include <vector>

namespace distanceMeasure
{
	//forward declarations
	class FileObjectManager;
	class FileObject;
	
	class DistanceMeasureCalculator
	{
	public:
		//BIG 4
		DistanceMeasureCalculator() = default;
		DistanceMeasureCalculator(const DistanceMeasureCalculator&) = delete;
		DistanceMeasureCalculator& operator=(const DistanceMeasureCalculator&) = delete;
		virtual ~DistanceMeasureCalculator() = default;

		//main calculator driver func -- output matrix as text-file
		virtual void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id) = 0;
		//internal calc specific
		//virtual void create_tree(const std::vector<std::string>& sequence_set_names, const int batch_id) = 0;

		virtual std::string GetCalculatorName() const = 0;
	protected:
		//internal method?
		//const int getArrayIndex(int row, int col, int rowCount) const {return (row * rowCount) + col;}
		static int GetQuartetCombinations(int n);

		static void swap_space_with_underscores(std::string& description_string);

	private:


	};
}


#endif // !_DistanceMeasureCalculator
