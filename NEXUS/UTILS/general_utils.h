#pragma once

#include "logging.h"

class Vector2D;
namespace SDK
{
	class CUserCmd;
}
namespace UTILS
{
	template<class T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		return (v >= lo && v <= hi) ? v : (v < lo ? lo : hi);
	}
    template<unsigned int IIdx, typename TRet, typename ... TArgs>
    static auto CallVFunc(void* thisptr, TArgs ... argList) -> TRet
    {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

	bool is_point_in_range(Vector2D, Vector2D, Vector2D);
	float GetFraction(float, float, float);
	float RandomFloat(float min, float max);
	float GetValueFromFraction(float, float, float);
	unsigned int GetNumberOfDigits(int);
	float RoundToDecimalPlaces(float, int);
	std::string FloatToString(float, int);
	Vector CalcAngle(Vector src, Vector dst);
	float GRD_TO_BOG(float GRD);
	void calculate_angle(Vector src, Vector dst, Vector & angles);
	bool IsOnScreen(Vector origin, Vector& screen);
	float GetCurtime();
	float Lerp(float fraction, float min, float max);

	DWORD find_signature_new(const char * szModuleName, const char * PatternName, char * szPattern);

	DWORD FindPatternV2(std::string moduleName, std::string pattern);

	uint64_t FindSignature(const char* szModule, const char* szSignature);
	DWORD WaitOnModuleHandle(std::string moduleName);
	bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask);
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);
	bool ClampLemon(Vector& angles);
	Vector TraceToEnd(Vector start, Vector end);
	const char* GetHitgroupName(int hitgroup);
	bool ConsolePrint(bool logToFile, const char *fmt, ...);
	const std::string currentDateTime();
}
