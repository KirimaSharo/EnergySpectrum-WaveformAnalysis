
#include "stdafx.h"

#include"AnalysisLibrary.h"

#include"math.h"
#include"algorithm"
#include"Calculate.h"
#define DLL_A _declspec(dllexport)
#define PI 3.1415926535898

struct WRC
{
	Analysis::WaveResult WR;
	WRC* Next;
};


inline void swap(double &a, double &b)
{
	double t;
	t = a;
	a = b;
	b = t;
}

void bitrp(double xreal[], double ximag[], int n)
{
	// 位反转置换 Bit-reversal Permutation
	int i, j, a, b, p;

	for (i = 1, p = 0; i < n; i *= 2)
	{
		p++;
	}
	for (i = 0; i < n; i++)
	{
		a = i;
		b = 0;
		for (j = 0; j < p; j++)
		{
			b = (b << 1) + (a & 1);    // b = b * 2 + a % 2;
			a >>= 1;        // a = a / 2;
		}
		if (b > i)
		{
			swap(xreal[i], xreal[b]);
			swap(ximag[i], ximag[b]);
		}
	}
}

void FFTCore(double xreal[], double ximag[], int n)
{
	// 快速傅立叶变换，将复数 x 变换后仍保存在 x 中，xreal, ximag 分别是 x 的实部和虚部
	double treal, timag, ureal, uimag, arg;
	double* wreal = new double[n / 2];
	double* wimag = new double[n / 2];
	int m, k, j, t, index1, index2;

	bitrp(xreal, ximag, n);

	// 计算 1 的前 n / 2 个 n 次方根的共轭复数 W'j = wreal [j] + i * wimag [j] , j = 0, 1, ... , n / 2 - 1
	arg = -2 * PI / n;
	treal = cos(arg);
	timag = sin(arg);
	wreal[0] = 1.0;
	wimag[0] = 0.0;
	for (j = 1; j < n / 2; j++)
	{
		wreal[j] = wreal[j - 1] * treal - wimag[j - 1] * timag;
		wimag[j] = wreal[j - 1] * timag + wimag[j - 1] * treal;
	}

	for (m = 2; m <= n; m *= 2)
	{
		for (k = 0; k < n; k += m)
		{
			for (j = 0; j < m / 2; j++)
			{
				index1 = k + j;
				index2 = index1 + m / 2;
				t = n * j / m;    // 旋转因子 w 的实部在 wreal [] 中的下标为 t
				treal = wreal[t] * xreal[index2] - wimag[t] * ximag[index2];
				timag = wreal[t] * ximag[index2] + wimag[t] * xreal[index2];
				ureal = xreal[index1];
				uimag = ximag[index1];
				xreal[index1] = ureal + treal;
				ximag[index1] = uimag + timag;
				xreal[index2] = ureal - treal;
				ximag[index2] = uimag - timag;
			}
		}
	}
	delete wreal, wimag;
}

void IFFTCore(double xreal[], double ximag[], int n)
{
	// 快速傅立叶逆变换
	double treal, timag, ureal, uimag, arg;
	double* wreal = new double[n / 2];
	double* wimag = new double[n / 2];
	int m, k, j, t, index1, index2;

	bitrp(xreal, ximag, n);

	// 计算 1 的前 n / 2 个 n 次方根 Wj = wreal [j] + i * wimag [j] , j = 0, 1, ... , n / 2 - 1
	arg = 2 * PI / n;
	treal = cos(arg);
	timag = sin(arg);
	wreal[0] = 1.0;
	wimag[0] = 0.0;
	for (j = 1; j < n / 2; j++)
	{
		wreal[j] = wreal[j - 1] * treal - wimag[j - 1] * timag;
		wimag[j] = wreal[j - 1] * timag + wimag[j - 1] * treal;
	}

	for (m = 2; m <= n; m *= 2)
	{
		for (k = 0; k < n; k += m)
		{
			for (j = 0; j < m / 2; j++)
			{
				index1 = k + j;
				index2 = index1 + m / 2;
				t = n * j / m;    // 旋转因子 w 的实部在 wreal [] 中的下标为 t
				treal = wreal[t] * xreal[index2] - wimag[t] * ximag[index2];
				timag = wreal[t] * ximag[index2] + wimag[t] * xreal[index2];
				ureal = xreal[index1];
				uimag = ximag[index1];
				xreal[index1] = ureal + treal;
				ximag[index1] = uimag + timag;
				xreal[index2] = ureal - treal;
				ximag[index2] = uimag - timag;
			}
		}
	}

	for (j = 0; j < n; j++)
	{
		xreal[j] /= n;
		ximag[j] /= n;
	}
	delete wreal, wimag;
}

