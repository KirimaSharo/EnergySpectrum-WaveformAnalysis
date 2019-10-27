#define DLL_IO _declspec(dllexport)
#include <Windows.h>
#define UNDEFINED_RUN -1
#define FRAMEHEAD 0xAA55  ////////////////Little Endian
#define FRAMEHEAD_RAW 0x90EB
#define UCHAR8 unsigned char
#define USHORT16 unsigned short
#define INT32I int
#define FRAME_LEN_EXT 4

#define ESP_TOTALCHANNEL 4096
#define TSP_TOTALCHANNEL 1024

#define LOC_BYTE_DE_LOADCAT 8
#define LOC_BYTE_DE_SRC 6
#define LOADCAT_ESP 0x04
#define LOADCAT_TSP 0x05
#define LOADCAT_WAV 0x06
#define LOADCAT_EVT 0x07
#define LOADCAT_FLS 0x08
#define LOADCAT_ST 0x03

#define LOC_BYTE_DE_RCAT 3
#define RCAT_CMD 0x00
#define RCAT_CMDR 0x10
#define RCAT_ST 0x20
#define RCAT_ESP 0xA0
#define RCAT_TSP 0xB0
#define RCAT_WAV 0xC0
#define RCAT_EVT 0xD0
#define REND 0xA55A
#define REND_LEN 2


#define TIMEPOS 9
#define STCODEPOS 15
#define STPARAMPOS 17

#define FLS_PAYLOADPOS 9
#define FLS_PAYLOADLEN 256

#define RAW_FILE_EXTENSION ".bin"

//#include "stdafx.h"
#include "fstream"
#include "IOlibrary.h"
#include "CRC.h"

struct STC
{
	IOLib::StateCode st;
	STC* Next;
};
struct ESPC
{
	Analysis::EnergySP esp;
	ESPC* Next;
};
struct WAVC
{
	IOLib::WaveFormRaw wav;
	WAVC* Next;
};
struct EVTC
{
	IOLib::EventRaw evt;
	EVTC* Next;
};

//Judged by LOC_BYTE_DE_RCAT
int RCATLen(UCHAR8 RCAT,USHORT16 RefLen)
{
	switch (RCAT)
	{
	case RCAT_CMD:
		return 16;
	case RCAT_CMDR:
		return 14;
	case RCAT_ST:
		return 18;
	}
	if (RCAT - RCAT_EVT < 16 && RCAT - RCAT_EVT >= 0)
	{
		return 18;
	}
	else
	{
		return RefLen;
	}
}


void TimeAnalysis(UCHAR8* Frame, tm& Time)
{
	Time.tm_year = Frame[0] + 100;
	Time.tm_mon = Frame[1] - 1;
	Time.tm_mday = Frame[2];
	Time.tm_hour = Frame[3];
	Time.tm_min = Frame[4];
	Time.tm_sec = Frame[5];
	time_t time = mktime(&Time);
	localtime_s(&Time, &time);
}


IOLib::StateCode State(UCHAR8* Frame, int FLen, bool& CRC8,const CRC::Table<std::uint8_t, 8> & table)
{
	UCHAR8 crc = CRC::Calculate(Frame, (size_t)(FLen - 1), table);
	if (Frame[FLen - 1] == crc)
	{
		CRC8 = true;
	}
	else
	{
		CRC8 = false;
	}
	IOLib::StateCode ret;

	TimeAnalysis(Frame + TIMEPOS,ret.Time);
	
	for (int i = 0;i < 6;i++)
	{
		ret.Sparam[i] = Frame[STPARAMPOS + i];
	}
	ret.STCode = *(USHORT16*)Frame[STCODEPOS];
	return ret;
}

//FrameLengthFixedTo6+266//////////////////////////////////////
bool ESPF(UCHAR8* Frame, int FLen, bool& CRC16, const CRC::Table<std::uint16_t, 16> & table, Analysis::EnergySP& ESP)
{
	USHORT16 crc = CRC::Calculate(Frame + 6, (size_t)(FLen - 8), table);
	if (*(USHORT16*)(Frame + FLen - 2) == crc)
	{
		CRC16 = true;
	}
	else
	{
		CRC16 = false;
	}
	TimeAnalysis(Frame, ESP.UploadTime);
	UCHAR8 n = *(Frame + 11);
	
	if ((int)n * 128 + 128 > ESP_TOTALCHANNEL)
	{
		return false;
	}
	ESP.TotalChannel = (int)n * 128 + 128;

	for (int i = 0;i < 128;i++)
	{
		ESP.Count[n * 128 + i] = *(USHORT16*)(Frame + 12 + 2 * i);
	}
	return true;
}

bool TSPF(UCHAR8* Frame, int FLen, bool& CRC16, const CRC::Table<std::uint16_t, 16> & table, Analysis::EnergySP& TSP)
{
	USHORT16 crc = CRC::Calculate(Frame + 6, (size_t)(FLen - 8), table);
	if (*(USHORT16*)(Frame + FLen - 2) == crc)
	{
		CRC16 = true;
	}
	else
	{
		CRC16 = false;
	}
	TimeAnalysis(Frame, TSP.UploadTime);
	UCHAR8 n = *(Frame + 11);
	if ((int)n * 128 + 128 > TSP_TOTALCHANNEL)
	{
		return false;
	}
	TSP.TotalChannel = (int)n * 128 + 128;
	for (int i = 0;i < 128;i++)
	{
		TSP.Count[n * 128 + i] = *(USHORT16*)(Frame + 12 + 2 * i);
	}
	return true;
}

