#pragma once
#include <iostream>
#include <Source/Times/Schedule/Schedule.h>

void TestDate();
void TestBoostGregorian();
void TestCalendar();
void TestJointCalendar();
void TestDayCounter();
void TestNormalDistribution();
void TestBlackCalculator();
void TestBlackEuropeanEngine();
void TestQuantoBlackEuropeanOptionEngine();
void TestQuantoBlackEuropeanOptionEngineGreeks();
void TestEstimator1D();
void TestSchedule();
void TestDateScheduleConverter();

void printDateSchedule(const DateSchedule&);
std::string KindsOfDateToString(KindsOfDate);