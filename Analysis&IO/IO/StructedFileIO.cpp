#include <Windows.h>
#define DLL_IO _declspec(dllexport)
#define STRUCTED_FILE_EXTENSION ".txt"
#define UNDEFINED_RUN -1

//#include "stdafx.h"
#include "fstream"
#include <iomanip>
#include<io.h>
#include "IOlibrary.h"


bool ESPSaveCore(const Analysis::EnergySP &Esp, std::ofstream &savestream)//Not Save Smooth Results
{
	savestream << "Upload Time: " << std::to_string(Esp.UploadTime.tm_year + 1900) << "/"
		<< std::to_string(Esp.UploadTime.tm_mon + 1) << "/" << std::to_string(Esp.UploadTime.tm_mday) << " "
		<< std::to_string(Esp.UploadTime.tm_hour) << ":" << std::to_string(Esp.UploadTime.tm_min) << ":" << std::to_string(Esp.UploadTime.tm_sec)
		<< "\n" << "\n";
	if (Esp.AnalysisResultLength)
	{
		savestream << "Analysis Result(s): " << Esp.AnalysisResultLength << "\n";
		for (int i = 1;i <= Esp.AnalysisResultLength;i++)
		{
			savestream << "Result " << i << ": " << " Peak: " << Esp.AnalysisResult[i - 1].Peak << " FWHM: " << Esp.AnalysisResult[i - 1].FWHM << " Area: " << Esp.AnalysisResult[i - 1].Area << "\n";
		}
		savestream << "\n";
	}

	savestream << "Total Channel: " << Esp.TotalChannel << "\n";

	savestream << "Count:";
	for (int i = 1;i <= Esp.TotalChannel;i++)
	{
		if (i % 10 == 1)
		{
			savestream << "\n";
			savestream << std::right << std::setw(6) << std::setfill(' ') << i;
			savestream << "--";
			savestream << std::left << std::setw(6) << std::setfill(' ') << ((i + 9) > Esp.TotalChannel ? Esp.TotalChannel : i + 9);
			savestream << ":";
		}
		savestream << " ";
		savestream << std::right << std::setw(6) << std::setfill(' ') << Esp.Count[i - 1];
	}
	savestream << "\n";
	return 1;
}
bool ESPLoadCore(Analysis::EnergySP &Esp, std::ifstream &loadstream)
{
	std::string sc;
	char c1;
	loadstream >> sc;
	if (sc != "Upload")
	{
		return false;
	}
	loadstream >> sc;
	if (sc != "Time:")
	{
		return false;
	}
	loadstream >> Esp.UploadTime.tm_year >> c1 >> Esp.UploadTime.tm_mon >> c1 >> Esp.UploadTime.tm_mday
		>> Esp.UploadTime.tm_hour >> c1 >> Esp.UploadTime.tm_min >> c1 >> Esp.UploadTime.tm_sec;
	Esp.UploadTime.tm_year -= 1900;
	Esp.UploadTime.tm_mon -= 1;
	time_t time = mktime(&Esp.UploadTime);
	localtime_s(&Esp.UploadTime, &time);
	loadstream >> sc;
	if (sc == "Analysis")
	{
		loadstream >> sc;
		loadstream >> Esp.AnalysisResultLength;
		if (Esp.AnalysisResult != nullptr)
		{
			Esp.AnalysisResult = nullptr;
		}
		delete Esp.AnalysisResult;
		Esp.AnalysisResult = new Analysis::ESPResult[Esp.AnalysisResultLength];
		for (int i = 0;i < Esp.AnalysisResultLength;i++)
		{
			loadstream >> sc;
			loadstream >> sc;
			loadstream >> sc;
			loadstream >> Esp.AnalysisResult[i].Peak >> sc >> Esp.AnalysisResult[i].FWHM >> sc >> Esp.AnalysisResult[i].Area;
		}
		loadstream >> sc;
	}
	else
	{
		Esp.AnalysisResultLength = 0;
		if (Esp.AnalysisResult != nullptr)
		{
			Esp.AnalysisResult = nullptr;
		}
	}
	if (sc != "Total")
	{
		return false;
	}
	loadstream >> sc;
	loadstream >> Esp.TotalChannel;
	loadstream >> sc;

	delete Esp.Count;
	Esp.Count = new int[Esp.TotalChannel];
	for (int i = 0;i < Esp.TotalChannel;i++)
	{
		if (i % 10 == 0)
		{
			loadstream >> sc;
			loadstream >> sc;
		}
		loadstream >> Esp.Count[i];
	}
	return true;
}

