#pragma once

#ifndef DLL_A
#define DLL_A _declspec(dllimport)
#endif

#include <string>
#include <ctime>

namespace Analysis
{
	//Energy Spectrum Analysis Result
	struct ESPResult
	{
		double Peak;
		double FWHM;
		double Area;
	};

	//Waveform Analysis Result
	struct WaveResult
	{
		double WaveWidth;
		double RiseTime;
		double Max;
		double TimeofMax;
		double FallTime;
		double Area; 
	};

	//Parameters to determine a Sine waveform
	struct SinWave
	{
		double Amp;
		double Freq;
		//Phase Ranged from 0 to 2Pi
		double Phase;	
	};

	//Some Common Parameters for Analysis Argorithm
	struct AnalysisParam
	{
		std::string ESPCalibrationX;
		std::string ESPCalibrationY;
		std::string WaveCalibrationX;
		std::string WaveCalibrationY;
		double WaveThreshold;
		int nmax;
		double ESPMaxResolution;
	};

	class DLL_A EnergySP
	{
	public:
		int TotalChannel;
		int *Count;

		int AnalysisResultLength;
		ESPResult *AnalysisResult;
		double *SmoothResult;//Will Not be Saved into files

		tm UploadTime;

		bool Smooth(int n, int m);

		bool Analysis_Derivative(AnalysisParam A);
		bool Analysis_ExpectMaxium(AnalysisParam A);
		bool Analysis_LevenbergMarquardt(AnalysisParam A);
		bool Analysis_De_LM_EM(AnalysisParam A);

		std::string RadioisotopeIdentificate(AnalysisParam A);

		EnergySP();
		EnergySP(const EnergySP &c);
		EnergySP& operator =(const EnergySP& c);
		~EnergySP();

	private:
		double* CalibrateX(const AnalysisParam& A);
		double* CalibrateY(const AnalysisParam& A);
		double* SmoothCore(int n, int m);
		bool EMCore(int& n, int start, int end, const AnalysisParam& Anp, ESPResult*& Esp);
		bool LMCore(int& n, int start, int end, const AnalysisParam& Anp, ESPResult*& Esp, double e1, double e2);
	};

	class DLL_A WaveForm
	{
	public:
		int TotalSample;
		double *SampleValue;


		int AnalysisResultLength;
		WaveResult *AnalysisResult;
		double *FFTResult;

		tm UploadTime;

		bool Analysis(AnalysisParam A);

		bool FFT(AnalysisParam A);

		SinWave* SinFit(AnalysisParam Anp);

		double* DNL(SinWave* StandardWave, int NumberofSWave, AnalysisParam A, int& n);

		double* INL(SinWave* StandardWave, int NumberofSWave, AnalysisParam A, int& n);

		double SNR(AnalysisParam A);

		double SINAD(AnalysisParam A);

		double ENOB(AnalysisParam A);

		double SFDR(AnalysisParam A);

		double THD(AnalysisParam A);

		WaveForm();
		WaveForm(const WaveForm& c);
		WaveForm& operator =(const WaveForm& c);
		~WaveForm();
	private:
		double* CalibrateX(const AnalysisParam& A);
		double* CalibrateY(const AnalysisParam& A);
	};

	class DLL_A Event
	{
	public:
		double Amp;
		double RiseTime;
		double ArrivialTime;
		double FallTime;
		double Area;
		unsigned char source;

		tm UploadTime;

		Event();
		Event(const Event& c);
		Event& operator =(const Event& c);
		~Event();

	};
}
