#pragma once

template <typename T>
class Visitor {
public:
	virtual void visit(T&) = 0;
};

class AcyclicVisitor {
public:
	virtual ~AcyclicVisitor() = default;
};