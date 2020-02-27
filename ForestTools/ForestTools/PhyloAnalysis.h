/******************************************************************************
DeAngelo Wilson
January 3 2020

              PhyloAnalysis (namespace for phylogenetics analysis tools)
******************************************************************************/

#include <string>

namespace PhyloAnalysis
{
    //prints to output the agreement between two given files (of quartetTrees)
    float computeQuartetAgreement(std::string quartetFilename1, std::string quartetFilename2);

    //Given 2 largeTreeFiles, compute the agreement between quartets (directly induced from largeTree)
    float computeInducedQuartetAgreement(std::string largeTreeFilename1, std::string largeTreeFilename2);

    //prints to output the compatability ratio between a large sequence tree file and derived quartet trees on sequences
    float computeQuartetCompatibility(std::string largeTreeFilename ,std::string quartetFilename);

    //uses bipartition method to compute symmetric difference --
        //both arguments are largeTreeFileDirectories
    float computeSymmetricDifference(std::string largeTreeFilename1, std::string largeTreeFilename2);
}