/******************************************************************************
DeAngelo Wilson
January 3 2020

              PhyloAnalysis (namespace for phylogenetics analysis tools)
******************************************************************************/

#include <string>

namespace PhyloAnalysis
{
    //prints to output the agreement between two given files (of quartetTrees)
    void computeQuartetAgreement(std::string quartetFilename1, std::string quartetFilename2);

    //Given 2 largeTreeFiles, compute the agreement between quartets (directly induced from largeTree)
    void computeInducedQuartetAgreement(std::string largeTreeFilename1, std::string largeTreeFilename2);

    //prints to output the compatability ratio between a large sequence tree file and derived quartet trees on sequences
    void computeQuartetCompatability(std::string quartetFilename, std::string largeTreeFilename);

    //uses bipartition method to compute symmetric difference --
        //both arguments are largeTreeFileDirectories
    void computeSymmetricDifference(std::string largeTreeFilename1, std::string largeTreeFilename2);
}