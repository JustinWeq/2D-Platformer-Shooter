#pragma once
#define SAFE_SET_STRING( strToSet,strToSetToo){delete[] strToSet;strToSet = strToSetToo;}
#define SAFE_DELETE(object){if(object)delete object;object = NULL;}
#define TO_UPPER(str){for(int i =0;i < sizeof(str);i++) if(str[i] >96)str[i]-=31;}