bool TSPSaveCore(const Analysis::EnergySP &Tsp, std::ofstream &savestream)//Not Save Smooth Results
{
	savestream << "Upload Time: " << std::to_string(Tsp.UploadTime.tm_year + 1900) << "/"
		<< std::to_string(Tsp.UploadTime.tm_mon + 1) << "/" << std::to_string(Tsp.UploadTime.tm_mday) << " "
		<< std::to_string(Tsp.UploadTime.tm_hour) << ":" << std::to_string(Tsp.UploadTime.tm_min) << ":" << std::to_string(Tsp.UploadTime.tm_sec)
		<< "\n" << "\n";
	if (Tsp.AnalysisResultLength)
	{
		savestream << "Analysis Result(s): " << Tsp.AnalysisResultLength << "\n";
		for (int i = 1;i <= Tsp.AnalysisResultLength;i++)
		{
			savestream << "Result " << i << ": " << " Peak: " << Tsp.AnalysisResult[i - 1].Peak << " FWHM: " << Tsp.AnalysisResult[i - 1].FWHM << " Area: " << Tsp.AnalysisResult[i - 1].Area << "\n";
		}
		savestream << "\n";
	}

	savestream << "Total Channel: " << Tsp.TotalChannel << "\n";

	savestream << "Count:";
	for (int i = 1;i <= Tsp.TotalChannel;i++)
	{
		if (i % 10 == 1)
		{
			savestream << "\n";
			savestream << std::right << std::setw(6) << std::setfill(' ') << i;
			savestream << "--";
			savestream << std::left << std::setw(6) << std::setfill(' ') << ((i + 9) > Tsp.TotalChannel ? Tsp.TotalChannel : i + 9);
			savestream << ":";
		}
		savestream << " ";
		savestream << std::right << std::setw(6) << std::setfill(' ') << Tsp.Count[i - 1];
	}
	savestream << "\n";
	return 1;
}
bool TSPLoadCore(Analysis::EnergySP &Tsp, std::ifstream &loadstream)
{
	std::string sc;
	char c1;
	loadstream >> sc;
	if (sc != "Upload")
	{
		return false;
	}
	loadstream >> sc;
	if (sc != "Time:")
	{
		return false;
	}
	loadstream >> Tsp.UploadTime.tm_year >> c1 >> Tsp.UploadTime.tm_mon >> c1 >> Tsp.UploadTime.tm_mday
		>> Tsp.UploadTime.tm_hour >> c1 >> Tsp.UploadTime.tm_min >> c1 >> Tsp.UploadTime.tm_sec;
	Tsp.UploadTime.tm_year -= 1900;
	Tsp.UploadTime.tm_mon -= 1;
	time_t time = mktime(&Tsp.UploadTime);
	localtime_s(&Tsp.UploadTime, &time);
	loadstream >> sc;
	if (sc == "Analysis")
	{
		loadstream >> sc;
		loadstream >> Tsp.AnalysisResultLength;
		if (Tsp.AnalysisResult != nullptr)
		{
			Tsp.AnalysisResult = nullptr;
		}
		delete Tsp.AnalysisResult;
		Tsp.AnalysisResult = new Analysis::ESPResult[Tsp.AnalysisResultLength];
		for (int i = 0;i < Tsp.AnalysisResultLength;i++)
		{
			loadstream >> sc;
			loadstream >> sc;
			loadstream >> sc;
			loadstream >> Tsp.AnalysisResult[i].Peak >> sc >> Tsp.AnalysisResult[i].FWHM >> sc >> Tsp.AnalysisResult[i].Area;
		}
		loadstream >> sc;
	}
	else
	{
		Tsp.AnalysisResultLength = 0;
		if (Tsp.AnalysisResult != nullptr)
		{
			Tsp.AnalysisResult = nullptr;
		}
	}
	if (sc != "Total")
	{
		return false;
	}
	loadstream >> sc;
	loadstream >> Tsp.TotalChannel;
	loadstream >> sc;

	delete Tsp.Count;
	Tsp.Count = new int[Tsp.TotalChannel];
	for (int i = 0;i < Tsp.TotalChannel;i++)
	{
		if (i % 10 == 0)
		{
			loadstream >> sc;
			loadstream >> sc;
		}
		loadstream >> Tsp.Count[i];
	}
	return true;
}

