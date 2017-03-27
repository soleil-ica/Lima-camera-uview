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

#include <cstdlib>
#include <winsock2.h>
#include <unistd.h>

#include "UviewCamera.h"

const int compression = 0;
using namespace lima;
using namespace lima::Uview;

const short ROIid1 = 1;
const short ROIid2 = 2;
const short ROIid3 = 3;
const short ROIid4 = 4;

const int collectInCurrentWindow = 1;
const bool simultaneouslyMode = true;
//---------------------------------------------------------------------------------------
//! Camera::CameraThread::CameraThread()
//---------------------------------------------------------------------------------------
Camera::CameraThread::CameraThread(Camera& cam)
		: m_cam(&cam)
{
	DEB_MEMBER_FUNCT();	
	DEB_TRACE()<<"CameraThread::CameraThread - BEGIN";	
	m_cam->m_acq_frame_nb = 0;
	m_force_stop = false;
	DEB_TRACE()<<"CameraThread::CameraThread - END";	
}

//---------------------------------------------------------------------------------------
//! Camera::CameraThread::start()
//---------------------------------------------------------------------------------------
void Camera::CameraThread::start()
{
	DEB_MEMBER_FUNCT();	
	DEB_TRACE()<<"CameraThread::start - BEGIN";	

	CmdThread::start();
	
	waitStatus(Ready);
	DEB_TRACE()<<"CameraThread::start - END";		
}

//---------------------------------------------------------------------------------------
//! Camera::CameraThread::init()
//---------------------------------------------------------------------------------------
void Camera::CameraThread::init()
{
	DEB_MEMBER_FUNCT();	
	DEB_TRACE()<<"CameraThread::init - BEGIN";		
	setStatus(Ready);
	DEB_TRACE()<<"CameraThread::init - END";		
}
//---------------------------------------------------------------------------------------
//! Camera::prepareAcq()
//---------------------------------------------------------------------------------------
void Camera::prepareAcq()
{DEB_MEMBER_FUNCT();	
	DEB_TRACE()<<"CameraThread::prepareAcq";
}

//---------------------------------------------------------------------------------------
//! Camera::CameraThread::execCmd()
//---------------------------------------------------------------------------------------
void Camera::CameraThread::execCmd(int cmd)
{
	DEB_MEMBER_FUNCT();	
	DEB_TRACE()<<"CameraThread::execCmd - BEGIN";		
	int status = getStatus();
	switch (cmd)
	{
		case StartAcq:
			if (status != Ready)
				throw LIMA_HW_EXC(InvalidValue, "Not Ready to StartAcq");
			execStartAcq();
		break;
	}
	DEB_TRACE()<<"CameraThread::execCmd - END";		
}