namespace Analysis
{
	double* WaveForm::CalibrateX(const AnalysisParam& A)
	{
		double t = 0;
		int err;
		double* ret = new double[this->TotalSample];
		for (int i = 0;i < this->TotalSample;i++)
		{
			EvaluateExpressionX(A.WaveCalibrationX, t, i, err);
			ret[i] = t;
		}
		return ret;
	}

	double* WaveForm::CalibrateY(const AnalysisParam& A)
	{
		double t = 0;
		int err;
		double* ret = new double[this->TotalSample];
		for (int i = 0;i < this->TotalSample;i++)
		{
			EvaluateExpressionX(A.WaveCalibrationY, t, this->SampleValue[i], err);
			ret[i] = t;
		}
		return ret;
	}
	WaveForm::WaveForm()
	{
		this->AnalysisResultLength = 0;
		this->AnalysisResult = nullptr;
		this->FFTResult = nullptr;
		this->SampleValue = nullptr;
		this->TotalSample = 0;
	}

	WaveForm::WaveForm(const WaveForm& c)
	{
		this->AnalysisResult = nullptr;
		this->SampleValue = nullptr;
		this->FFTResult = nullptr;
		this->AnalysisResultLength = c.AnalysisResultLength;
		if (c.AnalysisResultLength >= 0)
		{
			this->AnalysisResult = new WaveResult[this->AnalysisResultLength];
			for (int i = 0;i < c.AnalysisResultLength;i++)
			{
				this->AnalysisResult[i] = c.AnalysisResult[i];
			}
		}

		this->TotalSample = c.TotalSample;
		if (this->TotalSample > 0)
		{
			this->SampleValue = new double[this->TotalSample];
			for (int i = 0;i < c.TotalSample;i++)
			{
				this->SampleValue[i] = c.SampleValue[i];
			}
			if (c.FFTResult != nullptr)
			{
				this->FFTResult = new double[this->TotalSample];
				for (int i = 0;i < c.TotalSample;i++)
				{
					this->FFTResult[i] = c.FFTResult[i];
				}
			}
		}
		this->UploadTime = c.UploadTime;
	}

	WaveForm& WaveForm::operator =(const WaveForm& c)
	{

		delete this->AnalysisResult, this->SampleValue, this->FFTResult;
		this->AnalysisResult = nullptr;
		this->SampleValue = nullptr;
		this->FFTResult = nullptr;

		this->AnalysisResultLength = c.AnalysisResultLength;
		if (c.AnalysisResultLength >= 0)
		{
			this->AnalysisResult = new WaveResult[this->AnalysisResultLength];
			for (int i = 0;i < c.AnalysisResultLength;i++)
			{
				this->AnalysisResult[i] = c.AnalysisResult[i];
			}
		}
		
		this->TotalSample = c.TotalSample;
		if (this->TotalSample > 0)
		{
			this->SampleValue = new double[this->TotalSample];
			for (int i = 0;i < c.TotalSample;i++)
			{
				this->SampleValue[i] = c.SampleValue[i];
			}
			if (c.FFTResult != nullptr)
			{
				this->FFTResult = new double[this->TotalSample];
				for (int i = 0;i < c.TotalSample;i++)
				{
					this->FFTResult[i] = c.FFTResult[i];
				}
			}
		}
		this->UploadTime = c.UploadTime;

		return *this;
	}

	WaveForm::~WaveForm()
	{

		delete this->AnalysisResult, this->SampleValue, this->FFTResult;

		this->AnalysisResult = nullptr;
		this->SampleValue = nullptr;
		this->FFTResult = nullptr;
		
	}

