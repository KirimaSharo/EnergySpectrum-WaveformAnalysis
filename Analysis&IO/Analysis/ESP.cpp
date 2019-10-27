// AnalysisLibrary.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include"AnalysisLibrary.h"

#include"math.h"
#include"Calculate.h"
#include<Eigen/Dense>

#define PI 3.1415926535898

#define DLL_A _declspec(dllexport)
using namespace Eigen;

struct ESPRC
{
	Analysis::ESPResult ESPR;
	ESPRC* Next;
};

double Norm(double x, double U, double S)
{
	double ret = exp(-pow(x - U, 2) / (double)(2 * S * S)) / S / sqrt(2 * PI);
	return ret;
}

double X3(double x, double a0, double a1, double a2, double a3)
{
	return a0 + a1 * x + a2 * x * x + a3 * x * x * x;
}

double IntegrateX3P(double a0, double a1, double a2, double a3, double start, double end)
{
	double ret = 0;
	while (start <= end)
	{
		ret += max(X3(start, a0, a1, a2, a3), 0);
		start++;
	}
	return ret;
}

double IntegrateNorm(double U, double S, int start, int end)
{
	double ret = 0;
	while (start <= end)
	{
		ret += Norm(start, U, S);
		start++;
	}
	return ret;
}

bool ResultChoose(Analysis::ESPResult*& res, int& Len,double reslution)
{
	int i = 0, mov = 0, rlen = Len;
	for (i = 0;i < rlen;i++)
	{
		while(res[i + mov].FWHM > reslution*res[i + mov].Peak)
		{
			mov++;
			rlen--;
			if (rlen <= 0 || i >= rlen)
			{
				break;
			}
		}
		if (rlen <= 0 || i >= rlen)
		{
			break;
		}
		res[i] = res[i + mov];

	}
	Analysis::ESPResult* resc = new Analysis::ESPResult[rlen];
	for (i = 0;i < rlen;i++)
	{
		resc[i] = res[i];
	}	
	Len = rlen;
	delete res;
	res = nullptr;
	res = resc;
	return true;
}


namespace Analysis
{

	EnergySP::EnergySP()
	{
		this->TotalChannel = 0;
		this->Count = nullptr;
		this->AnalysisResultLength = 0;
		this->AnalysisResult = nullptr;
		this->SmoothResult = nullptr;
	}

	EnergySP::EnergySP(const EnergySP &c)
	{
		this->AnalysisResult = nullptr;
		this->Count = nullptr;
		this->SmoothResult = nullptr;
		this->AnalysisResultLength = c.AnalysisResultLength;
		if (this->AnalysisResultLength > 0)
		{
			this->AnalysisResult = new ESPResult[c.AnalysisResultLength];
			for (int i = 0;i < c.AnalysisResultLength;i++)
			{
				this->AnalysisResult[i] = c.AnalysisResult[i];
			}
		}
		
		this->TotalChannel = c.TotalChannel;
		if (this->TotalChannel > 0)
		{
			this->Count = new int[c.TotalChannel];
			for (int i = 0;i < c.TotalChannel;i++)
			{
				this->Count[i] = c.Count[i];
			}
			if (c.SmoothResult != nullptr)
			{
				this->SmoothResult = new double[c.TotalChannel];
				for (int i = 0;i < c.TotalChannel;i++)
				{
					this->SmoothResult[i] = c.SmoothResult[i];
				}
			}
		}
		this->UploadTime = c.UploadTime;

	}

	EnergySP::~EnergySP()
	{
		
		delete this->Count, this->AnalysisResult, this->SmoothResult;
		this->AnalysisResult = nullptr;
		this->Count = nullptr;
		this->SmoothResult = nullptr;
		
	}

	EnergySP& EnergySP::operator =(const EnergySP& c)
	{
		delete this->Count, this->AnalysisResult, this->SmoothResult;
		this->AnalysisResult = nullptr;
		this->Count = nullptr;
		this->SmoothResult = nullptr;

		this->AnalysisResultLength = c.AnalysisResultLength;
		if (this->AnalysisResultLength > 0)
		{
			this->AnalysisResult = new ESPResult[c.AnalysisResultLength];
			for (int i = 0;i < c.AnalysisResultLength;i++)
			{
				this->AnalysisResult[i] = c.AnalysisResult[i];
			}
		}

		this->TotalChannel = c.TotalChannel;
		if (this->TotalChannel > 0)
		{
			this->Count = new int[c.TotalChannel];
			for (int i = 0;i < c.TotalChannel;i++)
			{
				this->Count[i] = c.Count[i];
			}
			if (c.SmoothResult != nullptr)
			{
				this->SmoothResult = new double[c.TotalChannel];
				for (int i = 0;i < c.TotalChannel;i++)
				{
					this->SmoothResult[i] = c.SmoothResult[i];
				}
			}
		}
		this->UploadTime = c.UploadTime;
		return *this;
	}

