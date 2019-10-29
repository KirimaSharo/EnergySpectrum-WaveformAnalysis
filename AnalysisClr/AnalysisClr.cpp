// AnalysisClr.cpp : 定义 DLL 应用程序的导出函数。
//

#include "AnalysisClr.h"
void AnalysisClr::ESPResult::TSget(Analysis::ESPResult& source)
{
	Peak = source.Peak;
	FWHM = source.FWHM;
	Area = source.Area;
}

void AnalysisClr::WaveResult::TSget(Analysis::WaveResult& source)
{
	Wavewidth = source.WaveWidth;
	RiseTime = source.RiseTime;
	Max = source.Max;
	TimeofMax = source.TimeofMax;
	FallTime = source.FallTime;
	Area = source.Area;
}

void AnalysisClr::SinWave::TSget(Analysis::SinWave& source)
{
	Amp = source.Amp;
	Freq = source.Freq;
	Phase = source.Phase;
}

void AnalysisClr::SinWave::TSput(Analysis::SinWave& dest)
{
	dest.Amp = Amp;
	dest.Freq = Freq;
	dest.Phase = Phase;
}

void AnalysisClr::AnalysisParam::TSput(Analysis::AnalysisParam& dest)
{
	const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(ESPCalibrationX);
	dest.ESPCalibrationX = tmp;
	const char* tmp2 = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(ESPCalibrationY);
	dest.ESPCalibrationY = tmp2;
	const char* tmp3 = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(WaveCalibrationX);
	dest.WaveCalibrationX = tmp3;
	const char* tmp4 = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(WaveCalibrationY);
	dest.WaveCalibrationY = tmp4;
	dest.nmax = nmax;
	dest.ESPMaxResolution = ESPMaxResolution;
	dest.WaveThreshold = WaveThreshold;
}

bool AnalysisClr::EnergySP::Analysis_Derivative(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->Analysis_Derivative(A1);
}
bool AnalysisClr::EnergySP::Analysis_ExpectMaxium(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->Analysis_ExpectMaxium(A1);
}
bool AnalysisClr::EnergySP::Analysis_LevenbergMarquardt(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->Analysis_LevenbergMarquardt(A1);
}
bool AnalysisClr::EnergySP::Analysis_De_LM_EM(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->Analysis_De_LM_EM(A1);
}

System::String^ AnalysisClr::EnergySP::RadioisotopeIdentificate(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return gcnew System::String(CORE->RadioisotopeIdentificate(A1).c_str());
}

AnalysisClr::EnergySP::EnergySP()
{
	CORE = new Analysis::EnergySP;
}
AnalysisClr::EnergySP::EnergySP(const EnergySP^ c)
{
	CORE = new Analysis::EnergySP(*c->CORE);
}
AnalysisClr::EnergySP^ AnalysisClr::EnergySP:: operator =(const EnergySP^ c)
{
	delete CORE;
	CORE = new Analysis::EnergySP(*c->CORE);
	return this;
}
AnalysisClr::EnergySP::~EnergySP()
{
	delete CORE;
}

bool AnalysisClr::WaveForm::Analysis(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->Analysis(A1);
}

bool AnalysisClr::WaveForm::FFT(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->FFT(A1);
}

AnalysisClr::SinWave^ AnalysisClr::WaveForm::SinFit(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	SinWave^ ret = gcnew SinWave();
	ret->TSget(*CORE->SinFit(A1));
	return ret;
}

array <System::Double>^ AnalysisClr::WaveForm::DNL(array <SinWave^>^ StandardWave, int NumberofSWave, AnalysisParam^ A)
{
	Analysis::SinWave*stdw = new Analysis::SinWave[StandardWave->Length];
	for (int i = 0;i < StandardWave->Length;i++)
	{
		StandardWave[i]->TSput(stdw[i]);
	}
	int n;
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	double*retd;
	retd = CORE->DNL(stdw, StandardWave->Length, A1, n);
	array <System::Double>^ ret = gcnew array <System::Double>(n);
	for (int i = 0;i < n;i++)
	{
		ret[i] = retd[i];
	}
	return ret;
}

array <System::Double>^ AnalysisClr::WaveForm::INL(array <SinWave^>^ StandardWave, int NumberofSWave, AnalysisParam^ A)
{
	Analysis::SinWave*stdw = new Analysis::SinWave[StandardWave->Length];
	for (int i = 0;i < StandardWave->Length;i++)
	{
		StandardWave[i]->TSput(stdw[i]);
	}
	int n;
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	double*retd;
	retd = CORE->INL(stdw, StandardWave->Length, A1, n);
	array <System::Double>^ ret = gcnew array <System::Double>(n);
	for (int i = 0;i < n;i++)
	{
		ret[i] = retd[i];
	}
	return ret;
}

System::Double AnalysisClr::WaveForm::SNR(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->SNR(A1);
}
System::Double AnalysisClr::WaveForm::SINAD(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->SINAD(A1);
}
System::Double AnalysisClr::WaveForm::ENOB(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->ENOB(A1);
}
System::Double AnalysisClr::WaveForm::SFDR(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->SFDR(A1);
}
System::Double AnalysisClr::WaveForm::THD(AnalysisParam^ A)
{
	Analysis::AnalysisParam A1;
	A->TSput(A1);
	return CORE->THD(A1);
}

AnalysisClr::WaveForm::WaveForm()
{
	CORE = new Analysis::WaveForm();
}
AnalysisClr::WaveForm::WaveForm(const WaveForm^ c)
{
	CORE = new Analysis::WaveForm(*c->CORE);
}
AnalysisClr::WaveForm^ AnalysisClr::WaveForm::operator =(const WaveForm^ c)
{
	delete CORE;
	CORE = new Analysis::WaveForm(*c->CORE);
	return this;
}
AnalysisClr::WaveForm::~WaveForm()
{
	delete CORE;
}

AnalysisClr::Event::Event()
{
	CORE = new Analysis::Event();
}
AnalysisClr::Event::Event(const Event^ c)
{
	CORE = new Analysis::Event(*c->CORE);
}
AnalysisClr::Event^ AnalysisClr::Event::operator =(const Event^ c)
{
	delete CORE;
	CORE = new Analysis::Event(*c->CORE);
	return this;
}
AnalysisClr::Event::~Event()
{
	delete CORE;
}

void AnalysisClr::EnergySP::ChangeCORE(Analysis::EnergySP* src)
{
	delete CORE;
	CORE = src;
}

void AnalysisClr::WaveForm::ChangeCORE(Analysis::WaveForm* src)
{
	delete CORE;
	CORE = src;
}

void AnalysisClr::Event::ChangeCORE(Analysis::Event* src)
{
	delete CORE;
	CORE = src;
}

Analysis::EnergySP AnalysisClr::EnergySP::getCORE()
{
	return *CORE;
}
Analysis::WaveForm AnalysisClr::WaveForm::getCORE()
{
	return *CORE;
}
Analysis::Event AnalysisClr::Event::getCORE()
{
	return *CORE;
}