	bool WaveForm::Analysis(AnalysisParam A)
	{
		AnalysisResultLength = 0;
		WRC*init = new WRC;
		WRC*cur = init;
		init->Next = nullptr;
		double start, end;
		start = -1;
		end = -1;
		double* spv;
		double* spt;
		spv = CalibrateY(A);
		spt = CalibrateX(A);
		AnalysisResultLength = 0;
		if (spv[0] > A.WaveThreshold)
		{
			start = spt[0];
			cur->WR.Max = -1;
		}
		for (int i = 1;i < TotalSample - 1;i++)
		{
			if (start == -1)
			{
				if (spv[i - 1]<= A.WaveThreshold&&spv[i] > A.WaveThreshold)
				{
					start = spt[i - 1] + (A.WaveThreshold - spv[i - 1]) / (spv[i] - spv[i - 1]);
					cur->WR.Max = -1;
				}
			}
			else if (spv[i - 1] > A.WaveThreshold&&spv[i] <= A.WaveThreshold)
			{
				end = spt[i - 1] + (spv[i - 1] - A.WaveThreshold) / (spv[i - 1] - spv[i]);
			}

			if (start >= 0 && end < 0)
			{
				if (cur->WR.Max < spv[i])
				{
					cur->WR.TimeofMax = spt[i] - 0.5 + (spv[i + 1] - spv[i]) / (spv[i] + spv[i - 1] - 2 * spv[i]);
					cur->WR.Max = spv[i] + (cur->WR.TimeofMax - spt[i] + 0.5)*(spv[i] - spv[i - 1]);
				}
			}

			if (i == TotalSample - 2 && end < 0)
			{
				end = TotalSample - 1;
			}

			if (start >= 0 && end > start)
			{
				cur->WR.RiseTime = 0;
				for (int j = cur->WR.TimeofMax;j >= 0 && j < TotalSample;j--)
				{
					if (spv[j] < 0.9*cur->WR.Max&&cur->WR.RiseTime == 0)
					{
						cur->WR.RiseTime = (0.9*cur->WR.Max - spv[j]) / (spv[j + 1] - spv[j]) + spt[j];
					}
					if (spv[j] < 0.1*cur->WR.Max&&cur->WR.RiseTime != 0)
					{
						cur->WR.RiseTime -= (0.1*cur->WR.Max - spv[j]) / (spv[j + 1] - spv[j]) + spt[j];
						break;
					}
				}
				cur->WR.FallTime = 0;
				for (int j = cur->WR.TimeofMax;j >= 0 && j < TotalSample;j++)
				{
					if (spv[j] < 0.9*cur->WR.Max&&cur->WR.RiseTime == 0)
					{
						cur->WR.FallTime = spt[TotalSample - 1] - (0.9*cur->WR.Max - spv[j]) / (spv[j - 1] - spv[j]) + spt[j];
					}
					if (spv[j] < 0.1*cur->WR.Max&&cur->WR.RiseTime != 0)
					{
						cur->WR.FallTime = cur->WR.FallTime + (0.1*cur->WR.Max - spv[j]) / (spv[j - 1] - spv[j]) + spt[j] - spt[TotalSample - 1];
						break;
					}
				}
				cur->WR.WaveWidth = spt[min(TotalSample - 1, (int)end)] - spt[max(0, (int)start)];

				cur->WR.Area = 0;
				for (int j = max(0, (int)start);j < min(TotalSample - 1, (int)end);j++)
				{
					cur->WR.Area += spv[j];
				}

				end = -1;
				start = -1;
				cur->Next = new WRC;
				cur = cur->Next;
				cur->Next = nullptr;
				AnalysisResultLength++;
			}

			
		}


		delete AnalysisResult;
		AnalysisResult = new WaveResult[AnalysisResultLength];

		cur = init;
		for (int i = 0;i < AnalysisResultLength;i++)
		{
			AnalysisResult[i] = cur->WR;
			init = cur;
			cur = cur->Next;
			delete init;
			init = nullptr;
		}
		delete spv, spt;
		spv = nullptr;
		spt = nullptr;
		return true;
	}

	bool WaveForm::FFT(AnalysisParam A)
	{
		int i = 1;
		for (;i < TotalSample;i *= 2)
		{
		}
		if (i != TotalSample)
		{
			return false;
		}

		delete FFTResult;
		FFTResult = nullptr;

		double *ximag = new double[TotalSample];
		FFTResult = CalibrateX(A);
		for (i = 0;i < TotalSample;i++)
		{
			ximag[i] = 0;
		}
		FFTCore(FFTResult, ximag, TotalSample);
		
		for (i = 0;i < TotalSample;i++)
		{
			FFTResult[i] = sqrt(ximag[i] * ximag[i] + FFTResult[i] * FFTResult[i]);
		}
		return true;
	}