bool WaveSaveCore(const IOLib::WaveFormRaw &Wav, std::ofstream &savestream)
{
	savestream << "Upload Time: " << std::to_string(Wav.UploadTime.tm_year + 1900) << "/"
		<< std::to_string(Wav.UploadTime.tm_mon + 1) << "/" << std::to_string(Wav.UploadTime.tm_mday) << " "
		<< std::to_string(Wav.UploadTime.tm_hour) << ":" << std::to_string(Wav.UploadTime.tm_min) << ":" << std::to_string(Wav.UploadTime.tm_sec)
		<< "\n" << "\n";

	savestream << "Total Sample: " << Wav.TotalSample << "\n";

	savestream << "Sample Value:";
	for (int i = 1;i <= Wav.TotalSample;i++)
	{
		if (i % 10 == 1)
		{
			savestream << "\n";
			savestream << std::right << std::setw(6) << std::setfill(' ') << i;
			savestream << "--";
			savestream << std::left << std::setw(6) << std::setfill(' ') << ((i + 9) > Wav.TotalSample ? Wav.TotalSample : i + 9);
			savestream << ": ";
		}
		savestream << " ";
		savestream << std::right << std::setw(6) << std::setfill(' ') << Wav.SampleValue[i - 1];
	}
	savestream << "\n";
	return 1;
}
bool WaveLoadCore(IOLib::WaveFormRaw &Wav, std::ifstream &loadstream)
{
	std::string sc;
	char c1;
	loadstream >> sc;
	if (sc != "Upload")
	{
		return false;
	}
	loadstream >> sc;
	if (sc != "Time:")
	{
		return false;
	}
	loadstream >> Wav.UploadTime.tm_year >> c1 >> Wav.UploadTime.tm_mon >> c1 >> Wav.UploadTime.tm_mday
		>> Wav.UploadTime.tm_hour >> c1 >> Wav.UploadTime.tm_min >> c1 >> Wav.UploadTime.tm_sec;
	Wav.UploadTime.tm_year -= 1900;
	Wav.UploadTime.tm_mon -= 1;
	time_t time = mktime(&Wav.UploadTime);
	localtime_s(&Wav.UploadTime, &time);
	loadstream >> sc;
	if (sc != "Total")
	{
		return false;
	}
	loadstream >> sc >> Wav.TotalSample;
	
	loadstream >> sc;
	loadstream >> sc;

	delete Wav.SampleValue;
	Wav.SampleValue = new int[Wav.TotalSample];

	for (int i = 0;i < Wav.TotalSample;i++)
	{
		if (i % 10 == 0)
		{
			loadstream >> sc;
			loadstream >> sc;
		}
		loadstream >> Wav.SampleValue[i];
	}
	
	return true;
}

