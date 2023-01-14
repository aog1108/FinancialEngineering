#pragma once
#include <Source/Math/Interpolation/Extrapolator.h>
#include <memory>

class Interpolation1D : public Extrapolator {
public:
	Interpolation1D() = default;
	template <typename X, typename Y>
	Interpolation1D(const X& xbegin, const X& xend, const Y& ybegin, const Y& yend);

	double value(double x) const;
	double primitive(double x) const;
	double derivative(double x) const;
	double second_derivative(double x) const;

protected:
	class Impl;

	std::shared_ptr<Impl> impl_;
};