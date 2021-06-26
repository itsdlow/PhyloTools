/******************************************************************************
DeAngelo Wilson
July 20 2020

						CalculatorType (strategy -- base)
******************************************************************************/
#ifndef _CalculatorType
#define _CalculatorType

#include <string>

//NOTE:: could be written by perl program, based on meta data... (: Calculator names, line by line [index])

//used to create calcualtor in batchCalculators + ForestTools (main)...
//ALTERNATIVE:: create calculator type class??? ----> put in SystemParams
	//has enum type property
	//===> convert ENUM -> ID (int) || name (string)
namespace distanceMeasure
{
	//forward declarations
	class DistanceMeasureCalculator;
	struct RunFlags;
	
	//enum class CalculatorTypes
	//{
	//	LCS = 0,
	//	PVALUE,
	//	MRBAYES,
	//	NCD,

	//};
	class CalculatorType
	{
	public:
		enum class Type
		{
			DEFAULT,
			NCD
		};
		
	public:

		CalculatorType(std::string name, Type type = Type::DEFAULT);
		virtual ~CalculatorType() = default;
		CalculatorType(const CalculatorType&) = delete;
		CalculatorType& operator=(const CalculatorType&) = delete;

		virtual DistanceMeasureCalculator* visit(RunFlags* pFlags) = 0;
		unsigned int GetBitmask() const;// { return (this->index + 1) * (this->index + 1); };
		
		std::string name;
		int index;
		Type type;
		//unsigned int bitmask;
		//command string
	};

	/*	
	 * Derived CalculatorTypes -----------
	 */
	class BatchCalculatorType : public CalculatorType
	{
	private:
		BatchCalculatorType();

