///###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2014
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//############################################################################

#include "UviewSendReceive.h"

using namespace lima;
using namespace lima::Uview;

//- CTOR
UviewSendReceive::UviewSendReceive()
: IUView(0)
{

}

//- DTOR
UviewSendReceive::~UviewSendReceive()
{	

}
//+------------------------------------------------------------------
/**
 *	METHODS TO SEND COMMANDS --> COM/DCOM
 */
//+------------------------------------------------------------------
//+------------------------------------------------------------------
/**
 *	method:	INIT COMMUNICATION
 */
//+------------------------------------------------------------------
int UviewSendReceive::initConnection()
{
	// Initial remark
    // In theory it would be logic to initialize instance through the method CoCreateInstance only once and keep the handle in a member of the class
	// When doing like this, the first call works 

	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 
	
	if(FAILED(hr))
		{
			return 1;
		}
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return 0;
}
//+------------------------------------------------------------------
/**
 *	method:GetCamSettings
 */
//+------------------------------------------------------------------
int UviewSendReceive::GetCamSettings(long *mode, long *trig, long *roixmin, long *roixmax, long *roiymin, long *roiymax, long *hbin, long *vbin, long *expo)
{
    int response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 
	//SENDING COMMAND
	response = UView->GetCamSettings(mode, trig, roixmin, roixmax, roiymin, roiymax, hbin, vbin, expo);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;
}
//+------------------------------------------------------------------
/**
 *	method:AcquireSingleImage
 */
