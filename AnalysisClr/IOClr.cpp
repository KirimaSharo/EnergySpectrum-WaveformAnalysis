#include "IOClr.h"

void IOClr::StateCode::TSget(IOLib::StateCode& source)
{
	STCode = source.STCode;
	for (int i = 0;i < 6;i++)
	{
		Sparam[i] = source.Sparam[i];
	}
	Time->Year = source.Time.tm_year + 1900;
	Time->Mon = source.Time.tm_mon + 1;
	Time->Day = source.Time.tm_mday;
	Time->Hour = source.Time.tm_hour;
	Time->Min = source.Time.tm_min;
	Time->Sec = source.Time.tm_sec;
}
void IOClr::StateCode::TSput(IOLib::StateCode& dest)
{
	dest.STCode = STCode;
	for (int i = 0;i < 6;i++)
	{
		dest.Sparam[i] = Sparam[i];
	}
	dest.Time.tm_year = Time->Year - 1900;
	dest.Time.tm_mon = Time->Mon - 1;
	dest.Time.tm_mday = Time->Day;
	dest.Time.tm_hour = Time->Hour;
	dest.Time.tm_min = Time->Min;
	dest.Time.tm_sec = Time->Sec;
}

void IOClr::StateNote::TSget(IOLib::StateNote& source)
{
	Statenote = gcnew System::String(source.Statenote.c_str());
	Paramsize = source.Paramsize;
	for (int i = 0;i < Paramsize;i++)
	{
		Paramnote[i] = gcnew System::String(source.Paramnote[i].c_str());
		Paramstart[i] = source.Paramstart[i];
		Paramend[i] = source.Paramend[i];
	}
}

IOClr::StateNote^ IOClr::StateCode::ParamNote()
{
	IOLib::StateCode mid;
	IOLib::StateNote retf;
	IOClr::StateNote^ ret = gcnew IOClr::StateNote();
	this->TSput(mid);
	IOLib::ParamNote(mid, retf);
	ret->TSget(retf);
	return ret;
}

IOClr::WaveFormRaw::WaveFormRaw()
{
	CORE = new IOLib::WaveFormRaw;
}
IOClr::WaveFormRaw::~WaveFormRaw()
{
	delete CORE;
}
AnalysisClr::WaveForm^ IOClr::WaveFormRaw::Transf()
{
	AnalysisClr::WaveForm^ ret = gcnew AnalysisClr::WaveForm();
	array<double>^ value = gcnew array<double>(TotalSample);
	for (int i = 0;i < TotalSample;i++)
	{
		value[i] = SampleValue[i];
	}
	ret->UploadTime = UploadTime;
	return ret;
}
void IOClr::WaveFormRaw::ChangeCORE(IOLib::WaveFormRaw* src)
{
	delete CORE;
	CORE = src;
}
IOLib::WaveFormRaw IOClr::WaveFormRaw::getCORE()
{
	return *CORE;
}

IOClr::EventRaw::EventRaw()
{
	CORE = new IOLib::EventRaw;
}
IOClr::EventRaw::~EventRaw()
{
	delete CORE;
}
AnalysisClr::Event^ IOClr::EventRaw::Transf()
{
	AnalysisClr::Event^ ret = gcnew AnalysisClr::Event();
	ret->Amp = Amp;
	ret->Area = Area;
	ret->ArrivialTime = ArrivialTime;
	ret->FallTime = FallTime;
	ret->RiseTime = RiseTime;
	ret->UploadTime = UploadTime;
	ret->source = source;
	return ret;
}
void IOClr::EventRaw::ChangeCORE(IOLib::EventRaw* src)
{
	delete CORE;
	CORE = src;
}
IOLib::EventRaw IOClr::EventRaw::getCORE()
{
	return *CORE;
}

bool IOClr::Structedfile::ReadFile(System::String^ filepath, System::String^ filename)
{
	std::string fp, fn;
	const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filepath);
	fp = tmp;
	const char* tmp1 = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename);
	fn = tmp1;
	return CORE->ReadFile(fp, fn);
}
bool IOClr::Structedfile::WriteFile(System::String^ filepath, System::String^ filename)
{
	std::string fp, fn;
	const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filepath);
	fp = tmp;
	const char* tmp1 = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename);
	fn = tmp1;
	return CORE->WriteFile(fp, fn);
}

IOClr::Structedfile::Structedfile()
{
	CORE = new IOLib::Structedfile;
}
IOClr::Structedfile::Structedfile(const Structedfile^ c)
{
	CORE = new IOLib::Structedfile(*c->CORE);
}
IOClr::Structedfile^ IOClr::Structedfile::operator = (const Structedfile^ c)
{
	delete CORE;
	CORE = new IOLib::Structedfile(*c->CORE);
	return this;
}
IOClr::Structedfile::~Structedfile()
{
	delete CORE;
}
void IOClr::Structedfile::ChangeCORE(IOLib::Structedfile* src)
{
	delete CORE;
	CORE = src;
}

bool IOClr::Rawdatafile::ReadFile(System::String^ filepath, System::String^ filename)
{
	std::string fp, fn;
	const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filepath);
	fp = tmp;
	const char* tmp1 = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename);
	fn = tmp1;
	return CORE->ReadFile(fp, fn);
}
bool IOClr::Rawdatafile::WriteFrame(System::IntPtr Framedata)
{
	return CORE->WriteFrame((unsigned char*)Framedata.ToPointer());
}
IOClr::Structedfile^ IOClr::Rawdatafile::AnalysisFrameCache()
{
	IOClr::Structedfile^ ret = gcnew IOClr::Structedfile();
	ret->ChangeCORE(& CORE->AnalysisFrameCache());
	return ret;
}
bool IOClr::Rawdatafile::WriteFile(System::String^ filepath, System::String^ filename)
{
	std::string fp, fn;
	const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filepath);
	fp = tmp;
	const char* tmp1 = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename);
	fn = tmp1;
	return CORE->WriteFile(fp, fn);
}
bool IOClr::Rawdatafile::AddOnFile(System::String^ filepath, System::String^ filename)
{
	std::string fp, fn;
	const char* tmp = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filepath);
	fp = tmp;
	const char* tmp1 = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename);
	fn = tmp1;
	return CORE->AddOnFile(fp, fn);
}