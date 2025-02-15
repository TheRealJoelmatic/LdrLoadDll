
# DLL Redirection PoC

This project is a **Proof of Concept (PoC)** to demonstrate a technique for **manually mapping a DLL** into a target process and then loading another DLL in a way that appears legitimate.

# why?
I kinda got board and had an idea

## 📌 Features

- **Call Stack Spoofing**: Uses [CallStack-Spoofer](https://github.com/Barracudach/CallStack-Spoofer) to hide function calls.
- **LdrLoadDll Hooking**: calls LdrLoadDll to load kernel32.dll and intercepts it to redirect module loading.
- **Stealth Techniques**: Avoids direct WinAPI calls.

## ⚙️ How It Works

1. The **manual mapped DLL** is injected into the target process.
2. The **LdrLoadDll** function is hooked with a midhook using `SafetyHook`.
3. The hook **intercepts DLL load requests** and redirects them to a specified DLL (`Internal.dll`).
4. All hooks are **removed after**.

## 🔧 Setup & Usage

### Prerequisites
- Windows 10/11 (64-bit)
- Visual Studio with C++ support

### Compilation
1. Clone the repository:
   ```sh
   git clone https://github.com/TheRealJoelmatic/LdrLoadDll
   cd LdrLoadDll
   ```
2. Open the project in **Visual Studio**.
3. Compile in **x64 Release Mode**.
4. Ensure `Internal.dll` exists at the specified path.

### Injection
Use your preferred manual mapping injector to inject the compiled DLL into the target process.
An example  of a manlue mapper is [Simple-Manual-Map-Injector](https://github.com/TheCruZ/Simple-Manual-Map-Injector)

## 🛠️ Code Structure

```
📂 InternalLoader
├── 📂 ext
│   │   ├── 📂 callstack
│   │   ├── 📂 stafyhook
├── 📂 src
│   ├── 📂 hooks
│   │   ├── hooks.cpp
│   │   ├── hooks.hpp
│   ├── 📂 utils
│   │   ├── manlueModules.cpp
│   │   ├── manlueModules.hpp
│   ├── dllmain.cpp

📂 Internal
│   dllmain.cpp
```

## 🚀 Credits / resources
- **Call Stack Spoofing**: [CallStack-Spoofer](https://github.com/Barracudach/CallStack-Spoofer)
- **Hooking Library**: [SafetyHook](https://github.com/stevemk14ebr/SafetyHook)
- **Developed by**: [Joelmatic](https://github.com/TheRealJoelmatic)
- **LdrLoadDll**: [API Reference](http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FExecutable%20Images%2FLdrLoadDll.html)

## ⚠️ Disclaimer
This project is for **educational purposes only**. Misuse of this code in a malicious manner is strictly discouraged. The author holds no responsibility for any damages caused by improper use.

---

📢 **For research and learning purposes only!**

