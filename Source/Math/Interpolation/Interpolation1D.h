#pragma once
#include <Source/Math/Interpolation/Interpolation.h>
#include <memory>

class Interpolation1D : public Interpolation {
public:
	Interpolation1D() = default;
	
	double value(double x) const { return impl_->value(x); }
	double primitive(double x) const { return impl_->primitive(x); }
	double derivative(double x) const { return impl_->derivative(x); }
	double second_derivative(double x) const { return impl_->second_derivative(x); }
	bool isValidRange(double x) const { return impl_->isValidRange(x); }

protected:
	class Impl {
	public:
		Impl() = default;
		virtual ~Impl() = default;

		virtual double value(double x) const = 0;
		virtual double primitive(double x) const = 0;
		virtual double derivative(double x) const = 0;
		virtual double second_derivative(double x) const = 0;
		virtual bool isValidRange(double x) const = 0;
	
	protected:
		virtual void initialize() = 0;
	};

	std::shared_ptr<Impl> impl_;
};