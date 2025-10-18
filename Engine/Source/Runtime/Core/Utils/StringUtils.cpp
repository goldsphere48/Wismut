#include "StringUtils.h"

#include <cstring>

namespace Wi::Utils
{
	bool HasSubstringInCString(const char* extList, const char* ext)
	{
		if (!extList || !ext)
			return false;

		const char* start = extList;
		const char* where;
		size_t len = strlen(ext);
		while ((where = strstr(start, ext)))
		{
			const char* terminator = where + len;

			if ((where == start || *(where - 1) == ' ') && (*terminator == ' ' || *terminator == '\0'))
				return true;

			start = terminator;
		}
		return false;
	}
}
