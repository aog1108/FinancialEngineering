#pragma once
#include <Source/Math/Extrapolation/Extrapolation.h>
#include <memory>

//Extrapolation ��ü ��ü�� ���ø� Ŭ������ �ƴϵ��� ����� ����,
//Impl ���� Ŭ������ �����ϰ�, �� Extrapolation ��ü�� �Ļ� Ŭ���� ���ο� 
//Impl Ŭ������ ��ӹ��� Ŭ������ ���ø� Ŭ������ ����.
class Extrapolation1D : public Extrapolation {
public:
	enum class Location { Front, End };
	
	Extrapolation1D() = default;
	Extrapolation1D(Location location) : location_(location) { }

	double value(double x) const { return impl_->value(x); }
	bool isValidRange(double x) const { return impl_->isValidRange(x); }
	Location getLocation() const { return location_; }

protected:
	class Impl {
	public:
		Impl() = default;
		Impl(Extrapolation1D& extrapolation) : extrapolation_(extrapolation) { }
		virtual ~Impl() = default;

		virtual double value(double x) const = 0;
		virtual bool isValidRange(double x) const = 0;

	protected:
		Extrapolation1D& extrapolation_;
	};

	Location location_;
	std::shared_ptr<Extrapolation1D::Impl> impl_;
};