//---------------------------------------------------------------------------------------
//! Camera::CameraThread::execStartAcq()
//---------------------------------------------------------------------------------------
void Camera::CameraThread::execStartAcq()
{
	DEB_MEMBER_FUNCT();
	DEB_TRACE()<<"CameraThread::execStartAcq - BEGIN";
	
	int status;
	int acq_frame_nb;
    int nb_frames = m_cam->m_nb_frames;
	int& frame_nb = m_cam->m_acq_frame_nb;
	m_cam->m_Acq_running = true;
	m_cam->m_acq_frame_nb = 0;
	acq_frame_nb = 0;

    long width=0, height=0;

	setStatus(Exposure);
    //--Lima's mandatory stuff--//
	StdBufferCbMgr& buffer_mgr = m_cam->m_buffer_ctrl_obj.getBuffer();
	buffer_mgr.setStartTimestamp(Timestamp::now());

	while(m_cam->m_Acq_running && (!m_cam->m_nb_frames || m_cam->m_acq_frame_nb < m_cam->m_nb_frames))
	{		
        //--Variant pointer-initialisation--//
        VARIANT idata;
        VariantInit(&idata);
        VariantClear(&idata);
        //--Ask for new Image--//
        m_cam->m_uview_com->AcquireSingleImage(collectInCurrentWindow);
        //--If new Image is ready, get data--//
		do
		{
			//do nothing while Acquisition in progress
		}
		while (m_cam->m_uview_com->GetAcquisitionInProgress());
        //is_image_ready = 0;
        long width=0, height=0;
        VariantClear(&idata);
        //--Ask for image data--//
        long what = m_cam->m_uview_com->GetImageData(&idata, &width, &height, compression);
        //--Ask for IvST values--//
        m_cam->getIvsROIValues();
        int arraySize = width * height * 2;
        //--Access to data with safe array--//
        BSTR HUGEP *pbstr;
        SafeArrayAccessData(idata.parray, (void HUGEP**)&pbstr);
        //--Prepare the Frame ptr--//
        void *ptr = buffer_mgr.getFrameBufferPtr(frame_nb);
        //--copy data into the Frame ptr--//
        memcpy((unsigned short *)ptr, (unsigned short *)pbstr, arraySize); 
        //--Unlock the variant data--//
        SafeArrayUnaccessData(idata.parray);
        //--destroy safe aray--//
        SafeArrayDestroy(idata.parray);
        //--Set Time Stamp--//
        buffer_mgr.setStartTimestamp(Timestamp::now());
        //-- NEW FRAME READY--//	
        HwFrameInfoType frame_info;
        frame_info.acq_frame_nb = acq_frame_nb;
        buffer_mgr.newFrameReady(frame_info);
        //-- NEXT STEP --//
        acq_frame_nb++;
        nb_frames--;
        m_cam->m_acq_frame_nb = acq_frame_nb;	
      
        if (m_force_stop)
			        goto ForceTheStop;
        	
        ForceTheStop:
        if (m_force_stop)
        {
            DEB_TRACE()<<"Current acquisition aborded";
            m_cam->m_Acq_running = false;
            m_force_stop = false;
            setStatus(Ready);
            break;
        }
		
	} //--End while--//

	if (m_cam->m_frame)
	{
		delete[] m_cam->m_frame;
		m_cam->m_frame = 0;
	}
	m_cam->m_Acq_running = false;
	setStatus(Ready);
}

//---------------------------------------------------------------------------------------
//! Camera::CameraThread::getNbAcquiredFrames()
//---------------------------------------------------------------------------------------
int Camera::CameraThread::getNbAcquiredFrames()
{
	return m_cam->m_acq_frame_nb;
}
	
//---------------------------------------------------------------------------------------
//! Camera::Camera()
//---------------------------------------------------------------------------------------
Camera::Camera():
	m_thread(*this)
	