		static BatchCalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new BatchCalculatorType();
			}
			return *pInstance;
		}
		static BatchCalculatorType* pInstance;
	public:
		//LcsCalculatorType(std::string name, int index);
		virtual ~BatchCalculatorType() = default;
		BatchCalculatorType(const BatchCalculatorType&) = delete;
		BatchCalculatorType& operator=(const BatchCalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { BatchCalculatorType::Instance(); };
		static void Terminate() { delete BatchCalculatorType::pInstance; };

	};
	
	class LcsCalculatorType: public CalculatorType
	{
	private:
		LcsCalculatorType();

		static LcsCalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new LcsCalculatorType();
			}
			return *pInstance;
		}
		static LcsCalculatorType* pInstance;
	public:
		//LcsCalculatorType(std::string name, int index);
		virtual ~LcsCalculatorType() = default;
		LcsCalculatorType(const LcsCalculatorType&) = delete;
		LcsCalculatorType& operator=(const LcsCalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { LcsCalculatorType::Instance(); };
		static void Terminate() { delete LcsCalculatorType::pInstance; };
	};
	
	class PValueCalculatorType : public CalculatorType
	{
	private:
		PValueCalculatorType();

		static PValueCalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new PValueCalculatorType();
			}
			return *pInstance;
		}
		static PValueCalculatorType* pInstance;
	public:
		//PValueCalculatorType(std::string name, int index);
		virtual ~PValueCalculatorType() = default;
		PValueCalculatorType(const PValueCalculatorType&) = delete;
		PValueCalculatorType& operator=(const PValueCalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { PValueCalculatorType::Instance(); };
		static void Terminate() { delete PValueCalculatorType::pInstance; };

	};
	
	class MrBayesCalculatorType : public CalculatorType
	{
	private:
		MrBayesCalculatorType();

		static MrBayesCalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new MrBayesCalculatorType();
			}
			return *pInstance;
		}
		static MrBayesCalculatorType* pInstance;
	public:
		virtual ~MrBayesCalculatorType() = default;
		MrBayesCalculatorType(const MrBayesCalculatorType&) = delete;
		MrBayesCalculatorType& operator=(const MrBayesCalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { MrBayesCalculatorType::Instance(); };
		static void Terminate() { delete MrBayesCalculatorType::pInstance; };

	};

	//abstarct interface class for Ncd calculator types
	class NcdCalculatorType: public CalculatorType
	{
	public:
		NcdCalculatorType(std::string name, std::string extension, std::string compress_command_format_string, bool inout);
		virtual ~NcdCalculatorType() = default;
		NcdCalculatorType(const NcdCalculatorType&) = delete;
		NcdCalculatorType& operator=(const NcdCalculatorType&) = delete;

		std::string extension;
		std::string compress_command_format_string;
		//TRUE:: in-out ---- FALSE:: out-in
		bool command_in_out_order;
	};
	
	class Ncd_7ZipCalculatorType : public NcdCalculatorType
	{
	private:
		Ncd_7ZipCalculatorType();

		static Ncd_7ZipCalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new Ncd_7ZipCalculatorType();
			}
			return *pInstance;
		}
		static Ncd_7ZipCalculatorType* pInstance;
	public:
		virtual ~Ncd_7ZipCalculatorType() = default;
		Ncd_7ZipCalculatorType(const Ncd_7ZipCalculatorType&) = delete;
		Ncd_7ZipCalculatorType& operator=(const Ncd_7ZipCalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { Ncd_7ZipCalculatorType::Instance(); };
		static void Terminate() { delete Ncd_7ZipCalculatorType::pInstance; };

	};

	class Ncd_MfcCalculatorType : public NcdCalculatorType
	{
	private:
		Ncd_MfcCalculatorType();

		static Ncd_MfcCalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new Ncd_MfcCalculatorType();
			}
			return *pInstance;
		}
		static Ncd_MfcCalculatorType* pInstance;

		std::string formatName;
	public:
		virtual ~Ncd_MfcCalculatorType() = default;
		Ncd_MfcCalculatorType(const Ncd_MfcCalculatorType&) = delete;
		Ncd_MfcCalculatorType& operator=(const Ncd_MfcCalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { Ncd_MfcCalculatorType::Instance(); };
		static void Terminate() { delete Ncd_MfcCalculatorType::pInstance; };

	};
	class Ncd_Mfc1CalculatorType : public NcdCalculatorType
	{
	private:
		Ncd_Mfc1CalculatorType();

		static Ncd_Mfc1CalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new Ncd_Mfc1CalculatorType();
			}
			return *pInstance;
		}
		static Ncd_Mfc1CalculatorType* pInstance;
	public:
		virtual ~Ncd_Mfc1CalculatorType() = default;
		Ncd_Mfc1CalculatorType(const Ncd_Mfc1CalculatorType&) = delete;
		Ncd_Mfc1CalculatorType& operator=(const Ncd_Mfc1CalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { Ncd_Mfc1CalculatorType::Instance(); };
		static void Terminate() { delete Ncd_Mfc1CalculatorType::pInstance; };

	};
	class Ncd_Mfc2CalculatorType : public NcdCalculatorType
	{
	private:
		Ncd_Mfc2CalculatorType();

		static Ncd_Mfc2CalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new Ncd_Mfc2CalculatorType();
			}
			return *pInstance;
		}
		static Ncd_Mfc2CalculatorType* pInstance;
	public:
		virtual ~Ncd_Mfc2CalculatorType() = default;
		Ncd_Mfc2CalculatorType(const Ncd_Mfc2CalculatorType&) = delete;
		Ncd_Mfc2CalculatorType& operator=(const Ncd_Mfc2CalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { Ncd_Mfc2CalculatorType::Instance(); };
		static void Terminate() { delete Ncd_Mfc2CalculatorType::pInstance; };

	};


	class Ncd_Geco1CalculatorType : public NcdCalculatorType
	{
	private:
		Ncd_Geco1CalculatorType();

		static Ncd_Geco1CalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new Ncd_Geco1CalculatorType();
			}
			return *pInstance;
		}
		static Ncd_Geco1CalculatorType* pInstance;
	public:
		virtual ~Ncd_Geco1CalculatorType() = default;
		Ncd_Geco1CalculatorType(const Ncd_Geco1CalculatorType&) = delete;
		Ncd_Geco1CalculatorType& operator=(const Ncd_Geco1CalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { Ncd_Geco1CalculatorType::Instance(); };
		static void Terminate() { delete Ncd_Geco1CalculatorType::pInstance; };

	};

	class Ncd_PpmzCalculatorType : public NcdCalculatorType
	{
	private:
		Ncd_PpmzCalculatorType();

		static Ncd_PpmzCalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new Ncd_PpmzCalculatorType();
			}
			return *pInstance;
		}
		static Ncd_PpmzCalculatorType* pInstance;
	public:
		virtual ~Ncd_PpmzCalculatorType() = default;
		Ncd_PpmzCalculatorType(const Ncd_PpmzCalculatorType&) = delete;
		Ncd_PpmzCalculatorType& operator=(const Ncd_PpmzCalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { Ncd_PpmzCalculatorType::Instance(); };
		static void Terminate() { delete Ncd_PpmzCalculatorType::pInstance; };

	};


	class Ncd_Bzip2CalculatorType : public NcdCalculatorType
	{
	private:
		Ncd_Bzip2CalculatorType();

		static Ncd_Bzip2CalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new Ncd_Bzip2CalculatorType();
			}
			return *pInstance;
		}
		static Ncd_Bzip2CalculatorType* pInstance;
	public:
		virtual ~Ncd_Bzip2CalculatorType() = default;
		Ncd_Bzip2CalculatorType(const Ncd_Bzip2CalculatorType&) = delete;
		Ncd_Bzip2CalculatorType& operator=(const Ncd_Bzip2CalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { Ncd_Bzip2CalculatorType::Instance(); };
		static void Terminate() { delete Ncd_Bzip2CalculatorType::pInstance; };

	};


	
	class CompareTreeCalculatorType : public CalculatorType
	{
	private:
		CompareTreeCalculatorType();

		static CompareTreeCalculatorType& Instance()
		{
			if (!pInstance)
			{
				pInstance = new CompareTreeCalculatorType();
			}
			return *pInstance;
		}
		static CompareTreeCalculatorType* pInstance;
	public:
		virtual ~CompareTreeCalculatorType() override = default;
		CompareTreeCalculatorType(const CompareTreeCalculatorType&) = delete;
		CompareTreeCalculatorType& operator=(const CompareTreeCalculatorType&) = delete;

		DistanceMeasureCalculator* visit(RunFlags* pFlags) override;
		static void Initialize() { CompareTreeCalculatorType::Instance(); };
		static void Terminate() { delete CompareTreeCalculatorType::pInstance; };

	};

}

#endif

