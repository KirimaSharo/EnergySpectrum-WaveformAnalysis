
#include<AnalysisLibrary.h>
#include<IOLibrary.h>
#include<iostream>
#include<fstream>

using namespace Analysis;
using namespace std;
using namespace IOLib;

void main()
{

	EnergySP e, e1;
	WaveForm w, w1;
	Event evt;

	int i = 0;

	time_t t = time(NULL);
	localtime_s(&e.UploadTime, &t);
	localtime_s(&w.UploadTime, &t);
	localtime_s(&evt.UploadTime, &t);


	e.AnalysisResultLength = 5;
	e.AnalysisResult = new ESPResult[e.AnalysisResultLength];
	for (i = 0;i < e.AnalysisResultLength;i++)
	{
		e.AnalysisResult[i].Peak = i;
		e.AnalysisResult[i].FWHM = 2 * i;
		e.AnalysisResult[i].Area = -i;
	}

	/*e.TotalChannel = 4096;
	e.Count = new int[e.TotalChannel];
	//e.SmoothResult = new double[e.TotalChannel];
	for (int i = 0;i < e.TotalChannel;i++)
	{
		e.Count[i] = i;
		//e.SmoothResult[i] = 10 * i;
	}
	*/

	string s = "a.dat";
	ifstream ei(s, std::ios::in | ios::binary);
	e.TotalChannel = 4096;
	e.Count = new int[e.TotalChannel];
	int sa;
	for (i = 0;i < e.TotalChannel;i++)
	{
		ei.read((char*)&sa, sizeof(int));
		e.Count[i] = sa;
	}
	e.Count[0] = 0;
	e.Count[1] = 0;
	/*
	e.Smooth(204, 10);
	AnalysisParam A;
	A.ESPCalibrationX = "x";
	A.ESPCalibrationY = "x";
	A.nmax = 100;
	A.ESPMaxResolution = 0.04;
	e.Analysis_ExpectMaxium(A);
	cout << e.AnalysisResultLength<<"\n";
	for (int k = 0;k < e.AnalysisResultLength;k++)
	{
		cout << e.AnalysisResult[k].Peak << "," << e.AnalysisResult[k].FWHM << ","<<e.AnalysisResult[k].Area<<"\n";
	}
	*/
	/*w.AnalysisResultLength = 5;
	w.AnalysisResult = new WaveResult[w.AnalysisResultLength];
	for (i = 0;i < w.AnalysisResultLength;i++)
	{
		w.AnalysisResult[i].Area = 2 * i;
		w.AnalysisResult[i].FallTime = -i;
		w.AnalysisResult[i].Max = 100 * i;
		w.AnalysisResult[i].RiseTime = i;
		w.AnalysisResult[i].TimeofMax = i;
		w.AnalysisResult[i].WaveWidth = 3 * i;
	}
	w.TotalSample = 1008;
	w.FFTResult = new double[w.TotalSample];
	w.SampleValue = new double[w.TotalSample];
	for (i = 0;i < w.TotalSample;i++)
	{
		w.FFTResult[i] = i * 3;
		w.SampleValue[i] = i + 10;
	}

	*/

	Structedfile sf, sf2;
	Rawdatafile rf;
	AnalysisParam A;
	/*
	A.ESPCalibrationX = "x";
	A.ESPCalibrationY = "x";
	A.nmax = 100;
	A.ESPMaxResolution = 0.04;


	sf.Run = 1234;

	sf.TotalESP = 2;
	sf.Esp = new EnergySP[2];
	sf.Esp[0] = e;
	sf.Esp[1] = sf.Esp[0];
	sf.Esp[1].Analysis_ExpectMaxium(A);

	sf.TotalState = 2;
	sf.State = new StateCode[2];
	sf.State[0].STCode = 0x0401;
	sf.State[0].Sparam[0] = 1;
	sf.State[0].Sparam[1] = 2;
	sf.State[0].Sparam[2] = 3;
	sf.State[0].Sparam[3] = 4;
	sf.State[0].Sparam[4] = 5;
	sf.State[0].Sparam[5] = 6;
	localtime_s(&sf.State[0].Time, &t);
	sf.State[1] = sf.State[0];
	sf.State[1].STCode = 0x0203;

	sf.TotalEvt = 2;
	sf.Evt = new EventRaw[2];
	sf.Evt[0].source = 0x50;
	sf.Evt[0].ArrivialTime = 1;
	localtime_s(&sf.Evt[0].UploadTime, &t);
	sf.Evt[1].source = 0x11;
	sf.Evt[1].Amp = 1;
	sf.Evt[1].RiseTime = 2;
	sf.Evt[1].ArrivialTime = 3;
	localtime_s(&sf.Evt[1].UploadTime, &t);

	sf.TotalWave = 1;
	sf.Wav = new WaveFormRaw[1];
	localtime_s(&sf.Wav[0].UploadTime, &t);
	sf.Wav[0].TotalSample = 12;
	sf.Wav[0].SampleValue = new int[sf.Wav[0].TotalSample];
	for (i = 0;i < sf.Wav[0].TotalSample;i++)
	{
		sf.Wav[0].SampleValue[i] = i;
	}

	sf.TotalTSP = 1;
	sf.Tsp = new EnergySP[1];
	sf.Tsp[0] = sf.Esp[0];

	sf.WriteFile("", "");
	*/
	//sf2.ReadFile("", "1234");
	//sf2.WriteFile("", "A1234");
	//w.TotalSample = 128;
	//w.SampleValue = new double[128];
	//for (i = 0;i < 128;i++)
	//{
	//	w.SampleValue[i] = i % 2;
	//	//w.SampleValue[i] = sin(i*3.14 + 2 * i*3.14 + i / 3 * 3.14);
	//}
	//SinWave sine;
	//sine.Amp = 0.5;
	//sine.Freq = 0.5;
	//sine.Phase = -0.25*3.1415926535898;
	//A.WaveCalibrationX = "x";
	//A.WaveCalibrationY = "x-0.5";
	//int ndnl;
	//double *dnl = w.DNL(&sine, 1, A, ndnl);
	//for (i = 0;i < ndnl;i++)
	//{
	//	cout << dnl[i] << "\n";
	//}
	//w.FFT(A);
	//for (i = 0;i < w.TotalSample;i++)
	//{
	//	cout << w.FFTResult[i] << "\n";
	//}
	//cout << w.SINAD(A);
	unsigned char Frame[] = { 0x55, 0xAA,
				0x14,0x01,0xFD,0x00,0x01,0x00,0x04,
				19,10,31,15,30,00,
				0xEB,0x90,0xA1,0x0A,0x01,
				0,
				0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0xC1,0xA1,
				0xEE
	};


	for (int j = 0; j < 32; j++)
	{
		Frame[20] = j;
		Frame[28] = j * 3;
		rf.WriteFrame(Frame);
	}
	rf.Run = 1000;
	sf = rf.AnalysisFrameCache();
	sf.WriteFile("", "");

	Frame[8] = 0x05;
	Frame[17] = 0xB0;
	for (int j = 0; j < 8; j++)
	{
		Frame[20] = j;
		Frame[28] = j * 3;
		rf.WriteFrame(Frame);
	}

	Frame[8] = 0x06;
	Frame[17] = 0xC7;
	for (int j = 0; j < 30; j++)
	{
		Frame[20] = j;
		Frame[28] = j * 3;
		rf.WriteFrame(Frame);
	}

	unsigned char FrameE[] = { 0x55, 0xAA,
				0x16,0x00,0xFD,0x00,0x01,0x00,0x07,
				19,10,31,15,30,00,
				0xEB,0x90,0x00,0xD0,0x12,0x00,
				10,3,5,0,
				0xC1,0xA1,
				0xEE
	};
	rf.WriteFrame(FrameE);
	unsigned char FrameST[] = { 0x55, 0xAA,
				0x14,0x00,0xFD,0x00,0x01,0x00,0x03,
				19,10,31,15,30,00,
				0x01,0x02,
				0x03,0x00,0x00,0x02,0x00,0x00,
				10,3,5,0,
				0xC1,0xA1,
				0xEE
	};
	rf.WriteFrame(FrameST);
	unsigned char FrameST2[] = { 0x55, 0xAA,
				0x14,0x00,0xFD,0x00,0x01,0x00,0x03,
				19,10,31,15,30,00,
				0x02,0x02,
				0x03,0x00,0x10,0x00,0x00,0x00,
				10,3,5,0,
				0xC1,0xA1,
				0xEE
	};
	rf.WriteFrame(FrameST2);
	sf = rf.AnalysisFrameCache();
	sf.WriteFile("", "AXS");
	
	cin >> i;
}