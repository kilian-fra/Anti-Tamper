# Anti-Tamper
Anti-Tamper tool for protecting windows pe files by using obfuscation and various other techniques

This is a private project designed to protect native (C, C++, ...) applications from reverse engineering and cracking.
The project is still ongoing. Currently there is only code virtualization as obfuscation technique implemented which is also not fully implemented.

The project consist of the follwing components:

Loader:
- The loader is embedded by the builder in the target pe file to hold for example the virtual machine that is being used to execute the protected code.
- It also does certain runtime checks like anti-debugging to prevent the protected file from being inspected dynmaically (not implemented yet)

BuilderLib:
- This is a dynamic link library that is being used by the builder gui
- Responsible for applying code obfuscation on x64 machine code level
- It virtualizes the code by converting the existing x64-instructions to virtual machine instructions (defined by a custom vm-architecture), so that existing reverse engieering tools like IDA Pro for x64dbg are not able to disassemble the protected code

BuilderUI
- A gui that interacts with the BuilderLib
- The pe file to be protected can be selected and different protection options can be configured by the user
- Not implemented yet