bool EventSaveCore(const IOLib::EventRaw &Evt, std::ofstream &savestream)
{
	savestream << "Upload Time: " << std::to_string(Evt.UploadTime.tm_year + 1900) << "/"
		<< std::to_string(Evt.UploadTime.tm_mon + 1) << "/" << std::to_string(Evt.UploadTime.tm_mday) << " "
		<< std::to_string(Evt.UploadTime.tm_hour) << ":" << std::to_string(Evt.UploadTime.tm_min) << ":" << std::to_string(Evt.UploadTime.tm_sec)
		<< "\n" << "\n";
	savestream << "Source: " << std::hex << (unsigned short)Evt.source<<std::dec<<" ";

	if (Evt.source >= 0x01 && Evt.source <= 0x1F)//CZT
	{
		savestream << "CZT" << "\n";
		savestream << "Amplitude: " << Evt.Amp<<"\n";
		savestream << "Rise Time: " << Evt.RiseTime << "\n";
		savestream << "Arrive Time: " << Evt.ArrivialTime << "\n";
	}
	else if (Evt.source >= 0x20 && Evt.source <= 0x3F)//Crystal
	{
		savestream << "Crystal" << "\n";
		savestream << "Amplitude: " << Evt.Amp << "\n";
		savestream << "Rise Time: " << Evt.RiseTime << "\n";
		savestream << "Fall Time: " << Evt.FallTime << "\n";
		savestream << "Area: " << Evt.Area << "\n";
	}
	else if (Evt.source >= 0x40 && Evt.source <= 0x5F)//Scatter
	{
		savestream << "Scatter" << "\n";
		savestream << "Back Time: " << Evt.ArrivialTime << "\n";
	}
	else if (Evt.source >= 0x60 && Evt.source <= 0x7F)//Capture
	{
		savestream << "Capture" << "\n";
		savestream << "Amplitude: " << Evt.Amp << "\n";
	}
	else
	{
		//Log Error
		return false;
	}

	return true;
}
bool EventLoadCore(IOLib::EventRaw &Evt, std::ifstream &loadstream)
{
	std::string sc;
	char c1;
	unsigned short st1;
	loadstream >> sc;
	if (sc != "Upload")
	{
		return false;
	}
	loadstream >> sc;
	if (sc != "Time:")
	{
		return false;
	}
	loadstream >> Evt.UploadTime.tm_year >> c1 >> Evt.UploadTime.tm_mon >> c1 >> Evt.UploadTime.tm_mday
		>> Evt.UploadTime.tm_hour >> c1 >> Evt.UploadTime.tm_min >> c1 >> Evt.UploadTime.tm_sec;
	Evt.UploadTime.tm_year -= 1900;
	Evt.UploadTime.tm_mon -= 1;
	time_t time = mktime(&Evt.UploadTime);
	localtime_s(&Evt.UploadTime, &time);
	loadstream >> sc;
	if (sc != "Source:")
	{
		return false;
	}
	loadstream >> std::hex >> st1 >> std::dec;
	Evt.source = st1;
	if (Evt.source >= 0x01 && Evt.source <= 0x1F)//CZT
	{
		loadstream >> sc;
		loadstream >> sc >> Evt.Amp;
		loadstream >> sc >> sc >> Evt.RiseTime;
		loadstream >> sc >> sc >> Evt.ArrivialTime;
	}
	else if (Evt.source >= 0x20 && Evt.source <= 0x3F)//Crystal
	{
		loadstream >> sc;
		loadstream >> sc >> Evt.Amp;
		loadstream >> sc >> sc >> Evt.RiseTime;
		loadstream >> sc >> sc >> Evt.FallTime;
		loadstream >> sc >> Evt.Area;
	}
	else if (Evt.source >= 0x40 && Evt.source <= 0x5F)//Scatter
	{
		loadstream >> sc;
		loadstream >> sc >> sc >> Evt.ArrivialTime;
	}
	else if (Evt.source >= 0x60 && Evt.source <= 0x7F)//Capture
	{
		loadstream >> sc;
		loadstream >> sc >> Evt.Amp;
	}
	else
	{
		//Log Error Source
		return false;
	}
	return true;

}

bool StateSaveCore(const IOLib::StateCode &St, std::ofstream &savestream)
{
	IOLib::StateNote Sn;
	IOLib::ParamNote(St, Sn);
	savestream << "Upload Time: " << std::to_string(St.Time.tm_year + 1900) << "/"
		<< std::to_string(St.Time.tm_mon + 1) << "/" << std::to_string(St.Time.tm_mday) << " "
		<< std::to_string(St.Time.tm_hour) << ":" << std::to_string(St.Time.tm_min) << ":" << std::to_string(St.Time.tm_sec)
		<< "\n" << "\n";
	savestream << std::hex << "STCode: " << St.STCode << " " << Sn.Statenote;

	for (int i = 0;i < Sn.Paramsize;i++)
	{
		savestream << "\n";
		savestream << Sn.Paramnote[i] << ": ";
		for (int j = Sn.Paramstart[i];j <= Sn.Paramend[i];j++)
		{
			savestream << St.Sparam[j] << " ";
		}
	}
	savestream << "\n" << "RawStateParams: ";
	for (int i = 0;i < 6;i++)
	{
		savestream << St.Sparam[i]<<" ";
	}
	savestream << std::dec << "\n";
	return true;
}
bool StateLoadCore(IOLib::StateCode &St, std::ifstream &loadstream)
{
	std::string sc;
	char c1;
	loadstream >> sc;
	if (sc != "Upload")
	{
		return false;
	}
	loadstream >> sc;
	if (sc != "Time:")
	{
		return false;
	}
	loadstream >> St.Time.tm_year >> c1 >> St.Time.tm_mon >> c1 >> St.Time.tm_mday
		>> St.Time.tm_hour >> c1 >> St.Time.tm_min >> c1 >> St.Time.tm_sec;
	St.Time.tm_year -= 1900;
	St.Time.tm_mon -= 1;
	time_t time = mktime(&St.Time);
	localtime_s(&St.Time, &time);
	loadstream >> sc;
	if (sc != "STCode:")
	{
		return false;
	}
	loadstream >> std::hex >> St.STCode;
	do
	{
		loadstream >> sc;
	} while (sc != "RawStateParams:");
	for (int i = 0;i < 6;i++)
	{
		loadstream >> St.Sparam[i];
	}
	loadstream >> std::dec;
	return true;

}