	double* WaveForm::DNL(SinWave* StandardWave, int NumberofSWave, AnalysisParam A, int& n)
	{
		double* spv;
		double* spt;
		double* level;
		n = 0;
		spv = CalibrateY(A);
		spt = CalibrateX(A);
		level = new double[TotalSample];
		for (int i = 0;i < TotalSample;i++)
		{
			bool flag = false;
			for (int j = 0;j < n;j++)
			{
				if (level[j] == spv[i])
				{
					flag = true;
					break;
				}
			}
			if (flag)
			{
				continue;
			}
			else
			{
				level[n] = spv[i];
				n++;
			}
		}

		std::sort(level, level + n - 1);
		double s;
		int *Pexp = new int[n];
		int *Pr = new int[n];
		for (int j = 0;j < n;j++)
		{
			Pexp[j] = 0;
			Pr[j] = 0;
		}
		for (int i = 0;i < TotalSample;i++)
		{
			s = 0;
			for (int j = 0;j < NumberofSWave;j++)
			{
				/*
				double w = 2 * PI*StandardWave[j].Freq;
				double wt = w * spt[i];
				double wf = wt + StandardWave[j].Phase;
				double sn = sin(wf);
				double sdf = StandardWave[j].Amp;
				double ss= sdf*sn;
				s += ss;
				*/
				s += StandardWave[j].Amp*sin(StandardWave[j].Phase + 2 * PI*StandardWave[j].Freq*spt[i]);
			}
			for (int j = 0;j < n - 1;j++)
			{
				if (s < 0.5*(level[j] + level[j + 1]))
				{
					Pexp[j]++;
					break;
				}
				if (j == n - 2)
				{
					Pexp[n - 1]++;
				}
			}
			for (int j = 0;j < n - 1;j++)
			{
				if (spv[i] < 0.5*(level[j] + level[j + 1]))
				{
					Pr[j]++;
					break;
				}
				if (j == n - 2)
				{
					Pr[n - 1]++;
				}
			}
		}
		double* ret = new double[n];
		for (int j = 0;j < n;j++)
		{
			if (Pexp[j] != 0)
			{
				ret[j] = (double)Pr[j] / Pexp[j] - 1;
			}
			else
			{
				//Log Error
				ret[j] = 0;
			}
		}
		
		delete spv, spt, Pr, Pexp, level;
		spv = nullptr;
		spt = nullptr;
		Pexp = nullptr;
		level = nullptr;
		Pr = nullptr;
		return ret;
	}

	double* WaveForm::INL(SinWave* StandardWave, int NumberofSWave, AnalysisParam A, int& n)
	{
		double* ret = DNL(StandardWave, NumberofSWave, A, n);
		for (int i = 1;i < n;i++)
		{
			ret[i] += ret[i - 1];
		}
		return ret;
	}

