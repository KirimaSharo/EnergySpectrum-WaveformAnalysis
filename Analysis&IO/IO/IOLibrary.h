#pragma once

#include "AnalysisLibrary.h"
#ifndef DLL_IO
#define DLL_IO _declspec(dllimport)
#endif


namespace IOLib
{
	struct FrameC
	{
		unsigned char* Framedata;
		int Flen;
		FrameC* Next;
		FrameC()
		{
			Framedata = nullptr;
			Next = nullptr;
		}
	};//No Need Export
	
	struct StateCode
	{
		unsigned short STCode;
		int Sparam[6];
		tm Time;
	};
	struct StateNote
	{
		std::string Statenote;
		unsigned char Paramsize;
		std::string Paramnote[6];
		unsigned char Paramstart[6];
		unsigned char Paramend[6];
	};

	void DLL_IO ParamNote(const IOLib::StateCode& State, IOLib::StateNote& SNote);

	class DLL_IO WaveFormRaw
	{
	public:
		int TotalSample;
		int *SampleValue;
		tm UploadTime;
		WaveFormRaw();
		WaveFormRaw(const WaveFormRaw& c);
		WaveFormRaw& operator = (const WaveFormRaw& c);
		~WaveFormRaw();
	};
	struct EventRaw
	{
		int Amp;
		int RiseTime;
		int ArrivialTime;
		int FallTime;
		int Area;
		unsigned char source;
		tm UploadTime;
	};

	class DLL_IO Structedfile
	{
	public:
		std::string filepath;
		std::string filename;
		int Run;
		int TotalState;
		int TotalESP;
		int TotalWave;
		int TotalEvt;
		int TotalTSP;
		StateCode* State;
		Analysis::EnergySP* Esp;
		IOLib::WaveFormRaw* Wav;
		IOLib::EventRaw* Evt;
		Analysis::EnergySP* Tsp;//************************************************

		Structedfile();
		Structedfile(const Structedfile& c);
		Structedfile& operator =(const Structedfile& c);
		~Structedfile();

		bool ReadFile(std::string filepath, std::string filename);
		bool WriteFile(std::string filepath, std::string filename);


	private:


	};

	class DLL_IO Rawdatafile
	{
	public:
		std::string filepath;
		std::string filename;
		int Run;

		bool ReadFile(std::string filepath, std::string filename);
		bool WriteFrame(unsigned char* Framedata);
		Structedfile AnalysisFrameCache();
		bool WriteFile(std::string filepath, std::string filename);
		bool AddOnFile(std::string filepath, std::string filename);


		Rawdatafile();
		Rawdatafile(const Rawdatafile &c);
		Rawdatafile& operator =(const Rawdatafile &c);
		~Rawdatafile();

		
	private:
		FrameC* init;
		FrameC* cur;
	};




}