{
    //--Initialize values--//
	long *mode=0;
  long *trig=0;
  long *roixmin=0;
  long *roixmax=0;
  long *roiymin=0;
  long *roiymax=0;
  long *hbin=0;
  long *vbin=0;
  long *expo=0;
	this->m_Acq_running = false;
	this-> m_ivs_roi_data_1 = 0.0;
	this-> m_ivs_roi_data_2 = 0.0;
	this-> m_ivs_roi_data_3 = 0.0;
	this-> m_ivs_roi_data_4 = 0.0;

  //--POINTER TO SEND/RECEIVE--//
	this->m_uview_com = 0;
	this->m_uview_com = new UviewSendReceive();

	DEB_CONSTRUCTOR();
    //--VARIANT VARIABLES INITIALIZATION--//
    VARIANT *manufacturer = new VARIANT;
    VariantInit(manufacturer);
    VARIANT *camtype = new VARIANT;
    VariantInit(camtype);
    VARIANT *CCDsize = new VARIANT;
    VariantInit(CCDsize);
    VARIANT *maxHBin = new VARIANT;
    VariantInit(maxHBin);
    VARIANT *maxVBin = new VARIANT;
    VariantInit(maxVBin);
    VARIANT *maxGain = new VARIANT;
    VariantInit(maxGain);
    VARIANT *maxBrightness = new VARIANT;
    VariantInit(maxBrightness);
    VARIANT *maxContrast = new VARIANT;
    VariantInit(maxContrast);
    VARIANT *eletemp = new VARIANT;
    VariantInit(eletemp);
    VARIANT *ccdtemp = new VARIANT;
    VariantInit(ccdtemp);
    VARIANT *maxExpTimeMSEC = new VARIANT;
    VariantInit(maxExpTimeMSEC);
    VARIANT *maxWith = new VARIANT;
    VariantInit(maxWith);
    VARIANT *maxHeight = new VARIANT;
    VariantInit(maxHeight);
	m_depth = 16;	
    
    /* Open comunication with the camera */
    DEB_TRACE()<<"Open comunication with the camera...";
	if (this->m_uview_com->initConnection() == 1)
	{
        std::string Err = "Unable to open comunication with the camera.";
		THROW_HW_ERROR(Error) << Err;
	}
    else
    {
        //-- GET INFORMATIONS--//
        short response = this->m_uview_com->GetCameraInfoLong(manufacturer, camtype, CCDsize, maxWith, maxHeight, 
                                                            maxHBin, maxVBin, maxGain, 
                                                            maxBrightness, maxContrast, eletemp, 
                                                            ccdtemp, maxExpTimeMSEC);

	    //--Set image size--//
    	this->refreshImageSize();

	    //-- Set max image size --//
        VARIANT mW = *maxWith;
        VariantChangeType (&mW, &mW, 0, VT_INT);
        m_max_width = V_INT(&mW);
	    VARIANT mH = *maxHeight;
        VariantChangeType (&mH, &mH, 0, VT_INT);
        m_max_height = V_INT(&mH);

	    if(response != 0)
        {
            std::string Err = UviewErrorString(response);
		    THROW_HW_ERROR(Error) << Err;
	    }
        /*default EXPOSURE unit is the microsec */
	    this->m_uview_com->SetCameraExpTime(100);
	    //  readout and  exposure  are done simultaneously
	    this->m_uview_com->SetSequential(simultaneouslyMode);
        //--Ok to start thread camera--//
	    m_thread.start();
    }
}

//---------------------------------------------------------------------------------------
//! Camera::~Camera()
//---------------------------------------------------------------------------------------
Camera::~Camera()
{DEB_DESTRUCTOR();
	
    stopAcq();
}
//---------------------------------------------------------------------------------------
//! Camera::reset()
//---------------------------------------------------------------------------------------
void Camera::reset()
{
	DEB_MEMBER_FUNCT();
	//--Nothing to do--//
	DEB_TRACE()<<"CameraThread::reset";
}

//---------------------------------------------------------------------------------------
//! Camera::getStatus()
//---------------------------------------------------------------------------------------
Camera::Status Camera::getStatus()
{DEB_MEMBER_FUNCT();

	int thread_status = m_thread.getStatus();

	DEB_RETURN() << DEB_VAR1(thread_status);

	switch (thread_status)
	{
		case CameraThread::Ready:
			return Camera::Ready;
		case CameraThread::Exposure:
			return Camera::Exposure;
		case CameraThread::Readout:
			return Camera::Readout;
		case CameraThread::Latency:
			return Camera::Latency;
		default:
			throw LIMA_HW_EXC(Error, "Invalid thread status");
	}
}

//---------------------------------------------------------------------------------------
//! Camera::setNbFrames()
//---------------------------------------------------------------------------------------
void Camera::setNbFrames(int nb_frames)
{DEB_MEMBER_FUNCT();
	
	if (nb_frames < 0)
		throw LIMA_HW_EXC(InvalidValue, "Invalid nb of frames");

	m_nb_frames = nb_frames;
}

