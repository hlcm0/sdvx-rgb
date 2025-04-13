#include <Windows.h>
#include <MinHook.h>
#include <cstdint>

// shared memory
HANDLE hMapFile;
uint8_t* lpBase = nullptr;

int TapeLedDataOffset[10] = { 0 * 3, 74 * 3, 86 * 3, 98 * 3, 154 * 3, 210 * 3, 304 * 3, 316 * 3, 328 * 3, 342 * 3 };
int TapeLedDataCount[10] = { 74 * 3, 12 * 3, 12 * 3, 56 * 3, 56 * 3, 94 * 3, 12 * 3, 12 * 3, 14 * 3, 86 * 3 };

// Define original function
typedef void(__fastcall* SetTapeLedData_t)(void* This, unsigned int index, uint8_t* data);

// Save original function pointer
SetTapeLedData_t fpOriginal = nullptr;

// Hook function
void __fastcall SetTapeLedDataHook(void* This, unsigned int index, uint8_t* data) {
    if (lpBase && index < 10) {
        // Copy data into shared memory
        memcpy(lpBase + TapeLedDataOffset[index], data, TapeLedDataCount[index]);
    }

    // Call original function
    fpOriginal(This, index, data);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        // Init MinHook
        if (MH_Initialize() != MH_OK) {
            return FALSE;
        }

        // Get target module address
        HMODULE hTarget = GetModuleHandleA("libaio-iob2_video.dll");
        if (!hTarget) {
            return FALSE;
        }

        // Get target function address
        void* pTarget = GetProcAddress(hTarget,
            "?SetTapeLedData@AIO_IOB2_BI2X_UFC@@QEAAXIPEBX@Z");
        if (!pTarget) {
            return FALSE;
        }

        // Create Hook
        if (MH_CreateHook(pTarget, &SetTapeLedDataHook,
            reinterpret_cast<void**>(&fpOriginal)) != MH_OK) {
            return FALSE;
        }

        // Enable Hook
        if (MH_EnableHook(pTarget) != MH_OK) {
            return FALSE;
        }

        // Init shared memory
        hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            1284,   // buffer size
            L"sdvxrgb"
        );
        if (hMapFile) {
            lpBase = static_cast<uint8_t*>(MapViewOfFile(
                hMapFile,
                FILE_MAP_ALL_ACCESS,
                0,
                0,
                1284   // buffer size
            ));
        }

        break;
    }
    case DLL_PROCESS_DETACH: {
        // Clean up shared memory
        if (lpBase) {
            UnmapViewOfFile(lpBase);
            lpBase = nullptr;
        }
        if (hMapFile) {
            CloseHandle(hMapFile);
            hMapFile = NULL;
        }

        // Clean up Hook
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        break;
    }
    }
    return TRUE;
}