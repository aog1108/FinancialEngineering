#pragma once
#include <Source/Math/Interpolation/Extrapolation.h>
#include <memory>

//Extrapolation 객체 자체는 템플릿 클래스가 아니도록 만들기 위해,
//Impl 내부 클래스를 도입하고, 각 Extrapolation 객체의 파생 클래스 내부에 
//Impl 클래스를 상속받은 클래스를 템플릿 클래스로 만듦.
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