#pragma once
#include <Source/Math/Interpolation/Extrapolation1D.h>
#include <memory>
#include <stdexcept>

//Extrapolation 객체 자체는 템플릿 클래스가 아니도록 만들기 위해,
//Impl 내부 클래스를 도입하고, 각 Extrapolation 객체의 파생 클래스 내부에 
//Impl 클래스를 상속받은 클래스를 템플릿 클래스로 만듦.
class FlatExtrapolation1D : public Extrapolation1D {
public:
	FlatExtrapolation1D() = default;
	template <typename X, typename Y>
	FlatExtrapolation1D(Location location, const X& x, const Y& y) : Extrapolation1D(location)
	{ 
		impl_ = std::make_shared<FlatExtrapolation1DImpl<X, Y>>(new FlatExtrapolation1DImpl<X, Y>(x, y));
	}

	double value(double x) { return impl_->value(x); }

private:
	template <typename X, typename Y>
	class FlatExtrapolation1DImpl : public Extrapolation1D::Impl {
	public:
		FlatExtrapolation1DImpl(const X& x, const Y& y) : x_(x), y_(y) { }
		
		double value(double x);
		void isValidRange(double x);

	private:
		X x_;
		Y y_;
	};
};

template <typename X, typename Y>
double FlatExtrapolation1D::FlatExtrapolation1DImpl<X, Y>::value(double x)
{
	isValidRange(x);
	return y_;
}

template <typename X, typename Y>
void FlatExtrapolation1D::FlatExtrapolation1DImpl<X, Y>::isValidRange(double x)
{
	bool is_valid = false;
	switch (location_) {
	case Location::Front:
		is_valid =  x <= x_ ? true : false;
		break;
	case Location::End:
		is_valid = x >= x_ ? true : false;
		break;
	default:
		throw std::domain_error("Unknown location type.");
	}

	if (!is_valid)
		throw std::range_error("Not valid range.");
}