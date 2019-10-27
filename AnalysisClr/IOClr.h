#pragma once

#include "IOLibrary.h"
#include "AnalysisClr.h"

namespace IOClr
{
	public ref class StateNote
	{
	public:
		System::String^ Statenote;
		System::Byte Paramsize;
		array <System::String^>^ Paramnote = gcnew array<System::String^>(6);
		array <System::Byte>^  Paramstart = gcnew array <System::Byte>(6);
		array <System::Byte>^  Paramend = gcnew array <System::Byte>(6);
		void TSget(IOLib::StateNote& source);
	};

	public ref class StateCode
	{
	public:
		System::UInt16 STCode;
		array<int>^Sparam = gcnew array<int>(6);
		AnalysisClr::Time^ Time;

		void TSget(IOLib::StateCode& source);
		void TSput(IOLib::StateCode& dest);
		StateNote^ ParamNote();
	};

	public ref class WaveFormRaw
	{
	public:
		property int TotalSample
		{
			int get()
			{
				return CORE->TotalSample;
			};
		}
		property array <int>^ SampleValue
		{
			array <int>^ get()
			{
				array <int>^ ret = gcnew array<int>(TotalSample);
				for (int i = 0;i < TotalSample;i++)
				{
					ret[i] = CORE->SampleValue[i];
				}
				return ret;
			};
			void set(array <int>^ src)
			{
				if (src->Length != TotalSample)
				{
					CORE->TotalSample = src->Length;
					delete CORE->SampleValue;
					CORE->SampleValue = new int[src->Length];
				}
				for (int i = 0;i < TotalSample;i++)
				{
					CORE->SampleValue[i] = src[i];
				}
			};
		}
		property AnalysisClr::Time^ UploadTime
		{
			AnalysisClr::Time^ get()
			{
				AnalysisClr::Time^ ret = gcnew AnalysisClr::Time();
				ret->Year = CORE->UploadTime.tm_year + 1900;
				ret->Mon = CORE->UploadTime.tm_mon + 1;
				ret->Day = CORE->UploadTime.tm_mday;
				ret->Hour = CORE->UploadTime.tm_hour;
				ret->Min = CORE->UploadTime.tm_min;
				ret->Sec = CORE->UploadTime.tm_sec;
				return ret;
			}
			void set(AnalysisClr::Time^ src)
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
		WaveFormRaw();
		~WaveFormRaw();
		AnalysisClr::WaveForm^ Transf();
		void ChangeCORE(IOLib::WaveFormRaw* src);
		IOLib::WaveFormRaw getCORE();
	private:
		IOLib::WaveFormRaw* CORE;
	};

	public ref class EventRaw
	{
	public:
		property int Amp
		{
			int get()
			{
				return CORE->Amp;
			}
			void set(int src)
			{
				CORE->Amp = src;
			}
		}
		property int RiseTime
		{
			int get()
			{
				return CORE->RiseTime;
			}
			void set(int src)
			{
				CORE->RiseTime = src;
			}
		}
		property int ArrivialTime
		{
			int get()
			{
				return CORE->ArrivialTime;
			}
			void set(int src)
			{
				CORE->ArrivialTime = src;
			}
		}
		property int FallTime
		{
			int get()
			{
				return CORE->FallTime;
			}
			void set(int src)
			{
				CORE->FallTime = src;
			}
		}
		property int Area
		{
			int get()
			{
				return CORE->Area;;
			}
			void set(int src)
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

		property AnalysisClr::Time^ UploadTime
		{
			AnalysisClr::Time^ get()
			{
				AnalysisClr::Time^ ret = gcnew AnalysisClr::Time();
				ret->Year = CORE->UploadTime.tm_year + 1900;
				ret->Mon = CORE->UploadTime.tm_mon + 1;
				ret->Day = CORE->UploadTime.tm_mday;
				ret->Hour = CORE->UploadTime.tm_hour;
				ret->Min = CORE->UploadTime.tm_min;
				ret->Sec = CORE->UploadTime.tm_sec;
				return ret;
			}
			void set(AnalysisClr::Time^ src)
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

		EventRaw();
		~EventRaw();
		AnalysisClr::Event^ Transf();
		void ChangeCORE(IOLib::EventRaw* src);
		IOLib::EventRaw getCORE();
	private:
		IOLib::EventRaw* CORE;
	};

	public ref class Structedfile
	{
	public:
		property System::String^ filepath
		{
			System::String^ get()
			{
				return gcnew System::String(CORE->filepath.c_str());
			}
			void set(System::String^ src)
			{
				const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(src);
				CORE->filepath = tmp;
			}
		}
		property System::String^ filename
		{
			System::String^ get()
			{
				return gcnew System::String(CORE->filename.c_str());
			}
			void set(System::String^ src)
			{
				const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(src);
				CORE->filename = tmp;
			}
		}
		property int Run
		{
			int get()
			{
				return CORE->Run;
			}
			void set(int src)
			{
				CORE->Run = src;
			}
		}
		property int TotalState
		{
			int get()
			{
				return CORE->TotalState;
			}
		}
		property int TotalESP
		{
			int get()
			{
				return CORE->TotalESP;
			}
		}
		property int TotalWave
		{
			int get()
			{
				return CORE->TotalWave;
			}
		}
		property int TotalEvt
		{
			int get()
			{
				return CORE->TotalEvt;
			}
		}
		property int TotalTSP
		{
			int get()
			{
				return CORE->TotalTSP;
			}
		}
		property array <StateCode^>^ State
		{
			array <StateCode^>^ get()
			{
				array <StateCode^>^ ret = gcnew array<StateCode^>(TotalState);
				for (int i = 0;i < TotalState;i++)
				{
					ret[i]->TSget(CORE->State[i]);
				}
				return ret;
			};
			void set(array <StateCode^>^ src)
			{
				if (src->Length != TotalState)
				{
					CORE->TotalState = src->Length;
					delete CORE->State;
					CORE->State = new IOLib::StateCode [src->Length];
				}
				for (int i = 0;i < TotalState;i++)
				{
					src[i]->TSput(CORE->State[i]);
				}
			};
		}
		property array <AnalysisClr::EnergySP^>^ Esp
		{
			array <AnalysisClr::EnergySP^>^ get()
			{
				array <AnalysisClr::EnergySP^>^ ret = gcnew array<AnalysisClr::EnergySP^>(TotalESP);
				for (int i = 0;i < TotalESP;i++)
				{
					ret[i]->ChangeCORE(&CORE->Esp[i]);
				}
				return ret;
			};
			void set(array <AnalysisClr::EnergySP^>^ src)
			{
				if (src->Length != TotalESP)
				{
					CORE->TotalESP = src->Length;
					delete CORE->Esp;
					CORE->Esp = new Analysis::EnergySP[src->Length];
				}
				for (int i = 0;i < TotalESP;i++)
				{
					CORE->Esp[i] = src[i]->getCORE();
				}
			};
		}
		property array <WaveFormRaw^>^ Wav
		{
			array <WaveFormRaw^>^ get()
			{
				array <WaveFormRaw^>^ ret = gcnew array<WaveFormRaw^>(TotalWave);
				for (int i = 0;i < TotalWave;i++)
				{
					ret[i]->ChangeCORE(&CORE->Wav[i]);
				}
				return ret;
			};
			void set(array <WaveFormRaw^>^ src)
			{
				if (src->Length != TotalWave)
				{
					CORE->TotalWave = src->Length;
					delete CORE->Wav;
					CORE->Wav = new IOLib::WaveFormRaw[src->Length];
				}
				for (int i = 0;i < TotalWave;i++)
				{
					CORE->Wav[i] = src[i]->getCORE();
				}
			};
		}
		property array <EventRaw^>^ Evt
		{
			array <EventRaw^>^ get()
			{
				array <EventRaw^>^ ret = gcnew array<EventRaw^>(TotalEvt);
				for (int i = 0;i < TotalEvt;i++)
				{
					ret[i]->ChangeCORE(&CORE->Evt[i]);
				}
				return ret;
			};
			void set(array <EventRaw^>^ src)
			{
				if (src->Length != TotalEvt)
				{
					CORE->TotalEvt = src->Length;
					delete CORE->Evt;
					CORE->Evt = new IOLib::EventRaw[src->Length];
				}
				for (int i = 0;i < TotalEvt;i++)
				{
					CORE->Evt[i] = src[i]->getCORE();
				}
			};
		}
		property array <AnalysisClr::EnergySP^>^ Tsp
		{
			array <AnalysisClr::EnergySP^>^ get()
			{
				array <AnalysisClr::EnergySP^>^ ret = gcnew array<AnalysisClr::EnergySP^>(TotalTSP);
				for (int i = 0;i < TotalTSP;i++)
				{
					ret[i]->ChangeCORE(&CORE->Tsp[i]);
				}
				return ret;
			};
			void set(array <AnalysisClr::EnergySP^>^ src)
			{
				if (src->Length != TotalTSP)
				{
					CORE->TotalTSP = src->Length;
					delete CORE->Tsp;
					CORE->Tsp = new Analysis::EnergySP[src->Length];
				}
				for (int i = 0;i < TotalTSP;i++)
				{
					CORE->Tsp[i] = src[i]->getCORE();
				}
			};
		}

		bool ReadFile(System::String^ filepath, System::String^ filename);
		bool WriteFile(System::String^ filepath, System::String^ filename);

		Structedfile();
		Structedfile(const Structedfile^ c);
		Structedfile^ operator =(const Structedfile^ c);
		~Structedfile();
		void ChangeCORE(IOLib::Structedfile* src);
	private:
		IOLib::Structedfile* CORE;
	};

	public ref class Rawdatafile
	{
	public:
		property System::String^ filepath
		{
			System::String^ get()
			{
				return gcnew System::String(CORE->filepath.c_str());
			}
			void set(System::String^ src)
			{
				const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(src);
				CORE->filepath = tmp;
			}
		}
		property System::String^ filename
		{
			System::String^ get()
			{
				return gcnew System::String(CORE->filename.c_str());
			}
			void set(System::String^ src)
			{
				const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(src);
				CORE->filename = tmp;
			}
		}
		property int Run
		{
			int get()
			{
				return CORE->Run;
			}
			void set(int src)
			{
				CORE->Run = src;
			}
		}

		bool ReadFile(System::String^ filepath, System::String^ filename);
		bool WriteFrame(System::IntPtr Framedata);
		Structedfile^ AnalysisFrameCache();
		bool WriteFile(System::String^ filepath, System::String^ filename);
		bool AddOnFile(System::String^ filepath, System::String^ filename);

		Rawdatafile();
		Rawdatafile(const Rawdatafile^ c);
		Rawdatafile^ operator =(const Rawdatafile^ c);
		~Rawdatafile();
	private:
		IOLib::Rawdatafile* CORE;
	};
}