bool OutKey(std::string sc)
{
	return sc == "StateSection:" ||
		sc == "EnergySpectrumSection:" ||
		sc == "TimeSpectrumSection:" ||
		sc == "WaveFormSection:" ||
		sc == "EventSection:";
}


namespace IOLib
{
	Structedfile::Structedfile()
	{
		Esp = nullptr;
		Tsp = nullptr;
		Evt = nullptr;
		Wav = nullptr;
		State = nullptr;
		TotalESP = 0;
		TotalTSP = 0;
		TotalWave = 0;
		TotalEvt = 0;
		TotalState = 0;
		Run = UNDEFINED_RUN;
		filepath = "";
		filename = "";
	}

	Structedfile::Structedfile(const Structedfile& c)
	{	
		Esp = nullptr;
		Tsp = nullptr;
		Evt = nullptr;
		Wav = nullptr;
		State = nullptr;

		TotalESP = c.TotalESP;
		TotalTSP = c.TotalTSP;
		TotalWave = c.TotalWave;
		TotalEvt = c.TotalEvt;
		TotalState = c.TotalState;

		
		if (TotalESP > 0)
		{
			Esp = new Analysis::EnergySP[TotalESP];
			for (int i = 0;i < TotalESP;i++)
			{
				Esp[i] = c.Esp[i];
			}
		}
		
		if (TotalTSP > 0)
		{
			Tsp = new Analysis::EnergySP[TotalTSP];
			for (int i = 0;i < TotalTSP;i++)
			{
				Tsp[i] = c.Tsp[i];
			}
		}

		if (TotalWave > 0)
		{
			Wav = new IOLib::WaveFormRaw[TotalWave];
			for (int i = 0;i < TotalWave;i++)
			{
				Wav[i] = c.Wav[i];
			}
		}
		
		if (TotalEvt > 0)
		{
			Evt = new IOLib::EventRaw[TotalEvt];
			for (int i = 0;i < TotalEvt;i++)
			{
				Evt[i] = c.Evt[i];
			}
		}
		
		if (TotalState > 0)
		{
			State = new IOLib::StateCode[TotalState];
			for (int i = 0;i < TotalState;i++)
			{
				State[i] = c.State[i];
			}
		}
		
		Run = c.Run;
		filepath = c.filepath;
		filename = c.filename;
	}

	Structedfile& Structedfile::operator =(const Structedfile& c)
	{
		delete[] Esp, Tsp, Evt, Wav, State;
		Esp = nullptr;
		Tsp = nullptr;
		Evt = nullptr;
		Wav = nullptr;
		State = nullptr;

		TotalESP = c.TotalESP;
		TotalTSP = c.TotalTSP;
		TotalWave = c.TotalWave;
		TotalEvt = c.TotalEvt;
		TotalState = c.TotalState;


		if (TotalESP > 0)
		{
			Esp = new Analysis::EnergySP[TotalESP];
			for (int i = 0;i < TotalESP;i++)
			{
				Esp[i] = c.Esp[i];
			}
		}

		if (TotalTSP > 0)
		{
			Tsp = new Analysis::EnergySP[TotalTSP];
			for (int i = 0;i < TotalTSP;i++)
			{
				Tsp[i] = c.Tsp[i];
			}
		}

		if (TotalWave > 0)
		{
			Wav = new IOLib::WaveFormRaw[TotalWave];
			for (int i = 0;i < TotalWave;i++)
			{
				Wav[i] = c.Wav[i];
			}
		}

		if (TotalEvt > 0)
		{
			Evt = new IOLib::EventRaw[TotalEvt];
			for (int i = 0;i < TotalEvt;i++)
			{
				Evt[i] = c.Evt[i];
			}
		}

		if (TotalState > 0)
		{
			State = new IOLib::StateCode[TotalState];
			for (int i = 0;i < TotalState;i++)
			{
				State[i] = c.State[i];
			}
		}

		Run = c.Run;
		filepath = c.filepath;
		filename = c.filename;
		return *this;
	}

