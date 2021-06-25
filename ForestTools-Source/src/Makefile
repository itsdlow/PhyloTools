#
# makefile for PhyloTools
# ---> TODO:: CREATE A PERL SCRIPT TO CREATE MAKEFILE -- includes and sources...
# 

# Requires G++/GCC version 4.8 or later [C++ 11 <chrono> used]

CC = g++
CFLAGS = -std=c++11 -g -Wall

PROGRAM = ../phylotools
phylotools_SOURCES = \
	ForestTools.cpp																\
	Aligned_InternalDMCalculator.cpp 		Aligned_InternalDMCalculator.h		\
	AlignedDistanceMeasureCalculator.cpp	AlignedDistanceMeasureCalculator.h	\
	BatchCalculatorsAnalyzer.cpp			BatchCalculatorsAnalyzer.h			\
	BatchDistanceCalculators.cpp			BatchDistanceCalculators.h			\
	CalculatorAligner.cpp					CalculatorAligner.h					\
	CalculatorFastaCompressor.cpp			CalculatorFastaCompressor.h			\
	CalculatorFastaFormatter.cpp			CalculatorFastaFormatter.h 			\
	CalculatorNexusFormatter.cpp			CalculatorNexusFormatter.h			\
	DistanceMatrixObject.cpp				DistanceMatrixObject.h				\
	DistanceMeasureCalculator.cpp			DistanceMeasureCalculator.h			\
	FileObject.cpp							FileObject.h						\
	FileObjectManager.cpp					FileObjectManager.h					\
	InternalCalculatorTools.cpp				InternalCalculatorTools.h			\
	LcsDistanceCalculator.cpp				LcsDistanceCalculator.h				\
	MrBayesDistanceCalculator.cpp			MrBayesDistanceCalculator.h			\
	NcdDistanceCalculator.cpp				NcdDistanceCalculator.h				\
	PhyloAnalysis.cpp						PhyloAnalysis.h						\
	PhyloTools.cpp							PhyloTools.h						\
	PValueDistanceCalculator.cpp			PValueDistanceCalculator.h			\
	SequenceFileProcessor.cpp				SequenceFileProcessor.h				\
	SequenceListsGenerator.cpp				SequenceListsGenerator.cpp			\
											SequenceProcessor.h					\
	SystemParameters.cpp					SystemParameters.h					\
	Tree.cpp								Tree.h								\
	Unaligned_InternalDMCalculator.cpp		Unaligned_InternalDMCalculator.h	\
											VectorKeyCompare.h					\
											RunFlags.h							\
											SequenceNamesStrategy.h				\
	SequenceNamesIDStrategy.cpp				SequenceNamesIDStrategy.h			\
	SequenceNamesDescriptionStrategy.cpp	SequenceNamesDescriptionStrategy.h	\
	SequenceNamesOrderedCustomStrategy.cpp	SequenceNamesOrderedCustomStrategy.h\
	SequenceNamesUnorderedFastaStrategy.cpp	SequenceNamesUnorderedFastaStrategy.h\
											CalculatorsWrapper.h				\
	TreeAnalyzer.cpp 						TreeAnalyzer.h						\
	CalculatorFactory.cpp					CalculatorFactory.h					\
	CalculatorType.cpp						CalculatorType.h					\
	AnalyzerFactory.cpp						AnalyzerFactory.h					\
	AnalyzerType.cpp						AnalyzerType.h						\
	CompleteTreeAnalyzer.cpp 				CompleteTreeAnalyzer.h				\
	LargeTreeAnalyzer.cpp 					LargeTreeAnalyzer.h					\
	NullBatchCalculatorsAnalyzer.cpp 		NullBatchCalculatorsAnalyzer.h		\
	DynamicSizedArray.cpp 					DynamicSizedArray.h					\
	ForestPlug.cpp 							ForestPlug.h						\
	MrBayesDataType.cpp 					MrBayesDataType.h					\
											InputSequenceFileSet.h				\
	SequenceNamesNexusStrategy.cpp  		SequenceNamesNexusStrategy.h		\
	NexusFormatProcessor.cpp 				NexusFormatProcessor.h				\
	CompareTreeCalculator.cpp				CompareTreeCalculator.h				\
	NullDistanceMeasureCalculator.cpp		NullDistanceMeasureCalculator.h 	

INCLUDES = \
	ForestTools.cpp							\
	Aligned_InternalDMCalculator.cpp 		\
	AlignedDistanceMeasureCalculator.cpp	\
	BatchCalculatorsAnalyzer.cpp			\
	BatchDistanceCalculators.cpp			\
	CalculatorAligner.cpp					\
	CalculatorFastaCompressor.cpp			\
	CalculatorFastaFormatter.cpp			\
	CalculatorNexusFormatter.cpp			\
	DistanceMatrixObject.cpp				\
	DistanceMeasureCalculator.cpp			\
	FileObject.cpp							\
	FileObjectManager.cpp					\
	InternalCalculatorTools.cpp				\
	LcsDistanceCalculator.cpp				\
	MrBayesDistanceCalculator.cpp			\
	NcdDistanceCalculator.cpp				\
	PhyloAnalysis.cpp						\
	PhyloTools.cpp							\
	PValueDistanceCalculator.cpp			\
	SequenceFileProcessor.cpp				\
	SequenceListsGenerator.cpp				\
	SystemParameters.cpp					\
	Tree.cpp								\
	Unaligned_InternalDMCalculator.cpp		\
	SequenceNamesIDStrategy.cpp				\
	SequenceNamesDescriptionStrategy.cpp	\
	SequenceNamesOrderedCustomStrategy.cpp	\
	SequenceNamesUnorderedFastaStrategy.cpp	\
	CalculatorFactory.cpp					\
	AnalyzerFactory.cpp						\
	AnalyzerType.cpp						\
	CalculatorType.cpp						\
	TreeAnalyzer.cpp 						\
	CompleteTreeAnalyzer.cpp 				\
	LargeTreeAnalyzer.cpp 					\
	NullBatchCalculatorsAnalyzer.cpp 		\
	DynamicSizedArray.cpp 					\
	ForestPlug.cpp 							\
	MrBayesDataType.cpp 					\
	SequenceNamesNexusStrategy.cpp  		\
	NexusFormatProcessor.cpp 				\
	CompareTreeCalculator.cpp				\
    NullDistanceMeasureCalculator.cpp

phylotools: $(phylotools_SOURCES)    
	$(CC) $(CFLAGS) -o $(PROGRAM) $(INCLUDES)
	
#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -f phylotools

