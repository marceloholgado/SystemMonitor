#pragma once
#include <string>

class CpuInfo
{
public:
    bool Initialize();

    std::wstring GetName() const;
    unsigned int GetPhysicalCores() const;
    unsigned int GetLogicalProcessors() const;
    unsigned int GetBaseSpeedMHz() const;
    void UpdateDynamicInfo();

    unsigned int GetUtilization() const;
    unsigned int GetProcessCount() const;
    unsigned int GetThreadCount() const;
    float GetCurrentSpeedMHz() const;
private:
    std::wstring name_;
    unsigned int physicalCores_ = 0;
    unsigned int logicalProcessors_ = 0;
    unsigned int baseSpeedMHz_ = 0;
    unsigned int utilization_ = 0;
    unsigned int processCount_ = 0;
    unsigned int threadCount_ = 0;
    float currentSpeedMHz_ = 0.0f;
};

