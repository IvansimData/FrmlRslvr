//CounterAggregationLookup.h

#pragma once

#include <string>

#include "ResolverErrors.h"

class CounterAggregationLookup //used for Simple Counter
{
public:
	virtual ResolverErrors GetAggregation(bool isElemAggr, std::string schemaName, std::string tableName, std::string counterSet, 
		std::string counterName, std::string& aggregation, std::string& errorString) = 0;
};

