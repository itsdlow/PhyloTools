/******************************************************************************
DeAngelo Wilson
July 24 2020

			A resource management class for char array, of given size
******************************************************************************/

#include "DynamicSizedArray.h"

DynamicSizedArray::DynamicSizedArray(int size):
array(new char[size])
{
}

DynamicSizedArray::~DynamicSizedArray()
{
	delete[] this->array;
}