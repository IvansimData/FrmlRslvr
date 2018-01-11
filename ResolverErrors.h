// ResolverErrors.h

#pragma once

enum ResolverErrors
{
	ResolverNoError,
	ResolverCounterNotFound,
	ResolverNoOpenBracket,
	ResolverNoCloseBracket,
	ResolverEmptyFormula,
	ResolverComObjectNotFound,
	ResolverKPINotFound,
	ResolverInternalError,
	ResolverError8,
	ResolverUndefinedError = 9
};

const std::string ResolverErrorStrings[10] =
{
	"No Error",//0
	"Counter Not Found",
	"No Open Bracket",
	"No Close Bracket",
	"Empty Formula",
	"Com Object Not Found",
	"KPI Not Found",
	"Internal Error",
	"",
	"Undefined Error"//9
};

