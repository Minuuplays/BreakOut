#pragma once

#include <SDL.h>
#include <string>
#include <iostream>
#include <vector>

// >> Template Functions << //
template <class T>
void deleteAndClear(std::vector<T*>& v)
{
	for (T* obj : v)
	{
		delete obj;
	}
	v.clear();
}

template <class T> T getMin(T a, T b) { 
	return (a < b) ? a : b;
}

template <class T> T getMax(T a, T b) {
	return (a > b) ? a : b;
}

template <class T>
T clamp(T value, T min, T max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

template <class T>
void safeDelete(T* & ptr)
{
	if (ptr != nullptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <class T>
void swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}
