int iData = 0;
String stringData = "";
bool bPrint = false;
bool bSendToDisplay = false;

float fTemp = 0;
int iTemp_ = 2;
float fHum = 0;
int iHum_ = 2;
float fMoist = 0;
int iMoist_ = 2;
int iMin_Moist_percent = 90;
int iStop_Moist_percent = 91;
#define Chart_length 3000
int iMoist_Array[Chart_length+1];

int iTemp_color_count = 0;
int iHum_color_count = 0;
int iMoist_color_count = 0;

int iSecond = 0;
int iMinute = 0;
int iHour = 0;
int iDay = 0;
int iMonth = 0;
int iYear = 0;
int iTimeUpdate = 0;
int iSystime = 0;