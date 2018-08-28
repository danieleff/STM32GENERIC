# Arduino tools menu

The following options can be selected in the Arduino tools menu, below the selected board:

### Upload method

See the [uploading](upload) page.

### USB

The following options are available for USB

* Serial. A Virtual COM port will be available when connected. Note: On Windows 7, you will need to install drivers located in tool/win/drivers folder.
* Disabled, no USB. Note: If you use USB based uploading method, the board will not reset automatically.

### Default serial communication selection

Select the default for `Serial`:

Automatically selected:

* If the board is Nucleo, automatic Serial is aliased to [SerialUART2](Serial UART) (ST-Link Virtual COM port).
* If the upload method is based on UART (Serial upload, ST-Link), automatic Serial is aliased to [SerialUART1](Serial UART).
* If the upload method is based on USB (DFU), automatic Serial aliased to [SerialUSB](Serial USB).

SerialUSB

SerialUART1
