/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017     Statoil ASA
// 
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RiaDateStringParser.h"

#include "RiaStdStringTools.h"
#include "RiaQDateTimeTools.h"

#include <algorithm>

const std::string MONTH_NAMES[] =
{
    "january",
    "february",
    "march",
    "april",
    "may",
    "june",
    "july",
    "august",
    "september",
    "october",
    "november",
    "december"
};

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QDateTime RiaDateStringParser::parseDateString(const QString& dateString)
{
    return RiaDateStringParser::parseDateString(dateString.toStdString());
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QDateTime RiaDateStringParser::parseDateString(const std::string& dateString)
{
    int year, month, day;
    bool parsedOk =
        tryParseYearFirst(dateString, year, month, day) ||
        tryParseDayFirst(dateString, year, month, day) ||
        tryParseMonthFirst(dateString, year, month, day);

    QDateTime dt;
    dt.setTimeSpec(RiaQDateTimeTools::currentTimeSpec());
    if (parsedOk) dt.setDate(QDate(year, month, day));

    return dt;
}

//--------------------------------------------------------------------------------------------------
/// Try parse formats
/// 'yyyy mm dd'
/// 'yyyy MMM dd'
/// 'yyyy_mm_dd'
/// 'yyyy_MMM_dd'
/// 'yyyy-mm-dd'
/// 'yyyy-MMM-dd'
/// 'yyyy.MMM.dd'
/// 'yyyy.MMM.dd'
/// MMM is month name (shortened)
//--------------------------------------------------------------------------------------------------
bool RiaDateStringParser::tryParseYearFirst(const std::string& s, int& year, int& month, int& day)
{
    auto firstSep = s.find_first_of(" -_.");
    auto lastSep = s.find_first_of(" -_.", firstSep + 1);

    if (firstSep == std::string::npos || lastSep == std::string::npos) return false;

    auto sYear = s.substr(0, firstSep);
    auto sMonth = s.substr(firstSep + 1, lastSep - firstSep - 1);
    auto sDay = s.substr(lastSep + 1);

    return tryParseYear(sYear, year) && tryParseMonth(sMonth, month) && tryParseDay(sDay, day);
}

//--------------------------------------------------------------------------------------------------
/// Try parse formats
/// 'dd mm yyyy' 
/// 'dd MMM yyyy'
/// 'dd_mm_yyyy' 
/// 'dd_MMM_yyyy'
/// 'dd-mm-yyyy'
/// 'dd-MMM-yyyy'
/// 'dd.mm.yyyy'
/// 'dd.MMM.yyyy'
/// MMM is month name (shortened)
//--------------------------------------------------------------------------------------------------
bool RiaDateStringParser::tryParseDayFirst(const std::string& s, int& year, int& month, int& day)
{
    auto firstSep = s.find_first_of(" -_.");
    auto lastSep = s.find_first_of(" -_.", firstSep + 1);

    if (firstSep == std::string::npos || lastSep == std::string::npos) return false;

    auto sDay = s.substr(0, firstSep);
    auto sMonth = s.substr(firstSep + 1, lastSep - firstSep - 1);
    auto sYear = s.substr(lastSep + 1);

    return tryParseYear(sYear, year) && tryParseMonth(sMonth, month) && tryParseDay(sDay, day);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool RiaDateStringParser::tryParseMonthFirst(const std::string& s, int& year, int& month, int& day)
{
    auto firstSep = s.find_first_of(" -_.");
    auto lastSep = s.find_first_of(" -_.", firstSep + 1);

    if (firstSep == std::string::npos || lastSep == std::string::npos) return false;

    auto sMonth = s.substr(0, firstSep);
    auto sDay = s.substr(firstSep + 1, lastSep - firstSep - 1);
    auto sYear = s.substr(lastSep + 1);

    return tryParseYear(sYear, year) && tryParseMonth(sMonth, month) && tryParseDay(sDay, day);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool RiaDateStringParser::tryParseYear(const std::string& s, int &year)
{
    if (RiaStdStringTools::containsAlphabetic(s)) return false;

    auto today = QDate::currentDate();
    int y = RiaStdStringTools::toInt(s);
    if (y > 1970 && y <= today.year())
    {
        year = y;

        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool RiaDateStringParser::tryParseMonth(const std::string& s, int &month)
{
    if (RiaStdStringTools::containsAlphabetic(s))
    {
        auto sMonth = s;
        sMonth = trimString(sMonth);
        std::transform(sMonth.begin(), sMonth.end(), sMonth.begin(),
                       [](const char c) -> char { return (char)::tolower(c); });

        for (int i = 0; i < 12; i++)
        {
            if (MONTH_NAMES[i].compare(0, sMonth.size(), sMonth) == 0)
            {
                month = i + 1;
                return true;
            }
        }
    }
    else
    {
        int m = RiaStdStringTools::toInt(s);
        if (m >= 1 && m <= 12)
        {
            month = m;
            
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool RiaDateStringParser::tryParseDay(const std::string& s, int &day)
{
    if (RiaStdStringTools::containsAlphabetic(s)) return false;

    int d = RiaStdStringTools::toInt(s);
    if (d >= 1 && d <= 31)
    {
        day = d;

        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
std::string RiaDateStringParser::trimString(const std::string& s)
{
    auto sCopy = s.substr(0, s.find_last_not_of(' ') + 1);
    sCopy = sCopy.substr(sCopy.find_first_not_of(' '));
    return sCopy;
}
