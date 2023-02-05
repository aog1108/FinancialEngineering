#pragma once
#include <Source/Math/Interpolation/Extrapolation1D.h>
#include <stdexcept>

class FlatExtrapolation1D : public Extrapolation1D {
public:
	FlatExtrapolation1D() = default;
	//X, Y는 반복자
	template <typename X, typename Y> 
	FlatExtrapolation1D(Location location, const X& x, const Y& y) : Extrapolation1D(location)
	{ 
		impl_ = std::make_shared<FlatExtrapolation1DImpl<X, Y>>(new FlatExtrapolation1DImpl<X, Y>(x, y));
	}

private:
	template <typename X, typename Y>
	class FlatExtrapolation1DImpl : public Extrapolation1D::Impl {
	public:
		FlatExtrapolation1DImpl(const X& x, const Y& y) : x_(x), y_(y) { }
		
		double value(double x) const override;
		void isValidRange(double x) const override;

	private:
		X x_;
		Y y_;
	};
};

template <typename X, typename Y>
double FlatExtrapolation1D::FlatExtrapolation1DImpl<X, Y>::value(double x) const
{
	isValidRange(x);
	return y_;
}

template <typename X, typename Y>
void FlatExtrapolation1D::FlatExtrapolation1DImpl<X, Y>::isValidRange(double x) const
{
	bool is_valid = false;
	switch (location_) {
	case Location::Front:
		is_valid =  x <= *x_ ? true : false;
		break;
	case Location::End:
		is_valid = x >= *x_ ? true : false;
		break;
	default:
		throw std::domain_error("Unknown location type.");
	}

	if (!is_valid)
		throw std::range_error("Not valid range.");
}