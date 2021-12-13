#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <algorithm>
#include <regex>
#include <cstdlib>
#include <ctime>
#include <sstream>

class Account { public:
	std::string name;
	std::string account;
	std::string date;
	int accountNumber;

	// Overload comparison to allow sets to sort.
	friend bool operator< (const Account& acc1, const Account& acc2);
};

bool operator< (const Account& acc1, const Account& acc2) {
	return acc1.accountNumber < acc2.accountNumber;
}

class BankCLI {
	private:
	std::set<Account> accounts;
	Account GenerateSearchAccountNumber(std::string account) {
		// Declare a new account.
		Account retAcc;

		// Set the account string then clean it of whitespace, then convert it into an int.
		retAcc.account = account;
		account.erase(std::remove(account.begin(), account.end(), ' '), account.end());
		retAcc.accountNumber = std::atoi(account.c_str());

		return retAcc;
	}

	Account GenerateSearchAccountName(std::string name) {
		// Declare an account to build and return.
		Account retAcc;

		// Set the name and return.
		retAcc.name = name;
		return retAcc;
	}

	public:
	bool NewAccount(std::string name, std::string account) {
		// Pass the string account to verify it meets the required parameters (i.e. "### ## ####" with only numbers and
		// whitespace characters).
		if (FormatMatch(account)) {
			// Declare a new account.
			Account newAcc;

			// Set the variables
			newAcc.name = name;
			newAcc.account = account;
			newAcc.date = GenerateDate();

			// Create an integer representation of the account's number for sorting into a set to allow high efficiency
			// with reading (finding).
			account.erase(std::remove(account.begin(), account.end(), ' '), account.end());
			newAcc.accountNumber = std::atoi(account.c_str());

			// Attempt to add a new account, tracking the returned pair if a new element was made or if it was already found.
			if (accounts.insert(newAcc).second) {
				return true;
			}
			else {
				std::cout << "Account already exists." << '\n';
				return false;
			}
		}
		else {
			return false;
		}
	}

	bool FormatMatch(std::string account) {
		// Check account doesn't break length restrictions.
		if (account.size() == 11) {
			// Create a RegEx to match the format of '### ## ####'.
			std::regex expression("[0-9][0-9][0-9]\\s[0-9][0-9]\\s[0-9][0-9][0-9][0-9]");
			if (regex_match (account, expression)) {
				return true;
			} // Print an error and return false.
			else {
				std::cout << "Invalid account number- must only contain digits and spaces (### ## ####)." << '\n';
				return false;
			}
		} // Print an error and return false.
		else {
			std::cout << "Invalid account number- must be 11 digits long (### ## ####)." << '\n';
			return false;
		}
	}

	void DisplayAccount(std::string account) {
		// Check if search key is a valid account number.
		if (FormatMatch(account)) {
			// Generate an empty account to use to search the set.
			Account searchAcc = GenerateSearchAccountNumber(account);
			auto userAcc = accounts.find(searchAcc);

			// Check if the account was found.
			if (userAcc != accounts.end()) {
				// Display account information.
				std::cout << "Customer Name: " << userAcc->name << '\n';
				std::cout << "SSN: ### ## " << userAcc->account.substr(7, 4) << '\n';
				std::cout << "Date Opened: " << userAcc->date << '\n';
			}
			else {
				std::cout << "Account with that SSN not found." << '\n';
			}
		}
		else {
			std::cout << "Invalid SSN." << '\n';
		}
	}

	void DisplayAccounts() {
		std::set<Account>::iterator it = accounts.begin();
		while (it != accounts.end()) {
			DisplayAccount(it->account);
			it++;
		}
	}

	void CloseAccount(std::string account) {
		if (FormatMatch(account)) {
			auto delAcc = accounts.find(GenerateSearchAccountNumber(account));
			if (delAcc != accounts.end()) {
				std::cout << "Are you sure you want to delete account " << account << " belonging to " << delAcc->name << "? (Y or N)" << '\n';
				std::string userInput;

				bool breakLoop = false;
				while (true) {
					std::cin >> userInput;
					if (userInput.compare("Y") == 0) {
						accounts.erase(delAcc);
						std::cout << "Account deleted." << '\n';
						break;
					}
					else if (userInput.compare("N") == 0) {
						std::cout << "Deletion aborted." << '\n';
						break;
					}
					else {
						std::cout << "Invalid entry: Enter Y to delete, N to cancel." << '\n';
					}
				}
			}
			else {
				std::cout << "Account " << account << " not found." << '\n';
			}
		}
		else {
			std::cout << "Invalid account number- must contain only digits and spaces (### ## ####)." << '\n';
		}
	}

	std::string GenerateDate() {
		// Generate a time object for extracting time data from.
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		// Create a stringstream to put the time data into, then convert the time data into a string-friendly format.
		std::ostringstream oss;
		oss << std::put_time(&tm, "%m-%d-%y");

		// Create a string to return the date information.
		std::string timeString = oss.str();

		return timeString;
	}
};

int main() {
	BankCLI bank;

	std::cout << "---------------" << '\n';
	bank.NewAccount("John Smith", "123 45 6789");
	bank.NewAccount("Jane Doe", "098 76 5432");

	bank.DisplayAccounts();
	std::cout << "---------------" << '\n';

	bank.DisplayAccount("123 45 6789");

	std::cout << "---------------" << '\n';
	bank.CloseAccount("123 45 6789");

	std::cout << "---------------" << '\n';
	bank.DisplayAccounts();

	std::cout << "---------------" << '\n';
	bank.NewAccount("Jane Doe", "098 76 5432");

	std::cout << "---------------" << '\n';
	bank.DisplayAccounts();
	return 0;
}

/*
Implemented Functions:
- Show accounts
- Display account
- New account
- Close account

Unimplemented Functions:
- Password system
- Menu system (and thus quit)
- Search name
*/
