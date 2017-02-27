#pragma once
#define SAFE_SET_STRING( strToSet,strToSetToo){delete[] strToSet;strToSet = strToSetToo;}
#define SAFE_DELETE(object){if(object)delete object;object = NULL;}