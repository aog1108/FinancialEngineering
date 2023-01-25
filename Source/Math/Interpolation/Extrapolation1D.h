#pragma once
#include <Source/Math/Interpolation/Extrapolation.h>

class Extrapolation1D : public Extrapolation {
public:
	enum class Location { Front, End };
	
	Extrapolation1D() = default;
	Extrapolation1D(Location location) : location_(location) { }
	
	virtual double value(double x) = 0;
	virtual bool isValidRange(double x) = 0;

protected:
	class Impl {
	public:
		virtual ~Impl() = default;

		virtual double value(double x) = 0;
		virtual void isValidRange(double x) = 0;
	};

	Location location_;
	std::shared_ptr<Extrapolation1D::Impl> impl_;
};