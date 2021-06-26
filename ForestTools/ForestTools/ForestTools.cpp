/******************************************************************************
DeAngelo Wilson
January 3 2020

                        ForestTools (main)
******************************************************************************/

#include "ForestPlug.h"


//TODO::
    // 0) Refactor LcsCalculator code*
    // 3) How to get amino acid sequences
    // 5) GUARDS!!!
            //safe-failing/execption safe
            //Matrixes not created...
                //Bad file names?
                //failed alignment?
            //Trees not created...
                //failed fastme?
    // 6) FileObject::sequence_size --> long -- allow for 3billion bp sequences

/*****   (using sequence data) create matrix --> tree (fastme)   ****/



//------------------------------------------------------------
//
//------------------------------------------------------------
int main()
{
	
    ForestPlug fp;
    fp.run();
}
