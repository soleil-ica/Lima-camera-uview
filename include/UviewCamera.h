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
#ifndef UVIEWCAMERA_H
#define UVIEWCAMERA_H

#include "lima/Debug.h"
#include "lima/Constants.h"
#include "lima/HwBufferMgr.h"
#include "lima/ThreadUtils.h"
#include "lima/Exceptions.h"
#include "UviewSendReceive.h"
#include "UviewCompatibility.h"

namespace lima
{
namespace Uview
{
//    class Camera
class LIBUVIEW_API Camera
{
	DEB_CLASS_NAMESPC(DebModCamera,"Camera","Uview");
	friend class Interface;
public:
	
	enum Status
	{
		Ready, Exposure, Readout, Latency, Fault
	};

	Camera();
	~Camera();

    void getExpTime(double& exp_time);
    void setExpTime(double  exp_time);

	Status getStatus();
	int getNbAcquiredFrames();
	    
    void startAcq();
	void stopAcq();
	void prepareAcq();

    void reset();


	void setNbFrames(int nb_frames);
	void getNbFrames(int& nb_frames);

	HwBufferCtrlObj* getBufferCtrlObj() { return &m_buffer_ctrl_obj; };

	void setImageType(ImageType type);
    void getImageType(ImageType& type);

	void getDetectorModel(std::string& model);

	void refreshImageSize();

	int getMaxWidth();
	int getMaxHeight();
	int getImageWidth();
	int getImageHeight();

	int getUseFullFrame();
	void setUseFullFrame(int val);

	void setBin(const Bin& bin);
	void getBin(Bin& bin);
	void checkBin(Bin& bin);

    void checkRoi(const Roi& set_roi, Roi& hw_roi);
    void setRoi(const Roi& set_roi);
    void getRoi(Roi& hw_roi);   

    std::string UviewErrorString(short response);

    UviewSendReceive * _uViewCom;

    void setAverage(long value);
	 	   
	void setCameraROI(short originX, short originY, short roiWidth, short roiHeight);
    void setBinning(short hBin, short vBin);
    void getCameraROI(short originX, short originY, short roiWidth, short roiHeight);
    void getBinning(short hBin, short vBin);
	
    //IvsT specifique   
    float getROIdata(short ROIid);
   
	bool m_Acq_running;
	bool m_ivs_roi_data_1_enable;
	bool m_ivs_roi_data_2_enable;
	bool m_ivs_roi_data_3_enable;
	bool m_ivs_roi_data_4_enable;
	bool m_ivs_roi_data_5_enable;

	float checkIvsROIValues(short ROIid);
	void IvSRoiDataImported();
	bool IsIvSRoiDataReady();
    
private:
/////////////////////////////////////////////////////////////////
	class CameraThread: public CmdThread
	{
	    DEB_CLASS_NAMESPC(DebModCamera, "CameraThread", "Uview");
	public:
		enum
		{ // Status
			Ready = MaxThreadStatus, Exposure, Readout, Latency,
		};

		enum
		{ // Cmd
			StartAcq = MaxThreadCmd, StopAcq,
		};

		CameraThread(Camera& cam);

		virtual void start();

		int getNbAcquiredFrames();
		bool m_force_stop;

	protected:
		virtual void init();
		virtual void execCmd(int cmd);
		void execStartAcq();
	private:
		Camera* m_cam;
  };
/////////////////////////////////////////////////////////////////

	friend class CameraThread;

	/* Lima buffer control object */
	SoftBufferCtrlObj m_buffer_ctrl_obj;
	
	unsigned short m_sbin;
	unsigned short m_pbin;
	/* Related to API PvCam */
	int short m_handle; 
	char m_name[128];
	double m_exposure;

	char m_error_msg[200];
	int m_error_code;
	int m_nb_frames;

    int m_size;

	int m_int_acq_mode;	

	int m_max_width;
	int m_max_height;
	int m_depth;
	int m_width;
	int m_height;
	
	std::string m_adc_rate;
	
	unsigned short *m_frame;
	unsigned short *m_pr_buffer;

	//--Private IVS Specific--//
	//lima::Mutex _IvSRoIAccess;
	void getIvsROIValues();

	float m_ivs_roi_data_1;
	float m_ivs_roi_data_2;
	float m_ivs_roi_data_3;
	float m_ivs_roi_data_4;

	/* main acquisition thread*/
	CameraThread 	m_thread;
	int 			m_acq_frame_nb;
	bool	_IvsTRoiReady;

	Roi m_roi;


};
}
}
#endif
