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
		impl_ = std::make_shared<LinearInterpolation1DImpl<X, Y>>(LinearInterpolation1DImpl<X, Y>(xbegin, xend, ybegin, yend));
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
		bool isValidRange(double x) const override;

	private:
		void initialize();

		X xbegin_, xend_;
		Y ybegin_, yend_;

		std::vector<double> slopes_;
		std::vector<double> primitives_;
	};
};

template <typename X, typename Y>
bool LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::isValidRange(double x) const
{
	bool is_valid = x < *xbegin_ || x > *(xend_ - 1) ? false : true;
	return is_valid;
}

template <typename X, typename Y>
void LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::isDifferentiablePoint(double x) const
{
	if (std::find(xbegin_, xend_, x) != xend_)
		throw std::runtime_error("Non-differentiable point");
}

template <typename X, typename Y>
void LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::initialize()
{
	auto length = std::distance(xbegin_, xend_);
	X iter_x = xbegin_;
	Y iter_y = ybegin_;

	slopes_.resize(length - 1);
	primitives_.resize(length - 1);
	std::size_t iteration = 0;

	auto x = *iter_x++;
	auto y = *iter_y++;
	auto next_x = *iter_x;
	auto next_y = *iter_y;
	double slope = (next_y - y) / (next_x - x);
	slopes_[iteration] = slope;
	primitives_[iteration] = 0;

	while (iter_x != xend_ - 1) {
		primitives_[iteration + 1] = primitives_[iteration] + 1 / 2. * slopes_[iteration] * (next_x - x) * (next_x - x) + y * (next_x - x);

		x = *iter_x++;
		y = *iter_y++;
		next_x = *iter_x;
		next_y = *iter_y;

		double slope = (next_y - y) / (next_x - x);

		slopes_[++iteration] = slope;
	}
}

template <typename X, typename Y>
auto LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::locate(double x) const
{
	auto it = std::lower_bound(xbegin_, xend_, x);

	if (it != xend_) {
		if (it != xbegin_)
			return std::distance(xbegin_, it - 1);
		else
			return std::distance(xbegin_, xbegin_);
	}
	else
		return std::distance(xbegin_, xend_ -2);

}

template <typename X, typename Y>
double LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::value(double x) const
{
	auto location = locate(x);
	double value = slopes_[location] * (x - *(xbegin_ + location)) + *(ybegin_ + location);
	return value;
}

template <typename X, typename Y>
double LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::derivative(double x) const
{
	isDifferentiablePoint(x);
	auto location = locate(x);
	return slopes_[location];
}

template <typename X, typename Y>
double LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::second_derivative(double x) const
{
	isDifferentiablePoint(x);
	return 0.;
}

template <typename X, typename Y>
double LinearInterpolation1D::LinearInterpolation1DImpl<X, Y>::primitive(double x) const
{
	auto location = locate(x);
	double previous_premitive = primitives_[location];
	double dx = x - *(xbegin_ + location);
	double value = previous_premitive + 1 / 2. * slopes_[location] * dx * dx + *(ybegin_ + location) * dx;
	return value;
}