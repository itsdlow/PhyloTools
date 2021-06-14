/******************************************************************************
DeAngelo Wilson
July 20 2020

						AnalyzerType (strategy -- base)
******************************************************************************/

#include "AnalyzerType.h"

#include "CompleteTreeAnalyzer.h"
#include "LargeTreeAnalyzer.h"

//SINGLETON INSTANCE INITIALIZATIONS
namespace phylo
{
	CompleteAnalyzerType* CompleteAnalyzerType::pInstance = nullptr;
	LargeTreeAnalyzerType* LargeTreeAnalyzerType::pInstance = nullptr;


	/*
	 * ANALYZER TYPE visitors...
	 */
	TreeAnalyzer* CompleteAnalyzerType::visit(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count)
	{
		return new CompleteTreeAnalyzer(pCalculators, calculator_count);
	}

	TreeAnalyzer* phylo::LargeTreeAnalyzerType::visit(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count)
	{
		return new LargeTreeAnalyzer(pCalculators, calculator_count);
	}
}