//---------------------------------------------------------------------------------------
//! Camera::getNbFrames()
//---------------------------------------------------------------------------------------
void Camera::getNbFrames(int& nb_frames)
{DEB_MEMBER_FUNCT();

	nb_frames = m_nb_frames;
}

//---------------------------------------------------------------------------------------
//! Camera::CameraThread::getNbAcquiredFrames()
//---------------------------------------------------------------------------------------
int Camera::getNbAcquiredFrames()
{DEB_MEMBER_FUNCT();

	return m_acq_frame_nb;
}

//---------------------------------------------------------------------------------------
//! Camera::startAcq()
//---------------------------------------------------------------------------------------
void Camera::startAcq()
{DEB_MEMBER_FUNCT();

	m_thread.m_force_stop = false;
	m_acq_frame_nb = 0;

	m_thread.sendCmd(CameraThread::StartAcq);
	m_thread.waitNotStatus(CameraThread::Ready);
}

//---------------------------------------------------------------------------------------
//! Camera::stopAcq()
//---------------------------------------------------------------------------------------
void Camera::stopAcq()
{DEB_MEMBER_FUNCT();

    this->m_Acq_running = false;
	m_thread.m_force_stop = true;
}

//---------------------------------------------------------------------------------------
//! Camera::getExpTime()
//---------------------------------------------------------------------------------------
void Camera::getExpTime(double& exp_time)
{
	DEB_MEMBER_FUNCT();
	
	exp_time = this->m_uview_com->GetCameraExpTime();
	DEB_RETURN() << DEB_VAR1(exp_time);
}

//---------------------------------------------------------------------------------------
//! Camera::setExpTime()
//---------------------------------------------------------------------------------------
void Camera::setExpTime(double exp_time)
{
	DEB_MEMBER_FUNCT();
	//We set the expeosure time if it has changed..
	if ( m_exposure != exp_time)
	{
		float expTimeMs = exp_time*1000;	
		this->m_uview_com->SetCameraExpTime(expTimeMs);
		m_exposure = exp_time;	
	}
}
//---------------------------------------------------------------------------------------
//! Camera::getMaxWidth()
//---------------------------------------------------------------------------------------
int Camera::getMaxWidth()
{
	DEB_MEMBER_FUNCT();
	
	return m_max_width;
}	
//---------------------------------------------------------------------------------------
//! Camera::getMaxHeight()
//---------------------------------------------------------------------------------------
int Camera::getMaxHeight()
{
	DEB_MEMBER_FUNCT();

	return m_max_height;
}
//---------------------------------------------------------------------------------------
//! Camera::getImageWidth()
//---------------------------------------------------------------------------------------
int Camera::getImageWidth()
{
	DEB_MEMBER_FUNCT();

	return m_width;
}
//---------------------------------------------------------------------------------------
//! Camera::getImageHeight()
//---------------------------------------------------------------------------------------
int Camera::getImageHeight()
{
	DEB_MEMBER_FUNCT();
	
	return m_height;
}	
//---------------------------------------------------------------------------------------
//! Camera::setImageType()
//---------------------------------------------------------------------------------------
void Camera::setImageType(ImageType type)
{
	DEB_MEMBER_FUNCT();

	DEB_TRACE()<<"Camera::setImageType";
	switch(type)
	{
	case Bpp16 : m_depth = 16;
		break;
	default:
		THROW_HW_ERROR(Error) << "This pixel format of the camera is not managed, only 16 bits cameras are already managed!";
		break;
	}
	return;
}
//---------------------------------------------------------------------------------------
//! Camera::getImageType()
//---------------------------------------------------------------------------------------
void Camera::getImageType(ImageType& type)
{

	DEB_MEMBER_FUNCT();	
	switch(m_depth)
	{
	case 16 : type = Bpp16;
		break;
	default:
		THROW_HW_ERROR(Error) << "This pixel format of the camera is not managed, only 16 bits cameras are already managed!";
		break;
	}
	return;
}
//---------------------------------------------------------------------------------------
//! Camera::getDetectorModel(std::string& model)
//---------------------------------------------------------------------------------------
void Camera::getDetectorModel(std::string& model)
{
DEB_MEMBER_FUNCT();

model = "PCO" ;

}
//---------------------------------------------------------------------------------------
//! Camera::setBin()
//---------------------------------------------------------------------------------------
void Camera::setBin(const Bin& bin)
{DEB_MEMBER_FUNCT();
	
	unsigned short tmp_sbin = bin.getX();
	unsigned short tmp_pbin = bin.getY();
    //--Binning H and V must be the same--//
    if (tmp_sbin == tmp_pbin)
    {
        //--Only 1x1, 2x2, 4x4, 8x8--//
        if (tmp_sbin == 1 || tmp_sbin == 2 || tmp_sbin == 4 || tmp_sbin ==8) 
        {
			//We set the bin if it has changed..
			if ( m_sbin != tmp_sbin)
			{
				m_sbin = tmp_sbin;
				m_pbin = tmp_pbin;
				this->setBinning(m_sbin, m_pbin);
			}
        }
        else
        {
            DEB_ERROR() << "Binning must be set at 1, 2, 4 or 8";
            THROW_HW_ERROR(Error) << "Cannot set detector BIN, Binning must be set at 1x1, 2x2, 4x4, 8x8";                                        
        }
    }
    else
    {
        DEB_ERROR() << "H Binning must be the same as V Binning";
        THROW_HW_ERROR(Error) << "Cannot set detector BIN, H Binning must be the same as V Binning";                                        
    }
}

