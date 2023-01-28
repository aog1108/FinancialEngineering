#pragma once
#include <Source/Math/Interpolation/Extrapolation.h>
#include <memory>

//Extrapolation ��ü ��ü�� ���ø� Ŭ������ �ƴϵ��� ����� ����,
//Impl ���� Ŭ������ �����ϰ�, �� Extrapolation ��ü�� �Ļ� Ŭ���� ���ο� 
//Impl Ŭ������ ��ӹ��� Ŭ������ ���ø� Ŭ������ ����.
class Extrapolation1D : public Extrapolation {
public:
	enum class Location { Front, End };
	
	Extrapolation1D() = default;
	Extrapolation1D(Location location) : location_(location) { }

	double value(double x) { return impl_->value(x); }

protected:
	class Impl {
	public:
		virtual ~Impl() = default;

		virtual double value(double x) = 0;
		virtual void isValidRange(double x) = 0;
	};

	Location location_;
	std::shared_ptr<Extrapolation1D::Impl> impl_;
};