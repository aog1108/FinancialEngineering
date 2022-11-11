#pragma once

template <typename T>
class Singleton {
public:
	static T& get()
	{
		static T instance;
		return instance;
	}

private:
	Singleton() = default;
	Singleton(const Singleton&) = default;
	Singleton& operator=(const Singleton&) = default;
	~Singleton() = default;
};