	double WaveForm::SNR(AnalysisParam A)
	{
		int i = 1;
		for (;i < TotalSample;i *= 2)
		{
		}
		double* Frq = new double[i];
		double *ximag = new double[i];
		int n = i;
		double* x = CalibrateX(A);
		for (i = 0;i < n;i++)
		{
			if (i < TotalSample)
			{
				Frq[i] = x[i];
			}
			else
			{
				Frq[i] = 0;
			}
		}
		for (i = 0;i < TotalSample;i++)
		{
			ximag[i] = 0;
		}
		FFTCore(Frq, ximag, n);

		for (i = 0;i < n;i++)
		{
			Frq[i] = sqrt(ximag[i] * ximag[i] + Frq[i] * Frq[i]);
		}

		double max = -1;
		int fmax;
		for (int i = 0;i < n / 2;i++)
		{
			if (Frq[i] > max)
			{
				max = Frq[i];
				fmax = i;
			}
		}
		
		double Noise = 0;

		for (int i = 1;i < 9;i++)
		{
			Frq[(i*fmax) % (n / 2)] = 0;
		}
		for (int i = 0;i < n / 2;i++)
		{
			Noise += Frq[i];
		}
		delete Frq, x, ximag;
		Frq = nullptr;
		x = nullptr;
		ximag = nullptr;
		return 20*log(max / Noise);
	}
	double WaveForm::SINAD(AnalysisParam A)
	{
		int i = 1;
		for (;i < TotalSample;i *= 2)
		{
		}
		double* Frq = new double[i];
		double *ximag = new double[i];
		int n = i;
		double* x = CalibrateX(A);
		for (i = 0;i < n;i++)
		{
			if (i < TotalSample)
			{
				Frq[i] = x[i];
			}
			else
			{
				Frq[i] = 0;
			}
		}
		for (i = 0;i < TotalSample;i++)
		{
			ximag[i] = 0;
		}
		FFTCore(Frq, ximag, n);

		for (i = 0;i < n;i++)
		{
			Frq[i] = sqrt(ximag[i] * ximag[i] + Frq[i] * Frq[i]);
		}

		double max = -1;
		int fmax;
		for (int i = 0;i < n / 2;i++)
		{
			if (Frq[i] > max)
			{
				max = Frq[i];
				fmax = i;
			}
		}

		double ND = 0;

		for (int i = 0;i < n / 2;i++)
		{
			ND += Frq[i];
		}
		delete Frq, x, ximag;
		Frq = nullptr;
		x = nullptr;
		ximag = nullptr;
		return 20*log(max / ND);
	}
	double WaveForm::ENOB(AnalysisParam A)
	{
		int i = 1;
		for (;i < TotalSample;i *= 2)
		{
		}
		double* Frq = new double[i];
		double *ximag = new double[i];
		int n = i;
		double* x = CalibrateX(A);
		for (i = 0;i < n;i++)
		{
			if (i < TotalSample)
			{
				Frq[i] = x[i];
			}
			else
			{
				Frq[i] = 0;
			}
		}
		for (i = 0;i < TotalSample;i++)
		{
			ximag[i] = 0;
		}
		FFTCore(Frq, ximag, n);

		for (i = 0;i < n;i++)
		{
			Frq[i] = sqrt(ximag[i] * ximag[i] + Frq[i] * Frq[i]);
		}

		double max = -1;
		int fmax;
		for (int i = 0;i < n / 2;i++)
		{
			if (Frq[i] > max)
			{
				max = Frq[i];
				fmax = i;
			}
		}

		double ND = 0;

		for (int i = 0;i < n / 2;i++)
		{
			ND += Frq[i];
		}
		delete Frq, x, ximag;
		Frq = nullptr;
		x = nullptr;
		ximag = nullptr;
		return (20 * log(max / ND) - 1.76) / 6.02;
	}
	double WaveForm::THD(AnalysisParam A)
	{
		int i = 1;
		for (;i < TotalSample;i *= 2)
		{
		}
		double* Frq = new double[i];
		double *ximag = new double[i];
		int n = i;
		double* x = CalibrateX(A);
		for (i = 0;i < n;i++)
		{
			if (i < TotalSample)
			{
				Frq[i] = x[i];
			}
			else
			{
				Frq[i] = 0;
			}
		}
		for (i = 0;i < TotalSample;i++)
		{
			ximag[i] = 0;
		}
		FFTCore(Frq, ximag, n);

		for (i = 0;i < n;i++)
		{
			Frq[i] = sqrt(ximag[i] * ximag[i] + Frq[i] * Frq[i]);
		}

		double max = -1;
		int fmax;
		for (int i = 0;i < n / 2;i++)
		{
			if (Frq[i] > max)
			{
				max = Frq[i];
				fmax = i;
			}
		}

		double Noise = 0;
		double N[9];
		N[1] = fmax;
		for (int i = 2;i < 9;i++)
		{
			N[i] = i * fmax;
			bool flag = true;
			for (int j = 1;j < i;j++)
			{
				if (N[j] == N[i])
				{
					flag = false;
				}
			}
			if (flag)
			{
				Noise += Frq[(i*fmax) % (n / 2)];
			}
		}
		delete Frq, x, ximag;
		Frq = nullptr;
		x = nullptr;
		ximag = nullptr;
		return 20 * log(max / Noise);
	}
	double WaveForm::SFDR(AnalysisParam A)
	{
		int i = 1;
		for (;i < TotalSample;i *= 2)
		{
		}
		double* Frq = new double[i];
		double *ximag = new double[i];
		int n = i;
		double* x = CalibrateX(A);
		for (i = 0;i < n;i++)
		{
			if (i < TotalSample)
			{
				Frq[i] = x[i];
			}
			else
			{
				Frq[i] = 0;
			}
		}
		for (i = 0;i < TotalSample;i++)
		{
			ximag[i] = 0;
		}
		FFTCore(Frq, ximag, n);

		for (i = 0;i < n;i++)
		{
			Frq[i] = sqrt(ximag[i] * ximag[i] + Frq[i] * Frq[i]);
		}

		double max = -1;
		double max2 = -1;
		for (int i = 0;i < n / 2;i++)
		{
			if (Frq[i] > max)
			{
				max2 = max;
				max = Frq[i];
			}
			else if (Frq[i] > max2)
			{
				max2 = Frq[i];
			}
		}
		delete Frq, x, ximag;
		Frq = nullptr;
		x = nullptr;
		ximag = nullptr;
		return log(max / max2);
	}
}