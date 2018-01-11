//Resolver.cpp

#include "stdafx.h"

#include <iostream>
#include <exception>
#include <string>
using namespace std;

#include "Resolver.h"

Resolver::Resolver()
{
	BuildOracleAggrAndAnalyticFunctions();
}

Resolver::~Resolver()
{
	OracleAggrAndAnalyticFunctions.clear();
}

void Resolver::BuildOracleAggrAndAnalyticFunctions()
{
	if (OracleAggrAndAnalyticFunctions.size() > 0) return;
	OracleAggrAndAnalyticFunctions.insert("AVG");
	OracleAggrAndAnalyticFunctions.insert("APPROX_COUNT_DISTINCT");
	OracleAggrAndAnalyticFunctions.insert("COLLECT");
	OracleAggrAndAnalyticFunctions.insert("CORR");
	OracleAggrAndAnalyticFunctions.insert("CORR_S");
	OracleAggrAndAnalyticFunctions.insert("CORR_K");
	OracleAggrAndAnalyticFunctions.insert("COUNT");
	OracleAggrAndAnalyticFunctions.insert("COVAR_POP");
	OracleAggrAndAnalyticFunctions.insert("COVAR_SAMP");
	OracleAggrAndAnalyticFunctions.insert("CUME_DIST");
	OracleAggrAndAnalyticFunctions.insert("DENSE_RANK");
	OracleAggrAndAnalyticFunctions.insert("FIRST");
	OracleAggrAndAnalyticFunctions.insert("GROUP_ID");
	OracleAggrAndAnalyticFunctions.insert("GROUPING");
	OracleAggrAndAnalyticFunctions.insert("GROUPING_ID");
	OracleAggrAndAnalyticFunctions.insert("LAST");
	OracleAggrAndAnalyticFunctions.insert("LISTAGG");
	OracleAggrAndAnalyticFunctions.insert("MAX");
	OracleAggrAndAnalyticFunctions.insert("MEDIAN");
	OracleAggrAndAnalyticFunctions.insert("MIN");
	OracleAggrAndAnalyticFunctions.insert("PERCENT_RANK");
	OracleAggrAndAnalyticFunctions.insert("PERCENTILE_CONT");
	OracleAggrAndAnalyticFunctions.insert("PERCENTILE_DISC");
	OracleAggrAndAnalyticFunctions.insert("RANK");
	OracleAggrAndAnalyticFunctions.insert("REGR_SLOPE");
	OracleAggrAndAnalyticFunctions.insert("REGR_INTERCEPT");
	OracleAggrAndAnalyticFunctions.insert("REGR_COUNT");
	OracleAggrAndAnalyticFunctions.insert("REGR_R2");
	OracleAggrAndAnalyticFunctions.insert("REGR_AVGX");
	OracleAggrAndAnalyticFunctions.insert("REGR_AVGY");
	OracleAggrAndAnalyticFunctions.insert("REGR_SXX");
	OracleAggrAndAnalyticFunctions.insert("REGR_SYY");
	OracleAggrAndAnalyticFunctions.insert("REGR_SXY");
	OracleAggrAndAnalyticFunctions.insert("STATS_BINOMIAL_TEST");
	OracleAggrAndAnalyticFunctions.insert("STATS_CROSSTAB");
	OracleAggrAndAnalyticFunctions.insert("STATS_F_TEST");
	OracleAggrAndAnalyticFunctions.insert("STATS_KS_TEST");
	OracleAggrAndAnalyticFunctions.insert("STATS_MODE");
	OracleAggrAndAnalyticFunctions.insert("STATS_MW_TEST");
	OracleAggrAndAnalyticFunctions.insert("STATS_ONE_WAY_ANOVA");
	OracleAggrAndAnalyticFunctions.insert("STATS_T_TEST_ONE");
	OracleAggrAndAnalyticFunctions.insert("STATS_T_TEST_PAIRED");
	OracleAggrAndAnalyticFunctions.insert("STATS_T_TEST_INDEP");
	OracleAggrAndAnalyticFunctions.insert("STATS_T_TEST_INDEPU");
	OracleAggrAndAnalyticFunctions.insert("STATS_WSR_TEST");
	OracleAggrAndAnalyticFunctions.insert("STDDEV");
	OracleAggrAndAnalyticFunctions.insert("STDDEV_POP");
	OracleAggrAndAnalyticFunctions.insert("STDDEV_SAMP");
	OracleAggrAndAnalyticFunctions.insert("SUM");
	OracleAggrAndAnalyticFunctions.insert("SYS_OP_ZONE_ID");
	OracleAggrAndAnalyticFunctions.insert("SYS_XMLAGG");
	OracleAggrAndAnalyticFunctions.insert("VAR_POP");
	OracleAggrAndAnalyticFunctions.insert("VAR_SAMP");
	OracleAggrAndAnalyticFunctions.insert("VARIANCE");
	OracleAggrAndAnalyticFunctions.insert("XMLAGG");
}

