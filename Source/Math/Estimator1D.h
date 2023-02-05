#pragma once
#include <Source/Math/Estimator.h>
#include <memory>

class Extrapolation1D;
class Interpolation1D;

class Estimator1D : public Estimator {
public:
	Estimator1D() = default;
	Estimator1D(const std::shared_ptr<Extrapolation1D>& front_extrapolation,
		const std::shared_ptr<Interpolation1D>& interpolation,
		const std::shared_ptr<Extrapolation1D>& end_extrapolation);

	double value(double x) const;
	double derivative(double x) const;
	double second_derivative(double x) const;
	double primitive(double x) const;

private:
	std::shared_ptr<Extrapolation1D> front_extrapolation_;
	std::shared_ptr<Interpolation1D> interpolation_;
	std::shared_ptr<Extrapolation1D> end_extrapolation_;
};