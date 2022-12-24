#pragma once
#include <Source/DesignPattern/Singleton.h>
#include <Source/Times/Date.h>

class Settings : public Singleton<Settings> {
	friend class Singleton<Settings>;
public:
	Date& ValueDate() { return ValueDate_; }
	const Date& ValueDate() const { return ValueDate_; }

private:
	Settings() = default;
	Date ValueDate_;
};