//---------------------------------------------------------------------------------------
//! Camera::getBin()
//---------------------------------------------------------------------------------------
void Camera::getBin(Bin& bin)
{DEB_MEMBER_FUNCT();
	Bin tmp_bin(m_sbin, m_pbin);
	bin = tmp_bin;
	DEB_RETURN() << DEB_VAR1(bin);	
}

//---------------------------------------------------------------------------------------
//! Camera::checkBin()
//---------------------------------------------------------------------------------------
void Camera::checkBin(Bin& aBin)
{DEB_MEMBER_FUNCT();
	/*Bin tmp_bin(m_roi_sbin, m_roi_pbin);
	aBin = tmp_bin;*/

	/*DEB_TRACE()<<"Camera::checkBin";	
	DEB_PARAM() << DEB_VAR1(bin);*/
}

//-----------------------------------------------------
//! Camera::checkRoi()
//-----------------------------------------------------
void Camera::checkRoi(const Roi& set_roi, Roi& hw_roi)
{DEB_MEMBER_FUNCT();	
    DEB_PARAM() << DEB_VAR1(set_roi);

	hw_roi = set_roi;
	DEB_RETURN() << DEB_VAR1(hw_roi);
}

//---------------------------------------------------------------------------------------
//! Camera::getRoi()
//---------------------------------------------------------------------------------------
void Camera::getRoi(Roi& hw_roi)
{DEB_MEMBER_FUNCT();
	//--no way to read the roi, Uview does not provide any function to do that!--//
    hw_roi = m_roi;
	DEB_RETURN() << DEB_VAR1(hw_roi);
}

