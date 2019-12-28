#ifndef IOMNG
#define IOMNG
#include "sysDefine.h"
#include "sysFunc.h"
#include "FS.h"
#include "time.h"
#include "pinDevice.h"
pinDevice device[NUM_OF_DEVICE];
class ioMng
{
public:
	ioMng(){
		JsonObject mainObj = getObj("");
		JsonObject ioMngObj = mainObj["IOMNG"];

		int i=0;
		for (JsonPair kv : ioMngObj) {
			if(i>=NUM_OF_DEVICE)
				break;
		    device[i++] = pinDevice(
		    	kv.key().c_str(),
		    	kv.value().as<JsonObject>()["state"],
		    	kv.value().as<JsonObject>()["start"],
		    	kv.value().as<JsonObject>()["end"]
		    	);
		}
	};

};

#endif
