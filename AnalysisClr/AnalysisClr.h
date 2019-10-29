#pragma once

#include "AnalysisLibrary.h"

namespace AnalysisClr
{
	public ref class ESPResult
	{
	public:
		System::Double Peak;
		System::Double FWHM;
		System::Double Area;
		void TSget(Analysis::ESPResult& source);
		//void TSput(Analysis::ESPResult& dest);
	};

	public ref class WaveResult
	{
	public:
		System::Double Wavewidth;
		System::Double RiseTime;
		System::Double Max;
		System::Double TimeofMax;
		System::Double FallTime;
		System::Double Area;
		void TSget(Analysis::WaveResult& source);
		//void TSput(Analysis::WaveResult& dest);
	};

	public ref class SinWave
	{
	public:
		System::Double Amp;
		System::Double Freq;
		//Phase Ranged from 0 to 2Pi
		System::Double Phase;
		void TSget(Analysis::SinWave& source);
		void TSput(Analysis::SinWave& dest);
	};

	public ref class AnalysisParam
	{
	public:
		System::String^ ESPCalibrationX;
		System::String^ ESPCalibrationY;
		System::String^ WaveCalibrationX;
		System::String^ WaveCalibrationY;
		System::Double WaveThreshold;
		int nmax;
		System::Double ESPMaxResolution;
		//void TSget(Analysis::AnalysisParam& source);
		void TSput(Analysis::AnalysisParam& dest);
	};

	public ref class Time
	{
	public:
		int Year;
		int Mon;
		int Day;
		int Hour;
		int Min;
		int Sec;
	};


	public ref class EnergySP
	{
	public:
		property int TotalChannel
		{
			int get()
			{
				return CORE->TotalChannel;
			};
		};
		property array <int>^Count
		{
			array <int>^ get()
			{
				array <int>^ ret = gcnew array<int>(TotalChannel);
				for (int i = 0;i < TotalChannel;i++)
				{
					ret[i] = CORE->Count[i];
				}
				return ret;
			};
			void set(array <int>^ src)
			{
				if (src->Length != TotalChannel)
				{
					CORE->TotalChannel = src->Length;
					delete CORE->Count;
					CORE->Count = new int[src->Length];
				}
				for (int i = 0;i < TotalChannel;i++)
				{
					CORE->Count[i] = src[i];
				}
			};
			
		};

		property int AnalysisResultLength
		{
			int get()
			{
				return CORE->AnalysisResultLength;
			};
		};
		property array <ESPResult^>^ AnalysisResult
		{
			array <ESPResult^>^ get()
			{
				array <ESPResult^>^ret = gcnew array <ESPResult^>(AnalysisResultLength);
				for (int i = 0;i < AnalysisResultLength;i++)
				{
					ret[i] = gcnew ESPResult();////////////////////////////////////////////////////////////////////////////////////////////////
					ret[i]->TSget(CORE->AnalysisResult[i]);
				}
				return ret;
			};
		}
		property array <System::Double>^ SmoothResult
		{
			array <System::Double>^ get()
			{
				array<System::Double>^ ret = gcnew array<System::Double>(TotalChannel);
				if (CORE->SmoothResult != nullptr)
				{
					for (int i = 0;i < TotalChannel;i++)
					{
						ret[i] = CORE->SmoothResult[i];
					}
				}
				else
				{
					for (int i = 0;i < TotalChannel;i++)
					{
						ret[i] = CORE->Count[i];
					}
				}
				return ret;
			};
		}
		property Time^ UploadTime
		{
			Time^ get()
			{
				Time^ ret = gcnew Time();
				ret->Year = CORE->UploadTime.tm_year + 1900;
				ret->Mon = CORE->UploadTime.tm_mon + 1;
				ret->Day = CORE->UploadTime.tm_mday;
				ret->Hour = CORE->UploadTime.tm_hour;
				ret->Min = CORE->UploadTime.tm_min;
				ret->Sec = CORE->UploadTime.tm_sec;
				return ret;
			}
			void set(Time^ src)
			{
				CORE->UploadTime.tm_year = src->Year - 1900;
				CORE->UploadTime.tm_mon = src->Mon - 1;
				CORE->UploadTime.tm_mday = src->Day;
				CORE->UploadTime.tm_hour = src->Hour;
				CORE->UploadTime.tm_min = src->Min;
				CORE->UploadTime.tm_sec = src->Sec;
				time_t time = mktime(&CORE->UploadTime);
				localtime_s(&CORE->UploadTime, &time);
			}
		}

		bool Analysis_Derivative(AnalysisParam^ A);
		bool Analysis_ExpectMaxium(AnalysisParam^ A);
		bool Analysis_LevenbergMarquardt(AnalysisParam^ A);
		bool Analysis_De_LM_EM(AnalysisParam^ A);

		System::String^ RadioisotopeIdentificate(AnalysisParam^ A);

		EnergySP();
		EnergySP(const EnergySP^ c);
		EnergySP^ operator =(const EnergySP^ c);
		~EnergySP();

		void ChangeCORE(Analysis::EnergySP* src);
		Analysis::EnergySP getCORE();
	private:
		Analysis::EnergySP* CORE;
	};

