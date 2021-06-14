/******************************************************************************
DeAngelo Wilson
July 20 2020

						Calculator Factory (singleton)
******************************************************************************/

#include "CalculatorFactory.h"
#include "CalculatorType.h"
#include <cassert>

#include "SystemParameters.h"

namespace distanceMeasure
{
	CalculatorFactory* CalculatorFactory::pInstance = nullptr;


	void distanceMeasure::CalculatorFactory::Initialize()
	{
		CalculatorFactory::Instance();

		//INITIALIZE ALL CALCULATORTYPE's ==> Pushes onto CalculatorFactory::calculator_types [vector]
		///determines calculator indexing order...
		BatchCalculatorType::Initialize();
		LcsCalculatorType::Initialize();
		PValueCalculatorType::Initialize();
		MrBayesCalculatorType::Initialize();
		Ncd_7ZipCalculatorType::Initialize();
		Ncd_Mfc1CalculatorType::Initialize();
		Ncd_Mfc2CalculatorType::Initialize();
		
		CompareTreeCalculatorType::Initialize();

		//NOTE:: hack -- should be moved to SystemParameters...
		if (SystemParameters::isOSWindows() == false)
		{
			Ncd_Bzip2CalculatorType::Initialize();
			Ncd_PpmzCalculatorType::Initialize();
		}
		
	}


	void distanceMeasure::CalculatorFactory::Terminate()
	{
		delete CalculatorFactory::pInstance;
	}
	distanceMeasure::CalculatorFactory::~CalculatorFactory()
	{
		//CLEAN ALL CALCULATORTYPE's from vector => no dangling pointers
		this->calculator_types.clear();
		
		//reap type resources
		BatchCalculatorType::Terminate();
		LcsCalculatorType::Terminate();
		PValueCalculatorType::Terminate();
		MrBayesCalculatorType::Terminate();
		Ncd_7ZipCalculatorType::Terminate();
		Ncd_Mfc1CalculatorType::Terminate();
		Ncd_Mfc2CalculatorType::Terminate();
		
		CompareTreeCalculatorType::Terminate();

		//NOTE:: hack -- should be moved to SystemParameters...
		if (SystemParameters::isOSWindows() == false)
		{
			Ncd_Bzip2CalculatorType::Terminate();
			Ncd_PpmzCalculatorType::Terminate();
		}
	}
	
	void distanceMeasure::CalculatorFactory::PushCalculatorType(CalculatorType* pType)
	{
		CalculatorFactory::Instance().calculator_types.push_back(pType);
	}

	unsigned int distanceMeasure::CalculatorFactory::privGetCalculatorBitmask(int id) const
	{
		return this->privGetCalculatorType(id)->GetBitmask();
	}
	unsigned int distanceMeasure::CalculatorFactory::privGetAllCalculatorsBitmask() const
	{
		const int calc_count = static_cast<int>(this->calculator_types.size());
		unsigned int mask = 0;
		for(int i = 0; i < calc_count; i++)
		{
			mask <<= 1;
			mask |= 1;
		}
		//Clear batch calculator bit -- recursively creates batch calcs...
		int batch_index;
		CalculatorFactory::GetBatchCalculatorIndex(batch_index);
		mask &= ~(1 << batch_index);
		
		return mask;
	}
	
	void distanceMeasure::CalculatorFactory::GetBatchCalculatorIndex(int& index)
	{
		CalculatorType* pType = nullptr;
		if( (pType = CalculatorFactory::GetCalculatorType("Batch")) )
		{
			index = pType->index;
		}
		else
		{
			index = -1;
		}
	}

	void distanceMeasure::CalculatorFactory::GetCompareTreeCalculatorIndex(int& index)
	{
		CalculatorType* pType = nullptr;
		if ((pType = CalculatorFactory::GetCalculatorType("CompareTree")))
		{
			index = pType->index;
		}
		else
		{
			index = -1;
		}
	}

	
	DistanceMeasureCalculator* distanceMeasure::CalculatorFactory::privCreate(int index, RunFlags* pFlags) const
	{
		DistanceMeasureCalculator* pDMC = nullptr;
		//check index within bounds
		if(index >= 0 && index < CalculatorFactory::GetCalculatorTypeCount())
		{
			//creates new DMC...
			pDMC = this->calculator_types[index]->visit(pFlags);
		}
		return pDMC;
	}


	CalculatorType* distanceMeasure::CalculatorFactory::privGetCalculatorType(int id) const
	{
		CalculatorType* pType = nullptr;
		for (auto it = this->calculator_types.begin(); it != this->calculator_types.end(); it++)
		{
			//calculator type
			if ((*it)->index == id)
			{
				pType = *it;
				break;
			}
		}
		assert(pType);
		return pType;
	}
	NcdCalculatorType* distanceMeasure::CalculatorFactory::privGetNcdCalculatorType(const std::string& ext) const
	{
		NcdCalculatorType* pType = nullptr;
		
		for (auto it = this->calculator_types.begin(); it != this->calculator_types.end(); it++)
		{
			//calculator type
			if ((*it)->type == CalculatorType::Type::NCD)
			{
				NcdCalculatorType* pTmp = static_cast<NcdCalculatorType*>(*it);
				if(pTmp->extension == ext)
				{
					pType = pTmp;
					break;
				}

			}
		}
		assert(pType);
		return pType;
	}
	
	CalculatorType* distanceMeasure::CalculatorFactory::privGetCalculatorType(const std::string& name) const
	{
		CalculatorType* pType = nullptr;
		
		for (auto it = this->calculator_types.begin(); it != this->calculator_types.end(); it++)
		{
			//calculator type
			if ((*it)->name == name)
			{
				pType = *it;
				break;
			}
		}
		assert(pType);
		return pType;
	}



	std::string distanceMeasure::CalculatorFactory::Dump()
	{
		const int max_loop_index = CalculatorFactory::GetCalculatorTypeCount() - 1;
		std::string ret;
		for(int i = 0; i < max_loop_index; i++)
		{
			CalculatorType* calcType = CalculatorFactory::GetCalculatorType(i);
			const std::string a = calcType->name + " (" + std::to_string(calcType->index) + "), ";
			ret.append(a);
		}
		//add last type...
		CalculatorType* calcType = CalculatorFactory::GetCalculatorType(max_loop_index);
		const std::string a = calcType->name + " (" + std::to_string(calcType->index) + ")";
		ret.append(a);

		return ret;
	}

	//HACK... compare tree should not be shown unless batch calculation
	std::string distanceMeasure::CalculatorFactory::Dump_NonBatch()
	{
		//HACK:: TODO:: create array of illegal calcs...
		//Get illegal calculator (CompareTree) --> can only be instantiated for BatchCalculation
		int illegal_index;
		CalculatorFactory::GetCompareTreeCalculatorIndex(illegal_index);
		
		
		const int max_loop_index = CalculatorFactory::GetCalculatorTypeCount();
		std::string ret;
		for (int i = 0; i < max_loop_index; i++)
		{
			if(i != illegal_index)
			{
				CalculatorType* calcType = CalculatorFactory::GetCalculatorType(i);
				const std::string a = calcType->name + " (" + std::to_string(calcType->index) + "), ";
				ret.append(a);
			}
		}
		assert(ret.size() > 2);
		ret.pop_back();
		ret.pop_back();
		
		return ret;
	}
}
