//CounterAggregationLookup.h

#pragma once

#include <string>

#include "ResolverErrors.h"

#pragma once

class KpiEquationLookup
{
public:
	virtual ResolverErrors GetEquation(std::string schema, std::string tableName, std::string counterSet, 
		std::string inKPIname, std::string& equation, std::string& errorString) = 0;
	virtual ResolverErrors GetAggregation(bool isElemAggr, std::string schema, std::string tableName, std::string counterSet,
		std::string inKPIname, std::string& aggregation, std::string& errorString) = 0;
};


