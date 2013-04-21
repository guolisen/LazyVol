#include "MainHead.h"
#include "OfficeVolModule.h"
#include <libconfig.h>
using namespace libconfig;

OfficeVolModule* OfficeVolModule::m_pInstance;


OfficeVolModule::OfficeVolModule(void): mCfgFileName("config/config.cfg")
{
}

OfficeVolModule::~OfficeVolModule(void)
{
	if(mVol)
		delete mVol;
}

int OfficeVolModule::Init(OfficeVolDelegate* delegate)
{
	if(!delegate)
		return OFF_ERROR_PARAMTER;

	SetDelegate(delegate);
	mVol = new CVolumeOutMaster;
	mNowVolClass = MASTER_VOL;
	mMaxVol = mVol->GetMaximalVolume();
	mMinVol = mVol->GetMinimalVolume();

	// Read the file. If there is an error, report it and exit.
	try
	{
		mCfg.readFile(mCfgFileName.c_str());
	}
	catch(const FileIOException& fioex)
	{
		std::cerr << "I/O error while reading file." << std::endl;
		Off_DegMsg("I/O error while reading file.");
		return OFF_ERROR_CONFIG_FILE;
	}
	catch(const ParseException &pex)
	{
		Off_DegMsg(pex.getError());
		std::cout << "Parse error at " << pex.getFile() << ":" << pex.getLine()
		          << " - " << pex.getError() << std::endl;
		return OFF_ERROR_CONFIG_FILE;
	}

	try
	{
		std::string tmp = mCfg.lookup("OFF_VOLUP");
	
		mDelegate->ReConfig(OFF_KEY_UPDATE, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_VOLUP"));
		
		tmp = static_cast<std::string>(mCfg.lookup("OFF_VOLDOWN"));
		mDelegate->ReConfig(OFF_KEY_UPDATE, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_VOLDOWN"));
		
		tmp = static_cast<std::string>(mCfg.lookup("OFF_VOLON"));
		mDelegate->ReConfig(OFF_KEY_UPDATE, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_VOLON"));
		
		tmp = static_cast<std::string>(mCfg.lookup("OFF_VOLOFF"));
		mDelegate->ReConfig(OFF_KEY_UPDATE, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_VOLOFF"));
		
		tmp = static_cast<std::string>(mCfg.lookup("OFF_VOLSELECT_MASTER"));
		mDelegate->ReConfig(OFF_KEY_UPDATE, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_VOLSELECT_MASTER"));
		
		tmp = static_cast<std::string>(mCfg.lookup("OFF_VOLSELECT_WAVE"));
		mDelegate->ReConfig(OFF_KEY_UPDATE, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_VOLSELECT_WAVE"));
		
		tmp = static_cast<std::string>(mCfg.lookup("OFF_VOLSELECT_MIC"));
		mDelegate->ReConfig(OFF_KEY_UPDATE, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_VOLSELECT_MIC"));
		
		tmp = static_cast<std::string>(mCfg.lookup("OFF_OPTION"));
		mDelegate->ReConfig(OFF_KEY_UPDATE, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_OPTION"));
		
		tmp = static_cast<std::string>(mCfg.lookup("OFF_QUIT"));
		mDelegate->ReConfig(OFF_KEY_UPDATE, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_QUIT"));		

		tmp = static_cast<std::string>(mCfg.lookup("OFF_WINSTART"));
		//mDelegate->ReConfig(OFF_WIN_START, tmp, "");
		mActionMap.insert(make_pair(tmp, "OFF_WINSTART"));		
	}
	catch(const SettingNotFoundException &nfex)
	{
		Off_DegMsg(nfex.what());
		return OFF_ERROR;
	}

	mDelegate->InitOffVol();
	return OFF_OK;
}

void OfficeVolModule::UnInit()
{
	mDelegate->UnInitOffVol();
}


void OfficeVolModule::VolUp()
{

	//if(mVol->IsAvailable())
	{
		assert(mVol);
		mVol->Enable();
		DWORD nowVol = mVol->GetCurrentVolume();
		nowVol += mMaxVol/20;
		mVol->SetCurrentVolume(nowVol>mMaxVol?mMaxVol:nowVol);
	}
}

void OfficeVolModule::VolDown()
{
	//if(mVol->IsAvailable())
	{
		assert(mVol);
		mVol->Enable();
		DWORD nowVol = (DWORD)mVol->GetCurrentVolume();
		nowVol -= mMaxVol/20;
		int setVal = 0;
		if(nowVol <= mMinVol || nowVol<0)
			setVal = 0;
		else
			setVal = nowVol;
		
		mVol->SetCurrentVolume(setVal);
	}
}

void OfficeVolModule::ChangeVolType(VOL_CLASS vol_class)
{
	switch(vol_class)
	{
		case MASTER_VOL:
			if(MASTER_VOL == mNowVolClass)
				return;
			if(mVol)
			{
				delete mVol;
				mVol = NULL;
			}

			mVol = new CVolumeOutMaster;
			mNowVolClass = MASTER_VOL;
			break;
		case WAVE_VOL:	
			if(WAVE_VOL == mNowVolClass)
				return;
			if(mVol)
			{
				delete mVol;
				mVol = NULL;
			}

			mVol = new CVolumeOutWave;
			mNowVolClass = WAVE_VOL;
			break;
		default:
			break;
	};

}

int OfficeVolModule::UpdateStatus(const std::string& key_down)
{
	ACTION_MAP::iterator iter;
	iter = mActionMap.find(key_down);
	
	assert(mDelegate);
	mDelegate->UiUpdate(iter->second);
	return 0;
}

int OfficeVolModule::SetConfigStr(OFF_CONFIG_CLASS config_class, 
									 const std::string& name_str,
	                                 const std::string& value_str)
{
	assert(mDelegate);
	if(name_str.empty())
	{
		Off_DegMsg("Para name_str empty Error!");
		return OFF_ERROR;
	}
	std::string tmpOldVal;
	if(GetConfigStr(name_str, tmpOldVal))
	{
		Off_DegMsg("GetConfigStr Error!");
		return OFF_ERROR;
	}

	if(tmpOldVal == value_str)
		return OFF_OK;
	
	int ret = mDelegate->ReConfig(config_class, value_str, tmpOldVal);
	if(ret)
	{
		return ret;
	}

	ACTION_MAP::iterator iter;
	iter = mActionMap.find(tmpOldVal);
	std::string tmpAction = iter->second;
	mActionMap.erase(iter);
	mActionMap.insert(make_pair(value_str, tmpAction));

	try
	{
		Setting &root  = mCfg.getRoot();
		root[name_str] = value_str;
		mCfg.writeFile(mCfgFileName.c_str());
	}
	catch(const FileIOException &fioex)
	{
		std::cerr << "I/O error while writing file: " << "config.cfg" << std::endl;
		return OFF_ERROR;
	}	
	catch(const SettingNotFoundException &nfex)
	{
		return OFF_ERROR;
	}	

	return OFF_OK;
}

int OfficeVolModule::GetConfigStr(const std::string& name_str, std::string& value_ret)
{
	try
	{
		value_ret = static_cast<std::string>(mCfg.lookup(name_str.c_str()));
	}
	catch(const SettingNotFoundException &nfex)
	{
		std::cerr << "No '"<< name_str <<"' setting in configuration file." << std::endl;
		Off_DegMsg("Can not Find the Config Name!\n");

		return OFF_ERROR;
	}	
	
	return OFF_OK;
}


ModifierKeyParser::ModifierKeyParser(const std::string &hotkeyStr) :
	modifier_(0),
	key_(0),
	valid_(false)
{
	if (hotkeyStr.empty())
		return;

	std::string s(hotkeyStr);
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	std::string::size_type n = s.find_last_of('+');
	if (n == std::string::npos)
		return;

	if (s.find("AL") != std::string::npos) modifier_ |= MOD_ALT;
	if (s.find("SHT") != std::string::npos) modifier_ |= MOD_SHIFT;
	if (s.find("CTL") != std::string::npos) modifier_ |= MOD_CONTROL;
	if (modifier_ == 0)
		return;

	std::string mainKey = s.substr(n+1);
	int keyVal = StrToKeyVal(mainKey);
	if(keyVal)
	{
		key_ = keyVal;
		valid_ = true;
	}
}

ModifierKeyParser::~ModifierKeyParser()
{
}

bool ModifierKeyParser::IsValid()
{
	return valid_;
}

UINT ModifierKeyParser::GetModifier()
{
	return modifier_;
}

UINT ModifierKeyParser::GetKey()
{
	return key_;
}


std::string ModifierKeyParser::KeyValToStr(TCHAR key)
{
	switch(key)
	{
		case (TCHAR)VK_MENU:
			return "ALT";
			break;			
		case (TCHAR)VK_CONTROL:
			return "CTRL";
			break;		
		case (TCHAR)VK_SHIFT:
			return "SHIFT";
			break;
		case (TCHAR)VK_ESCAPE:  
			return "ESC";
			break;
		case (TCHAR)VK_SPACE: 
			return "SPACE";
			break;
		case (TCHAR)VK_PRIOR: 
			return "PRIOR";
			break;
		case (TCHAR)VK_NEXT:     
			return "NEXT";
			break;
		case (TCHAR)VK_END:     
			return "END";
			break;
		case (TCHAR)VK_HOME:    
			return "HOME";
			break;
		case (TCHAR)VK_LEFT:  
			return "¡û";
			break;
		case (TCHAR)VK_UP:    
			return "¡ü";
			break;
		case (TCHAR)VK_RIGHT:  
			return "¡ú";
			break;
		case (TCHAR)VK_DOWN: 
			return "¡ý";
			break;
		case (TCHAR)VK_SELECT: 
			return "SELECT";
			break;
		case (TCHAR)VK_PRINT:  
			return "PRINT";
			break;
		case (TCHAR)VK_EXECUTE: 
			return "EXECUTE";
			break;
		case (TCHAR)VK_SNAPSHOT: 
			return "SNAPSHOT";
			break;
		case (TCHAR)VK_INSERT:  
			return "INS";
			break;
		case (TCHAR)VK_DELETE: 
			return "DEL";
			break;
		case (TCHAR)VK_HELP:  
			return "HELP";
			break;
		case (TCHAR)VK_LWIN:  
			return "LWIN";
			break;
		case (TCHAR)VK_RWIN:  
			return "RWIN";
			break;
		case (TCHAR)VK_APPS:  
			return "APPS";
			break;
		case (TCHAR)VK_NUMPAD0:
			return "NUM0";
			break;
		case (TCHAR)VK_NUMPAD1: 
			return "NUM1";
			break;
		case (TCHAR)VK_NUMPAD2: 
			return "NUM2";
			break;
		case (TCHAR)VK_NUMPAD3:
			return "NUM3";
			break;
		case (TCHAR)VK_NUMPAD4:
			return "NUM4";
			break;
		case (TCHAR)VK_NUMPAD5:
			return "NUM5";
			break;
		case (TCHAR)VK_NUMPAD6: 
			return "NUM6";
			break;
		case (TCHAR)VK_NUMPAD7: 
			return "NUM7";
			break;
		case (TCHAR)VK_NUMPAD8: 
			return "NUM8";
			break;
		case (TCHAR)VK_NUMPAD9: 
			return "NUM9";
			break;
		case (TCHAR)VK_MULTIPLY:
			return "NUM*";
			break;
		case (TCHAR)VK_ADD:     
			return "NUM+";
			break;	
		//case (TCHAR)VK_SEPARATOR: 
		//	return ";";
		//	break;
		case (TCHAR)VK_SUBTRACT: 
			return "NUM-";
			break;
		case (TCHAR)VK_DECIMAL:  
			return "NUM.";
			break;
		case (TCHAR)VK_DIVIDE: 
			return "NUM/";
			break;
		case (TCHAR)VK_F1:   
			return "F1";
			break;
		case (TCHAR)VK_F2:   
			return "F2";
			break;
		case (TCHAR)VK_F3:  
			return "F3";
			break;
		case (TCHAR)VK_F4: 
			return "F4";
			break;
		case (TCHAR)VK_F5: 
			return "F5";
			break;
		case (TCHAR)VK_F6:  
			return "F6";
			break;
		case (TCHAR)VK_F7:  
			return "F7";
			break;
		case (TCHAR)VK_F8:  
			return "F8";
			break;
		case (TCHAR)VK_F9:  
			return "F9";
			break;
		case (TCHAR)VK_F10:   
			return "F10";
			break;
		case (TCHAR)VK_F11:  
			return "F11";
			break;
		case (TCHAR)VK_F12:   
			return "F12";
			break;
		case (TCHAR)VK_BACK:  
			return "BACK";
			break;
		case (TCHAR)VK_TAB:  
			return "TAB";
			break;
		case (TCHAR)VK_LBUTTON:  
			return "LBUTTON";
			break;
		case (TCHAR)VK_RBUTTON:   
			return "RBUTTON";
			break;
		case (TCHAR)VK_MBUTTON:   
			return "MBUTTON";
			break;

		case (TCHAR)VK_OEM_1:   
			return ";";
			break;
		case (TCHAR)VK_OEM_PLUS:   
			return "=";
			break;
		case (TCHAR)VK_OEM_COMMA:   
			return ",";
			break;
		case (TCHAR)VK_OEM_MINUS:   
			return "-";
			break;	
		case (TCHAR)VK_OEM_PERIOD:   
			return ".";
			break;
		case (TCHAR)VK_OEM_2:   
			return "/";
			break;
		case (TCHAR)VK_OEM_3:   
			return "`";
			break;			
		case (TCHAR)VK_OEM_4:   
			return "[";
			break;
		case (TCHAR)VK_OEM_5:   
			return "\\";
			break;	
		case (TCHAR)VK_OEM_6:   
			return "]";
			break;
		case (TCHAR)VK_OEM_7:   
			return "'";
			break;
	};

	//key &= 0x7F;
	if(key>=27 && key<=127)
		return static_cast<std::string>((char*)&key);

	return static_cast<std::string>("");
}

int ModifierKeyParser::StrToKeyVal(const std::string& key_str)
{
	if(key_str == "ALT")
		return VK_MENU;

	else if(key_str == "CTRL")
		return VK_CONTROL;

	else if(key_str == "SHIFT")	
		return VK_SHIFT;

	else if(key_str == "ESC")
		return VK_ESCAPE;  

	else if(key_str == "SPACE")
		return VK_SPACE;

	else if(key_str == "PRIOR")
		return VK_PRIOR;
	
	else if(key_str == "NEXT")
		return VK_NEXT;   

	else if(key_str == "END")
		return VK_END; 

	else if(key_str == "HOME")
		return VK_HOME; 

	else if(key_str == "¡û")
		return VK_LEFT; 

	else if(key_str == "¡ü")
		return VK_UP;  

	else if(key_str == "¡ú")
		return VK_RIGHT;

	else if(key_str == "¡ý")
		return VK_DOWN;

	else if(key_str == "SELECT")
		return VK_SELECT;

	else if(key_str == "PRINT")
		return VK_PRINT; 

	else if(key_str == "EXECUTE")
		return VK_EXECUTE; 

	else if(key_str == "SNAPSHOT")
		return VK_SNAPSHOT;

	else if(key_str == "INS")
		return VK_INSERT; 

	else if(key_str == "DEL")
		return VK_DELETE;

	else if(key_str == "HELP")
		return VK_HELP;  

	else if(key_str == "LWIN")
		return VK_LWIN;  

	else if(key_str == "RWIN")
		return VK_RWIN; 

	else if(key_str == "APPS")
		return VK_APPS;  

	else if(key_str == "NUM0")
		return VK_NUMPAD0;

	else if(key_str == "NUM1")
		return VK_NUMPAD1; 

	else if(key_str == "NUM2")
		return VK_NUMPAD2;

	else if(key_str == "NUM3")
		return VK_NUMPAD3;

	else if(key_str == "NUM4")
		return VK_NUMPAD4;

	else if(key_str == "NUM5")
		return VK_NUMPAD5;

	else if(key_str == "NUM6")
		return VK_NUMPAD6;

	else if(key_str == "NUM7")
		return VK_NUMPAD7;

	else if(key_str == "NUM8")
		return VK_NUMPAD8;

	else if(key_str == "NUM9")
		return VK_NUMPAD9;

	else if(key_str == "NUM*")
		return VK_MULTIPLY;

	else if(key_str == "NUM+")
		return VK_ADD;    

	//else if(key_str == ";")
	//	return VK_SEPARATOR;

	else if(key_str == "NUM-")
		return VK_SUBTRACT; 

	else if(key_str == "NUM.")
		return VK_DECIMAL; 

	else if(key_str == "NUM/")
		return VK_DIVIDE;

	else if(key_str == "F1")
		return VK_F1;

	else if(key_str == "F2")
		return VK_F2;  

	else if(key_str == "F3")
		return VK_F3;  

	else if(key_str == "F4")
		return VK_F4; 

	else if(key_str == "F5")
		return VK_F5;

	else if(key_str == "F6")
		return VK_F6;  

	else if(key_str == "F7")
		return VK_F7;  

	else if(key_str == "F8")
		return VK_F8;  

	else if(key_str == "F9")
		return VK_F9;  

	else if(key_str == "F10")
		return VK_F10;   

	else if(key_str == "F11")
		return VK_F11; 

	else if(key_str == "F12")
		return VK_F12;  

	else if(key_str == "BACK")
		return VK_BACK;  

	else if(key_str == "TAB")
		return VK_TAB; 

	else if(key_str == "LBUTTON")
		return VK_LBUTTON; 

	else if(key_str == "RBUTTON")
		return VK_RBUTTON;  

	else if(key_str == "MBUTTON")
		return VK_MBUTTON;  

	else if(key_str == ">" || key_str == ".")
		return VK_OEM_PERIOD;  
	else if(key_str == "<" || key_str == ",")
		return VK_OEM_COMMA;  
	else if(key_str == ";" || key_str == ":")
		return VK_OEM_1;  
	else if(key_str == "'" || key_str == "\"")
		return VK_OEM_7;  
	else if(key_str == "/" || key_str == "?")
		return VK_OEM_2;  
	else if(key_str == "\\" || key_str == "|")
		return VK_OEM_5;  
	else if(key_str == "[" || key_str == "{")
		return VK_OEM_4;  
	else if(key_str == "]" || key_str == "}")
		return VK_OEM_6; 
	else if(key_str == "=" || key_str == "+")
		return VK_OEM_PLUS; 
	else if(key_str == "-" || key_str == "_")
		return VK_OEM_MINUS; 
	else if(key_str == "`" || key_str == "~")
		return VK_OEM_3; 	
	

	else if(key_str[0]>=27 && key_str[0]<=127)
		return (char)key_str[0];

	return 0;

}
	