	Structedfile::~Structedfile()
	{
		delete[] Esp, Tsp, Wav, Evt, State;
		Esp = nullptr;
		Tsp = nullptr;
		Wav = nullptr;
		Evt = nullptr;
		State = nullptr;
	}

	bool Structedfile::WriteFile(std::string filepath, std::string filename)
	{
		if (filename.size() == 0)
		{
			if (this->filename.size() == 0)
			{
				filename = std::to_string(Run);
				//log info////////////////////////
			}
			else
			{
				filename = this->filename;
			}

		}
		if (filepath.size() == 0)
		{
			if (this->filepath.size() == 0)
			{
				filepath = ".";
				//log info////////////////////////
			}
			else
			{
				filepath = this->filepath;
			}
		}
		else
		{
			CreateDirectory((LPCWSTR)filepath.c_str(), NULL);
		}

		filepath = filepath + "/" + filename + STRUCTED_FILE_EXTENSION;
		std::ofstream savestream(filepath, std::ios::out);
		savestream << "Run Number: " << Run << "\n";
		savestream << "\n" << "\n";

		if (TotalState > 0)
		{
			savestream << "StateSection: " << TotalState << " State(s)\n";
			savestream << "\n";
		}
		for (int i = 0;i < TotalState;i++)
		{
			savestream << "State " << i << " :" << "\n";
			savestream << "\n";
			StateSaveCore(State[i], savestream);
			savestream << "\n" << "\n";
		}
		savestream << "\n";

		if (TotalESP > 0)
		{
			savestream << "EnergySpectrumSection: " << TotalESP << " ESP(s)\n";
			savestream << "\n";
		}
		for (int i = 0;i < TotalESP;i++)
		{
			savestream << "EnergySP " << i << " :" << "\n";
			savestream << "\n";
			ESPSaveCore(Esp[i], savestream);
			savestream << "\n" << "\n";
		}
		savestream << "\n";

		if (TotalTSP > 0)
		{
			savestream << "TimeSpectrumSection: " << TotalTSP << " TSP(s)\n";
			savestream << "\n";
		}
		for (int i = 0;i < TotalTSP;i++)
		{
			savestream << "TimeSP " << i << " :" << "\n";
			savestream << "\n";
			TSPSaveCore(Tsp[i], savestream);
			savestream << "\n" << "\n";
		}
		savestream << "\n";

		if (TotalWave > 0)
		{
			savestream << "WaveFormSection: " << TotalWave << " WaveForm(s)\n";
			savestream << "\n";
		}
		for (int i = 0;i < TotalWave;i++)
		{
			savestream << "WaveForm " << i << " :" << "\n";
			savestream << "\n";
			WaveSaveCore(Wav[i], savestream);
			savestream << "\n" << "\n";
		}
		savestream << "\n";

		if (TotalEvt > 0)
		{
			savestream << "EventSection: " << TotalEvt << "Event(s)\n";
			savestream << "\n";
		}
		for (int i = 0;i < TotalEvt;i++)
		{
			savestream << "Event " << i << " :" << "\n";
			savestream << "\n";
			EventSaveCore(Evt[i], savestream);
			savestream << "\n" << "\n";
		}
		savestream << "\n";
		savestream.close();
		return true;
	}