	double* EnergySP::SmoothCore(int n, int m)
	{
		int i, j, k;
		int n1 = (n + 1) / 2;
		double *f = new double[TotalChannel];
		double *ret = new double[TotalChannel];
		double *x = new double[n];
		double y;


		for (i = 0; i < TotalChannel; i++)
		{
			ret[i] = Count[i];
		}
		for (i = 0; i < n; i++)
		{
			x[i] = 0;
		}

		for (i = 0; i < n1; i++)
		{
			x[n1 - 1 + i] = 1 + 15.0 / (n*n - 4.0) * ((n*n - 1.0) / 12.0 - i * i);
			x[n1 - 1 - i] = x[n1 - 1 + i];
		}
			

		for (int a = 0; a < m; a++)
		{

			for (i = 0; i < TotalChannel; i++)
			{
				f[i] = 0;
				y = 0;
				for (j = 0; j < n; j++)
				{
					k = i - n1 + j + 1;
					if (k >= 0 && k < TotalChannel)
					{
						f[i] = f[i] + x[j] * ret[k];
						y = y + x[j];
					}
				}
				f[i] = f[i] / y;
			}
			for (i = 0; i < TotalChannel; i++)
			{

				ret[i] = f[i];
				
			}
		}
		delete f, x;
		f = nullptr;
		x = nullptr;
		return ret;
	}

	bool EnergySP::Smooth(int n, int m)
	{
		if (TotalChannel == 0 || Count == nullptr)
			return 0;

		delete SmoothResult;

		SmoothResult = SmoothCore(n, m);

		return 1;
	}

	double* EnergySP::CalibrateX(const AnalysisParam& A)
	{
		double t = 0;
		int err;
		double* ret = new double[this->TotalChannel];
		for (int i = 0;i < this->TotalChannel;i++)
		{
			EvaluateExpressionX(A.ESPCalibrationX, t, i, err);
			ret[i] = t;
		}
		return ret;
	}

	double* EnergySP::CalibrateY(const AnalysisParam& A)
	{
		double t = 0;
		int err;
		double* ret = new double[this->TotalChannel];
		for (int i = 0;i < this->TotalChannel;i++)
		{
			EvaluateExpressionX(A.ESPCalibrationY, t, this->Count[i], err);
			ret[i] = t;
		}
		return ret;
	}

