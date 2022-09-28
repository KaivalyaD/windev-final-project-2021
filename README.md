Welcome to my WinDev2021 Final Project Repository!

================================================================================================================
About Project:

This is a programming exercise written using only
the Win32 SDK and the design patterns from Microsoft's
Component Object Model (COM).

Syntax highlighting, Intellisence and the Text Editor
in Visual Studio 2019 were the only features of the
IDE utilized.

Everything including the GUI is typed by hand. All
files were built using only command line utilities.

Development began on 20-09-2021 and lasted until
12-03-2022.

Developer:   Kaivalya Vishwakumar Deshpande
Nationality: Indian
Email: kaivalya.v.deshpande@gmail.com

================================================================================================================
How To Install:

Pre-requisites:
a. You have Visual Studio Command Line Utilities and .NET installed
b. Your Windows/ directory exists on the C:/ drive

1. Open an Administrative x64 Native Tools Command Prompt Instance

2. Build and Install the COM Server by running the following commands
   (project_dir is the path to this project's directory):
   
   	   > cd project_dir\Server
   	   > BuildInstallServer.bat  // You would be prompted 2 message boxes after this command, keep clicking OK

COM Server is now Built and Installed (Do Not Close the Command Prompt Instance yet)

3. Building and Installing the Client by running the following commands:

	> cd ..\Client
	> BuildInstallClient.bat

Client has now been Built and Installed.

================================================================================================================
How To Run:

Do either of the following:

1. Navigate to project_dir in the file explorer and double-click on AMCMathServices.exe; or

2. Navigate to project_dir in a command prompt instance, and run the following command:

	> AMCMathServices.exe

================================================================================================================
How To Uninstall:

1. Open an Administrative x64 Native Tools Command Prompt Instance

2. Uninstall the Client by running the following commands
   (project_dir is the path to this project's directory):

	> cd project_dir\Client
	> UninstallClient.bat

Client has now been Uninstalled (Do Not Close the Command Prompt Instance yet)

3. Uninstall the COM Server by running the following commands
   
   	   > cd ..\Server
   	   > UninstallServer.bat  // You would be prompted 2 message boxes after this command, keep clicking OK

COM Server has now been Uninstalled.

================================================================================================================
Previews

![Splash Screen](Previews/01_Splash.png)
The Splash Screen

![Spot Validation](Previews/02_SpotValidation.png)
All errors are validated on the spot

![Registration](Previews/03_UponRegistration.png)
On Successful Registration

================================================================================================================
- Enjoy!
