#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <mutex>
#include <memory>
#include <future>

#include "CUESDKDevice.h"
#include "CorsairPluginDevice.h"
#include "json/json.hpp"

class NetworkClient;
class RGBController;

class CorsairPluginDeviceManager
{
public:
	CorsairPluginDeviceManager(void* pluginContext, _DeviceConnectionStatusChangeCallback callback,
		std::function<std::string(const std::string&)> imageHasher,
		std::function<std::string(const std::string&)> deviceHasher,
		std::function<std::wstring(const std::wstring&)> localPath);
	virtual ~CorsairPluginDeviceManager();

	virtual void Start();
	virtual void Stop();
	virtual bool ReadJson();
	virtual void ConnectDevices();
	virtual void DisconnectDevices();

	// CUE Calls
	virtual bool SetColor(const char* deviceId, std::int32_t size, cue::dev::plugin::LedColor* ledsColors);
	virtual cue::dev::plugin::DeviceInfo* GetDeviceInfo(const char* deviceId);
	virtual cue::dev::plugin::DeviceView* GetDeviceView(const char* deviceId, std::int32_t index);

protected:
	void* mPluginContext;
	_DeviceConnectionStatusChangeCallback mDeviceCallback;
	nlohmann::json mSettings;
	nlohmann::json mDevices;

private:
	void UpdateDevices(std::unordered_set<std::string> deviceSet, bool notifyHost);

	std::mutex mDeviceLock;
	std::unordered_map<std::string, std::unique_ptr<CorsairPluginDevice>> mDeviceMap;
	std::future<void> mDeviceUpdateRequest;
	std::unique_ptr<NetworkClient> mNetworkClient;
	std::vector<RGBController*> mControllerList;

	std::function<std::string(const std::string&)> mImageHasher;
	std::function<std::string(const std::string&)> mDeviceHasher;
	std::function<std::wstring(const std::wstring&)> mLocalFile;
};