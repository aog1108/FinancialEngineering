#pragma once
#include <Source/Math/Interpolation/Interpolation1D.h>
#include <algorithm>
#include <stdexcept>
#include <vector>

class LinearInterpolation1D : public Interpolation1D {
public:
	LinearInterpolation1D() = default;
	template <typename X, typename Y>
	LinearInterpolation1D(const X& xbegin, const X& xend, const Y& ybegin, const Y& yend)
	{
		impl_ = std::make_shared<LinearInterpolation1DImpl<X, Y>>(new LinearInterpolation1DImpl<X, Y>(xbegin, xend, ybegin, yend));
	};

private:
	template <typename X, typename Y>
	class LinearInterpolation1DImpl : public Interpolation1D::Impl {
	public:
		LinearInterpolation1DImpl() = default;
		LinearInterpolation1DImpl(const X& xbegin, const X& xend, const Y& ybegin, const Y& yend) 
			: xbegin_(xbegin), xend_(xend), ybegin_(ybegin), yend_(yend) 
		{
			if (std::is_sorted(xbegin, xend))
				initialize();
			else
				throw std::runtime_error("X values should be sorted");
		}

		auto locate(double x) const;
		void isDifferentiablePoint(double x) const;

		double value(double x) const override;
		double primitive(double x) const override;
		double derivative(double x) const override;
		double second_derivative(double x) const override;
		void isValidRange(double x) const override;

	private:
		void initialize();

		X xbegin_, xend_;
		Y ybegin_, yend_;

		std::vector<double> slopes_;
	};
};

template <typename X, typename Y>
void LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::isValidRange(double x) const
{
	if (x < *xbegin_ || x > *(xend_ - 1))
		throw std::range_error("Not valid range");
}

template <typename X, typename Y>
void LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::isDifferentiablePoint(double x) const
{
	if ((auto it = std::find(xbegin_, xend_, x)) != xend_)
		throw std::runtime_error("Non-differentiable point");
}

template <typename X, typename Y>
void LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::initialize()
{
	auto length = std::distance(xbegin_, xend_);
	X iter_x = xbegin_;
	Y iter_y = ybegin_;

	slopes_.resize(length - 1);
	std::size_t iteration = 0;

	while (iter_x != xend_ - 1) {
		auto x = *iter_x++;
		auto y = *iter_y++;
		double slope = (*iter_y - y) / (*iter_x - x);
		slopes_[iteration++] = slope;
	}
}

template <typename X, typename Y>
auto LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::locate(double x) const
{
	auto it = std::lower_bound(xbegin_, xend_, x);

	if (it != xend_) {
		if (it != xbegin_)
			return it - 1 - xbegin_;
		else
			return 0;
	}
	else
		return (xend_ - 2) - xbegin_;

}

template <typename X, typename Y>
double LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::value(double x) const
{
	isValidRange(x);
	auto location = locate(x);
	double value = slopes_[location] * (x - *(xbegin_ + location)) + *(ybegin_ + location);
	return value;
}

template <typename X, typename Y>
double LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::derivative(double x) const
{
	isValidRange(x);
	isDifferentiablePoint(x);
	auto location = locate(x);
	return slopes_[location];
}

template <typename X, typename Y>
double LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::second_derivative(double x) const
{
	isValidRange(x);
	isDifferentiablePoint(x);
	return 0.;
}

template <typename X, typename Y>
double LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::primitive(double x) const
{

}