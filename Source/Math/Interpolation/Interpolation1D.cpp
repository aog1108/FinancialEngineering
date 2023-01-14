#include <Source/Math/Interpolation/Interpolation1D.h>

class Interpolation1D::Impl {
public:
	virtual double value(double x) = 0;
	virtual double primitive(double x) = 0;
	virtual double derivative(double x) = 0;
	virtual double second_derivative(double x) = 0;
};

