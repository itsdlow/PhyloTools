/******************************************************************************
DeAngelo Wilson
July 24 2020

			A resource management class for char array, of given size
******************************************************************************/

#ifndef _DynamicSizedArray
#define _DynamicSizedArray

class DynamicSizedArray
{
public:
	DynamicSizedArray(int size);
	DynamicSizedArray(const DynamicSizedArray&) = delete;
	DynamicSizedArray& operator=(const DynamicSizedArray&) = delete;
	~DynamicSizedArray();
	
public:
	char* array;
};

#endif
