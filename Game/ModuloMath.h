#pragma once




namespace Nitro {



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
	int ModuloDecrement(int a, int modulo)
	{
		a -= 1;
		if (a < 0)
		{
			a += modulo;
		}
		return a;
	}
	int ModuloIncrement(int a, int modulo)
	{
		return (a + 1) % modulo;
	}

	int ModuloSub(int a, int b, int modulo)
	{
		a -= b;
		if (a < 0)
		{
			a += modulo;
		}
		return a;
	}

	int ModuloAdd(int a, int b, int modulo)
	{
		a += b;
		return a % modulo;
	}



}