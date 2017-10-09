


// Windows Kernel versions look like 6.2.1234. We care about 6 and 2.

function WinMajor()
{
    arr = systemInfo.kernelVersion.split('.', 1)
    return parseInt(arr);
}

function WinMinor()
{
    arr = systemInfo.kernelVersion.split('.', 2)
    return parseInt(arr[1]);
}

function Component()
{
    // default constructor

    console.log("CPU Architecture: " +  systemInfo.currentCpuArchitecture);
    console.log("OS: " + systemInfo.productType);
    console.log("KernelType: " + systemInfo.kernelType);
    console.log("KernelVersion: " + systemInfo.kernelVersion);
    console.log("prettyProductName" + systemInfo.prettyProductName);

    console.log("Major: " + WinMajor() + " Minor: " + WinMinor());

    component.setValue("Virtual", "true"); // Disable this component by default

    // Ok, we only build the installer on Windows, but still...
    if ( systemInfo.kernelType == "winnt" ) {
        console.log("Win NT");

        /*
         * Windows versions:
         * 5.1	Windows XP
         * 5.2	Windows Server 2003
         * 6.0	Windows Vista, Windows Server 2008
         * 6.1	Windows 7, Windows Server 2008 R2
         * 6.2	Windows 8
         * 6.3	Windows 8.1
         * 10.0	Windows 10
         *
         * Source: http://www.geoffchappell.com/studies/windows/km/ntoskrnl/history/index.htm
         */

        /* We install the driver on XP and later, all the way up to Win 8. */
        if ( (WinMajor() == 5 && WinMinor() >= 1) || // At least Win XP
             (WinMajor() == 6 && WinMinor() < 2) ) { // Less than Win 8

             component.setValue("Virtual", "false");
             console.log("Driver component enabled");
        }

        /* Win 8 is not supported at all */
        if (WinMajor() == 6 && WinMinor() >= 2 ) {

            var result = QMessageBox.critical("quit.question", "Windows compatibility probelm",
                                              systemInfo.prettyProductName + " is not supported. \n" +
                                              "You can install the software anyway, but it's probably not going to work.\n" +
                                              "You can Use Windows XP, Vista, or Windows 10.",
                                              QMessageBox.Ignore | QMessageBox.Abort);
            if (result == QMessageBox.Abort) {

                installer.setValue("FinishedText", "Unfortunately, <b>Windows 8</b> and <b>Windows Server 2012</b> are not supported at the moment.<br>" +
                                   "Supported Windows versions:<ul>" +
                                   "<li>Windows XP</li>" +
                                   "<li>Windows Vista</li>" +
                                   "<li>Windows 7</li>" +
                                   "<li>Windows 10</li>" +
                                   "<li>Windows Server 2003 (including R2)</li>" +
                                   "<li>Windows Server 2008 (including R2)</li>" +
                                   "<li>Windows Server 2016</li>" +
                                   "</ul>");
                installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
                installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
                installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
                installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
                installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
                installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
                gui.clickButton(buttons.NextButton);
            }

        }


        console.log("PATH = "+ installer.environmentVariable("PATH"));

        /* Win 10 does not require the driver */


    }
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    console.log("Archives: " + component.archives);

    // No way we are not on Windows, but just for good style
    if ( systemInfo.kernelType == "winnt" ) {

        windir = installer.environmentVariable("windir");
        console.log("WINDIR: " + windir);

        if (windir == "") {
            //Just in case.
            console.log("Undefined WINDIR? Assuming c:\\windows\\");
            windir = "c:\\windows";
        }

        if (WinMajor() == 5 && WinMinor() >= 1) {
            // XP or Server 2003. It's not trivial, or at least I don't have time
            // to figure it out yet. Just tell the user to install the drivers
            // manually.

            QMessageBox.information("winxp.information", systemInfo.prettyProductName + " information",
                                    "On  " + systemInfo.prettyProductName + ", you need to install the DI-Lambda driver manually.<br>" +
                                    "Please see <a href=http://koi.science/downloads/DI-Lambda-driver-installation-WinXP.pdf>instructions</a>" +
                                    " for details", QMessageBox.OK);


        }

        if (WinMajor() == 6 && WinMinor() < 2) {
            // Vista, Win7, or Server 2008. Use the PnPUtil.
            // Sysnative is used to access system32 without a redirect, see
            // https://msdn.microsoft.com/en-us/library/windows/desktop/aa384187.aspx
            pnputil = windir + "\\sysnative\\pnputil.exe";

            console.log("pnputil = " + pnputil);

            // It would be nice if there was a simple way to uninstall the driver too. But there is not, so
            // we leave the driver behind after uninstall. No big deal, and it's possible to remove it manuylly
            // through the control panel.

            // pnputil returns 259 on partial success, when it could not find a device for the driver immediatelly.
            component.addElevatedOperation("Execute", "{0,259}", pnputil, "-i", "-a", "@TargetDir@\\di-lambda.inf");
        }

    }
}
