#include <windows.h> 
#include <setupapi.h> 
#include <dbt.h> 
#include <iostream> 
#include <fstream> 
#include <string> 
 
using namespace std; 
 
ofstream logfile("usb_log.txt", ios::app); 
HWND hStatusLabel; 
 
void SetUSBRegistryStatus(DWORD value) { 
    HKEY hKey; 
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
        "SYSTEM\\CurrentControlSet\\Services\\USBSTOR", 0, KEY_SET_VALUE, &hKey) == 
ERROR_SUCCESS) { 
        RegSetValueEx(hKey, "Start", 0, REG_DWORD, (const BYTE*)&value, sizeof(value)); 
        RegCloseKey(hKey); 
    } 
} 
 
void RunCommand(const char* cmd) { 
    system(cmd); 
} 
 
void StopUSBService() { 
    RunCommand("sc stop USBSTOR > nul 2>&1"); 
    logfile << "[SERVICE] USBSTOR service stopped." << endl; 
} 
 
void StartUSBService() { 
    RunCommand("sc start USBSTOR > nul 2>&1"); 
    logfile << "[SERVICE] USBSTOR service started." << endl; 
} 
 
void BlockUSBStorage() { 
    SetUSBRegistryStatus(4); // Disable 
    StopUSBService(); 
    logfile << "[BLOCKED] USB Storage devices disabled and service stopped." << endl; 
    SetWindowText(hStatusLabel, "USB Blocked"); 
} 
 
void UnblockUSBStorage() { 
    SetUSBRegistryStatus(3); // Enable 
    StartUSBService(); 
    logfile << "[UNBLOCKED] USB Storage devices enabled and service started." << endl; 
    SetWindowText(hStatusLabel, "USB Unblocked"); 
} 
string GetUSBDeviceDescription(LPARAM lParam) { 
    PDEV_BROADCAST_DEVICEINTERFACE pDevInf = 
(PDEV_BROADCAST_DEVICEINTERFACE)lParam; 
    if (pDevInf && pDevInf->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE) { 
        return string(pDevInf->dbcc_name); 
    } 
    return "Unknown USB Device"; 
} 
 
#define BTN_BLOCK 1 
#define BTN_UNBLOCK 2 
#define BTN_EXIT 3 
 
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM 
lParam) { 
    switch (uMsg) { 
    case WM_CREATE: 
        CreateWindow("BUTTON", "Block USB", WS_VISIBLE | WS_CHILD, 
            20, 20, 100, 30, hwnd, (HMENU)BTN_BLOCK, NULL, NULL); 
 
        CreateWindow("BUTTON", "Unblock USB", WS_VISIBLE | WS_CHILD, 
            140, 20, 100, 30, hwnd, (HMENU)BTN_UNBLOCK, NULL, NULL); 
 
        CreateWindow("BUTTON", "Exit", WS_VISIBLE | WS_CHILD, 
            260, 20, 100, 30, hwnd, (HMENU)BTN_EXIT, NULL, NULL); 
 
        hStatusLabel = CreateWindow("STATIC", "Status: Unknown", WS_VISIBLE | WS_CHILD, 
            20, 70, 340, 30, hwnd, NULL, NULL, NULL); 
        break; 
 
    case WM_COMMAND: 
        if (LOWORD(wParam) == BTN_BLOCK) { 
            BlockUSBStorage(); 
        } else if (LOWORD(wParam) == BTN_UNBLOCK) { 
            UnblockUSBStorage(); 
        } else if (LOWORD(wParam) == BTN_EXIT) { 
            SendMessage(hwnd, WM_CLOSE, 0, 0); 
        } 
        break; 
 
    case WM_DEVICECHANGE: 
        if (wParam == DBT_DEVICEARRIVAL) { 
            logfile << "[INFO] USB device inserted." << endl; 
            SetWindowText(hStatusLabel, "USB Device Inserted"); 
 
            BlockUSBStorage(); 
 
            string devName = GetUSBDeviceDescription(lParam); 
            string msg = "USB Device Detected:\n" + devName + "\n\nAllow access?"; 
 
            int result = MessageBoxA(hwnd, msg.c_str(), "USB Firewall", MB_ICONQUESTION | 
MB_YESNO); 
            if (result == IDYES) { 
                logfile << "[USER] Allowed USB: " << devName << endl; 
                UnblockUSBStorage(); 
            } else { 
                logfile << "[USER] Blocked USB: " << devName << endl; 
                BlockUSBStorage(); 
            } 
        } else if (wParam == DBT_DEVICEREMOVECOMPLETE) { 
            logfile << "[INFO] USB device removed." << endl; 
            SetWindowText(hStatusLabel, "USB Device Removed"); 
        } 
        break; 
 
    case WM_CLOSE: 
        logfile << "[INFO] Exiting USB Firewall." << endl; 
        DestroyWindow(hwnd); 
        break; 
 
    case WM_DESTROY: 
        PostQuitMessage(0); 
        break; 
    } 
    return DefWindowProc(hwnd, uMsg, wParam, lParam); 
} 
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, 
int nCmdShow) { 
    const char CLASS_NAME[] = "USBFirewallWindow"; 
 
    WNDCLASS wc = {}; 
    wc.lpfnWndProc = WindowProc; 
    wc.hInstance = hInstance; 
    wc.lpszClassName = CLASS_NAME; 
 
    RegisterClass(&wc); 
 
    HWND hwnd = CreateWindowEx( 
        0, 
        CLASS_NAME, 
        "USB Firewall", 
        WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 150, 
        NULL, 
        NULL, 
        hInstance, 
        NULL 
    ); 
 
    if (hwnd == NULL) { 
        logfile << "[ERROR] Failed to create window." << endl; 
        return 0; 
    } 
 
    ShowWindow(hwnd, nCmdShow); 
    UpdateWindow(hwnd); 
 
    MSG msg = {}; 
    logfile << "[STARTED] USB Firewall running with UI..." << endl; 
    while (GetMessage(&msg, NULL, 0, 0)) { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    } 
 
    logfile.close(); 
    return 0; 
} 