bool WAVF(UCHAR8* Frame, int FLen, bool& CRC16, const CRC::Table<std::uint16_t, 16> & table, Analysis::WaveForm& Wav)
{
	USHORT16 crc = CRC::Calculate(Frame + 6, (size_t)(FLen - 8), table);
	if (*(USHORT16*)(Frame + FLen - 2) == crc)
	{
		CRC16 = true;
	}
	else
	{
		CRC16 = false;
	}
	TimeAnalysis(Frame, Wav.UploadTime);
	UCHAR8 n = *(Frame + 11);
	if ((int)n * 128 > Wav.TotalSample)
	{
		return false;
	}
	Wav.TotalSample = (int)n * 128 + 128;
	double* a = new double[Wav.TotalSample];
	for (int i = 0;i < (int)n * 128;i++)
	{
		a[i] = Wav.SampleValue[i];
	}
	delete Wav.SampleValue;
	Wav.SampleValue = a;

	for (int i = 0;i < 128;i++)
	{
		Wav.SampleValue[n * 128 + i] = *(USHORT16*)(Frame + 12 + 2 * i);
	}
	return true;
}

bool WAVF(UCHAR8* Frame, int FLen, bool& CRC16, const CRC::Table<std::uint16_t, 16> & table, IOLib::WaveFormRaw& Wav)
{
	USHORT16 crc = CRC::Calculate(Frame + 6, (size_t)(FLen - 8), table);
	if (*(USHORT16*)(Frame + FLen - 2) == crc)
	{
		CRC16 = true;
	}
	else
	{
		CRC16 = false;
	}
	TimeAnalysis(Frame, Wav.UploadTime);
	UCHAR8 n = *(Frame + 11);
	if ((int)n * 128 > Wav.TotalSample)
	{
		return false;
	}
	Wav.TotalSample = (int)n * 128 + 128;
	int* a = new int[Wav.TotalSample];
	for (int i = 0;i < (int)n * 128;i++)
	{
		a[i] = Wav.SampleValue[i];
	}
	delete Wav.SampleValue;
	Wav.SampleValue = a;
	for (int i = 0;i < 128;i++)
	{
		Wav.SampleValue[n * 128 + i] = *(USHORT16*)(Frame + 12 + 2 * i);
	}
	return true;
}

bool EVTF(UCHAR8* Frame, int FLen, bool& CRC16, const CRC::Table<std::uint16_t, 16> & table, Analysis::Event& Evt, UCHAR8 src)
{
	USHORT16 crc = CRC::Calculate(Frame + 6, (size_t)(FLen - 8), table);
	if (*(USHORT16*)(Frame + FLen - 2) == crc)
	{
		CRC16 = true;
	}
	else
	{
		CRC16 = false;
	}
	Evt.source = src;
	if (src >= 0x01 && src <= 0x1F)//CZT
	{
		Evt.Amp = *(USHORT16*)(Frame + 12);
		Evt.RiseTime= *(USHORT16*)(Frame + 14);
		Evt.ArrivialTime= *(USHORT16*)(Frame + 16);
		Evt.FallTime = -1;
		Evt.Area = -1;
	}
	else if (src >= 0x20 && src <= 0x3F)//Crystal
	{
		Evt.Amp = *(USHORT16*)(Frame + 12);
		Evt.RiseTime = *(USHORT16*)(Frame + 14);
		Evt.FallTime = *(USHORT16*)(Frame + 16);
		Evt.Area= *(USHORT16*)(Frame + 18);
		Evt.ArrivialTime = -1;
	}
	else if (src >= 0x40 && src <= 0x5F)//Scatter
	{
		Evt.Amp = -1;
		Evt.RiseTime = -1;
		Evt.ArrivialTime = *(USHORT16*)(Frame + 12);
		Evt.FallTime = -1;
		Evt.Area = -1;
	}
	else if (src >= 0x60 && src <= 0x7F)//Capture
	{
		Evt.Amp = *(USHORT16*)(Frame + 12);
		Evt.RiseTime = -1;
		Evt.ArrivialTime = -1;
		Evt.FallTime = -1;
		Evt.Area = -1;
	}
	else
	{
		//Log Error
		return false;
	}
	return true;

}