bool Resolver::IsOracleAggrAndAnalyticFunctions(std::string field)
{
	return (OracleAggrAndAnalyticFunctions.find(field) != OracleAggrAndAnalyticFunctions.end());
}

bool IsCounterChar(char ch)
{
	return (isalnum(ch) || (ch == '_') || (ch == '$') || (ch == '#'));
}

bool IsCounterCharIX(char ch)
{
	return (isalnum(ch) || (ch == '_') || (ch == '$') || (ch == '#') || (ch == '%'));
}

bool IsStartLetterIX(char ch)
{
	return (isalpha(ch) || (ch == '%'));
}

#pragma warning ( disable : 4018 )

/////

int Resolver::ExtractFirstFieldFromFormula(int searchPos, std::string inEquation, std::string& outField)
{
	outField = "";//no "simple"-field found
	int i = searchPos;
	int fieldSt = 0;
	int bracketCount = 0;
	std::string field = "";
	while (i < inEquation.length())
	{
		if (isalpha(inEquation[i]))
		{
			fieldSt = i;
			field = "";
			bracketCount = 0;
			while (i < inEquation.length() && IsCounterChar(inEquation[i]))
			{
				field = field + inEquation[i];
				i++;
			}
			while (i < inEquation.length() && isspace(inEquation[i]))
			{
				i++;
			}
			if (i < inEquation.length() && inEquation[i] == '(')
			{
				if (IsOracleAggrAndAnalyticFunctions(field))
				{
					field = "";   
					bracketCount++;
					i++;
					while (i < inEquation.length())
					{
						if (inEquation[i] == '(')
							bracketCount++;
						else
						if (inEquation[i] == ')')
							bracketCount--;
						if (bracketCount == 0) break;
						i++;
					} 
				}
				continue;
			}
			if (field.length() > 0)
			{
				outField = field;
				return (i - field.length());
			}
		}
		else
			i++;
	}
	return -1;
}

/////

//Returns position of next token
//OutField is populated if it is a Counter, but if it is a function it is empty
int Resolver::ExtractNextCounter(int searchPos, std::string inEquation, std::string& outField)
{
	outField = "";//no "simple"-field found
	int i = searchPos;
	int fieldSt = 0;
	std::string field = "";
	while (i < inEquation.length())
	{
		if (isalpha(inEquation[i]))
		{
			fieldSt = i;
			field = "";
			while (i < inEquation.length() && IsCounterChar(inEquation[i]))
			{
				field = field + inEquation[i];
				i++;
			}
			while (i < inEquation.length() && isspace(inEquation[i]))
			{
				i++;
			}
			if (i < inEquation.length() && inEquation[i] == '(')
			{
				outField = "";
				return (i);
			}

			if (field.length() > 0)
			{
				outField = field;
				return (i - field.length());
			}
		}
		else
			i++;
	}
	return -1;
}

/////

int Resolver::ExtractFirstPatternFromFormula(int SearchPos, std::string inEquation, std::string& outField)
{
	outField = "";//no "pattern"-field found
	int i = SearchPos;
	int fieldSt = 0;
	std::string field = "";
	while (i < inEquation.length())
	{
		if (IsStartLetterIX(inEquation[i]))
		{
			fieldSt = i;
			field = "";
			while (i < inEquation.length() && IsCounterCharIX(inEquation[i]))
			{
				field = field + inEquation[i];
				i++;
			}
			while (i < inEquation.length() && isspace(inEquation[i]))
			{
				i++;
			}
			if (i < inEquation.length() && inEquation[i] == '(')
			{
				if (field == AggPlaceholderPrefix)
				{
					outField = field;
					return (i - field.length());
				}
				if (field == KPIPlaceholderPrefix)
				{
					outField = field;
					return (i - field.length());
				}
				if (IsOracleAggrAndAnalyticFunctions(field))
				{

					outField = field;
					return (i - field.length());
				}
				continue;
			}
			if (field.length() > 0)
				i++;
		}
		else
			i++;
	}
	return -1;
}

/////