//---------------------------------------------------------------------------------------
//! Camera::setRoi()
//---------------------------------------------------------------------------------------
void Camera::setRoi(const Roi& set_roi)
{DEB_MEMBER_FUNCT();

    //--Get ROI values : x, y, width, height--//
	Point minXmaxY = set_roi.getBottomLeft();
	Point maxXminY = set_roi.getTopRight();
    //--Extract ROI values--//
	short minX = minXmaxY.x;
	short minY = maxXminY.y;
    //--Lima starts from 0, API doesn't...--//
    short width = maxXminY.x + 1;
    short height = minXmaxY.y + 1;
    //--check if width and height are divisible by 32--// 
    //--otherwise, api is gonna recalculate ROI, image size won't be good--// 
    short test_modulo_32 = ((height - minY) % 32) + ((width - minX) % 32);
    //--test_modulo_32 must be 0, means that height and width are divisable by 32--//
    if(test_modulo_32 == 0)
    {
	    //--Ready to send ROI values if ROI changed---//
		if (m_roi != set_roi)
		{
			m_roi = set_roi;
			this->setCameraROI(minX, minY, width, height);
			//--refresh Image Size--//
			this->refreshImageSize();
		}
    }
    else 
    {
        DEB_ERROR() << "Cannot set detector ROI, ROI Width and Height must be divisible by 32";
        THROW_HW_ERROR(Error) << "Cannot set detector ROI, ROI Width and Height must be divisible by 32";                                        
    }
}

//---------------------------------------------------------------------------------------
//! Camera::getIvsROIValues()
//
//--Used to get IvsT ROI values from API--//
//--Once data is refresh, flag "data enable" is set to true--//
//---------------------------------------------------------------------------------------
void Camera::getIvsROIValues()
{
	DEB_MEMBER_FUNCT();
	//lima::MutexLock gard(this->_IvSRoIAccess);
	m_ivs_roi_data_1 = this->getROIdata(ROIid1);
	m_ivs_roi_data_2 = this->getROIdata(ROIid2);
	m_ivs_roi_data_3 = this->getROIdata(ROIid3);
	m_ivs_roi_data_4 = this->getROIdata(ROIid4);
	//Data ready to be imported by uviewCCD
	if (!this->m_ivst_roi_ready)
		this->m_ivst_roi_ready = true;
}