bool EVTF(UCHAR8* Frame, int FLen, bool& CRC16, const CRC::Table<std::uint16_t, 16> & table, IOLib::EventRaw& Evt, UCHAR8 src)
{
	USHORT16 crc = CRC::Calculate(Frame + 6, (size_t)(FLen - 8), table);
	if (*(USHORT16*)(Frame + FLen - 2) == crc)
	{
		CRC16 = true;
	}
	else
	{
		CRC16 = false;
	}
	Evt.source = src;
	if (src >= 0x01 && src <= 0x1F)//CZT
	{
		Evt.Amp = *(USHORT16*)(Frame + 12);
		Evt.RiseTime = *(USHORT16*)(Frame + 14);
		Evt.ArrivialTime = *(USHORT16*)(Frame + 16);
		Evt.FallTime = -1;
		Evt.Area = -1;
	}
	else if (src >= 0x20 && src <= 0x3F)//Crystal
	{
		Evt.Amp = *(USHORT16*)(Frame + 12);
		Evt.RiseTime = *(USHORT16*)(Frame + 14);
		Evt.FallTime = *(USHORT16*)(Frame + 16);
		Evt.Area = *(USHORT16*)(Frame + 18);
		Evt.ArrivialTime = -1;
	}
	else if (src >= 0x40 && src <= 0x5F)//Scatter
	{
		Evt.Amp = -1;
		Evt.RiseTime = -1;
		Evt.ArrivialTime = *(USHORT16*)(Frame + 12);
		Evt.FallTime = -1;
		Evt.Area = -1;
	}
	else if (src >= 0x60 && src <= 0x7F)//Capture
	{
		Evt.Amp = *(USHORT16*)(Frame + 12);
		Evt.RiseTime = -1;
		Evt.ArrivialTime = -1;
		Evt.FallTime = -1;
		Evt.Area = -1;
	}
	else
	{
		//Log Error
		return false;
	}
	return true;
}





namespace IOLib
{
	void ParamNote(const IOLib::StateCode& State, IOLib::StateNote& SNote)
	{
		switch (State.STCode)
		{
		case 0x0001:
			SNote.Paramsize = 3;
			SNote.Paramnote[0] = "(0)WorkMode";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 0;
			SNote.Paramnote[1] = "(1)SelfTestResult";
			SNote.Paramstart[1] = 1;
			SNote.Paramend[1] = 1;
			SNote.Paramnote[2] = "(2-5)RemainingFlashStorage";
			SNote.Paramstart[2] = 2;
			SNote.Paramend[2] = 5;
			SNote.Statenote = "GenWorkStat";
			break;
		case 0x0002:
			SNote.Paramsize = 2;
			SNote.Paramnote[0] = "(0-1)Temperature";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 1;
			SNote.Paramnote[1] = "(2-5)Acceleration&Angle";
			SNote.Paramstart[1] = 2;
			SNote.Paramend[1] = 5;
			SNote.Statenote = "GenSensorStat";
			break;
		case 0x0003:
			SNote.Paramsize = 1;
			SNote.Paramnote[0] = "(0)Address";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 0;
			SNote.Statenote = "DevAddr";
			break;
		case 0x0200:
			SNote.Paramsize = 2;
			SNote.Paramnote[0] = "(0)ADCConfig";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 0;
			SNote.Paramnote[1] = "(1-2)Params";
			SNote.Paramstart[1] = 1;
			SNote.Paramend[1] = 2;
			SNote.Statenote = "DebugCZTInfo";
			break;
		case 0x0201:
			SNote.Paramsize = 2;
			SNote.Paramnote[0] = "(0-1)Temperature";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 1;
			SNote.Paramnote[1] = "(2-3)Voltage";
			SNote.Paramstart[1] = 2;
			SNote.Paramend[1] = 3;
			SNote.Statenote = "CZTStatues";
			break;
		case 0x0202:
			SNote.Paramsize = 2;
			SNote.Paramnote[0] = "(0)Channel";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 0;
			SNote.Paramnote[1] = "(1-2)Thershold";
			SNote.Paramstart[1] = 1;
			SNote.Paramend[1] = 2;
			SNote.Statenote = "CZTChThreshold";
			break;
		case 0x0300:
			SNote.Paramsize = 2;
			SNote.Paramnote[0] = "(0-1)Temperature";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 1;
			SNote.Paramnote[1] = "(2-3)Voltage";
			SNote.Paramstart[1] = 2;
			SNote.Paramend[1] = 3;
			SNote.Statenote = "CrystalStatus";
			break;
		case 0x0400:
			SNote.Paramsize = 2;
			SNote.Paramnote[0] = "(0)Address";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 0;
			SNote.Paramnote[1] = "(1)Value";
			SNote.Paramstart[1] = 1;
			SNote.Paramend[1] = 1;
			SNote.Statenote = "SRMReg";
			break;
		case 0x0401:
			SNote.Paramsize = 1;
			SNote.Paramnote[0] = "(0-1)Temperature";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 1;
			SNote.Statenote = "SRMStatus";
			break;
		case 0x0402:
			SNote.Paramsize = 1;
			SNote.Paramnote[0] = "Undefined";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 5;
			SNote.Statenote = "NeuTubeStatus";
			break;
		case 0x0403:
			SNote.Paramsize = 1;
			SNote.Paramnote[0] = "(0)Undefined";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 0;
			SNote.Statenote = "DiagSRMInfo";
			break;
		case 0x0500:
			SNote.Paramsize = 2;
			SNote.Paramnote[0] = "(0-1)Address";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 1;
			SNote.Paramnote[1] = "(2)Value";
			SNote.Paramstart[1] = 2;
			SNote.Paramend[1] = 2;
			SNote.Statenote = "CaptReg";
			break;
		case 0x0501:
			SNote.Paramsize = 1;
			SNote.Paramnote[0] = "Undefined";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 5;
			SNote.Statenote = "CaptStatus";
			break;
		case 0x0502:
			SNote.Paramsize = 1;
			SNote.Paramnote[0] = "Undefined";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 5;
			SNote.Statenote = "NeuTubeStatus";
			break;
		case 0x0503:
			SNote.Paramsize = 1;
			SNote.Paramnote[0] = "Undefined";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 5;
			SNote.Statenote = "CaptDiagInfo";
			break;
		case 0x0600:
			SNote.Paramsize = 1;
			SNote.Paramnote[0] = "(0)Address";
			SNote.Paramstart[0] = 0;
			SNote.Paramend[0] = 0;
			SNote.Statenote = "DevAddr";
			break;
		default:
			SNote.Paramsize = 0;
			SNote.Statenote = "Resvered";
		}
		return;
	}

