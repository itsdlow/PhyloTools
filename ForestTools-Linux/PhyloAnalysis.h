/******************************************************************************
DeAngelo Wilson
January 3 2020

              PhyloAnalysis (namespace for phylogenetics analysis tools)
******************************************************************************/

#include <string>

//TODO:: make class --> privately inherits PhyloTools
namespace PhyloAnalysis
{
    //prints to output the agreement between two given files (of quartetTrees)
    float computeQuartetAgreement(const std::string& quartetFilename1, const std::string& quartetFilename2);

    //Given 2 largeTreeFiles, compute the agreement between quartets (directly induced from largeTree)
    float computeInducedQuartetAgreement(const std::string& largeTreeFilename1, const std::string& largeTreeFilename2);

    //prints to output the compatability ratio between a large sequence tree file and derived quartet trees on sequences
    float computeQuartetCompatibility(const std::string& largeTreeFilename ,const std::string& quartetFilename);

    //uses bipartition method to compute symmetric difference --
        //both arguments are largeTreeFileDirectories
    float computeSymmetricDifference(const std::string& largeTreeFilename1, const std::string& largeTreeFilename2);

    //NOTE:: new analysis tools
    float computeRFRate(const float differenceCount, const int sequenceCount);

    float computeCompatibilityMetric_LtoL(const std::string& largeTreeFilename1, const std::string& largeTreeFilename2);
    float computeCompatibilityMetric_IQtoQ(const std::string& largeTreeFilename, const std::string& quartetsFilename);
    float computeCompatibilityMetric_IQtoIQ(const std::string& largeTreeFilename1, const std::string& largeTreeFilename2);
    float computeCompatibilityMetric_QtoQ(const std::string& quartetsFilename1, const std::string& quartetsFilename2);
	
}