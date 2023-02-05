#pragma once
#include <Source/Math/Extrapolation/Extrapolation1D.h>

class LinearExtrapolation1D : public Extrapolation1D {
public:
	LinearExtrapolation1D() = default;
	//X, Y�� �ݺ���. STL�� ȣȯ���� end point�� ���� extrapolation ���ϴ� ��ġ���� +1�ؼ� �־��־�� ��.
	template <typename X, typename Y>
	LinearExtrapolation1D(Location location, const X& xbegin, const X& xend, const Y& ybegin, const Y& yend) : Extrapolation1D(location)
	{
		impl_ = std::make_shared<LinearExtrapolation1DImpl<X, Y>>(LinearExtrapolation1DImpl<X, Y>(*this, xbegin, xend, ybegin, yend));
	}

private:
	template <typename X, typename Y>
	class LinearExtrapolation1DImpl : public Extrapolation1D::Impl {
	public:
		LinearExtrapolation1DImpl(LinearExtrapolation1D& extrapolation, const X& xbegin, const X& xend, const Y& ybegin, const Y& yend)
			: Extrapolation1D::Impl(extrapolation), xbegin_(xbegin), xend_(xend), ybegin_(ybegin), yend_(yend) { }

		double value(double x) const override;
		bool isValidRange(double x) const override;
		
	private:
		X xbegin_, xend_;
		Y ybegin_, yend_;
	};
};

template <typename X, typename Y>
double LinearExtrapolation1D::LinearExtrapolation1DImpl<X, Y>::value(double x) const
{
	isValidRange(x);
	double slope = (*(yend_ - 1) - *ybegin_) / (*(xend_ - 1) - *xbegin_);

	return slope * (x - *xbegin_) + *ybegin_;
}

template <typename X, typename Y>
bool LinearExtrapolation1D::LinearExtrapolation1DImpl<X, Y>::isValidRange(double x) const
{
	bool is_valid = false;
	switch (extrapolation_.getLocation()) {
	case Location::Front:
		is_valid = x <= *xbegin_ ? true : false;
		break;
	case Location::End:
		is_valid = x >= *(xend_ - 1) ? true : false;
		break;
	default:
		throw std::domain_error("Unknown location type.");
	}
	
	return is_valid;
}