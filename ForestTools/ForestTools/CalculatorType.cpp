/******************************************************************************
DeAngelo Wilson
July 20 2020

						CalculatorType (strategy -- base)
******************************************************************************/

#include "CalculatorType.h"

#include "CalculatorsWrapper.h"
#include "SystemParameters.h"
#include "CalculatorFactory.h"

#include <math.h>

//ADDING NEW CALCULATOR STEPS:
//#1 - create calc type -- Constructor and Visit
//#2 - Create singleton instance initialization
//#3 - Add to Calculator Factory -- Initialization and Termination list 


//SINGLETON INSTANCE INITIALIZATIONS
namespace distanceMeasure
{
	BatchCalculatorType* BatchCalculatorType::pInstance = nullptr;
	LcsCalculatorType* LcsCalculatorType::pInstance = nullptr;
	PValueCalculatorType* PValueCalculatorType::pInstance = nullptr;
	MrBayesCalculatorType* MrBayesCalculatorType::pInstance = nullptr;
	Ncd_7ZipCalculatorType* Ncd_7ZipCalculatorType::pInstance = nullptr;
	Ncd_Mfc1CalculatorType* Ncd_Mfc1CalculatorType::pInstance = nullptr;
	Ncd_Mfc2CalculatorType* Ncd_Mfc2CalculatorType::pInstance = nullptr;

	Ncd_Geco1CalculatorType* Ncd_Geco1CalculatorType::pInstance = nullptr;

}

unsigned int distanceMeasure::CalculatorType::GetBitmask() const
{
	//index 0 calc --> 1 bit mask
	if(this->index == 0)
	{
		return 1;
	}
	return static_cast<unsigned int>(pow(2, this->index));
}

distanceMeasure::CalculatorType::CalculatorType(std::string name)
:name(std::move(name)), index(CalculatorFactory::GetCalculatorTypeCount())
{
	//push to type array...
	CalculatorFactory::PushCalculatorType(this);
};
distanceMeasure::NcdCalculatorType::NcdCalculatorType(std::string name, std::string extension, std::string compress_command_format_string)
	:CalculatorType(std::move(name)),
extension(std::move(extension)),
compress_command_format_string(std::move(compress_command_format_string))
{};


/*
 * CALCULATOR TYPE constructors...
 */
distanceMeasure::BatchCalculatorType::BatchCalculatorType() :
	CalculatorType("Batch")
{
}

distanceMeasure::LcsCalculatorType::LcsCalculatorType():
CalculatorType("LCS")
{
}

distanceMeasure::PValueCalculatorType::PValueCalculatorType() :
CalculatorType("PValue")
{
}

distanceMeasure::MrBayesCalculatorType::MrBayesCalculatorType() :
CalculatorType("MrBayes")
{
}

distanceMeasure::Ncd_7ZipCalculatorType::Ncd_7ZipCalculatorType():
NcdCalculatorType("7Zip", "7z", SystemParameters::Get7ZipCommandString())
{
}
distanceMeasure::Ncd_Mfc1CalculatorType::Ncd_Mfc1CalculatorType() :
NcdCalculatorType("Mfc1", "mfc1", SystemParameters::GetMFC1CommandString())
{
}
distanceMeasure::Ncd_Mfc2CalculatorType::Ncd_Mfc2CalculatorType() :
	NcdCalculatorType("Mfc2", "mfc2", SystemParameters::GetMFC2CommandString())
{
}
distanceMeasure::Ncd_Geco1CalculatorType::Ncd_Geco1CalculatorType() :
	NcdCalculatorType("Geco", "co", SystemParameters::GetMFC2CommandString())
{
}

/*
 * CALCULATOR TYPE visitors...
 */
distanceMeasure::DistanceMeasureCalculator* distanceMeasure::BatchCalculatorType::visit(RunFlags* pFlags)
{
	return new BatchDistanceCalculators(pFlags, this->name);
}
distanceMeasure::DistanceMeasureCalculator* distanceMeasure::LcsCalculatorType::visit(RunFlags* pFlags)
{
	//initialize things on calc here?
	return new LcsDistanceCalculator(pFlags, this->name);
}
distanceMeasure::DistanceMeasureCalculator* distanceMeasure::PValueCalculatorType::visit(RunFlags* pFlags)
{
	return new PValueDistanceCalculator(pFlags, this->name);
}
distanceMeasure::DistanceMeasureCalculator* distanceMeasure::MrBayesCalculatorType::visit(RunFlags* pFlags)
{
	return new MrBayesDistanceCalculator(pFlags, this->name);
}

distanceMeasure::DistanceMeasureCalculator* distanceMeasure::Ncd_7ZipCalculatorType::visit(RunFlags* pFlags)
{
	return new NcdDistanceCalculator(pFlags, this->name, this->extension, this->compress_command_format_string);
}
distanceMeasure::DistanceMeasureCalculator* distanceMeasure::Ncd_Mfc1CalculatorType::visit(RunFlags* pFlags)
{
	return new NcdDistanceCalculator(pFlags, this->name, this->extension, this->compress_command_format_string);
}
distanceMeasure::DistanceMeasureCalculator* distanceMeasure::Ncd_Mfc2CalculatorType::visit(RunFlags* pFlags)
{
	return new NcdDistanceCalculator(pFlags, this->name, this->extension, this->compress_command_format_string);
}

distanceMeasure::DistanceMeasureCalculator* distanceMeasure::Ncd_Geco1CalculatorType::visit(RunFlags* pFlags)
{
	return new NcdDistanceCalculator(pFlags, this->name, this->extension, this->compress_command_format_string);
}


