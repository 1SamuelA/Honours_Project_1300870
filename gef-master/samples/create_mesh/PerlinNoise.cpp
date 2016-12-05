#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
{
	persistence = 0;
	frequency = 0;
	amplitude = 0;
	octaves = 0;
	randomseed = 0;
}

PerlinNoise::PerlinNoise(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed)
{
	persistence =	_persistence;
	frequency =		_frequency;
	amplitude =		_amplitude;
	octaves =		_octaves;
	randomseed =	2 + _randomseed * _randomseed;
}

void PerlinNoise::Set(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed)
{
	persistence = _persistence;
	frequency = _frequency;
	amplitude = _amplitude;
	octaves = _octaves;
	randomseed = 2 + _randomseed * _randomseed;
}

double PerlinNoise::GetHeight(double x, double y) const
{
	return amplitude * Total(x, y);
}

double PerlinNoise::Total(double i, double j) const
{
	//properties of one octave (changing each loop)
	double t = 0.0f;
	double _amplitude = 1;
	double freq = frequency;

	for (int k = 0; k < octaves; k++)
	{
		t += GetValue(j * freq + randomseed, i * freq + randomseed) * _amplitude;
		_amplitude *= persistence;
		freq *= 2;
	}

	return t;
}

double PerlinNoise::GetValue(double x, double y) const
{
	int Xint = (int)x;
	int Yint = (int)y;
	double Xfrac = x - Xint;
	double Yfrac = y - Yint;

	//noise values
	double noise01 = Noise(Xint - 1, Yint - 1)	+ (Noise((Xint - 1) * 2, (Yint - 1) * 2) / 2)	+ (Noise((Xint - 1) * 4, (Yint - 1) * 4) / 4)	 + (Noise((Xint - 1) * 8, (Yint - 1) * 8) / 8);
	double noise02 = Noise(Xint + 1, Yint - 1)	+ (Noise((Xint + 1) * 2, (Yint - 1) * 2) / 2)	+ (Noise((Xint + 1) * 4, (Yint - 1) * 4) / 4)	 +(Noise((Xint + 1) * 8, (Yint - 1) * 8) / 8);
	double noise03 = Noise(Xint - 1, Yint + 1)	+ (Noise((Xint - 1) * 2, (Yint + 1) * 2) / 2)	+ (Noise((Xint - 1) * 4, (Yint + 1) * 4) / 4)	 +(Noise((Xint - 1) * 8, (Yint + 1) * 8) / 8);
	double noise04 = Noise(Xint + 1, Yint + 1)	+ (Noise((Xint + 1) * 2, (Yint + 1) * 2) / 2)	+ (Noise((Xint + 1) * 4, (Yint + 1) * 4) / 4)	 +(Noise((Xint + 1) * 8, (Yint + 1) * 8) / 8);
	double noise05 = Noise(Xint - 1, Yint)		+ (Noise((Xint - 1) * 2, (Yint) * 2) / 2)		+ (Noise((Xint - 1) * 4, (Yint) * 4) / 4)		 +(Noise((Xint - 1) * 8, (Yint)* 8) / 8);
	double noise06 = Noise(Xint + 1, Yint)		+ (Noise((Xint + 1) * 2, (Yint) * 2) / 2)		+ (Noise((Xint + 1) * 4, (Yint) * 4) / 4)		 +(Noise((Xint + 1) * 8, (Yint)* 8) / 8);
	double noise07 = Noise(Xint, Yint - 1)		+ (Noise((Xint) * 2, (Yint - 1)* 2) / 2)		+ (Noise((Xint) * 4, (Yint - 1)* 4) / 4)		 +(Noise((Xint)* 8, (Yint - 1) * 8) / 8);
	double noise08 = Noise(Xint, Yint + 1)		+ (Noise((Xint) * 2, (Yint + 1)* 2) / 2)		+ (Noise((Xint) * 4, (Yint + 1)* 4) / 4)		 +(Noise((Xint)* 8, (Yint + 1) * 8) / 8);
	double noise09 = Noise(Xint, Yint)			+ (Noise((Xint)* 2, (Yint) * 2) / 2)			+ (Noise((Xint)* 4, (Yint) * 4) / 4)			 +(Noise((Xint)* 8, (Yint)* 8) / 8);
												
	double noise12 = Noise(Xint + 2, Yint - 1)	+ (Noise((Xint + 2)* 2, (Yint - 1)* 2) / 2)		+ (Noise((Xint + 2)* 4, (Yint - 1)* 4) / 4)		 +(Noise((Xint + 2) * 8, (Yint - 1) * 8) / 8);
	double noise14 = Noise(Xint + 2, Yint + 1)	+ (Noise((Xint + 2)* 2, (Yint + 1)* 2) / 2)		+ (Noise((Xint + 2)* 4, (Yint + 1)* 4) / 4)		 +(Noise((Xint + 2) * 8, (Yint + 1) * 8) / 8);
	double noise16 = Noise(Xint + 2, Yint)		+ (Noise((Xint+ 2)* 2, (Yint)* 2) / 2)			+ (Noise((Xint+ 2)* 4, (Yint)* 4) / 4)			 +(Noise((Xint + 2) * 8, (Yint)* 8) / 8);
																																	 
	double noise23 = Noise(Xint - 1, Yint + 2)	+ (Noise((Xint - 1)* 2, (Yint + 2)* 2) / 2)		+ (Noise((Xint - 1)* 4, (Yint + 2)* 4) / 4)		 +(Noise((Xint - 1) * 8, (Yint + 2) * 8) / 8);
	double noise24 = Noise(Xint + 1, Yint + 2)	+ (Noise((Xint + 2)* 2, (Yint + 2)* 2) / 2)		+ (Noise((Xint + 2)* 4, (Yint + 2)* 4) / 4)		 +(Noise((Xint + 2) * 8, (Yint + 2) * 8) / 8);
	double noise28 = Noise(Xint, Yint + 2)		+ (Noise((Xint)* 2, (Yint + 2)* 2) / 2)			+ (Noise((Xint)* 4, (Yint + 2)* 4) / 4)			 +(Noise((Xint)* 8, (Yint + 2) * 8) / 8);
												;//																					 
	double noise34 = Noise(Xint + 2, Yint + 2)	+ (Noise((Xint + 2)* 2, (Yint + 2)* 2) / 2)		+ (Noise((Xint + 2)* 4, (Yint + 2)* 4) / 4)		 +(Noise((Xint + 2) * 8, (Yint + 2) * 8) / 8);


	//find the noise values of the four corners
	double x0y0 = 0.0625*(noise01 + noise02 + noise03 + noise04) + 0.125*(noise05 + noise06 + noise07 + noise08) + 0.25*(noise09);
	double x1y0 = 0.0625*(noise07 + noise12 + noise08 + noise14) + 0.125*(noise09 + noise16 + noise02 + noise04) + 0.25*(noise06);
	double x0y1 = 0.0625*(noise05 + noise06 + noise23 + noise24) + 0.125*(noise03 + noise04 + noise09 + noise28) + 0.25*(noise08);
	double x1y1 = 0.0625*(noise09 + noise16 + noise28 + noise34) + 0.125*(noise08 + noise14 + noise06 + noise24) + 0.25*(noise04);

	//interpolate between those values according to the x and y fractions
	double v1 = Interpolate(x0y0, x1y0, Xfrac); //interpolate in x direction (y)
	double v2 = Interpolate(x0y1, x1y1, Xfrac); //interpolate in x direction (y+1)
	double fin = Interpolate(v1, v2, Yfrac);  //interpolate in y direction

	return fin;
}

double PerlinNoise::Interpolate(double x, double y, double a) const
{
	double negA = 1.0 - a;
	double negASqr = negA * negA;
	double fac1 = 3.0 * (negASqr)-2.0 * (negASqr * negA);
	double aSqr = a * a;
	double fac2 = 3.0 * aSqr - 2.0 * (aSqr * a);

	return x * fac1 + y * fac2; //add the weighted factors
}

double PerlinNoise::Noise(int x, int y) const
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff; //Max int value
	return 1.0 - double(t) * 0.931322574615478515625e-9;/// 1073741824.0);
}