	Rawdatafile::Rawdatafile()
	{
		Run = UNDEFINED_RUN;
		filepath = "";
		filename = "";
		cur = nullptr;
		init = nullptr;
	}

	Rawdatafile::Rawdatafile(const Rawdatafile& c)
	{
		cur = init;
		while (cur != nullptr)
		{
			init = cur;
			cur = cur->Next;
			delete init->Framedata;
			delete init;
			init = nullptr;
		}
		
		FrameC* CCur = c.init;

		if (CCur != nullptr)
		{
			init = new FrameC;
			cur = init;
		}


		while (CCur != nullptr)
		{
			cur->Flen = CCur->Flen;
			cur->Framedata = new UCHAR8[cur->Flen];
			for (int i = 0;i < cur->Flen;i++)
			{
				cur->Framedata[i] = CCur->Framedata[i];
			}
			CCur = CCur->Next;
			if (CCur != nullptr)
			{
				cur->Next = new FrameC;
				cur = cur->Next;
			}
			else
			{
				cur->Next = nullptr;
			}

		}

		Run = c.Run;
		filepath = c.filepath;
		filename = c.filename;

	}

	Rawdatafile& Rawdatafile::operator =(const Rawdatafile& c)
	{
		cur = init;
		while (cur != nullptr)
		{
			init = cur;
			cur = cur->Next;
			delete init->Framedata;
			delete init;
			init = nullptr;
		}

		FrameC* CCur = c.init;

		if (CCur != nullptr)
		{
			init = new FrameC;
			cur = init;
		}


		while (CCur != nullptr)
		{
			cur->Flen = CCur->Flen;
			cur->Framedata = new UCHAR8[cur->Flen];
			for (int i = 0;i < cur->Flen;i++)
			{
				cur->Framedata[i] = CCur->Framedata[i];
			}
			CCur = CCur->Next;
			if (CCur != nullptr)
			{
				cur->Next = new FrameC;
				cur = cur->Next;
			}
			else
			{
				cur->Next = nullptr;
			}

		}

		Run = c.Run;
		filepath = c.filepath;
		filename = c.filename;
		return *this;
	}

	Rawdatafile::~Rawdatafile()
	{
		cur = init;
		while (cur != nullptr)
		{
			init = cur;
			cur = cur->Next;
			delete init->Framedata;
			delete init;
			init = nullptr;
		}
	}

	bool Rawdatafile::ReadFile(std::string filepath, std::string filename)
	{
		if (filename == (std::string)nullptr || filename.size() == 0)
		{
			//log error/////////////////////////////
			return false;

		}
		if (filepath == (std::string)nullptr || filepath.size() == 0)
		{
			filepath = ".";
		}
		cur = init;
		while (cur != nullptr)
		{
			init = cur;
			cur = cur->Next;
			delete init->Framedata;
			delete init;
			init = nullptr;
		}

		filepath = filepath + "/" + filename + RAW_FILE_EXTENSION;
		std::ifstream loadstream(filepath, std::ios::in | std::ios::binary);
		UCHAR8 ch = 0;

		USHORT16 ch2 = 0;
		USHORT16* ps1 = nullptr;
		loadstream.read((char*)&Run, sizeof(INT32I));

		if (Run = UNDEFINED_RUN)
		{
			//Log error//////////
			return false;
		}


		while (!loadstream.eof())
		{
			loadstream.read((char*)&ch2, sizeof(USHORT16));
			if (ch2 != FRAMEHEAD)
			{
				///////////////////////////////////Log Error
				return false;
			}
			
			if (init == nullptr)
			{
				cur = new FrameC;
				init = cur;
			}
			else
			{
				cur->Next = new FrameC;
				cur = cur->Next;
			}
			cur->Next = nullptr;

			loadstream.read((char*)&ch2, sizeof(USHORT16));
			
			cur->Flen = ch2 + FRAME_LEN_EXT;
			cur->Framedata = new UCHAR8[cur->Flen];
			ps1 = (USHORT16*)cur->Framedata;
			*ps1 = FRAMEHEAD;

			ps1 = ps1 + sizeof(USHORT16);
			*ps1 = ch2;

			ps1 = ps1 + sizeof(USHORT16);
			loadstream.read((char*)ps1, ch2*sizeof(UCHAR8));
		}
		this->filepath = filepath;
		this->filename = filename;
		loadstream.close();
		return true;

	}