	bool EnergySP::EMCore(int& n, int start, int end, const AnalysisParam &Anp, ESPResult*& Esp)
	{
		if (n <= 0)
		{
			return false;
		}

		int i, j, k, l;
		MatrixXd Ax, Lx, Ux, c;
		Ax.resize(3, 3);
		Lx.resize(3, 3);
		Ux.resize(3, 3);
		c.resize(n, end - start + 1);
		double* b = new double[3];

		double* y = new double[3];

		for (i = 0; i < 3; i++)
			y[i] = 0;

		double c0, C;


		double* A = new double[n];
		double* U = new double[n];
		double* S = new double[n];

		double* a = new double[n];
		double* u = new double[n];
		double* s = new double[n];

		double total = 0;
		double* count = CalibrateY(Anp);
		double* channelX = CalibrateX(Anp);

		for (i = start; i <= end; i++)
		{
			total += count[i];
		}
		double* fix = SmoothCore((int)(TotalChannel / 20.0), 10);

		double unnorm = (fix[start] + fix[end]) * (end - start) / 1.8;

		unnorm = 0.5 + unnorm / total / 2;

		y[0] = (count[end] + count[start]) / 2.0;
		double sj = 0;
		for (i = start; i <= end; i++)
		{
			sj += count[i];
		}
		c0 = IntegrateX3P(y[0], y[1], y[2], 0, start, end) / sj;
		y[0] = c0 / (end - start);

		for (i = 0;i < n;i++)
		{
			U[i] = Esp[i].Peak;
			S[i] = Esp[i].FWHM / 2.335;
			A[i] = Esp[i].Area;
		}
		
		for (i = 0; i < Anp.nmax; i++)
		{	
			for (j = 0; j < n; j++)
			{		
				a[j] = 0;
				u[j] = 0;
				s[j] = 0;
			}

			for (j = 0; j < 3; j++)
			{
				b[j] = 0;
				for (k = 0; k < 3; k++)
				{
					Ax(j, k) = 0;
					Lx(j, k) = 0;
					Ux(j, k) = 0;
				}
			}

			for (j = start, k = 0; j <= end; j++, k++)
			{
				c0 = max(X3(channelX[j], y[0], y[1], y[2], 0), 0);
				C = c0;
				for (l = 0;l < n;l++)
				{
					c(l, k) = A[l] * Norm(channelX[j], U[l], S[l]);
					C = c(l, k) + C;
				}

				c0 = c0 / C;

				for (l = 0;l < n;l++)
				{
					c(l, k) = c(l, k) / C;
				}

				c0 = c0 * count[j];

				for (l = 0;l < n;l++)
				{
					a[l] += c(l, k) * count[j];
					u[l] += channelX[j] * c(l, k) * count[j];
				}

				Ax(0, 0) += 1;
				Ax(0, 1) += channelX[j];
				Ax(0, 2) += channelX[j] * channelX[j];
				Ax(1, 2) += channelX[j] * channelX[j] * channelX[j];
				Ax(2, 2) += channelX[j] * channelX[j] * channelX[j] * channelX[j];

				b[0] += c0;
				b[1] += c0 * channelX[j];
				b[2] += c0 * channelX[j] * channelX[j];
			}

			Ax(1, 0) = Ax(0, 1);
			Ax(2, 0) = Ax(0, 2);
			Ax(1, 1) = Ax(0, 2);
			Ax(2, 1) = Ax(1, 2);


			for (l = 0;l < n;l++)
			{
				u[l] = u[l] / a[l];
			}

			for (l = 0;l < n;l++)
			{
				for (j = start, k = 0; j <= end; j++, k++)
				{
					s[l] += c(l, k) * count[j] * pow(channelX[j] - u[l], 2);
				}
				s[l] = s[l] / a[l];
				s[l] = sqrt(s[l]);
				a[l] = a[l] / IntegrateNorm(u[l], s[l], start, end);
			}
			int mov = 0;
			for (l = 0;l < n;l++)
			{
				if (u[l] == NAN || u[l] == INFINITY)
				{
					return false;
				}
				
				while (s[l+mov] <= 1 || a[l+mov] <= 1)
				{
					mov++;
					n--;
					if (n <= 0)
					{
						return false;
					}
					if (l >= n)
					{
						break;
					}
				}
				if (l >= n)
				{
					break;
				}
				U[l] = u[l + mov];
				A[l] = a[l + mov];
				S[l] = s[l + mov];
			}

			for (k = 0; k < 3; k++)
			{
				for (j = k; j < 3; j++)
				{
					Ux(k, j) = Ax(k, j);
					for (l = 0; l < k; l++)
						Ux(k, j) -= Lx(k, l) * Ux(l, j);
				}

				for (l = k + 1; l < 3; l++)
				{
					Lx(l, k) = Ax(l, k);
					for (j = 0; j < k; j++)
						Lx(l, k) -= Lx(l, j) * Ux(j, k);
					Lx(l, k) = Lx(l, k) / Ux(k, k);
				}

			}

			for (l = 0; l < 3; l++)
			{
				for (j = 0; j < l; j++)
					b[l] -= Lx(l, j) * b[j];
			}

			for (l = 2; l >= 0; l--)
			{
				for (j = l + 1; j < 3; j++)
					b[l] -= Ux(l, j) * b[j];
				b[l] = b[l] / Ux(l, l);
				y[l] = b[l];
			}
			c0 = IntegrateX3P(y[0], y[1], y[2], 0, start, end);
			C = 0;
			for (l = 0;l < n;l++)
			{
				C += A[l] * IntegrateNorm(U[l], S[l], start, end);
			}

			if (c0 > (c0 + C) * unnorm)
			{
				for (j = 0; j < 3; j++)
				{
					y[j] = y[j] / c0 * C * 0.5;
				}
			}

		}

		for (i = 0;i < n;i++)
		{
			Esp[i].Peak = U[i];
			Esp[i].FWHM = S[i] * 2.335;
			Esp[i].Area = A[i];
		}

		delete U, S, A, u, s, a;

		delete b, y;

		delete channelX, count, fix;


		return true;





	}

