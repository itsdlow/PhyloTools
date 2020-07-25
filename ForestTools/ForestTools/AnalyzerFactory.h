/******************************************************************************
DeAngelo Wilson
July 20 2020

						AnalyzerFactory
******************************************************************************/
#ifndef _AnalyzerFactory
#define _AnalyzerFactory


namespace distanceMeasure
{
	//forward declarations
	class DistanceMeasureCalculator;
	class BatchCalculatorsAnalyzer;
}

//used to create analyzer in batchCalculators
namespace phylo
{
	//forward declarations
	class TreeAnalyzer;

	//singleton class holds all meta data on calculators... (commands, extensions, index, calculator return (Create) )
	class AnalyzerFactory
	{
	private:
		AnalyzerFactory() = default;

		static AnalyzerFactory& Instance()
		{
			if (!pInstance)
			{
				pInstance = new AnalyzerFactory();
			}
			return *pInstance;
		}
		static AnalyzerFactory* pInstance;
	public:
		~AnalyzerFactory();
		AnalyzerFactory(const AnalyzerFactory&) = delete;
		AnalyzerFactory& operator=(const AnalyzerFactory&) = delete;

		static void Initialize();
		static void Terminate();

		static distanceMeasure::BatchCalculatorsAnalyzer* Create(distanceMeasure::DistanceMeasureCalculator** calculators, const int calculator_count, bool generate_analysis, bool generate_quartets) { return AnalyzerFactory::Instance().privCreate(calculators, calculator_count, generate_analysis, generate_quartets); };

	private:
		distanceMeasure::BatchCalculatorsAnalyzer* privCreate(distanceMeasure::DistanceMeasureCalculator** calculators, const int calculator_count, bool generate_analysis, bool generate_quartets) const;
		TreeAnalyzer* privCreateAnalyzerStrategy(bool generate_quartets, distanceMeasure::DistanceMeasureCalculator** calculators, const int calculator_count) const;
	};

}

#endif

