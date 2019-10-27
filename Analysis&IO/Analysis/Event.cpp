
#include "stdafx.h"

#include"AnalysisLibrary.h"

#include"math.h"

#define DLL_A _declspec(dllexport)

namespace Analysis
{
	Event::Event()
	{
		this->RiseTime = NULL;
		this->Amp = NULL;
	}

	Event::Event(const Event& c)
	{
		this->Amp = c.Amp;
		this->RiseTime = c.RiseTime;
		this->Area = c.Area;
		this->ArrivialTime = c.ArrivialTime;
		this->FallTime = c.FallTime;
		this->source = c.source;
		this->UploadTime = c.UploadTime;
	}

	Event& Event::operator = (const Event& c)
	{
		this->Amp = c.Amp;
		this->RiseTime = c.RiseTime;
		this->Area = c.Area;
		this->ArrivialTime = c.ArrivialTime;
		this->FallTime = c.FallTime;
		this->source = c.source;
		this->UploadTime = c.UploadTime;
		return *this;
	}

	Event::~Event()
	{

	}
}