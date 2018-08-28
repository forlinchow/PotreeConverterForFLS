#pragma once


#import "iQOpen.dll" no_namespace
#include <string>
#include <Windows.h>
#include <math.h>
#include <unistd.h> 
#include <iostream> 

// 应该独立写一个reader类
class flsReader {
	
public:
	flsReader() { CoInitialize(NULL); }
	~flsReader() {
		libRef = NULL;
		liPtr = NULL;
		scanRef = NULL;
		CoUninitialize();
	}

public:
	IiQLibIfPtr getLibRef() {
		return libRef;
	}
	IiQScanObjIfPtr getScanRef() {
		return scanRef;
	}
	bool read(std::string name) {
		//if (access(name.data(), 0)) // fls don‘t exist
		//{
		//	return false;
		//}

		std::cout << "read sdk" << std::endl;
		BSTR licenseCode =
			L"FARO Open Runtime License\n"
			L"Key:W2CW4PNRTCTXXJ6T6KXYSRUPL\n" // License Key
			L"\n"
			L"The software is the registered property of "
			L"FARO Scanner Production GmbH, Stuttgart, Germany.\n"
			L"All rights reserved.\n"
			L"This software may only be used with written permission "
			L"of FARO Scanner Production GmbH, Stuttgart, Germany.";
		IiQLicensedInterfaceIfPtr _liPtr(__uuidof(iQLibIf));
		_liPtr->License = licenseCode;
		IiQLibIfPtr _libRef = static_cast<IiQLibIfPtr>(_liPtr);//点云数据IO

		_libRef->load(name.data());//加载数据
		if (_libRef == NULL)
		{
			return false;
		}

		IiQObjectIfPtr libObj = libRef->getScanObject(0);
		IiQScanObjIfPtr _scanRef = libObj->getScanObjSpecificIf();//法如扫描属性IO
		_scanRef->load();//加载属性

		scanRef = _scanRef;
		libRef = _libRef;
		return true;
	}
	bool readDataArray(IiQLibIfPtr dataRef, double *d); //读取数据

//public:
	IiQLicensedInterfaceIfPtr liPtr;
	IiQLibIfPtr libRef;
	IiQScanObjIfPtr scanRef;
};
