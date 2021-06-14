/******************************************************************************
DeAngelo Wilson
July 20 2020

						Calculator Factory (singleton)
******************************************************************************/

#include "AnalyzerFactory.h"

#include "BatchCalculatorsAnalyzer.h"
#include "NullBatchCalculatorsAnalyzer.h"
#include "AnalyzerType.h"

namespace phylo
{
	AnalyzerFactory* AnalyzerFactory::pInstance = nullptr;


	void phylo::AnalyzerFactory::Initialize()
	{
		AnalyzerFactory::Instance();

		//INITIALIZE ALL ANALYZER's type
			//NOTE:: using singleton types as singletons ==> statically access (->no need for initialization, before use)
	}

	void phylo::AnalyzerFactory::Terminate()
	{
		delete AnalyzerFactory::pInstance;
	}

	phylo::AnalyzerFactory::~AnalyzerFactory()
	{
		//reap all calc type resources...
		CompleteAnalyzerType::Terminate();
		LargeTreeAnalyzerType::Terminate();
	}
	
	TreeAnalyzer* phylo::AnalyzerFactory::privCreateAnalyzerStrategy(bool generate_quartets, distanceMeasure::DistanceMeasureCalculator** calculators, const int calculator_count) const
	{
		TreeAnalyzer* pAnalyzer = nullptr;
		switch(generate_quartets)
		{
		case true:
			pAnalyzer = CompleteAnalyzerType::Visit(calculators, calculator_count);
			break;
		case false:
			pAnalyzer = LargeTreeAnalyzerType::Visit(calculators, calculator_count);
			break;
		}
		
		return pAnalyzer;
	}

	distanceMeasure::BatchCalculatorsAnalyzer* phylo::AnalyzerFactory::privCreate(distanceMeasure::DistanceMeasureCalculator** calculators, const int calculator_count, bool generate_analysis, bool generate_quartets) const
	{
		distanceMeasure::BatchCalculatorsAnalyzer* pBatchAnalyzer = nullptr;
		
		//no analysis generation...
		if(generate_analysis)
		{
			pBatchAnalyzer = new distanceMeasure::BatchCalculatorsAnalyzer(calculator_count, this->privCreateAnalyzerStrategy(generate_quartets, calculators, calculator_count));
		}
		else
		{
			pBatchAnalyzer = new distanceMeasure::NullBatchCalculatorsAnalyzer(calculator_count, nullptr);
		}

		return pBatchAnalyzer;
	}
}