	//CMD¡¢CMDR¡¢ST UNWRITTEN////////////////////////////////////////
	bool Rawdatafile::WriteFrame(unsigned char* Framedata)
	{
		
		USHORT16* ps1;
		USHORT16 s2;
		s2 = *(USHORT16*)Framedata;
		if (s2 != FRAMEHEAD)
		{
			//log error////////////////////////////////////////
			return false;
		}
		
		ps1 = (USHORT16*)Framedata + sizeof(USHORT16);
		s2 = *ps1;

		if (init == nullptr)
		{
			cur = new FrameC;
			init = cur;
		}
		else
		{
			cur->Next = new FrameC;
			cur = cur->Next;
		}
		cur->Next = nullptr;

		cur->Flen = s2 + FRAME_LEN_EXT;
		cur->Framedata = new UCHAR8[cur->Flen];
		for (int i = 0;i < cur->Flen;i++)
		{
			cur->Framedata[i] = Framedata[i];
		}
		return true;

	}

	bool Rawdatafile::WriteFile(std::string filepath, std::string filename)
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

		filepath = filepath + "/" + filename + RAW_FILE_EXTENSION;
		std::ofstream savestream(filepath, std::ios::out | std::ios::binary);
		savestream.write((char*)&Run, sizeof(int));
		cur = init;
		while (cur != nullptr)
		{
			if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_ESP)
			{
				savestream.write((char*)cur->Framedata, cur->Flen * sizeof(UCHAR8));
			}
		}

		cur = init;
		while (cur != nullptr)
		{
			if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_TSP)
			{
				savestream.write((char*)cur->Framedata, cur->Flen * sizeof(UCHAR8));
			}
		}

		cur = init;
		while (cur != nullptr)
		{
			if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_WAV)
			{
				savestream.write((char*)cur->Framedata, cur->Flen * sizeof(UCHAR8));
			}
		}

		cur = init;
		while (cur != nullptr)
		{
			if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_EVT)
			{
				savestream.write((char*)cur->Framedata, cur->Flen * sizeof(UCHAR8));
			}
		}

		cur = init;
		while (cur != nullptr)
		{
			if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_FLS)
			{
				savestream.write((char*)cur->Framedata, cur->Flen * sizeof(UCHAR8));
			}
		}
		savestream.close();
		return true;
	}

	bool Rawdatafile::AddOnFile(std::string filepath, std::string filename)
	{
		if (filename.size() == 0)
		{
			//log error/////////////////////////////////////////
		}
		if (filepath.size() == 0)
		{
			filepath = ".";
		}
		Rawdatafile* raw = new Rawdatafile();
		raw->ReadFile(filepath, filename);
		if (raw->Run = UNDEFINED_RUN)
		{
			//Log error//////////
			return false;
		}
		if (raw->Run != Run)
		{
			//Log Warning///////////////////////////////////////
		}
		FrameC* Curlog = raw->cur;
		if (Curlog == nullptr)
		{		
			//log Warning/////////////////////
			delete raw;
			return WriteFile(filepath, filename);
		}
		raw->cur->Next = init;
		raw->WriteFile(filepath, filename);
		raw->cur = Curlog;
		raw->cur->Next = nullptr;
		delete raw;
	}

	Structedfile Rawdatafile::AnalysisFrameCache()
	{
		Structedfile ret;
		ret.filepath = filepath;
		ret.filename = filename;
		ret.Run = Run;
		cur = init;
		UCHAR8 FlashCache[4 * FLS_PAYLOADLEN];
		int UsedFlashCache = 0;
		int i = 0;
		int AnalysisedFrames = 0;
		bool crc8 = true, crc16 = true, judge = true;

		STC* STinit = nullptr,* STcur = nullptr;
		ESPC* ESPinit = nullptr, *ESPcur = nullptr;
		ESPC* TSPinit = nullptr, *TSPcur = nullptr;
		WAVC* WAVinit = nullptr, *WAVcur = nullptr;
		EVTC* EVTinit = nullptr, *EVTcur = nullptr;
		UCHAR8 src = 0, srcf = 0;

		int stc = 0, espc = 0, tspc = 0, wavc = 0, evtc = 0;

		CRC::Table<std::uint8_t, 8> CRCT8(CRC::CRC_8());
		CRC::Table<std::uint16_t, 16> CRCT16(CRC::CRC_16_KERMIT());
		while (cur != nullptr)
		{
			AnalysisedFrames++;
			if (cur->Framedata == nullptr || cur->Flen == 0)
			{
				//Log error
				continue;
			}
			if (CRC::Calculate(cur->Framedata, cur->Flen, CRCT8) == false)
			{
				//Log Error CRC8
			}
			if (*(USHORT16*)cur->Framedata != FRAMEHEAD)
			{
				//Log Error Head
			}

			if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_FLS)
			{

				srcf = cur->Framedata[LOC_BYTE_DE_SRC];
				for (i = 0;i < FLS_PAYLOADLEN;i++)
				{
					FlashCache[i + UsedFlashCache] = cur->Framedata[i + FLS_PAYLOADPOS];
				}
				UsedFlashCache += FLS_PAYLOADLEN;
				int InDiff = 0;
				UCHAR8* FCache = FlashCache;
				while (UsedFlashCache >= 2 * FLS_PAYLOADLEN)
				{
					int diff = 0;
					if (UsedFlashCache >= 2 * FLS_PAYLOADLEN && (*(USHORT16*)FCache == FRAMEHEAD || *(USHORT16*)(FCache + 6) == FRAMEHEAD_RAW))
					{

						if (*(USHORT16*)(FCache + 6) != FRAMEHEAD_RAW)
						{
							if (STcur == nullptr)
							{
								STcur = new STC;
								STinit = STcur;
							}
							else
							{
								STcur->Next = new STC;
								STcur = STcur->Next;
							}
							stc++;
							STcur->Next = nullptr;
							STcur->st = State(FCache, 24, crc8, CRCT8);
							if (crc8 == false)
							{
								//Log Error CRC//////////////////
							}
							diff += 24;
						}
						else if (((*(UCHAR8*)(FCache + 8))&((USHORT16)0xF0)) == RCAT_ESP)
						{
							if (*(FCache + 11) == 0)
							{
								if (ESPcur == nullptr)
								{
									ESPcur = new ESPC;
									ESPinit = ESPcur;
								}
								else
								{
									ESPcur->Next = new ESPC;
									ESPcur = ESPcur->Next;
								}
								espc++;
								ESPcur->Next = nullptr;
								ESPcur->esp.TotalChannel = ESP_TOTALCHANNEL;
								ESPcur->esp.Count = new int[ESP_TOTALCHANNEL];
							}

							judge = ESPF(FCache, 270, crc16, CRCT16, ESPcur->esp);
							if (judge == false)
							{
								//Log Error Oversized ESP//////////////
							}
							if (crc16 == false)
							{
								//Log Error CRC16
							}
							diff += 270 + REND_LEN;
						}
						else if (((*(UCHAR8*)(FCache + 8))&((USHORT16)0xF0)) == RCAT_TSP)
						{
							if (*(FCache + 11) == 0)
							{
								if (TSPcur == nullptr)
								{
									TSPcur = new ESPC;
									TSPinit = TSPcur;
								}
								else
								{
									TSPcur->Next = new ESPC;
									TSPcur = TSPcur->Next;
								}
								tspc++;
								TSPcur->Next = nullptr;
								TSPcur->esp.TotalChannel = TSP_TOTALCHANNEL;
								TSPcur->esp.Count = new int[TSP_TOTALCHANNEL];
							}

							judge = TSPF(FCache, 270, crc16, CRCT16, TSPcur->esp);
							if (judge == false)
							{
								//Log Error Oversized ESP//////////////
							}
							if (crc16 == false)
							{
								//Log Error CRC16
							}
							diff += 270 + REND_LEN;
						}
						else if (((*(UCHAR8*)(FCache + 8))&((USHORT16)0xF0)) == RCAT_WAV)
						{
							if (*(FCache + 11) == 0)
							{
								if (WAVcur == nullptr)
								{
									WAVcur = new WAVC;
									WAVinit = WAVcur;
								}
								else
								{
									WAVcur->Next = new WAVC;
									WAVcur = WAVcur->Next;
								}
								wavc++;
								WAVcur->Next = nullptr;
							}

							judge = WAVF(FCache, 270, crc16, CRCT16, WAVcur->wav);
							if (judge == false)
							{
								//Log Error Section Number WAVE//////////////
							}
							if (crc16 == false)
							{
								//Log Error CRC16
							}
							diff += 270 + REND_LEN;
						}
						else if (((*(UCHAR8*)(FCache + 9))&((USHORT16)0xF0)) == RCAT_EVT)
						{
							if (EVTcur == nullptr)
							{
								EVTcur = new EVTC;
								EVTinit = EVTcur;
							}
							else
							{
								EVTcur->Next = new EVTC;
								EVTcur = EVTcur->Next;
							}
							evtc++;
							EVTcur->Next = nullptr;

							judge = EVTF(FCache, 22, crc16, CRCT16, EVTcur->evt, srcf);
							if (judge == false)
							{
								//Log Error Section Number WAVE//////////////
							}
							if (crc16 == false)
							{
								//Log Error CRC16
							}
							diff += 22 + REND_LEN;
						}

						InDiff += diff;
						UsedFlashCache -= diff;
						if (UsedFlashCache < 0)
						{
							//Log error Analysis dataFrame too short
							InDiff = 4 * FLS_PAYLOADLEN;
						}

						FCache = FCache + diff;
					}
					else
					{
						InDiff++;
						UsedFlashCache--;
						FCache++;
					}
				}
				
				for (i = 0;i < UsedFlashCache;i++)
				{
					FlashCache[i] = FlashCache[i + InDiff];
				}
			}
			else if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_ST)
			{
				if (STcur == nullptr)
				{
					STcur = new STC;
					STinit = STcur;
				}
				else
				{
					STcur->Next = new STC;
					STcur = STcur->Next;
				}
				stc++;
				STcur->Next = nullptr;
				STcur->st = State(cur->Framedata, 24, crc8, CRCT8);
				if (crc8 == false)
				{
					//Log Error CRC//////////////////
				}
			}
			else if(cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_ESP)
			{
				if (*(cur->Framedata + 20) == 0)
				{
					if (ESPcur == nullptr)
					{
						ESPcur = new ESPC;
						ESPinit = ESPcur;
					}
					else
					{
						ESPcur->Next = new ESPC;
						ESPcur = ESPcur->Next;
					}
					espc++;
					ESPcur->Next = nullptr;
					ESPcur->esp.TotalChannel = ESP_TOTALCHANNEL;
					ESPcur->esp.Count = new int[ESP_TOTALCHANNEL];	
				}
				judge = ESPF(cur->Framedata + 9, 270, crc16, CRCT16, ESPcur->esp);
				if (judge == false)
				{
					//Log Error Oversized ESP//////////////
				}
				if (crc16 == false)
				{
					//Log Error CRC16
				}
			}
			else if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_TSP)
			{
				if (*(cur->Framedata + 20) == 0)
				{
					if (TSPcur == nullptr)
					{
						TSPcur = new ESPC;
						TSPinit = TSPcur;
					}
					else
					{
						TSPcur->Next = new ESPC;
						TSPcur = TSPcur->Next;
					}
					tspc++;
					TSPcur->Next = nullptr;
					TSPcur->esp.TotalChannel = TSP_TOTALCHANNEL;
					TSPcur->esp.Count = new int[TSP_TOTALCHANNEL];
				}
				judge = TSPF(cur->Framedata + 9, 270, crc16, CRCT16, TSPcur->esp);
				if (judge == false)
				{
					//Log Error Oversized TSP//////////////
				}
				if (crc16 == false)
				{
					//Log Error CRC16
				}
			}
			else if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_WAV)
			{
				if (*(cur->Framedata + 20) == 0)
				{
					if (WAVcur == nullptr)
					{
						WAVcur = new WAVC;
						WAVinit = WAVcur;
					}
					else
					{
						WAVcur->Next = new WAVC;
						WAVcur = WAVcur->Next;
					}
					wavc++;
					WAVcur->Next = nullptr;		
				}
				judge = WAVF(cur->Framedata + 9, 270, crc16, CRCT16, WAVcur->wav);
				if (judge == false)
				{
					//Log Error Oversized TSP//////////////
				}
				if (crc16 == false)
				{
					//Log Error CRC16
				}
			}
			else if (cur->Framedata[LOC_BYTE_DE_LOADCAT] == LOADCAT_EVT)
			{
				if (EVTcur == nullptr)
				{
					EVTcur = new EVTC;
					EVTinit = EVTcur;
				}
				else
				{
					EVTcur->Next = new EVTC;
					EVTcur = EVTcur->Next;
				}
				evtc++;
				EVTcur->Next = nullptr;
				src = cur->Framedata[LOC_BYTE_DE_SRC];
				judge = EVTF(cur->Framedata + 9, 22, crc16, CRCT16, EVTcur->evt, src);
				if (judge == false)
				{
					//Log Error Section Number WAVE//////////////
				}
				if (crc16 == false)
				{
					//Log Error CRC16
				}
			}
			else
			{
			//Log Error Cannot Analysis Frame
			}
		}

		int InDiff = 0;
		UCHAR8* FCache = FlashCache;
		while (UsedFlashCache >= 0 && (*(USHORT16*)FCache == FRAMEHEAD || *(USHORT16*)(FCache + 6) == FRAMEHEAD_RAW))
		{
			int diff = 0;
			if (*(USHORT16*)(FCache + 6) != FRAMEHEAD_RAW)
			{
				if (STcur == nullptr)
				{
					STcur = new STC;
					STinit = STcur;
				}
				else
				{
					STcur->Next = new STC;
					STcur = STcur->Next;
				}
				stc++;
				STcur->Next = nullptr;
				STcur->st = State(FCache, 24, crc8, CRCT8);
				if (crc8 == false)
				{
					//Log Error CRC//////////////////
				}
				diff += 24;
			}
			else if (((*(UCHAR8*)(FCache + 8))&((USHORT16)0xF0)) == RCAT_ESP)
			{
				if (*(FCache + 11) == 0)
				{
					if (ESPcur == nullptr)
					{
						ESPcur = new ESPC;
						ESPinit = ESPcur;
					}
					else
					{
						ESPcur->Next = new ESPC;
						ESPcur = ESPcur->Next;
					}
					espc++;
					ESPcur->Next = nullptr;
					ESPcur->esp.TotalChannel = ESP_TOTALCHANNEL;
					ESPcur->esp.Count = new int[ESP_TOTALCHANNEL];
				}

				judge = ESPF(FCache, 270, crc16, CRCT16, ESPcur->esp);
				if (judge == false)
				{
					//Log Error Oversized ESP//////////////
				}
				if (crc16 == false)
				{
					//Log Error CRC16
				}
				diff += 270 + REND_LEN;
			}
			else if (((*(UCHAR8*)(FCache + 8))&((USHORT16)0xF0)) == RCAT_TSP)
			{
				if (*(FCache + 11) == 0)
				{
					if (TSPcur == nullptr)
					{
						TSPcur = new ESPC;
						TSPinit = TSPcur;
					}
					else
					{
						TSPcur->Next = new ESPC;
						TSPcur = TSPcur->Next;
					}
					tspc++;
					TSPcur->Next = nullptr;
					TSPcur->esp.TotalChannel = TSP_TOTALCHANNEL;
					TSPcur->esp.Count = new int[TSP_TOTALCHANNEL];
				}

				judge = TSPF(FCache, 270, crc16, CRCT16, TSPcur->esp);
				if (judge == false)
				{
					//Log Error Oversized ESP//////////////
				}
				if (crc16 == false)
				{
					//Log Error CRC16
				}
				diff += 270 + REND_LEN;
			}
			else if (((*(UCHAR8*)(FCache + 8))&((USHORT16)0xF0)) == RCAT_WAV)
			{
				if (*(FCache + 11) == 0)
				{
					if (WAVcur == nullptr)
					{
						WAVcur = new WAVC;
						WAVinit = WAVcur;
					}
					else
					{
						WAVcur->Next = new WAVC;
						WAVcur = WAVcur->Next;
					}
					wavc++;
					WAVcur->Next = nullptr;
				}

				judge = WAVF(FCache, 270, crc16, CRCT16, WAVcur->wav);
				if (judge == false)
				{
					//Log Error Section Number WAVE//////////////
				}
				if (crc16 == false)
				{
					//Log Error CRC16
				}
				diff += 270 + REND_LEN;
			}
			else if (((*(UCHAR8*)(FCache + 9))&((USHORT16)0xF0)) == RCAT_EVT)
			{
				if (EVTcur == nullptr)
				{
					EVTcur = new EVTC;
					EVTinit = EVTcur;
				}
				else
				{
					EVTcur->Next = new EVTC;
					EVTcur = EVTcur->Next;
				}
				evtc++;
				EVTcur->Next = nullptr;

				judge = EVTF(FCache, 22, crc16, CRCT16, EVTcur->evt, srcf);
				if (judge == false)
				{
					//Log Error Section Number WAVE//////////////
				}
				if (crc16 == false)
				{
					//Log Error CRC16
				}
				diff += 22 + REND_LEN;
			}
			InDiff += diff;
			UsedFlashCache -= diff;
			if (UsedFlashCache < 0)
			{
				//Log error Analysis dataFrame too short
				InDiff = 4 * FLS_PAYLOADLEN;
			}
			FCache = FCache + diff;
		}
		ret.TotalESP = espc;
		ret.TotalTSP = tspc;
		ret.TotalEvt = evtc;
		ret.TotalState = stc;
		ret.TotalWave = wavc;
		delete ret.Esp, ret.Tsp, ret.State, ret.Evt, ret.Wav;
		ret.Esp = new Analysis::EnergySP[ret.TotalESP];
		ret.Tsp = new Analysis::EnergySP[ret.TotalTSP];
		ret.Wav = new IOLib::WaveFormRaw[ret.TotalWave];
		ret.Evt = new IOLib::EventRaw[ret.TotalEvt];
		ret.State = new StateCode[ret.TotalState];

		ESPcur = ESPinit;
		for (int i = 0;i < ret.TotalESP;i++)
		{
			ret.Esp[i] = ESPcur->esp;
			ESPinit = ESPcur;
			ESPcur = ESPcur->Next;
			delete ESPinit;
			ESPinit = nullptr;
		}

		TSPcur = TSPinit;
		for (int i = 0;i < ret.TotalTSP;i++)
		{
			ret.Tsp[i] = TSPcur->esp;
			TSPinit = TSPcur;
			TSPcur = TSPcur->Next;
			delete TSPinit;
			TSPinit = nullptr;
		}

		WAVcur = WAVinit;
		for (int i = 0;i < ret.TotalWave;i++)
		{
			ret.Wav[i] = WAVcur->wav;
			WAVinit = WAVcur;
			WAVcur = WAVcur->Next;
			delete WAVinit;
			WAVinit = nullptr;
		}

		EVTcur = EVTinit;
		for (int i = 0;i < ret.TotalEvt;i++)
		{
			ret.Evt[i] = EVTcur->evt;
			EVTinit = EVTcur;
			EVTcur = EVTcur->Next;
			delete EVTinit;
			EVTinit = nullptr;
		}

		STcur = STinit;
		for (int i = 0;i < ret.TotalState;i++)
		{
			ret.State[i] = STcur->st;
			STinit = STcur;
			STcur = STcur->Next;
			delete STinit;
			STinit = nullptr;
		}

		return ret;

	}


}