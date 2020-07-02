# RpiControl

*[Qt 5.3.1]*

Controls **Kodi for Raspberry Pi** remotely with buttons or sending direct commands.  
Initially developed for custom Kodi menus that can be navigated with the remote control **numeric buttons**.  
Uses **Putty**'s **plink** tool to connect remotely to a device.

## How to use
- fill in the `Host`, `Password` and `Kodi type` (**'kodi'** or **'xbmc'**) inputs
- press **'Get Screen'** to view a Kodi screenshot
- fill in the **'Command'** input and press `Enter` to execute a custom command (like `'ls /storage'` or `'cat /proc/cpuinfo'`)

## Notes
- [**Putty**](https://putty.org) has to be installed and its directory should be in the **PATH** environment variable
- Before accessing a Raspberry from the application, it is required to connect to it from the command line first executing the `plink [IP]` command and adding the **host key** to the **Putty** cache (answer **'y'** to the **'Store key in cache? (y/n)'**). At this moment the application cannot intercept the questions of type **'y/n'**. This action is required only once, the consecutive connections from the application will not ask about the **host key** storage
