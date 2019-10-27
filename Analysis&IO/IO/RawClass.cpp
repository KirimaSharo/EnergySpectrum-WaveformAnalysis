#include "IOlibrary.h"
namespace IOLib
{
	WaveFormRaw::WaveFormRaw()
	{
		TotalSample = 0;
		SampleValue = nullptr;
	}
	WaveFormRaw::WaveFormRaw(const WaveFormRaw& c)
	{

		this->SampleValue = nullptr;
		this->TotalSample = c.TotalSample;
		if (this->TotalSample > 0)
		{
			this->SampleValue = new int[this->TotalSample];
			for (int i = 0;i < c.TotalSample;i++)
			{
				this->SampleValue[i] = c.SampleValue[i];
			}
		}
		this->UploadTime = c.UploadTime;
	}
	WaveFormRaw& WaveFormRaw::operator = (const WaveFormRaw& c)
	{
		delete this->SampleValue;
		this->SampleValue = nullptr;
		this->TotalSample = c.TotalSample;
		if (this->TotalSample > 0)
		{
			this->SampleValue = new int[this->TotalSample];
			for (int i = 0;i < c.TotalSample;i++)
			{
				this->SampleValue[i] = c.SampleValue[i];
			}
		}
		this->UploadTime = c.UploadTime;
		return *this;
	}
	WaveFormRaw::~WaveFormRaw()
	{
		delete this->SampleValue;
	}
}