//+------------------------------------------------------------------
long UviewSendReceive::AcquireSingleImage(int imageId)
{
	long response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
	response = UView->AcquireSingleImage(imageId);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:AcquisitionInProgress
 */
//+------------------------------------------------------------------
bool UviewSendReceive::AcquisitionInProgress()
{
    bool response = false;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
	response = UView->AcquisitionInProgress;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;
}
//+------------------------------------------------------------------
/**
 *	method:GetCameraExpTime
 */
//+------------------------------------------------------------------
float UviewSendReceive::GetCameraExpTime()
{
    float response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
	response = UView->CameraExpTime;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;
}
//+------------------------------------------------------------------
/**
 *	method:SetCameraExpTime
 */
//+------------------------------------------------------------------
void UviewSendReceive::SetCameraExpTime(float expTime)
{
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
	UView->CameraExpTime = expTime;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:saveImage
 */
//+------------------------------------------------------------------
short UviewSendReceive::saveImage(VARIANT* filname, VARIANT* imageformat)
{
    short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
	response = UView->GetFilename(filname, imageformat);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;
}
//+------------------------------------------------------------------
/**
 *	method:GetCamStatus
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetCamStatus(long *Camtype, long *eletemp, long *ccdtemp)
{
    short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetCamStatus( Camtype, eletemp, ccdtemp);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:GetImageData
 */
//+------------------------------------------------------------------
long UviewSendReceive::GetImageData(VARIANT* idata, long* width, long* height, short compression)
{
    long response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetImageData( idata, width, height, compression);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:GetCameraROIxMin
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetCameraROIxMin()
{
    short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetCameraROIxMin();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:SetCameraROIxMin
 */
//+------------------------------------------------------------------
void UviewSendReceive::SetCameraROIxMin(short value)
{
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->CameraROIxMin = value;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:GetCameraROIyMin
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetCameraROIyMin()
{
    short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetCameraROIyMin();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:SetCameraROIyMin
 */
//+------------------------------------------------------------------
void UviewSendReceive::SetCameraROIyMin(short value)
{
 	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->CameraROIyMin = value;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:GetCameraROIxMax
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetCameraROIxMax()
{
    short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetCameraROIxMax();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:SetCameraROIxMax
 */
//+------------------------------------------------------------------
void UviewSendReceive::SetCameraROIxMax(short value)
{
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->CameraROIxMax = value;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:GetCameraROIyMax
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetCameraROIyMax()
{
    short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetCameraROIyMax();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:SetCameraROIyMax
 */
//+------------------------------------------------------------------
void UviewSendReceive::SetCameraROIyMax(short value)
{
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->CameraROIyMax = value;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:setCameraRoi
 */
//+------------------------------------------------------------------
void UviewSendReceive::setCameraRoi(short originX, short originY, short limit_x, short limit_y)
{
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->CameraROIxMin = originX;
	UView->CameraROIyMin = originY;
    UView->CameraROIyMax = limit_y;
	UView->CameraROIxMax = limit_x;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:GetCameraHBin
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetCameraHBin()
{
    short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetCameraHBin();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:SetCameraHBin
 */
//+------------------------------------------------------------------
void UviewSendReceive::SetCameraHBin(short value)
{
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->CameraHBin = value;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:GetCameraVBin
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetCameraVBin()
{
    short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetCameraVBin();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:SetCameraVBin
 */
//+------------------------------------------------------------------
void UviewSendReceive::SetCameraVBin(short value)
{
    //INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->CameraVBin = value;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:SetCameraVBin
 */
//+------------------------------------------------------------------
void UviewSendReceive::setCameraBin(short sethBin, short setvBin)
{
	 //INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->CameraVBin = setvBin;
	UView->CameraHBin = sethBin;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:SetCameraVBin
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetCamImageSize(long *width, long *height)
{
    short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetCamImageSize(width, height);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:GetCameraInfoLong
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetCameraInfoLong(VARIANT* manufacturer, VARIANT* camtype, VARIANT* CCDsize,VARIANT* maxWith,VARIANT* maxHeight, 
                                          VARIANT* maxHBin, VARIANT* maxVBin, VARIANT* maxGain, 
                                          VARIANT* maxBrightness, VARIANT* maxContrast, VARIANT* eletemp, 
                                          VARIANT* ccdtemp, VARIANT* maxExpTimeMSEC)
{
short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetCameraInfoLong( manufacturer,  camtype,  CCDsize, maxWith, maxHeight, 
                                         maxHBin,  maxVBin,  maxGain, 
                                         maxBrightness,  maxContrast,  eletemp, 
                                         ccdtemp,  maxExpTimeMSEC);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:GetNewImageReady
 */
//+------------------------------------------------------------------
int UviewSendReceive::GetNewImageReady()
{
int response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetNewImageReady();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

} 
//+------------------------------------------------------------------
/**
 *	method:GetImageWidth
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetImageWidth()
{
short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetImageWidth();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

} 
//+------------------------------------------------------------------
/**
 *	method:GetImageWidth
 */
//+------------------------------------------------------------------
short UviewSendReceive::GetImageHeight()
{
short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetImageHeight();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

} 
//+------------------------------------------------------------------
/**
 *	method:IvsTInitRemoteMeasurement
 */
//+------------------------------------------------------------------
short UviewSendReceive::IvsTInitRemoteMeasurement()
{
short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->IvsTInitRemoteMeasurement();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

} 
//+------------------------------------------------------------------
/**
 *	method:IvsTStartAcquiring
 */
//+------------------------------------------------------------------
short UviewSendReceive::IvsTStartAcquiring(short start)
{
short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->IvsTStartAcquiring(start);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

} 
//+------------------------------------------------------------------
/**
 *	method:IvsTSetDialogXAxis
 */
//+------------------------------------------------------------------
short UviewSendReceive::IvsTSetDialogXAxis(BSTR Label, float min, float max, long nrChannels)
{
short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->IvsTSetDialogXAxis(Label, min, max, nrChannels);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

} 
//+------------------------------------------------------------------
/**
 *	method:ROIdata
 */
//+------------------------------------------------------------------
float UviewSendReceive::ROIdata(short ROIid)
{
float response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->ROIdata(ROIid);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

} 
//+------------------------------------------------------------------
/**
 *	method:IvsTDefineROI
 */
//+------------------------------------------------------------------
short UviewSendReceive::IvsTDefineROI(short ROIid, unsigned long color, short x1, short 
                        y1, short x2, short y2, char interactive)
{
short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->IvsTDefineROI(ROIid, color, x1, y1, x2, y2, interactive);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

} 
//+------------------------------------------------------------------
/**
 *	method:IvsTAddPoint
 */
//+------------------------------------------------------------------
short UviewSendReceive:: IvsTAddPoint(short ROIid, short Xindex, 
                        float Xvalue,float Yvalue)
{
short response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->IvsTAddPoint(ROIid, Xindex, Xvalue, Yvalue);
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:AverageImages
 */
//+------------------------------------------------------------------
void UviewSendReceive::SetAverageImages(long value)
{
    //INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->AverageImages = value;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}
//+------------------------------------------------------------------
/**
 *	method:GetVersion
 */
//+------------------------------------------------------------------
float UviewSendReceive::GetVersion()
{
	/*float version;
    //INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    version = UView->GetVersion();
	return version;
	//UNINITIALIZE INSTANCE
	CoUninitialize();*/
	return 1;
}
//+------------------------------------------------------------------
/**
 *	method:GetAcquisitionInProgress
 */
//+------------------------------------------------------------------
bool UviewSendReceive:: GetAcquisitionInProgress()
{
bool response = 0;
	
	//INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    response = UView->GetAcquisitionInProgress();
	//UNINITIALIZE INSTANCE
	CoUninitialize();
	return response;

}
//+------------------------------------------------------------------
/**
 *	method:AverageImages
 */
//+------------------------------------------------------------------
void UviewSendReceive::SetSequential(bool mode)
{
    //INITIALISE COM 
	CoInitialize(NULL);
	// CREATE COM POINTER
	IUViewInt *UView;
	//INITIALIZE INSTANCE
	HRESULT hr=CoCreateInstance (CLSID_UViewInt, NULL, CLSCTX_LOCAL_SERVER,DIID_IUViewInt , (void**)&UView);		 		 
	//SENDING COMMAND
    UView->Sequential = mode;
	//UNINITIALIZE INSTANCE
	CoUninitialize();
}