	bool EnergySP::LMCore(int& n, int start, int end, const AnalysisParam &Anp, ESPResult*& Esp, double e1, double e2)
	{
		int Maxtime = Anp.nmax;
		if (n < 1 || end <= start || e1 < 0 || e2 < 0 || Maxtime < 1)
			return false;
		bool flag = false;
		int nb = 3 * n + 2, nc = end - start + 1;
		double v = 2;
		
		MatrixXd b, bnew, h, J, JT, A, Inb, g, f, L;
		b.resize(nb, 1);
		bnew.resize(nb, 1);
		h.resize(nb, 1);
		J.resize(nc, nb);
		JT.resize(nb, nc);
		A.resize(nb, nb);
		Inb.resize(nb, nb);
		g.resize(nb, 1);
		f.resize(nc, 1);
		L.resize(1, 1);

		
		double u = (double)nb, p, F, Fnew;

		int i, j, k, l;

		double* count = CalibrateY(Anp);
		double* channelX = CalibrateX(Anp);

		double* fix = SmoothCore(end - start / 2 / n, -1);
		for (i = 0; i < nb; i++)
			for (j = 0; j < nb; j++)
			{
				if (i == j)
					Inb(i, j) = 1;
				else
					Inb(i, j) = 0;
			}


		j = 0;
		for (i = start; i <= end; i++)
		{
			j += count[i];
		}


		for (i = 0; i < n; i++)
		{
			b(3 * i + 1, 0) = Esp[i].Peak;
			b(3 * i + 2, 0) = Esp[i].FWHM / 2.335;
			b(3 * i, 0) = Esp[i].Area;
		}
		b(3 * n + 1, 0) = (fix[end] - fix[start]) / (end - start);
		b(3 * n, 0) = fix[start] - b(3 * n + 1, 0) * start;


		for (i = 0; i < Maxtime; i++)
		{
			for (j = 0; j < nc; j++)
			{
				int x = start + j;
				f(j, 0) = -count[x];
				for (k = 0; k < n; k++)
				{
					J(j, 3 * k) = Norm(channelX[x], b(3 * k + 1, 0), b(3 * k + 2, 0));
					J(j, 3 * k + 1) = b(3 * k, 0) * J(j, 3 * k) * (channelX[x] - b(3 * k + 1, 0)) / pow(b(3 * k + 2, 0), 2);
					J(j, 3 * k + 2) = -b(3 * k, 0) * J(j, 3 * k) / b(3 * k + 2, 0) + J(j, 3 * k + 1) * (channelX[x] - b(3 * k + 1, 0)) / b(3 * k + 2, 0);
					f(j, 0) += b(3 * k, 0) * J(j, 3 * k);
				}
				J(j, 3 * n) = 1;
				J(j, 3 * n + 1) = channelX[x];
				f(j, 0) += channelX[x] * b(3 * n + 1, 0) + b(3 * n, 0);

			}
			JT = J.transpose();
			f = -f;
			int Judge = 0;
		Loop: 
			if (Judge > Anp.nmax)
			{
				return false;
			}
			Judge++;
			A = JT * J;
			g = JT * f;
			if (g.lpNorm<Infinity>() < e1)
			{
				flag = true;
				break;
			}
			A = A + u * Inb;
			h = A.inverse() * g;
			if (h.lpNorm<Infinity>() < e2 * (b.lpNorm<Infinity>() + e2))
			{
				flag = true;
				break;
			}
			bnew = b + h;
			F = 0;
			Fnew = 0;
			for (j = 0; j < nc; j++)
			{
				int x = start + j;
				F += f(j, 0);
				for (k = 0; k < n; k++)
				{
					Fnew += bnew(3 * k, 0) * Norm(channelX[x], bnew(3 * k + 1, 0), bnew(3 * k + 2, 0));
				}
				Fnew += channelX[x] * bnew(3 * n + 1, 0) + bnew(3 * n, 0);
			}
			p = F - Fnew;
			L = -h.transpose() * JT * f - 0.5 * h.transpose() * JT * J * h;
			p = p / L(0, 0);
			if (p > 0)
			{
				b = bnew;
				u = max(1 / 3.0, 1 - pow(2 * p - 1, 3));
				v = 2;
			}
			else
			{
				u = u * v;
				v = 2 * v;
				goto Loop;
			}


		}

		for (i = 0; i < n; i++)
		{
			Esp[i].Peak = b(3 * i + 1, 0);
			Esp[i].FWHM = abs(b(3 * i + 2, 0))*2.335;
			Esp[i].Area = b(3 * i, 0);
		}
		return true;
	}

