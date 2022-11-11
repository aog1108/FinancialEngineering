#pragma once

class Calculator {
public:
	virtual ~Calculator() = default;

	virtual double value() const = 0;
};