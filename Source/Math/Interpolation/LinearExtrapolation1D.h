#pragma once
#include <Source/Math/Interpolation/Extrapolation1D.h>

class LinearExtrapolation1D : public Extrapolation1D {
public:
	LinearExtrapolation1D() = default;
	template <typename X, typename Y>
	LinearExtrapolation1D(Location location, const X& xbegin, const X& xend, const Y& ybegin, const Y& yend)
		: Extrapolation1D(location)
	{

	}

private:
	template <typename X, typename Y>
	class LinearExtrapolation1DImpl : public Extrapolation1D::Impl {
	public:
		LinearExtrapolation1DImpl(const X& xbegin, const X& xend, const Y& ybegin, const Y& yend)
			: xbegin_(xbegin), xend_(xend), ybegin_(ybegin), yend_(yend) { }

	private:
		X xbegin_, xend_;
		Y ybegin_, yend_;
	};
};