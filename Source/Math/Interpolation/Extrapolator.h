#pragma once

//Interpolation1D / Interpolation2D 등을 파생시키기 위한 기본 클래스.
class Extrapolator {
public:
	Extrapolator() = default;
	virtual ~Extrapolator() = default;
};