# 🛡️ USB Firewall --- Windows USB Intrusion Guard

A real-time USB storage access-control firewall for Windows endpoints,
built in C++ using native registry and service-level enforcement to
block unauthorized removable-drive attacks and maintain structured audit
logs for forensic analysis.

------------------------------------------------------------------------

## 🚔 Problem Overview

USB storage devices are widely misused for delivering endpoint threats
such as:

-   **BadUSB exploits**\
-   **USB worm attacks**\
-   **Ransomware via removable drives**\
-   **Insider exfiltration using USB storage**\
-   **Lack of true plug-in interception** in traditional AV/firewall
    tools

------------------------------------------------------------------------

## ✅ Solution Provided

This USB Firewall enforces:

-   🚫 **Instant USBSTOR driver & service lockdown**\
-   🔍 **User-based authorization prompt (real-time whitelist)**\
-   📜 **Structured text logging for SOC/forensic reviews**\
-   🖥️ **Manual Block / Unblock controls via Win32 UI**\
-   ⛔ **Does NOT block input USB devices** (keyboard, mouse, etc.)

------------------------------------------------------------------------

## ⚡ Features

-   Real-time USB insert/remove detection\
-   Automatic modification of USBSTOR registry key:
    -   `Start = 4` → Disable\
    -   `Start = 3` → Enable\
-   USB service control via `sc.exe`\
-   Authorization popup: **Allow or Deny device access**\
-   Persistent logging to `usb_log.txt` for audits\
-   Clean Win32 GUI\
-   No external libraries required

------------------------------------------------------------------------

## 🏗️ Architecture

    USB Firewall Engine
    ├─ USB Event Listener (WM_DEVICECHANGE)
    ├─ USB Blocker (Registry + Service Control)
    ├─ User Authorization Prompt
    └─ Audit Logger (usb_log.txt)

------------------------------------------------------------------------

## 📂 Project Structure

    USB Firewall/
    │── USB_Firewall.cpp      # Main firewall source code
    │── usb_log.txt           # Generated audit logs
    └── README.md             # Project documentation

------------------------------------------------------------------------

## 🛠️ Build & Run

### 📦 Compile with GCC (MinGW)

``` bash
g++ USB_Firewall.cpp -o USBFirewall -lsetupapi
```

### 🖥️ Compile using Dev-C++

1.  Open project\
2.  Press **F11** to compile & run

### ▶️ Running the Firewall

``` bash
USBFirewall.exe
```

⚠️ **Must be run with Administrator privileges**\
Required to modify registry values and start/stop Windows services.

------------------------------------------------------------------------

## 🖥️ UI Controls

  -----------------------------------------------------------------------
  Action       Function
  ------------ ----------------------------------------------------------
  **Block      Disables USBSTOR (registry + service stop)
  USB**        

  **Unblock    Restores driver/service access
  USB**        

  **Exit**     Closes firewall and writes logs
  -----------------------------------------------------------------------

------------------------------------------------------------------------

## 🔐 Security Principles Followed

-   **Zero-trust** for USB storage by default\
-   Driver/service-level reaction on plug-in\
-   Manual user authorization before access\
-   Local audit logs for forensic review\
-   Requires administrative execution

------------------------------------------------------------------------

## 🔥 Future Enhancements

-   Hardware-ID based USB whitelist file\
-   Hide blocked USB devices from File Explorer\
-   Auto malware scan before allowing access\
-   User authentication before granting permission\
-   Central dashboard for USB activity monitoring\
-   Detection of suspicious repeated plug-in attempts

------------------------------------------------------------------------

## 🤝 Contributing

You can help improve this endpoint-security tool:

1.  Fork the repository\
2.  Create a new feature branch\
3.  Test thoroughly on Windows (Admin mode)\
4.  Commit and submit a Pull Request

------------------------------------------------------------------------

## ⚠️ System Requirements

-   **OS:** Windows\
-   **Language:** C++\
-   **Privileges:** Administrator Access Required\
-   **Targets:** USB Storage devices only

------------------------------------------------------------------------

## 📌 Disclaimer

This project is intended strictly for **ethical cybersecurity learning
and endpoint-protection research**.\
Any usage for malicious, harmful, or unauthorized activity is strongly
discouraged and is **not the responsibility of the developer**.