	bool EnergySP::Analysis_Derivative(AnalysisParam A)
	{
		if (Count == nullptr || TotalChannel == 0)
		{
			return false;
		}

		int min1 = 1;
		int max = -1;
		int min2 = -1;
		int n = 0;
		double* fix = SmoothCore((int)(TotalChannel / 20.0), 10);
		ESPRC *init=new ESPRC;
		ESPRC *cur =init;
		AnalysisResultLength = 0;

		for (int i = 2; i < TotalChannel - 2; i++)//3rd Dirv Limit
		{
			if (fix[i] < fix[i + 1] && fix[i - 1] >= fix[i])
			{
				if (max == -1 || (i - min1) / (double)max(n, 1) > (int)(TotalChannel / 2.0))
					min1 = i;
				else if ((fix[max] - fix[min1]) / (double)(fix[max] - fix[i]) < 2 && (fix[max] - fix[min1]) / (double)(fix[max] - fix[i]) > 0.5)
					min2 = i;
			}
			else if (fix[i] > fix[i + 1] && fix[i - 1] <= fix[i])
			{
				max = i;
			}
			if (min1 > 0 && min2 > 0)
			{
				n = 0;
				double total = 0;
				double sigma = 0;
				for (int j = min1;j < min2;j++)
				{
					total += fix[j];
				}
				total = total / (min2 - min1);
				for (int j = min1;j < min2;j++)
				{
					sigma += (fix[j] - total)*(fix[j] - total);
				}
				sigma = sigma / (min2 - min1);
				sigma = sqrt(sigma);

				if (sigma < 1 || min2 - min1 < TotalChannel / 100.0)
				{
					min1 = min2;
					min2 = -1;
					max = -1;
					continue;
				}
				for (int j = min1;j < min2;j++)
				{
					double f0, f1, f2;
					f0 = fix[j - 2] + fix[j] - 2 * fix[j - 1];
					f1 = fix[j - 1] + fix[j + 1] - 2 * fix[j];
					f2 = fix[j] + fix[j + 2] - 2 * fix[j + 1];
					if (f1 < f2 && f1 <= f0 && f1 < 0)
					{
						cur->ESPR.Peak = j - 0.5 + (f2 - f1) / (f2 + f0 - 2 * f1);
						cur->ESPR.FWHM = 0;
						for (int k = 0;k < min(j, TotalChannel - j - 2);k++)
						{
							if (fix[j - 1 + k] + fix[j + 1 + k] - 2 * fix[j + k] <= 0 && fix[j + k] + fix[j + 2 + k] - 2 * fix[j + 1 + k] > 0)
							{
								cur->ESPR.FWHM = k;
								break;
							}
							else if (fix[j - 1 - k] + fix[j + 1 - k] - 2 * fix[j - k] >= 0 && fix[j - k] + fix[j + 2 - k] - 2 * fix[j + 1 - k] < 0)
							{
								cur->ESPR.FWHM = k;
								break;
							}
							else if (fix[j - k] < 0.5*fix[j] || fix[j + k] < 0.5*fix[j])
							{
								cur->ESPR.FWHM = k/1.1675;
								break;
							}
						}
						if (cur->ESPR.FWHM > 0)
						{							
							cur->ESPR.Area = fix[j];
							for (int k = 1;k < cur->ESPR.FWHM;k++)
							{
								cur->ESPR.Area += fix[j - k];
								cur->ESPR.Area += fix[j + k];
							}
							cur->ESPR.FWHM *= 2.335;
							if (cur->ESPR.FWHM > A.ESPMaxResolution*cur->ESPR.Peak)
							{
								continue;
							}
							n++;
							cur->Next = new ESPRC;
							cur = cur->Next;
							cur->Next = nullptr;
						}
						

					}
				}

				AnalysisResultLength += n;
				min1 = min2;
				min2 = -1;
				max = -1;
			}
		}

		delete AnalysisResult;
		AnalysisResult = new ESPResult[AnalysisResultLength];
		cur = init;
		for (int i = 0;i < AnalysisResultLength;i++)
		{
			AnalysisResult[i] = cur->ESPR;
			init = cur;
			cur = cur->Next;
			delete init;
			init = nullptr;
		}
		ResultChoose(AnalysisResult, AnalysisResultLength, A.ESPMaxResolution);
		delete fix;
		fix = nullptr;
		return true;
	}

