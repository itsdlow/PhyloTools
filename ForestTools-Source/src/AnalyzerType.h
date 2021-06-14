/******************************************************************************
DeAngelo Wilson
July 20 2020

						AnalyzerType (strategy -- base)
******************************************************************************/
#ifndef _AnalyzerType
#define _AnalyzerType

#include <string>

//NOTE:: could be written by perl program, based on meta data... (: Calculator names, line by line [index])
namespace distanceMeasure
{
	class DistanceMeasureCalculator;
}

namespace phylo
{
	class TreeAnalyzer;

	class AnalyzerType
	{
	public:

		AnalyzerType() = default;
		virtual ~AnalyzerType() = default;
		AnalyzerType(const AnalyzerType&) = delete;
		AnalyzerType& operator=(const AnalyzerType&) = delete;

	protected:
		virtual TreeAnalyzer* visit(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count) = 0;
	};

	/*
	 * Derived AnalyzerTypes -----------
	 */
	
	//All analysis methods (Quartets + RF measures...)
	class CompleteAnalyzerType : public AnalyzerType
	{
	private:
		CompleteAnalyzerType() = default;

		static CompleteAnalyzerType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new CompleteAnalyzerType();
			}
			return *pInstance;
		}
		static CompleteAnalyzerType* pInstance;
	public:
		//LcsAnalyzerType(std::string name, int index);
		virtual ~CompleteAnalyzerType() = default;
		CompleteAnalyzerType(const CompleteAnalyzerType&) = delete;
		CompleteAnalyzerType& operator=(const CompleteAnalyzerType&) = delete;

		static void Initialize() { CompleteAnalyzerType::Instance(); };
		static void Terminate() { delete CompleteAnalyzerType::pInstance; };

		//HACKS -- could do same as calculatorType --> push to Factory vector on constructions
		static TreeAnalyzer* Visit(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count) { return CompleteAnalyzerType::Instance().visit(pCalculators, calculator_count); };
		
	protected:
		TreeAnalyzer* visit(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count) override;
	};

	//All analysis methods (Quartets + RF measures...)
	class LargeTreeAnalyzerType : public AnalyzerType
	{
	private:
		LargeTreeAnalyzerType() = default;

		static LargeTreeAnalyzerType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new LargeTreeAnalyzerType();
			}
			return *pInstance;
		}
		static LargeTreeAnalyzerType* pInstance;
	public:
		//LcsAnalyzerType(std::string name, int index);
		virtual ~LargeTreeAnalyzerType() = default;
		LargeTreeAnalyzerType(const LargeTreeAnalyzerType&) = delete;
		LargeTreeAnalyzerType& operator=(const LargeTreeAnalyzerType&) = delete;

		static void Initialize() { LargeTreeAnalyzerType::Instance(); };
		static void Terminate() { delete LargeTreeAnalyzerType::pInstance; };

		//HACKS -- could do same as calculatorType --> push to Factory vector on constructions
		static TreeAnalyzer* Visit(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count) { return LargeTreeAnalyzerType::Instance().visit(pCalculators, calculator_count); };

	protected:
		TreeAnalyzer* visit(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count) override;
	};

	
}

#endif

