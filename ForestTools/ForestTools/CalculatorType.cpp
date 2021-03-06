/******************************************************************************
DeAngelo Wilson
July 20 2020

						CalculatorType (strategy -- base)
******************************************************************************/

#include "CalculatorType.h"

#include "CalculatorsWrapper.h"
#include "SystemParameters.h"
#include "CalculatorFactory.h"

//#include <math.h>


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
	
	Ncd_MfcCalculatorType* Ncd_MfcCalculatorType::pInstance = nullptr;

	Ncd_GecoCalculatorType* Ncd_GecoCalculatorType::pInstance = nullptr;

	Ncd_PpmzCalculatorType* Ncd_PpmzCalculatorType::pInstance = nullptr;
	Ncd_Bzip2CalculatorType* Ncd_Bzip2CalculatorType::pInstance = nullptr;

	
	CompareTreeCalculatorType* CompareTreeCalculatorType::pInstance = nullptr;

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

distanceMeasure::CalculatorType::CalculatorType(std::string name, Type type)
:name(std::move(name)), index(CalculatorFactory::GetCalculatorTypeCount()), type(type)
{
	//push to type array...
	CalculatorFactory::PushCalculatorType(this);
};
distanceMeasure::NcdCalculatorType::NcdCalculatorType(std::string name, std::string extension, std::string compress_command_format_string, bool inout)
	:CalculatorType(std::move(name), Type::NCD),
extension(std::move(extension)),
compress_command_format_string(std::move(compress_command_format_string)),
command_in_out_order(inout)
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
NcdCalculatorType("7Zip", "7z", SystemParameters::Get7ZipCommandString(), false)
{
}
distanceMeasure::Ncd_MfcCalculatorType::Ncd_MfcCalculatorType() :
	NcdCalculatorType("Mfc", "mfc%d", SystemParameters::GetMFCompressCommandString(), false),
	formatName("Mfc%d")
{
}

distanceMeasure::Ncd_GecoCalculatorType::Ncd_GecoCalculatorType() :
	NcdCalculatorType("Geco", "co", SystemParameters::GetGecoCommandString(), false)
{
}

distanceMeasure::Ncd_PpmzCalculatorType::Ncd_PpmzCalculatorType() :
	NcdCalculatorType("Ppmz", "ppmz%d", SystemParameters::GetPpmzCommandString(), true),
	formatName("Ppmz%d")
{
}
distanceMeasure::Ncd_Bzip2CalculatorType::Ncd_Bzip2CalculatorType() :
	NcdCalculatorType("Bzip2", "bz", SystemParameters::GetBzip2CommandString(), true)
{
}


distanceMeasure::CompareTreeCalculatorType::CompareTreeCalculatorType() :
	CalculatorType("CompareTree")
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

distanceMeasure::DistanceMeasureCalculator* distanceMeasure::Ncd_MfcCalculatorType::visit(RunFlags* pFlags)
{
	return new Ncd_MfcDistanceCalculator(pFlags, this->formatName, this->extension, this->compress_command_format_string);
}

distanceMeasure::DistanceMeasureCalculator* distanceMeasure::Ncd_GecoCalculatorType::visit(RunFlags* pFlags)
{
	//TODO:: create Geco derived NCD calc
	return new NcdDistanceCalculator(pFlags, this->name, this->extension, this->compress_command_format_string);
}

distanceMeasure::DistanceMeasureCalculator* distanceMeasure::Ncd_PpmzCalculatorType::visit(RunFlags* pFlags)
{
	return new Ncd_PpmzDistanceCalculator(pFlags, this->formatName, this->extension, this->compress_command_format_string);
}

distanceMeasure::DistanceMeasureCalculator* distanceMeasure::Ncd_Bzip2CalculatorType::visit(RunFlags* pFlags)
{
	return new NcdDistanceCalculator(pFlags, this->name, this->extension, this->compress_command_format_string);
}


distanceMeasure::DistanceMeasureCalculator* distanceMeasure::CompareTreeCalculatorType::visit(RunFlags* pFlags)
{
	return new CompareTreeCalculator(pFlags, this->name);
}


