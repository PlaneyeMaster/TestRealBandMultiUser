//Copyright(C) 2022 VAXPORT all rights reserved.


#ifndef FRealBandExportClass_H
#define FRealBandExportClass_H



#ifdef  REALBANDIMPORTER_EXPORTS 
/*Enabled as "export" while compiling the dll project*/
#define DLLEXPORT __declspec(dllexport)  
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define DLLEXPORT __declspec(dllimport)  
#endif

#endif