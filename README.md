# Welcome to my WinDev2021 Final Project Repository!

## About Project:

This is a programming exercise written using only  <br />
the Win32 SDK and the design patterns from Microsoft's  <br />
Component Object Model (COM).  <br />

Syntax highlighting, Intellisence and the Text Editor  <br />
in Visual Studio 2019 were the only features of the  <br />
IDE utilized.  <br />

Everything including the GUI is typed by hand. All  <br />
files were built using only command line utilities.  <br />

Development began on 20-09-2021 and lasted until  <br />
12-03-2022.  <br />

Developer:   Kaivalya Vishwakumar Deshpande  <br />
Nationality: Indian  <br />
Email: kaivalya.v.deshpande@gmail.com  <br />

# How To Install:

Pre-requisites:
a. You have Visual Studio Command Line Utilities and .NET installed
b. Your `Windows/` directory exists on the `C:/` drive

1. Open an Administrative `x64 Native Tools Command Prompt` Instance

2. Build and Install the COM Server by running the following commands
   (`project_dir` is the path to this project's directory):
   
   	```Batchfile
	
   	> cd project_dir\Server
   	> BuildInstallServer.bat  // You would be prompted 2 message boxes after this command, keep clicking OK
   	```

   COM Server is now Built and Installed (Do Not Close the Command Prompt Instance yet)

3. Building and Installing the Client by running the following commands: 
	
	```Batchfile
	
	> cd ..\Client
	> BuildInstallClient.bat
	```

   Client has now been Built and Installed.

# How To Run:

Do either of the following:

1. Navigate to `project_dir` in the file explorer and double-click on `AMCMathServices.exe`; or

2. Navigate to `project_dir` in a command prompt instance, and run the following command:

	```Batchfile
	
	> AMCMathServices.exe
	```

# How To Uninstall:

1. Open an Administrative `x64 Native Tools Command Prompt` Instance

2. Uninstall the Client by running the following commands
   (`project_dir` is the path to this project's directory):

	```Batchfile
	
	> cd project_dir\Client
	> UninstallClient.bat
	```

   Client has now been Uninstalled (Do Not Close the Command Prompt Instance yet)

3. Uninstall the COM Server by running the following commands:
   	
	```Batchfile
	
	> cd ..\Server
	> UninstallServer.bat  // You would be prompted 2 message boxes after this command, keep clicking OK
	```

   COM Server has now been Uninstalled.

## Previews

![Splash Screen](Previews/01_Splash.png)
What you see when you run the Application

![Spot Validation](Previews/02_SpotValidation.png)
All errors are validated on the spot

![Negative Square Roots](Previews/04_NegativeSquareRoots.png)
Non-Real Numbers or NaNs are Not Supported by the Server

![Results](Preview/05_Results.png)
The Results Dialog

![About](Preview/06_About.png)
The About Dialog

![Log](Preview/07_Log.png)
The Splash Window can become a Display for the User Log

![Retirement](Preview/08_UponRetirement.png)
Upon Successful Retirement

![Close](Preview/09_Close.png)
What you see when you close the Application

# Enjoy!