ResolverErrors Resolver::ResolvePlaceholders(bool isElemAggr, std::string inKPIFormula, std::string& outFormula,
	Complexity& complexity, std::string kpiAggFunction, bool useKPIAgg,
	std::string schemaName, std::string tableName, std::string counterSetName,
	KpiEquationLookup *kpiEquationLookup,
	CounterAggregationLookup *counterAggregationLookup, std::string& errorString)
{
	std::string tmpFormula = "";
	outFormula = "";
	std::string inFormula = "";
	int i = 0;
	while (i < inKPIFormula.length())
	{
		if (!(isspace(inKPIFormula[i]) || (inKPIFormula[i] == '\t') || (inKPIFormula[i] == '\r') || (inKPIFormula[i] == '\n')))
		{
			inFormula = inFormula + inKPIFormula[i];
		}
		i++;
	}
	if (inFormula.empty())
	{
		complexity = Complexity::Unknown;
		errorString = ResolverErrorStrings[(int)ResolverErrors::ResolverEmptyFormula];
		return ResolverErrors::ResolverEmptyFormula;
	}
	try
	{
		//deal with patterns :
		int pos1 = 0;
		int foundOpen = 0;
		while (true)
		{
			std::string functionFld = "(null)";
			pos1 = ExtractFirstPatternFromFormula(pos1, inFormula, functionFld);
			if (pos1 < 0) break;
			int found = pos1;

			tmpFormula = tmpFormula + inFormula.substr(foundOpen, found - foundOpen);

			//now get internal Expression
			int indCh = (foundOpen = found + functionFld.length()) + 1;
			int bracketCount = 1;
			std::string internalExpr = "";
			int foundClose = -1;
			while (indCh < inFormula.length())
			{
				if (inFormula[indCh] == '(')
				{
					bracketCount++;
				}
				else
				{
					if (inFormula[indCh] == ')')
						bracketCount--;
				}
				if (bracketCount == 0)
				{
					foundClose = indCh;
					break;
				}
				internalExpr = internalExpr + inFormula[indCh];
				indCh++;
			}
			//internal Expression - OK

			if (foundClose >= 0)
			{
				ResolverErrors rs;
				std::string kpiName = inFormula.substr(foundOpen + 1, foundClose - foundOpen - 1);
				if (functionFld != KPIPlaceholderPrefix)
				{
					if (functionFld != AggPlaceholderPrefix)
						tmpFormula = tmpFormula + functionFld;
					else if (useKPIAgg)
						tmpFormula = tmpFormula + kpiAggFunction;
				}

				//now deal with internalExpr :
				std::string outExpr = "";
				Complexity internalCompl = Complexity::Simple;
				if (functionFld == KPIPlaceholderPrefix)
				{
					//unit test >>>
					if (internalExpr == "UNIT_TEST_X1357924680_386E8831_B727_4646_9AA1_7B8A423B1DE9")
					{
						throw std::exception("RESOLVER UNIT TEST INTERNAL EXCEPTION");
					}
					//unit test <<<

					//get KPI:
					std::string nestedKpiName = internalExpr;
					std::string nestedKpiEquation = "";
					rs = kpiEquationLookup->GetEquation(schemaName, tableName, counterSetName, nestedKpiName, nestedKpiEquation, errorString);
					if (rs != ResolverErrors::ResolverNoError)
					{
						if (rs != ResolverErrors::ResolverInternalError)
							errorString = ResolverErrorStrings[(int)rs];
						return rs;
					}
					std::string NestedKpiAggrFun = "";
					rs = kpiEquationLookup->GetAggregation(isElemAggr, schemaName, tableName, counterSetName, nestedKpiName, NestedKpiAggrFun, errorString);
					if (rs != ResolverErrors::ResolverNoError)
					{
						if (rs != ResolverErrors::ResolverInternalError)
							errorString = ResolverErrorStrings[(int)rs];
						return rs;
					}
					rs = ResolvePlaceholders(isElemAggr, nestedKpiEquation, outExpr, internalCompl, NestedKpiAggrFun, 
						useKPIAgg, schemaName, tableName, counterSetName, kpiEquationLookup, counterAggregationLookup, errorString);
					if (rs != ResolverErrors::ResolverNoError)
					{
						if (rs != ResolverErrors::ResolverInternalError)
							errorString = ResolverErrorStrings[(int)rs];
						return rs;
					}
					if (internalCompl == Complexity::Aggregated) complexity = Complexity::Aggregated;
				}
				else
				{
					rs = ResolvePlaceholders(isElemAggr, internalExpr, outExpr, internalCompl, kpiAggFunction, false, 
						schemaName, tableName, counterSetName, kpiEquationLookup, counterAggregationLookup, errorString);
					if (rs != ResolverErrors::ResolverNoError)
					{
						if (rs != ResolverErrors::ResolverInternalError)
							errorString = ResolverErrorStrings[(int)rs];
						return rs;
					}
					complexity = Complexity::Aggregated;
				}
				if (rs != ResolverErrors::ResolverNoError)
				{
					if (rs != ResolverErrors::ResolverInternalError)
						errorString = ResolverErrorStrings[(int)rs];
					return rs;
				}
				tmpFormula = tmpFormula + "(" + outExpr + ")";
				pos1 = foundOpen = (found = foundClose) + 1;
			}
			else
			{
				complexity = Complexity::Unknown;
				errorString = ResolverErrorStrings[(int)ResolverErrors::ResolverNoCloseBracket];
				return ResolverErrors::ResolverNoCloseBracket;
			}
		}
		if (inFormula.length() > foundOpen)//the rest of formula :
			tmpFormula = tmpFormula + inFormula.substr(foundOpen, inFormula.length() - foundOpen);
	}
	catch (exception& ex)
	{
		complexity = Complexity::Unknown;
		errorString = ex.what();
		return ResolverErrors::ResolverInternalError;
	}
	outFormula = tmpFormula;
	errorString = ResolverErrorStrings[(int)ResolverErrors::ResolverNoError];
	return ResolverErrors::ResolverNoError;
}


