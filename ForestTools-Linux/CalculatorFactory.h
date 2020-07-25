/******************************************************************************
DeAngelo Wilson
July 20 2020

						CalculatorFactory
******************************************************************************/
#ifndef _CalculatorFactory
#define _CalculatorFactory


#include <string>
#include <vector>
//
//#include "SystemParameters.h"
//#include "CalculatorType.h"

//used to create calcualtor in batchCalculators + ForestTools (main)...
namespace distanceMeasure
{
	//forward declarations
	class DistanceMeasureCalculator;
	class CalculatorType;
	struct RunFlags;

	//singleton class holds all meta data on calculators... (commands, extensions, index, calculator return (Create) )
	class CalculatorFactory
	{
	private:
		//static const int calculator_type_count = 5;

		//array of calc types
		//CalculatorType* calculator_types[calculator_type_count];
		std::vector<CalculatorType*> calculator_types;
		
		CalculatorFactory() = default;

		static CalculatorFactory& Instance()
		{
			if (!pInstance)
			{
				pInstance = new CalculatorFactory();
			}
			return *pInstance;
		}
		static CalculatorFactory* pInstance;
	public:
		~CalculatorFactory();
		CalculatorFactory(const CalculatorFactory&) = delete;
		CalculatorFactory& operator=(const CalculatorFactory&) = delete;
		
		static void Initialize();
		static void Terminate();
		
		//static int GetCalculatorTypeCount() { return CalculatorFactory::calculator_type_count; };
		static int GetCalculatorTypeCount() { return static_cast<int>(CalculatorFactory::Instance().calculator_types.size()); };
		static void PushCalculatorType(CalculatorType* pType);
		
		static DistanceMeasureCalculator* Create(int index, RunFlags* pFlags) { return CalculatorFactory::Instance().privCreate(index, pFlags); };

		static CalculatorType* GetCalculatorType(int id) { return CalculatorFactory::Instance().privGetCalculatorType(id); };
		static CalculatorType* GetCalculatorType(const std::string& name) { return CalculatorFactory::Instance().privGetCalculatorType(name); };

		static unsigned int GetAllCalculatorsBitmask() { return CalculatorFactory::Instance().privGetAllCalculatorsBitmask(); };
		static unsigned int GetCalculatorBitmask(int id) { return CalculatorFactory::Instance().privGetCalculatorBitmask(id); };
		//returns batch calc index || -1 --> 'index' treated as OUT var
		static void GetBatchCalculatorIndex(int& index);

		
		static std::string Dump();

	private:
		DistanceMeasureCalculator* privCreate(int index, RunFlags* pFlags) const;
		CalculatorType* privGetCalculatorType(const std::string& name) const;
		CalculatorType* privGetCalculatorType(int id)  const;
		unsigned int privGetCalculatorBitmask(int id) const;
		unsigned int privGetAllCalculatorsBitmask() const;

	};
	//operator int() const
	//{
	//	return 0;
	//};

	
}

#endif

