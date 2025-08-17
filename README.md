# 📡 ESP32 Evil_Service

![Banner](img/banner.png)

## 🚀 Getting Started

A **captive portal project** running on an ESP32 board that simulates a **router firmware update**. Once connected to the Wi-Fi network named `Evil_Service`, navigate to `http://172.0.0.1/creds`. This page also includes a powerful feature: the ability to change the network's SSID (the fake portal's name) on the fly.

Users are redirected to a page prompting them to "update their router." When a password is entered, it gets saved locally.

The project contains files in two language versions **Polish** 🇵🇱 && **English** 🇬🇧

UPDATE: CompoPortal.ino (now You can choose portal style version)
---

---

## 🔥 Update
•	📄 Added file: Combined_CaptivePortal.ino
•	🌐 SocialMedia Portal: new option with social media login style
•   🎨 custom Portal: now you can choose the look of your portal directly from the /creeds tab

---

## ⚙️ How It Works

1. The **ESP32** launches a Wi-Fi Access Point named `Evil_Service`.
2. Devices that connect to the network are automatically redirected to a page simulating a router firmware update panel.
3. The unsuspecting user enters their Wi-Fi password, believing it to be legitimate.
4. The entered password is logged internally on the device.
5. The page at `http://172.0.0.1/creds` displays:
   - 📥 **Credential viewer** — collected passwords.
   - ✏️ **Change AP name (SSID)** — directly from the web interface without reflashing the device.

---

## 📸 Screenshots
ComboPortal.ino
| /creds | Social Media Portal |
|-----------|----------------|
| ![](img/4.jpg) | ![](img/5.jpg) |

Router_update_ESP32_PL.ino (PL🇵🇱 version)
| Main Page | Creds | Loading |
|-----------|----------------|--------------|
| ![](img/1.jpg) | ![](img/2.jpg) | ![](img/3.jpg) |

---

## 🛠️ Requirements

- Board: **ESP32**
 `Tools > Board > Boards Manager > esp32 by Espressif Systems`
- IDE: Arduino IDE
- Libraries: standard DNSServer, WiFi, WebServer, LittleFS

---

## 🫱🏻‍🫲🏾 Thanks

- Big thanks to my `Laboratorium` boyz [Janek](https://github.com/Janek79ax) && [Lazy](https://github.com/Nigdzie). 
- First inspiration: https://github.com/adamff-dev/ESP8266_WiFi_Captive_Portal_2.0

---

## ⚠️ Disclaimer

> 🛑 **This project is for educational and testing purposes only.**  
> Unauthorized use may be illegal.  
> The author is not responsible for any misuse or damages caused by this software.

---

## 🐲 Author

**/D3h420/**  
GitHub: [D3h420](https://github.com/D3h420)

---
