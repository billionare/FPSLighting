// color_pulse.cpp : Defines the entry point for the console application.
//

#ifdef __APPLE__
#include <CUESDK/CUESDK.h>
#else
#include <CUESDK.h>
#endif

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <unordered_set>
#include <cmath>

const char* toString(CorsairError error) 
{
	switch (error) {
	case CE_Success :
		return "CE_Success";
	case CE_ServerNotFound:
		return "CE_ServerNotFound";
	case CE_NoControl:
		return "CE_NoControl";
	case CE_ProtocolHandshakeMissing:
		return "CE_ProtocolHandshakeMissing";
	case CE_IncompatibleProtocol:
		return "CE_IncompatibleProtocol";
	case CE_InvalidArguments:
		return "CE_InvalidArguments";
	default:
		return "unknown error";
	}
}

std::vector<CorsairLedColor> getAvailableKeys() 
{
	auto colorsSet = std::unordered_set<CorsairLedId>();
	for (int deviceIndex = 0, size = CorsairGetDeviceCount(); deviceIndex < size; deviceIndex++) {
		if (const auto deviceInfo = CorsairGetDeviceInfo(deviceIndex)) {
			switch (deviceInfo->type) {
			case CDT_Mouse: {
				auto numberOfKeys = deviceInfo->physicalLayout - CPL_Zones1 + 1;
				for (auto i = 0; i < numberOfKeys; i++) {
					const auto ledId = static_cast<CorsairLedId>(CLM_1 + i);
					colorsSet.insert(ledId);
				}
			} break;
			case CDT_Headset: {
				colorsSet.insert(CLH_LeftLogo);
				colorsSet.insert(CLH_RightLogo);
			} break;
			case CDT_Keyboard:
			case CDT_MouseMat:
			case CDT_HeadsetStand:
			case CDT_CommanderPro:
			case CDT_LightingNodePro:
			case CDT_MemoryModule:
			case CDT_Cooler: {
				const auto ledPositions = CorsairGetLedPositionsByDeviceIndex(deviceIndex);
				if (ledPositions) {
					for (auto i = 0; i < ledPositions->numberOfLed; i++) {
						const auto ledId = ledPositions->pLedPosition[i].ledId;
						colorsSet.insert(ledId);
					}
				}
			} break;
			default:
				break;
			}
		}
	}

	std::vector<CorsairLedColor> colorsVector;
	colorsVector.reserve(colorsSet.size());
	for (const auto &ledId : colorsSet) {
		colorsVector.push_back({ledId, 0, 0, 0});
	}
	return colorsVector;
}

void performPulseEffect(int waveDuration, std::vector<CorsairLedColor> &ledColorsVec)
{
	const auto timePerFrame = 25;
	for (auto x = .0; x < 2; x += static_cast<double>(timePerFrame) / waveDuration) {
		auto val = static_cast<int>((1 - pow(x - 1, 2)) * 255);
		for (auto &ledColor : ledColorsVec)
			ledColor.g = val;
		CorsairSetLedsColorsAsync(static_cast<int>(ledColorsVec.size()), ledColorsVec.data(), nullptr, nullptr);
		std::this_thread::sleep_for(std::chrono::milliseconds(timePerFrame));
	}
}

int main()
{
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		std::cout << "Handshake failed: " << toString(error) << "\nPress any key tro quit." << std::endl;
		getchar();
		return -1;
	}

	std::atomic_int waveDuration{500};
	std::atomic_bool continueExecution{true};

	auto colorsVector = getAvailableKeys();
	if (colorsVector.empty()) {
		return 1;
	}

	std::cout << "Working... Use \"+\" or \"-\" to increase or decrease speed.\nPress \"q\" to close program..." << std::endl;

	std::thread lightingThread([&waveDuration, &continueExecution, &colorsVector]{
		while (continueExecution) {
			performPulseEffect(waveDuration.load(), colorsVector);
		}
	});

	while(continueExecution) {
		char c = getchar();
		switch (c) {
		case '+':
			if (waveDuration.load() > 100)
				waveDuration -= 100;
			break;
		case '-':
			if (waveDuration.load() < 2000)
				waveDuration += 100;
			break;
		case 'Q':
		case 'q':
			continueExecution = false;
			break;
		default:
			break;
		}
	}
	lightingThread.join();
	return 0;
}