	bool EnergySP::Analysis_ExpectMaxium(AnalysisParam A) 
	{
		if (Count == nullptr || TotalChannel == 0)
		{
			return false;
		}

		int min1 = 1;
		int max = -1;
		int min2 = -1;
		int n = 0;
		double* fix = SmoothCore((int)(TotalChannel / 20.0), 10);
		ESPRC *initR = new ESPRC;
		ESPRC *curR = initR;
		curR->Next = nullptr;
		AnalysisResultLength = 0;

		for (int i = 2; i < TotalChannel - 2; i++)//3rd Dirv Limit
		{
			if (fix[i] < fix[i + 1] && fix[i - 1] >= fix[i])
			{
				if (max == -1 || (i - min1) / (double)max(n, 1) > (int)(TotalChannel / 2.0))
					min1 = i;
				else if ((fix[max] - fix[min1]) / (double)(fix[max] - fix[i]) < 2 && (fix[max] - fix[min1]) / (double)(fix[max] - fix[i]) > 0.5)
					min2 = i;
			}
			else if (fix[i] > fix[i + 1] && fix[i - 1] <= fix[i])
			{
				max = i;
				n++;
			}
			if (min1 > 0 && min2 > 0)
			{
				n = 0;
				ESPRC *init = new ESPRC;
				init->Next = nullptr;
				ESPRC *cur = init;
				double total = 0;
				double sigma = 0;
				for (int j = min1;j < min2;j++)
				{
					total += fix[j];
				}
				total = total / (min2 - min1);
				for (int j = min1;j < min2;j++)
				{
					sigma += (fix[j] - total)*(fix[j] - total);
				}
				sigma = sigma / (min2 - min1);
				sigma = sqrt(sigma);

				if (sigma < 1 || min2 - min1 < TotalChannel / 100.0)
				{
					min1 = min2;
					min2 = -1;
					max = -1;
					continue;
				}
				for (int j = min1;j < min2;j++)
				{

					double f0, f1, f2;
					f0 = fix[j - 2] + fix[j] - 2 * fix[j - 1];
					f1 = fix[j - 1] + fix[j + 1] - 2 * fix[j];
					f2 = fix[j] + fix[j + 2] - 2 * fix[j + 1];
					if (f1 < f2 && f1 <= f0 && f1 < 0)
					{
						cur->ESPR.Peak = j - 0.5 + (f2 - f1) / (f2 + f0 - 2 * f1);
						cur->ESPR.FWHM = 0;
						for (int k = 0;k < min(j, TotalChannel - j - 2);k++)
						{
							if (fix[j - 1 + k] + fix[j + 1 + k] - 2 * fix[j + k] <= 0 && fix[j + k] + fix[j + 2 + k] - 2 * fix[j + 1 + k] > 0)
							{
								cur->ESPR.FWHM = k;
								break;
							}
							else if (fix[j - 1 - k] + fix[j + 1 - k] - 2 * fix[j - k] >= 0 && fix[j - k] + fix[j + 2 - k] - 2 * fix[j + 1 - k] < 0)
							{
								cur->ESPR.FWHM = k;
								break;
							}
							else if (fix[j - k] < 0.5*fix[j] || fix[j + k] < 0.5*fix[j])
							{
								cur->ESPR.FWHM = k / 1.1675;
								break;
							}
						}
						if (cur->ESPR.FWHM > 0)
						{
							n++;
							cur->ESPR.Area = fix[j];
							for (int k = 1;k < cur->ESPR.FWHM;k++)
							{
								cur->ESPR.Area += fix[j - k];
								cur->ESPR.Area += fix[j + k];
							}
							cur->ESPR.FWHM *= 2.335;

							cur->Next = new ESPRC;
							cur = cur->Next;
							cur->Next = nullptr;
						}


					}
				}

				Analysis::ESPResult* ER = new Analysis::ESPResult[n];
				cur = init;
				int n1 = 0;
				for (int j = 0;j < n;j++)
				{
					ESPRC* del = cur;
					ER[j] = cur->ESPR;
					cur = cur->Next;
					n1++;
					delete del;
					del = nullptr;
				}
				bool err = EMCore(n1, min1, min2, A, ER);

				if (!err)
				{
					min1 = min2;
					min2 = -1;
					max = -1;
					continue;
				}

				for (int j = 0;j < n1;j++)
				{
					curR->ESPR = ER[j];

					curR->Next = new ESPRC;
					curR = curR->Next;
					curR->Next = nullptr;
				}


				AnalysisResultLength += n1;
				min1 = min2;
				min2 = -1;
				max = -1;
			}
		}


		delete AnalysisResult;
		AnalysisResult = new ESPResult[AnalysisResultLength];
		curR = initR;
		for (int i = 0;i < AnalysisResultLength;i++)
		{
			AnalysisResult[i] = curR->ESPR;
			initR = curR;
			curR = curR->Next;
			delete initR;
			initR = nullptr;
		}
		ResultChoose(AnalysisResult, AnalysisResultLength, A.ESPMaxResolution);
		delete fix;
		fix = nullptr;
		return true;
	}

