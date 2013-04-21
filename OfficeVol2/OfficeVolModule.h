#ifndef _OFFICEVOLMODULE_H
#define _OFFICEVOLMODULE_H


class ModifierKeyParser
{
public:
	ModifierKeyParser(const std::string &hotkeyStr);
	virtual ~ModifierKeyParser();

	bool IsValid();
	UINT GetModifier();
	UINT GetKey();
	static int ModifierKeyParser::StrToKeyVal(const std::string& key_str);
	static std::string ModifierKeyParser::KeyValToStr(TCHAR key);

private:
	ModifierKeyParser(const ModifierKeyParser&);
	ModifierKeyParser& operator=(const ModifierKeyParser&);

private:
	UINT modifier_;
	UINT key_;
	bool valid_;
};


enum VOL_CLASS
{
	MASTER_VOL,
	WAVE_VOL,
	OTHER_VOL
};



typedef std::map<std::string, std::string> ACTION_MAP;


enum OFF_CONFIG_CLASS
{
	OFF_KEY_UPDATE,
	OFF_WIN_START,
	OFF_PAD_DISPLAY_TIME
};

class OfficeVolDelegate
{
public:
	virtual void InitOffVol(){};
	virtual int UiUpdate(const std::string& action) = 0;
	virtual int ReConfig(OFF_CONFIG_CLASS config_class, 
			             const std::string& value, 
			             const std::string& old_value) =0;
	virtual void UnInitOffVol(){};
};

class OfficeVolModule
{
public:
	OfficeVolModule(void);
	virtual ~OfficeVolModule(void);

public:
	static OfficeVolModule* GetInstance()
	{
		if ( m_pInstance == NULL ) //判断是否第一次调用
			m_pInstance = new OfficeVolModule();
		return m_pInstance;
	}

	virtual int  Init(OfficeVolDelegate* delegate);
	virtual void VolUp();
	virtual void VolDown();
	virtual void UnInit();
	virtual void ChangeVolType(VOL_CLASS vol_class);

	virtual VOL_CLASS GetVolType()
	{
		return mNowVolClass;
	};


	virtual DWORD GetMaxVol() const
	{
		return mMaxVol;
	};
	virtual DWORD GetMinVol() const
	{
		return mMinVol;
	};

	virtual int UpdateStatus(const std::string& key_down);
	
	virtual int SetDelegate(OfficeVolDelegate* delegate_para)
	{
		assert(delegate_para);
		mDelegate = delegate_para;
		return OFF_OK;
	};

	virtual int SetConfigStr(OFF_CONFIG_CLASS config_class, 
								const std::string& name_str,
		                        const std::string& value_str);
	virtual int GetConfigStr(const std::string& name_str, std::string& value_ret);

	//IVolume
	virtual bool	IsAvailable()
	{
		assert(mVol);
		return mVol->IsAvailable();
	};
	virtual void	Enable()	
	{
		assert(mVol);
		return mVol->Enable();
	};
	virtual void	Disable()
	{
		assert(mVol);
		return mVol->Disable();
	};

	virtual DWORD	GetVolumeMetric()
	{
		assert(mVol);
		return mVol->GetVolumeMetric();
	};

	virtual DWORD	GetMinimalVolume()
	{
		assert(mVol);
		return mVol->GetMinimalVolume();
	};
	virtual DWORD	GetMaximalVolume()
	{
		assert(mVol);
		return mVol->GetMaximalVolume();
	};

	virtual DWORD	GetCurrentVolume()
	{
		assert(mVol);
		return mVol->GetCurrentVolume();
	};
	virtual void	SetCurrentVolume( DWORD dwValue )
	{
		assert(mVol);
		return mVol->SetCurrentVolume(dwValue);
	};

	virtual void	RegisterNotificationSink( PONMICVOULUMECHANGE p, DWORD d)
	{
		assert(mVol);
		return mVol->RegisterNotificationSink(p, d);
	};

protected:
	IVolume* mVol;
	DWORD mMaxVol;
	DWORD mMinVol;
	VOL_CLASS mNowVolClass;
	ACTION_MAP mActionMap;
	std::string mCfgFileName;
	libconfig::Config mCfg;

protected:
	OfficeVolDelegate* mDelegate; 
protected:
	static OfficeVolModule* m_pInstance;
	
};


#endif
