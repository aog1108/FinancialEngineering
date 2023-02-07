#pragma once
#include <Source/Math/Extrapolation/Extrapolation1D.h>
#include <stdexcept>

class FlatExtrapolation1D : public Extrapolation1D {
public:
	FlatExtrapolation1D() = default;
	//X, Y는 반복자
	template <typename X, typename Y> 
	FlatExtrapolation1D(Location location, const X& x, const Y& y) : Extrapolation1D(location)
	{ 
		impl_ = std::make_shared<FlatExtrapolation1DImpl<X, Y>>(FlatExtrapolation1DImpl<X, Y>(*this, x, y));
	}

private:
	template <typename X, typename Y>
	class FlatExtrapolation1DImpl : public Extrapolation1D::Impl {
	public:
		FlatExtrapolation1DImpl(FlatExtrapolation1D& extrapolation, const X& x, const Y& y) : Extrapolation1D::Impl(extrapolation), x_(x), y_(y) { }
		
		double value(double x) const override;
		bool isValidRange(double x) const override;

	private:
		X x_;
		Y y_;
	};
};

template <typename X, typename Y>
double FlatExtrapolation1D::FlatExtrapolation1DImpl<X, Y>::value(double x) const
{
	return *y_;
}

template <typename X, typename Y>
bool FlatExtrapolation1D::FlatExtrapolation1DImpl<X, Y>::isValidRange(double x) const
{
	bool is_valid = false;
	switch (extrapolation_.getLocation()) {
	case Location::Front:
		is_valid =  x <= *x_ ? true : false;
		break;
	case Location::End:
		is_valid = x >= *x_ ? true : false;
		break;
	default:
		throw std::domain_error("Unknown location type.");
	}

	return is_valid;
}