	bool EnergySP::Analysis_LevenbergMarquardt(AnalysisParam A)
	{
		if (Count == nullptr || TotalChannel == 0)
		{
			return false;
		}

		int min1 = 1;
		int max = -1;
		int min2 = -1;
		int n = 0;
		double* fix = SmoothCore((int)(TotalChannel / 20.0), 10);
		ESPRC *initR = new ESPRC;
		ESPRC *curR = initR;
		curR->Next = nullptr;
		AnalysisResultLength = 0;

		for (int i = 2; i < TotalChannel - 2; i++)//3rd Dirv Limit
		{
			if (fix[i] < fix[i + 1] && fix[i - 1] >= fix[i])
			{
				if (max == -1 || (i - min1) / (double)max(n, 1) > (int)(TotalChannel / 2.0))
					min1 = i;
				else if ((fix[max] - fix[min1]) / (double)(fix[max] - fix[i]) < 2 && (fix[max] - fix[min1]) / (double)(fix[max] - fix[i]) > 0.5)
					min2 = i;
			}
			else if (fix[i] > fix[i + 1] && fix[i - 1] <= fix[i])
			{
				max = i;
				n++;
			}
			if (min1 > 0 && min2 > 0)
			{
				n = 0;
				ESPRC *init = new ESPRC;
				init->Next = nullptr;
				ESPRC *cur = init;
				double total = 0;
				double sigma = 0;
				for (int j = min1;j < min2;j++)
				{
					total += fix[j];
				}
				total = total / (min2 - min1);
				for (int j = min1;j < min2;j++)
				{
					sigma += (fix[j] - total)*(fix[j] - total);
				}
				sigma = sigma / (min2 - min1);
				sigma = sqrt(sigma);

				if (sigma < 1 || min2 - min1 < TotalChannel / 100.0)
				{
					min1 = min2;
					min2 = -1;
					max = -1;
					continue;
				}
				for (int j = min1;j < min2;j++)
				{

					double f0, f1, f2;
					f0 = fix[j - 2] + fix[j] - 2 * fix[j - 1];
					f1 = fix[j - 1] + fix[j + 1] - 2 * fix[j];
					f2 = fix[j] + fix[j + 2] - 2 * fix[j + 1];
					if (f1 < f2 && f1 <= f0 && f1 < 0)
					{
						cur->ESPR.Peak = j - 0.5 + (f2 - f1) / (f2 + f0 - 2 * f1);
						cur->ESPR.FWHM = 0;
						for (int k = 0;k < min(j, TotalChannel - j - 2);k++)
						{
							if (fix[j - 1 + k] + fix[j + 1 + k] - 2 * fix[j + k] <= 0 && fix[j + k] + fix[j + 2 + k] - 2 * fix[j + 1 + k] > 0)
							{
								cur->ESPR.FWHM = k;
								break;
							}
							else if (fix[j - 1 - k] + fix[j + 1 - k] - 2 * fix[j - k] >= 0 && fix[j - k] + fix[j + 2 - k] - 2 * fix[j + 1 - k] < 0)
							{
								cur->ESPR.FWHM = k;
								break;
							}
							else if (fix[j - k] < 0.5*fix[j] || fix[j + k] < 0.5*fix[j])
							{
								cur->ESPR.FWHM = k / 1.1675;
								break;
							}
						}
						if (cur->ESPR.FWHM > 0)
						{
							n++;
							cur->ESPR.Area = fix[j];
							for (int k = 1;k < cur->ESPR.FWHM;k++)
							{
								cur->ESPR.Area += fix[j - k];
								cur->ESPR.Area += fix[j + k];
							}
							cur->ESPR.FWHM *= 2.335;

							cur->Next = new ESPRC;
							cur = cur->Next;
							cur->Next = nullptr;
						}


					}
				}

				Analysis::ESPResult* ER = new Analysis::ESPResult[n];
				cur = init;
				int n1 = 0;
				for (int j = 0;j < n;j++)
				{
					ESPRC* del = cur;
					ER[j] = cur->ESPR;
					cur = cur->Next;
					n1++;
					delete del;
					del = nullptr;
				}
				bool err = LMCore(n1, min1, min2, A, ER, 0, 0);

				if (!err)
				{
					min1 = min2;
					min2 = -1;
					max = -1;
					continue;
				}

				for (int j = 0;j < n1;j++)
				{
					curR->ESPR = ER[j];

					curR->Next = new ESPRC;
					curR = curR->Next;
					curR->Next = nullptr;
				}


				AnalysisResultLength += n1;
				min1 = min2;
				min2 = -1;
				max = -1;
			}
		}


		delete AnalysisResult;
		AnalysisResult = new ESPResult[AnalysisResultLength];
		curR = initR;
		for (int i = 0;i < AnalysisResultLength;i++)
		{
			AnalysisResult[i] = curR->ESPR;
			initR = curR;
			curR = curR->Next;
			delete initR;
			initR = nullptr;
		}
		ResultChoose(AnalysisResult, AnalysisResultLength, A.ESPMaxResolution);
		delete fix;
		fix = nullptr;
		return true;
	}

