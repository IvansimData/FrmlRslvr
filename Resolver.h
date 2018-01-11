//Resolver.h

#pragma once

#include <set>
#include <string>

#include "ResolverErrors.h"
#include "KpiEquationLookup.h"
#include "CounterAggregationLookup.h"

enum Complexity
{
	Unknown = 0,
	Simple = 'S',
	Aggregated = 'A',
	Error = 'E'
};

const std::string AggPlaceholderPrefix = "%AGGR";
const std::string KPIPlaceholderPrefix = "%KPI";

class Resolver
{
public:
	Resolver();
	~Resolver();
private:
	std::set<std::string> OracleAggrAndAnalyticFunctions;
	void BuildOracleAggrAndAnalyticFunctions();
	int ExtractFirstFieldFromFormula(int searchPos, std::string inEquation, std::string& outField);
	int ExtractNextCounter(int searchPos, std::string inEquation, std::string& outField);
	bool IsOracleAggrAndAnalyticFunctions(std::string field);
	int ExtractFirstPatternFromFormula(int SearchPos, std::string inEquation, std::string& outField);

	ResolverErrors ResolvePlaceholders(bool isElemAggr, std::string inKPIFormula, std::string& outFormula, 
		Complexity& complexity, std::string kpiAggFunction, bool useKPIAgg, 
		std::string schemaName, std::string tableName, std::string counterSetName, 
		KpiEquationLookup *kpiEquationLookup,
		CounterAggregationLookup *counterAggregationLookup, std::string& errorString);

	ResolverErrors ResolveNonAggregatedCounters(bool isElemAggr, std::string inFormula, std::string& outFormula,
		std::string schemaName, std::string tableName, std::string counterSet, CounterAggregationLookup *counterAggregationLookup, std::string& errorString);

	void FullyQualifyCounters(std::string inFormula, std::string schemaName, std::string counterSet, std::string& outFormula);

public:
	ResolverErrors Resolve(bool isElemAggr, std::string inKpiFormula, std::string& outFormula, 
		Complexity& complexity, std::string aggFunction, std::string schemaName, std::string tableName, 
		std::string counterSet, KpiEquationLookup *kpiEquationaLookup,
		CounterAggregationLookup *counterAggregationLookup, std::string& errorString);
};

