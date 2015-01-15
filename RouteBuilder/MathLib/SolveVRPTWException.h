#pragma once
#include "MathLib.h"
#include "MathException.h"

namespace VRPTW
{

class CSolveVRPTWException : public CMathException
{
public:
	enum _SVRPTWexc
	{
		c_iterator = 1,
		c_RelationNotFound = 2
	};
public:
	CSolveVRPTWException(_SVRPTWexc code,string  details="");
	~CSolveVRPTWException(void);


};
}

