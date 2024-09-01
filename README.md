Account Management System


Overview


This project is a Qt-based application for managing accounts. The application provides functionalities to add, update, delete, search by name, and search by number of accounts. The project is intended to demonstrate the use of Qt for building GUI applications with functionalities typically required in account management systems.

Features


Add Account: Add a new account with details such as name, account number, etc.

Update Account: Update the details of an existing account.

Delete Account: Remove an account from the system.

Search by Name: Find an account using the account holder's name.

Search by Number: Find an account using the account number.

Admin Login: Secure login for admins to manage the account data.

Prerequisites

Before running this project, make sure you have the following installed:

Qt Framework: Version 5.15 or later.
C++ Compiler: Compatible with the Qt version.
CMake (optional): If using CMake for project build.
Installation

Copy code
git clone https://github.com/Lily0335/account-management-qt.git
cd account-management-qt

Open the Project:

Open the .pro file using Qt Creator or any other compatible IDE.
Build the Project:

In Qt Creator, click on the "Build" button or use Ctrl+B.
Run the Application:

After a successful build, click on the "Run" button or use Ctrl+R.
Usage
Login:

Start the application and log in using the admin credentials (if implemented).
Manage Accounts:

Use the main panel to add, update, delete, or search for accounts.
Data is saved to a file (if file saving is implemented), ensuring persistence between sessions.
File Operations:

Accounts data can be loaded from and saved to a file, allowing for persistent data storage.
File Structure
main.cpp: Entry point of the application.
MainPanel.cpp/.h: Implements the main panel with account management features.
AdminLogin.cpp/.h: Handles the admin login functionality.
Person.cpp/.h: Represents a person with basic attributes.
Account.cpp/.h: Represents an account with attributes such as account number.
SavingsAccount.cpp/.h: Inherits from Account, represents a savings account.
data.txt (or any other format): File for storing account data.
Future Enhancements
Implement user roles with different permissions.
Add validation for user inputs.
Enhance the UI with additional features and better usability.
Add unit tests for key functionalities.
Contributing
Feel free to fork this repository and contribute via pull requests. Any help to improve the project is welcome!

License
This project is licensed under the MIT License - see the LICENSE file for details.

Contact
If you have any questions or feedback, feel free to contact me at [laraib.r0205@gmail.com].