	bool Structedfile::ReadFile(std::string filepath, std::string filename)
	{
		if (filename.size() == 0)
		{
			//log error/////////////////////////////
			return false;

		}
		if (filepath.size() == 0)
		{
			filepath = ".";
		}
		filepath = filepath + "/" + filename + STRUCTED_FILE_EXTENSION;
		std::ifstream loadstream(filepath, std::ios::in);

		std::string sc;

		loadstream >> sc;
		if (sc != "Run")
		{
			//Log Error File Type////////////
			return false;
		}
		loadstream >> sc;
		if (sc != "Number:")
		{
			//Log Error File Type////////////
			return false;
		}
		loadstream >> Run;
		
		int swrn = 0;
		while (sc.size() > 0 && !loadstream.eof())
		{
			loadstream >> sc;
		OuterLayerOfSTFile:int i = 0;
			bool judge;
			if (sc == "StateSection:")
			{
				loadstream >> TotalState >> sc;

				delete State;
				State = new StateCode[TotalState];

				for (i = 0;i < TotalState;i++)
				{
					int j = 0;
					do
					{
						j++;
						loadstream >> sc;
					} while (sc != "State" && (!OutKey(sc)) && sc.size() > 0 && !loadstream.eof());
					if (j > 1)
					{
						///////Log Warning///////////////////////////////
					}
					if (OutKey(sc) || sc.size() <= 0 || loadstream.eof())
					{
						//Log Warning////////
						goto OuterLayerOfSTFile;
					}
					loadstream >> j >> sc;
					judge = StateLoadCore(State[j], loadstream);
					if (!judge)
					{
						//Log Warning///////////////
					}
				}
			}
			else if (sc == "EnergySpectrumSection:")
			{
				loadstream >> TotalESP >> sc;
				
				delete Esp;
				Esp = new Analysis::EnergySP[TotalESP];

				for (i = 0;i < TotalESP;i++)
				{
					int j = 0;
					do
					{
						j++;
						loadstream >> sc;
					} while (sc != "EnergySP" && (!OutKey(sc)) && sc.size() > 0 && !loadstream.eof());
					if (j > 1)
					{
						///////Log Warning///////////////////////////////
					}
					if (OutKey(sc) || sc.size() <= 0 || loadstream.eof())
					{
						//Log Warning////////
						goto OuterLayerOfSTFile;
					}
					loadstream >> j >> sc;
					judge = ESPLoadCore(Esp[j], loadstream);
					if (!judge)
					{
						//Log Warning///////////////
					}
				}
			}
			else if (sc == "TimeSpectrumSection:")
			{
				loadstream >> TotalTSP >> sc;

				delete Tsp;
				Tsp = new Analysis::EnergySP[TotalTSP];

				for (i = 0;i < TotalTSP;i++)
				{
					int j = 0;
					do
					{
						j++;
						loadstream >> sc;
					} while (sc != "TimeSP" && (!OutKey(sc)) && sc.size() > 0 && !loadstream.eof());
					if (j > 1)
					{
						///////Log Warning///////////////////////////////
					}
					if (OutKey(sc) || sc.size() <= 0 || loadstream.eof())
					{
						//Log Warning////////
						goto OuterLayerOfSTFile;
					}
					loadstream >> j >> sc;
					judge = TSPLoadCore(Tsp[j], loadstream);
					if (!judge)
					{
						//Log Warning///////////////
					}
				}
			}
			else if (sc == "WaveFormSection:")
			{
				loadstream >> TotalWave >> sc;

				delete Wav;
				Wav = new IOLib::WaveFormRaw[TotalWave];

				for (i = 0;i < TotalWave;i++)
				{
					int j = 0;
					do
					{
						j++;
						loadstream >> sc;
					} while (sc != "WaveForm" && (!OutKey(sc)) && sc.size() > 0 && !loadstream.eof());
					if (j > 1)
					{
						///////Log Warning///////////////////////////////
					}
					if (OutKey(sc) || sc.size() <= 0 || loadstream.eof())
					{
						//Log Warning////////
						goto OuterLayerOfSTFile;
					}
					loadstream >> j >> sc;
					judge = WaveLoadCore(Wav[j], loadstream);
					if (!judge)
					{
						//Log Warning///////////////
					}
				}
			}
			else if (sc == "EventSection:")
			{
				loadstream >> TotalEvt >> sc;

				delete Evt;
				Evt = new IOLib::EventRaw[TotalEvt];

				for (i = 0;i < TotalEvt;i++)
				{
					int j = 0;
					do
					{
						j++;
						loadstream >> sc;
					} while (sc != "Event" && (!OutKey(sc)) && sc.size() > 0 && !loadstream.eof());
					if (j > 1)
					{
						///////Log Warning///////////////////////////////
					}
					if (OutKey(sc) || sc.size() <= 0 || loadstream.eof())
					{
						//Log Warning////////
						goto OuterLayerOfSTFile;
					}
					loadstream >> j >> sc;
					judge = EventLoadCore(Evt[j], loadstream);
					if (!judge)
					{
						//Log Warning///////////////
					}
				}
			}
			else
			{
				swrn++;
			}
		}
		if (swrn > 0)
		{
			//Log Warning///////////////
		}
		loadstream.close();
		return true;
	}
}
