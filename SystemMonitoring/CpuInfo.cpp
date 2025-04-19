#include "stdafx.h"
#include "CpuInfo.h"
#include <pdh.h>
#include <pdhmsg.h>
#include <tlhelp32.h>

#include "CpuInfo.h"
#include <windows.h>
#include <Wbemidl.h>
#include <comdef.h>
#include <iostream>

#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "wbemuuid.lib")

bool CpuInfo::Initialize() {
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) return false;

    hres = CoInitializeSecurity(NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE, NULL);
    if (FAILED(hres)) {
        CoUninitialize();
        return false;
    }

    IWbemLocator* pLocator = nullptr;
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLocator);
    if (FAILED(hres)) {
        CoUninitialize();
        return false;
    }

    IWbemServices* pServices = nullptr;
    hres = pLocator->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pServices);
    if (FAILED(hres)) {
        pLocator->Release();
        CoUninitialize();
        return false;
    }

    hres = CoSetProxyBlanket(pServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
        NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE);
    if (FAILED(hres)) {
        pServices->Release();
        pLocator->Release();
        CoUninitialize();
        return false;
    }

    IEnumWbemClassObject* pEnumerator = nullptr;
    hres = pServices->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT Name, NumberOfCores, NumberOfLogicalProcessors, MaxClockSpeed FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL, &pEnumerator);
    if (FAILED(hres)) {
        pServices->Release();
        pLocator->Release();
        CoUninitialize();
        return false;
    }

    IWbemClassObject* pClassObject = nullptr;
    ULONG uReturn = 0;

    if (pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn) == S_OK) {
        VARIANT vtProp;

        if (SUCCEEDED(pClassObject->Get(L"Name", 0, &vtProp, 0, 0))) {
            name_ = vtProp.bstrVal;
            VariantClear(&vtProp);
        }

        if (SUCCEEDED(pClassObject->Get(L"NumberOfCores", 0, &vtProp, 0, 0))) {
            physicalCores_ = vtProp.uintVal;
            VariantClear(&vtProp);
        }

        if (SUCCEEDED(pClassObject->Get(L"NumberOfLogicalProcessors", 0, &vtProp, 0, 0))) {
            logicalProcessors_ = vtProp.uintVal;
            VariantClear(&vtProp);
        }

        if (SUCCEEDED(pClassObject->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0))) {
            baseSpeedMHz_ = vtProp.uintVal;
            VariantClear(&vtProp);
        }

        pClassObject->Release();
    }

    // Cleanup
    pEnumerator->Release();
    pServices->Release();
    pLocator->Release();
    CoUninitialize();

    return true;
}

void CpuInfo::UpdateDynamicInfo() {
    // Uso da CPU (%)
    static PDH_HQUERY cpuQuery;
    static PDH_HCOUNTER cpuTotal;
    static bool initialized = false;

    if (!initialized) {
        PdhOpenQuery(NULL, NULL, &cpuQuery);
        PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
        PdhCollectQueryData(cpuQuery);
        initialized = true;
        Sleep(100); // Espera para estabilizar a coleta
    }

    PdhCollectQueryData(cpuQuery);

    PDH_FMT_COUNTERVALUE counterVal;
    if (PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_LONG, NULL, &counterVal) == ERROR_SUCCESS) {
        utilization_ = counterVal.longValue;
    }

    // Contador de processos e threads
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    unsigned int procCount = 0, threadCount = 0;

    if (Process32First(hSnapshot, &pe32)) {
        do {
            ++procCount;
            threadCount += pe32.cntThreads;
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);

    processCount_ = procCount;
    threadCount_ = threadCount;

    // Velocidade atual da CPU (tentativa via registry)
    HKEY hKey;
    DWORD data;
    DWORD dataSize = sizeof(DWORD);
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, L"~MHz", NULL, NULL, (LPBYTE)&data, &dataSize) == ERROR_SUCCESS) {
            currentSpeedMHz_ = static_cast<float>(data);
        }
        RegCloseKey(hKey);
    }
}


std::wstring CpuInfo::GetName() const {
    return name_;
}

unsigned int CpuInfo::GetPhysicalCores() const {
    return physicalCores_;
}

unsigned int CpuInfo::GetLogicalProcessors() const {
    return logicalProcessors_;
}

unsigned int CpuInfo::GetBaseSpeedMHz() const {
    return baseSpeedMHz_;
}

unsigned int CpuInfo::GetUtilization() const {
    return utilization_;
}

unsigned int CpuInfo::GetProcessCount() const {
    return processCount_;
}

unsigned int CpuInfo::GetThreadCount() const {
    return threadCount_;
}

float CpuInfo::GetCurrentSpeedMHz() const {
    return currentSpeedMHz_;
}
