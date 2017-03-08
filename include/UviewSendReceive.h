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
#ifndef _U_VIEW_SENDRECEIVE_H_
#define _U_VIEW_SENDRECEIVE_H_

#include "UviewCompatibility.h"

#define _WIN32_DCOM
#include <yat/utils/XString.h>

#import "imageac.tlb" named_guids no_namespace


namespace lima
{
namespace Uview
{
class LIBUVIEW_API UviewSendReceive
{
public:

    //- Constuctor
	UviewSendReceive();
	//- Destuctor
	virtual ~UviewSendReceive();

    int initConnection();

    //ACQUISITION
    long AcquireSingleImage(int imageId);

	bool AcquisitionInProgress();

    float GetCameraExpTime();

    void SetCameraExpTime(float expTime);
    
    short saveImage(VARIANT* filname, VARIANT* imageformat);
    
    long GetImageData(VARIANT* idata, long* width, long* height, short compression);

    int GetNewImageReady();

	short GetImageWidth();

	short GetImageHeight();
	
	bool GetAcquisitionInProgress();

    //SETS AND GET X/Y ROI MIN AND MAX
    short GetCameraROIxMin();

    void SetCameraROIxMin(short value);

    short GetCameraROIyMin();

    void SetCameraROIyMin(short value);
    
    short GetCameraROIxMax();

    void SetCameraROIxMax(short value);

    short GetCameraROIyMax();

    void SetCameraROIyMax(short value);

	/// -- FOR TEST --//

	void setCameraRoi(short originX, short originY, short limit_x, short limit_y);

	void setCameraBin(short sethBin, short setvBin);

    //SETS AND GET BINNING VERTICAL/HORIZONTAL
    short GetCameraHBin();

    void SetCameraHBin(short value);
    
    short GetCameraVBin();

    void SetCameraVBin(short value);

    //SETTINGS
    short GetCamStatus(long *Camtype, long *eletemp, long *ccdtemp);

    int GetCamSettings(long *mode, long *trig, long *roixmin, long *roixmax, long *roiymin, long *roiymax, long *hbin, long *vbin, long *expo);
    
    short GetCameraInfoLong(VARIANT* manufacturer, VARIANT* camtype, VARIANT* CCDsize,VARIANT* maxWith,VARIANT* maxHeight, 
                            VARIANT* maxHBin, VARIANT* maxVBin, VARIANT* maxGain, 
                            VARIANT* maxBrightness, VARIANT* maxContrast, VARIANT* eletemp, 
                            VARIANT* ccdtemp, VARIANT* maxExpTimeMSEC);

    short GetCamImageSize(long *width, long *height);

	float GetVersion();
	
	void SetSequential(bool mode);

    // AVERAGE IMAGES

    void SetAverageImages(long value);

    //REMOTE SIGNAL PLOT
    short IvsTInitRemoteMeasurement();

    short IvsTStartAcquiring(short start); 

    short IvsTSetDialogXAxis(BSTR Label, float min, float max, long nrChannels);

    float ROIdata(short ROIid);

    short IvsTDefineROI(short ROIid, unsigned long color, short x1, short 
                        y1, short x2, short y2, char interactive);

    short IvsTAddPoint(short ROIid, short Xindex, 
                        float Xvalue,float Yvalue);




private :
    
    std::string UviewErrorString(short response);

	IUViewInt *IUView;

    bool _error;
};
}
}
#endif //_U_VIEW_SENDRECEIVE_H_
