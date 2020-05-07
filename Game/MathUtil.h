#pragma once

namespace Nitro {
	inline 
	int ModuloDist(int a, int b, int modulo)
	{
		int value = b - a;
		int mod = value % modulo;
		if (value < 0)
		{
			mod += modulo;
		}
		return mod;
	}

	inline
	int ModuloDecrement(int a, int modulo)
	{
		a -= 1;
		if (a < 0)
		{
			a += modulo;
		}
		return a;
	}

	inline
	int ModuloIncrement(int a, int modulo)
	{
		return (a + 1) % modulo;
	}

	inline
	int ModuloSub(int a, int b, int modulo)
	{
		a -= b;
		if (a < 0)
		{
			a += modulo;
		}
		return a;
	}

	inline
	int ModuloAdd(int a, int b, int modulo)
	{
		a += b;
		return a % modulo;
	}

	inline float RadiansToDegrees(float rd)
	{
		return rd * (180.0f / (float)M_PI);
	}

	inline float DegreesToRadians(float dg)
	{
		return dg * ((float)M_PI / 180.f);
	}


}