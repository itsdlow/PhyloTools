#
# Student makefile for Cache Lab
# 
CC = g++
CFLAGS = -g -Wall

PROGRAM = phylotools
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
	ExternalDistanceMeasureCalculator.cpp	ExternalDistanceMeasureCalculator.h	\
	FileObject.cpp							FileObject.h						\
	FileObjectManager.cpp					FileObjectManager.h					\
	InternalCalculatorTools.cpp				InternalCalculatorTools.h			\
	InternalDistanceMeasureCalculator.cpp	InternalDistanceMeasureCalculator.h	\
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
	ExternalDistanceMeasureCalculator.cpp	\
	FileObject.cpp							\
	FileObjectManager.cpp					\
	InternalCalculatorTools.cpp				\
	InternalDistanceMeasureCalculator.cpp	\
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
	SequenceNamesUnorderedFastaStrategy.cpp
	
phylotools: $(phylotools_SOURCES)    
	$(CC) $(CFLAGS) -o $(PROGRAM) $(INCLUDES)
	
#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -f phylotools