//---------------------------------------------------------------------------------------
//! Camera::checkIvsROIValues()
//
//--Used by uviewCCD device to get IvsT ROI values--//
//--Once data is returned, flag "data enable" is set to false--//
//---------------------------------------------------------------------------------------
float Camera::checkIvsROIValues(short ROIid)
{
	//yat::MutexLock gard(this->_IvSRoIAccess);
	switch(ROIid)
	{
		case 1 :
			return this->m_ivs_roi_data_1;
		case 2 :
			return this->m_ivs_roi_data_2;
		case 3 :
			return this->m_ivs_roi_data_3;
		case 4 :
			return this->m_ivs_roi_data_4;
		default :;
	}	
}
//---------------------------------------------------------------------------------------
//! Camera::checkIvsROIValues()
//
//--Used by uviewCCD device to know when to get IvsT ROI values--//
//--To return flag "data enable"--//
//---------------------------------------------------------------------------------------
bool Camera::IsIvSRoiDataReady()
{
	return this->m_ivst_roi_ready;
}
//---------------------------------------------------------------------------------------
//! Camera::checkIvsROIValues()
//
//--Used by uviewCCD device to know when to get IvsT ROI values--//
//--To return flag "data enable"--//
//---------------------------------------------------------------------------------------
void Camera::IvSRoiDataImported()
{
	this->m_ivst_roi_ready = false;
}
//---------------------------------------------------------------------------------------
//! Camera::refreshImageSize()
//---------------------------------------------------------------------------------------
void Camera::refreshImageSize()
{
    m_width = (int)this->m_uview_com->GetImageWidth();
    m_height = (int)this->m_uview_com->GetImageHeight();
}
//---------------------------------------------------------------------------------------
//! Camera::getROIdata(ROIid)
//---------------------------------------------------------------------------------------
float Camera::getROIdata(short ROIid)
{DEB_MEMBER_FUNCT();
    float response = this->m_uview_com->ROIdata(ROIid);
    return response;
}
//---------------------------------------------------------------------------------------
//! Camera::setCameraROI(short originX, short originY, short roiWidth, short roiHeight)
//---------------------------------------------------------------------------------------
void Camera::setCameraROI(short originX, short originY, short limit_x, short limit_y)
{DEB_MEMBER_FUNCT();

	this->m_uview_com->setCameraRoi(originX, originY, limit_x, limit_y);
}
//---------------------------------------------------------------------------------------
//! Camera::setBinning(short hBin, short vBin)
//---------------------------------------------------------------------------------------
void Camera::setBinning(short sethBin, short setvBin)
{DEB_MEMBER_FUNCT();

	this->m_uview_com->setCameraBin(sethBin, setvBin);
}
//---------------------------------------------------------------------------------------
//! Camera::getCameraROI(short originX, short originY, short roiWidth, short roiHeight)
//---------------------------------------------------------------------------------------
void Camera::getCameraROI(short xMin, short yMin, short xMax, short yMax)
{DEB_MEMBER_FUNCT();

        xMin = this->m_uview_com->GetCameraROIxMin();
        yMin = this->m_uview_com->GetCameraROIyMin();
        xMax = this->m_uview_com->GetCameraROIxMax();
        yMax = this->m_uview_com->GetCameraROIyMax();
}
//---------------------------------------------------------------------------------------
//! Camera::getBinning(short hBin, short vBin)
//---------------------------------------------------------------------------------------
void Camera::getBinning(short hBin, short vBin)
{DEB_MEMBER_FUNCT();

        hBin = this->m_uview_com->GetCameraVBin();
        vBin = this->m_uview_com->GetCameraVBin();        
}
//---------------------------------------------------------------------------------------
//! Camera::setAverage(long mode, long value)
//---------------------------------------------------------------------------------------
void Camera::setAverage(long value)
{DEB_MEMBER_FUNCT();
    this->m_uview_com->SetAverageImages(value);
}
//---------------------------------------------------------------------------------------
//! Camera::UviewErrorString(short response)
//---------------------------------------------------------------------------------------
std::string Camera::UviewErrorString(short response)
{
    std::string error ="";
    
	switch(response)
		{
		case 0 : error = "No error from board";
			break;
		case 1 : error = "initialization failed; no camera connected";
			break;
		case 2 : error = "timeout in any function";
			break;
		case 3 : error = "function call with wrong parameter";
			break;
		case 4 : error = "cannot locate PCI card or card driver";
			break;
		case 5 : error = "cannot allocate DMA buffer";
			break;
		case 6 : error = "reserved";
			break;
		case 7 : error = "DMA timeout";
			break;
        case 8 : error = "invalid camera mode";
			break;
		case 9 : error = "no driver installed";
			break;
		case 10 : error = "no PCI bios found";
			break;
		case 11 : error = "device is hold by another process";
			break;
		case 12: error = "error in reading or writing data to board";
			break;
		case 13: error = "wrong driver function";
			break;
		case 14 : error = "reserved";
			break;
		case 15 : error = "reserved";
			break;
        case 16 : error = "reserved";
			break;
		case 17 : error = "reserved";
			break;
		case 18 : error = "reserved";
			break;
		case 19 : error = "reserved";
			break;
		case 20 : error = "LOAD_COC error (camera runs program memory)";
			break;
		case 21 : error = "too many values in COC";
			break;
		case 22 : error = "CCD temperature or electronics temperature out of range";
			break;
		case 23 : error = "buffer allocate error";
			break;
        case 24 : error = "READ_IMAGE error";
			break;
		case 25 : error = "set/reset buffer flags is failed";
			break;
		case 26 : error = "buffer is used";
			break;
        case 27 : error = "call to a windows function is failed";
			break;
		case 28 : error = "DMA error";
			break;
		case 29 : error = "cannot open file";
			break;
		case 30 : error = "registry error";
			break;
		case 31 : error = "open dialog error";
			break;
		case 32 : error = "needs newer called vxd or dll";
			break;

		default : error = "Unknown error";
			return error;
		}	
}


//---------------------------------------------------------------------------------------
 