	bool EnergySP::Analysis_De_LM_EM(AnalysisParam A)
	{
		if (Count == nullptr || TotalChannel == 0)
		{
			return false;
		}

		int min1 = 1;
		int max = -1;
		int min2 = -1;
		int n = 0;
		double* fix = SmoothCore((int)(TotalChannel / 20.0), 10);
		ESPRC *initR = new ESPRC;
		ESPRC *curR = initR;
		curR->Next = nullptr;
		AnalysisResultLength = 0;

		for (int i = 2; i < TotalChannel - 2; i++)//3rd Dirv Limit
		{
			if (fix[i] < fix[i + 1] && fix[i - 1] >= fix[i])
			{
				if (max == -1 || (i - min1) / (double)max(n, 1) > (int)(TotalChannel / 2.0))
					min1 = i;
				else if ((fix[max] - fix[min1]) / (double)(fix[max] - fix[i]) < 2 && (fix[max] - fix[min1]) / (double)(fix[max] - fix[i]) > 0.5)
					min2 = i;
			}
			else if (fix[i] > fix[i + 1] && fix[i - 1] <= fix[i])
			{
				max = i;
				n++;
			}
			if (min1 > 0 && min2 > 0)
			{
				n = 0;
				ESPRC *init = new ESPRC;
				init->Next = nullptr;
				ESPRC *cur = init;
				double total = 0;
				double sigma = 0;
				for (int j = min1;j < min2;j++)
				{
					total += fix[j];
				}
				total = total / (min2 - min1);
				for (int j = min1;j < min2;j++)
				{
					sigma += (fix[j] - total)*(fix[j] - total);
				}
				sigma = sigma / (min2 - min1);
				sigma = sqrt(sigma);

				if (sigma < 1 || min2 - min1 < TotalChannel / 100.0)
				{
					min1 = min2;
					min2 = -1;
					max = -1;
					continue;
				}
				for (int j = min1;j < min2;j++)
				{
					
					double f0, f1, f2;
					f0 = fix[j - 2] + fix[j] - 2 * fix[j - 1];
					f1 = fix[j - 1] + fix[j + 1] - 2 * fix[j];
					f2 = fix[j] + fix[j + 2] - 2 * fix[j + 1];
					if (f1 < f2 && f1 <= f0 && f1 < 0)
					{
						cur->ESPR.Peak = j - 0.5 + (f2 - f1) / (f2 + f0 - 2 * f1);
						cur->ESPR.FWHM = 0;
						for (int k = 0;k < min(j, TotalChannel - j - 2);k++)
						{
							if (fix[j - 1 + k] + fix[j + 1 + k] - 2 * fix[j + k] <= 0 && fix[j + k] + fix[j + 2 + k] - 2 * fix[j + 1 + k] > 0)
							{
								cur->ESPR.FWHM = k;
								break;
							}
							else if (fix[j - 1 - k] + fix[j + 1 - k] - 2 * fix[j - k] >= 0 && fix[j - k] + fix[j + 2 - k] - 2 * fix[j + 1 - k] < 0)
							{
								cur->ESPR.FWHM = k;
								break;
							}
							else if (fix[j - k] < 0.5*fix[j] || fix[j + k] < 0.5*fix[j])
							{
								cur->ESPR.FWHM = k / 1.1675;
								break;
							}
						}
						if (cur->ESPR.FWHM > 0)
						{
							n++;
							cur->ESPR.Area = fix[j];
							for (int k = 1;k < cur->ESPR.FWHM;k++)
							{
								cur->ESPR.Area += fix[j - k];
								cur->ESPR.Area += fix[j + k];
							}
							cur->ESPR.FWHM *= 2.335;

							cur->Next = new ESPRC;
							cur = cur->Next;
							cur->Next = nullptr;
						}


					}
				}

				Analysis::ESPResult* ER = new Analysis::ESPResult[n];
				cur = init;
				int n1 = 0;
				for (int j = 0;j < n;j++)
				{
					ESPRC* del = cur;
					ER[j] = cur->ESPR;
					cur = cur->Next;
					n1++;
					delete del;
					del = nullptr;
				}
				bool err = LMCore(n1, min1, min2, A, ER, 0, 0);
				err = err && EMCore(n1, min1, min2, A, ER);

				if (!err)
				{
					min1 = min2;
					min2 = -1;
					max = -1;
					continue;
				}

				for (int j = 0;j < n1;j++)
				{
					curR->ESPR = ER[j];

					curR->Next = new ESPRC;
					curR = curR->Next;
					curR->Next = nullptr;
				}


				AnalysisResultLength += n1;
				min1 = min2;
				min2 = -1;
				max = -1;
			}
		}


		delete AnalysisResult;
		AnalysisResult = new ESPResult[AnalysisResultLength];
		curR = initR;
		for (int i = 0;i < AnalysisResultLength;i++)
		{
			AnalysisResult[i] = curR->ESPR;
			initR = curR;
			curR = curR->Next;
			delete initR;
			initR = nullptr;
		}
		ResultChoose(AnalysisResult, AnalysisResultLength, A.ESPMaxResolution);
		delete fix;
		fix = nullptr;
		return true;
	}

	//Not Finished
	std::string EnergySP::RadioisotopeIdentificate(AnalysisParam A)
	{
		if (AnalysisResult == nullptr)
		{
			return NULL;
		}
		return NULL;//////////////////////////
	}
}

