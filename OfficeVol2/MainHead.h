#ifndef MAIN_HEAD_H
#define MAIN_HEAD_H

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
 
#include <objbase.h>
#include <zmouse.h>
#include <exdisp.h>
#include <comdef.h>
#include <vector>
#include <sstream>
#include <exdisp.h>
#include <comdef.h>
#include <windows.h>
#include <shlobj.h>
#include "resource.h"

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <libconfig.h++>
#include <libconfig.h>

#include "Volume/IVolume.h"
#include "Volume/VolumeOutMaster.h"
#include "Volume/VolumeInXXX.h"
#include "Volume/VolumeOutWave.h"

#define		Off_Msg(msg)		::MessageBox(NULL, msg, "信息", 0)
#define		Off_DegMsg(msg)		OutputDebugStringA(msg)
#define		Off_DegMsgW(msg)	OutputDebugStringW(msg)


enum ERR_RET_CODE
{
	OFF_OK,
	OFF_ERROR,
	OFF_ERROR_PARAMTER,
	OFF_ERROR_RECONFIG,
	OFF_ERROR_CONFIG_FILE,
	OFF_ERROR_KEY_REGISTE,
	OFF_KEY_MULTIDEFINE
};

#endif