/////

ResolverErrors Resolver::ResolveNonAggregatedCounters(bool isElemAggr, std::string inFormula, std::string& outFormula,
	std::string schemaName, std::string tableName, std::string counterSet, CounterAggregationLookup *counterAggregationLookup, std::string& errorString)
{
	try
	{
		outFormula = "";
		int SearchPos = 0;
		while (true)
		{
			string outField = "(null)";
			int foundPos = ExtractFirstFieldFromFormula(SearchPos, inFormula, outField);
			if (foundPos < 0)
			{
				outFormula += inFormula.substr(SearchPos, inFormula.length() - SearchPos);
				break;
			}

			//unit test >>>
			if (outField == "COUNTER_UNIT_TEST_X1357924680_386E8831_B727_4646_9AA1_7B8A423B1DE9")
			{
				throw std::exception("COUNTER UNIT TEST INTERNAL EXCEPTION");
			}
			//unit test <<<

			std::string AGGR;
			ResolverErrors res = counterAggregationLookup->GetAggregation(isElemAggr, schemaName, tableName, counterSet, outField, AGGR, errorString);
			if (res != ResolverErrors::ResolverNoError)
			{
				outFormula = "";
				return res;
			}
			if (IsOracleAggrAndAnalyticFunctions(AGGR))
				outFormula += inFormula.substr(SearchPos, foundPos - SearchPos) + AGGR + "(" + outField + ")";
			else
				outFormula += inFormula.substr(SearchPos, foundPos - SearchPos) + "(" + AGGR + ")";
			SearchPos = foundPos + outField.length();
		}

	}
	catch (exception ex)
	{
		outFormula = "";
		errorString = ex.what();
		return ResolverErrors::ResolverInternalError;
	}
	errorString = ResolverErrorStrings[(int)ResolverErrors::ResolverNoError];
	return ResolverErrors::ResolverNoError;
}

/////

void Resolver::FullyQualifyCounters(std::string inFormula, std::string schemaName, std::string counterSet, std::string& outFormula)
{
	outFormula = "";
	int lastCounterEndPos = 0;
	int nextCounterStartPos;
	string counter = "(null)";
	const int counterNotFound = -1;

	while (counterNotFound != (nextCounterStartPos = ExtractNextCounter(lastCounterEndPos, inFormula, counter)))
	{
		string inBetweenString = inFormula.substr(lastCounterEndPos, nextCounterStartPos - lastCounterEndPos);
		outFormula += inBetweenString;
		if (counter != "")
			outFormula += schemaName + "." + counterSet + "." + counter;

		lastCounterEndPos = nextCounterStartPos + counter.length();
	}

	string restOfFormula = inFormula.substr(lastCounterEndPos, inFormula.length() - lastCounterEndPos);
	outFormula += restOfFormula;
}

/////

ResolverErrors Resolver::Resolve(bool isElemAggr, std::string inKpiFormula, std::string& outFormula, Complexity& complexity, 
	std::string aggFunction, std::string schemaName, std::string tableName, std::string counterSet, 
	KpiEquationLookup *kpiEquationaLookup, CounterAggregationLookup *counterAggregationLookup, std::string& errorString)
{
	ResolverErrors res;
	std::string tmpFormula;
	complexity = Complexity::Simple;
	for (unsigned int i = 0; i<inKpiFormula.length(); ++i)
		inKpiFormula[i] = toupper(inKpiFormula[i]);

	res = ResolvePlaceholders(isElemAggr, inKpiFormula, tmpFormula, complexity, aggFunction, true, schemaName, tableName, counterSet, kpiEquationaLookup, counterAggregationLookup, errorString);
	if (res != ResolverErrors::ResolverNoError)
	{
		complexity = Complexity::Unknown;
		outFormula = "";
		return res;
	}

	res = ResolveNonAggregatedCounters(isElemAggr, tmpFormula, tmpFormula, schemaName, tableName, counterSet, counterAggregationLookup, errorString);
	if (res != ResolverErrors::ResolverNoError)
	{
		complexity = Complexity::Unknown;
		outFormula = "";
		return res;
	}

	FullyQualifyCounters(tmpFormula, schemaName, counterSet, outFormula);

	return res;
}