	public ref class WaveForm
	{
	public:
		property int TotalSample
		{
			int get()
			{
				return CORE->TotalSample;
			};
		}
		property array <System::Double>^ SampleValue
		{
			array <System::Double>^ get()
			{
				array <System::Double>^ ret = gcnew array<System::Double>(TotalSample);
				for (int i = 0;i < TotalSample;i++)
				{
					ret[i] = CORE->SampleValue[i];
				}
				return ret;
			};
			void set(array <System::Double>^ src)
			{
				if (src->Length != TotalSample)
				{
					CORE->TotalSample = src->Length;
					delete CORE->SampleValue;
					CORE->SampleValue = new double[src->Length];
				}
				for (int i = 0;i < TotalSample;i++)
				{
					CORE->SampleValue[i] = src[i];
				}
			};
		}


		property int AnalysisResultLength
		{
			int get()
			{
				return CORE->AnalysisResultLength;
			}
		}
		property array <WaveResult^>^ AnalysisResult
		{
			array <WaveResult^>^ get()
			{
				array <WaveResult^>^ret = gcnew array <WaveResult^>(AnalysisResultLength);
				for (int i = 0;i < AnalysisResultLength;i++)
				{
					ret[i] = gcnew WaveResult();
					ret[i]->TSget(CORE->AnalysisResult[i]);
				}
				return ret;
			};
		}

		property array <System::Double>^ FFTResult
		{
			array <System::Double>^ get()
			{
				array<System::Double>^ ret = gcnew array<System::Double>(TotalSample);
				if (CORE->FFTResult != nullptr)
				{
					for (int i = 0;i < TotalSample;i++)
					{
						ret[i] = CORE->FFTResult[i];
					}
				}
				else
				{
					for (int i = 0;i < TotalSample;i++)
					{
						ret[i] = -1;
					}
				}
				return ret;
			};
		}

		property Time^ UploadTime
		{
			Time^ get()
			{
				Time^ ret = gcnew Time();
				ret->Year = CORE->UploadTime.tm_year + 1900;
				ret->Mon = CORE->UploadTime.tm_mon + 1;
				ret->Day = CORE->UploadTime.tm_mday;
				ret->Hour = CORE->UploadTime.tm_hour;
				ret->Min = CORE->UploadTime.tm_min;
				ret->Sec = CORE->UploadTime.tm_sec;
				return ret;
			}
			void set(Time^ src)
			{
				CORE->UploadTime.tm_year = src->Year - 1900;
				CORE->UploadTime.tm_mon = src->Mon - 1;
				CORE->UploadTime.tm_mday = src->Day;
				CORE->UploadTime.tm_hour = src->Hour;
				CORE->UploadTime.tm_min = src->Min;
				CORE->UploadTime.tm_sec = src->Sec;
				time_t time = mktime(&CORE->UploadTime);
				localtime_s(&CORE->UploadTime, &time);
			}
		}

		bool Analysis(AnalysisParam^ A);

		bool FFT(AnalysisParam^ A);

		SinWave^ SinFit(AnalysisParam^ A);

		array <System::Double>^ DNL(array <SinWave^>^ StandardWave, int NumberofSWave, AnalysisParam^ A);

		array <System::Double>^ INL(array <SinWave^>^ StandardWave, int NumberofSWave, AnalysisParam^ A);

		System::Double SNR(AnalysisParam^ A);

		System::Double SINAD(AnalysisParam^ A);

		System::Double ENOB(AnalysisParam^ A);

		System::Double SFDR(AnalysisParam^ A);

		System::Double THD(AnalysisParam^ A);

		WaveForm();
		WaveForm(const WaveForm^ c);
		WaveForm^ operator =(const WaveForm^ c);
		~WaveForm();

		void ChangeCORE(Analysis::WaveForm* src);
		Analysis::WaveForm getCORE();
	private:
		Analysis::WaveForm* CORE;
	};

	public ref class Event
	{
	public:
		property System::Double Amp
		{
			System::Double get()
			{
				return CORE->Amp;
			}
			void set(System::Double src)
			{
				CORE->Amp = src;
			}
		}
		property System::Double RiseTime
		{
			System::Double get()
			{
				return CORE->RiseTime;
			}
			void set(System::Double src)
			{
				CORE->RiseTime = src;
			}
		}
		property System::Double ArrivialTime
		{
			System::Double get()
			{
				return CORE->ArrivialTime;
			}
			void set(System::Double src)
			{
				CORE->ArrivialTime = src;
			}
		}
		property System::Double FallTime
		{
			System::Double get()
			{
				return CORE->FallTime;
			}
			void set(System::Double src)
			{
				CORE->FallTime = src;
			}
		}
		property System::Double Area
		{
			System::Double get()
			{
				return CORE->Area;;
			}
			void set(System::Double src)
			{
				CORE->Area = src;
			}
		}
		property System::Byte source
		{
			System::Byte get()
			{
				return CORE->source;
			}
			void set(System::Byte src)
			{
				CORE->source = src;
			}
		}

		property Time^ UploadTime
		{
			Time^ get()
			{
				Time^ ret = gcnew Time();
				ret->Year = CORE->UploadTime.tm_year + 1900;
				ret->Mon = CORE->UploadTime.tm_mon + 1;
				ret->Day = CORE->UploadTime.tm_mday;
				ret->Hour = CORE->UploadTime.tm_hour;
				ret->Min = CORE->UploadTime.tm_min;
				ret->Sec = CORE->UploadTime.tm_sec;
				return ret;
			}
			void set(Time^ src)
			{
				CORE->UploadTime.tm_year = src->Year - 1900;
				CORE->UploadTime.tm_mon = src->Mon - 1;
				CORE->UploadTime.tm_mday = src->Day;
				CORE->UploadTime.tm_hour = src->Hour;
				CORE->UploadTime.tm_min = src->Min;
				CORE->UploadTime.tm_sec = src->Sec;
				time_t time = mktime(&CORE->UploadTime);
				localtime_s(&CORE->UploadTime, &time);
			}
		}

		Event();
		Event(const Event^ c);
		Event^ operator =(const Event^ c);
		~Event();

		void ChangeCORE(Analysis::Event* src);
		Analysis::Event getCORE();
	private:
		Analysis::Event* CORE;
	};
}