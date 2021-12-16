#include <iostream>
#include <iomanip>
#include <regex>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <algorithm>
#include <iterator>
#include <set>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

class Account { public:
	std::string name;
	std::string account;
	std::string date;
	std::string password;
	int accountNumber;

	// Privelege: 0 = user, 1 = admin.
	int privilege : 1;

	// Overload less-than comparison to allow sets to sort.
	friend bool operator< (const Account& acc1, const Account& acc2);
};

bool operator< (const Account& acc1, const Account& acc2) {
	return acc1.accountNumber < acc2.accountNumber;
}

class BankCLI {
	private:
	std::set<Account> accounts;
	Account activeUser;
	Account inactiveUser;

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
	BankCLI() {
		Account admin;
		admin.name = "admin";
		admin.password = "default";
		admin.account = "0";
		admin.accountNumber = 0;
		admin.privilege = 1;

		inactiveUser.name = "inactive";
		inactiveUser.password = "";
		inactiveUser.account = "-1";
		inactiveUser.accountNumber = -1;
		inactiveUser.privilege = 0;

		activeUser = inactiveUser;
	}

	bool NewAccount(std::string name, std::string password, std::string account) {
		// Pass the string account to verify it meets the required parameters (i.e. "### ## ####" with only numbers and
		// whitespace characters).
		if (FormatMatch(account)) {
			// Declare a new account.
			Account newAcc;

			// Set the variables
			newAcc.name = name;
			newAcc.account = account;
			newAcc.date = GenerateDate();
			newAcc.privilege = 0;
			newAcc.password = password;

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

	bool DisplayAccount(std::string account) {
		// Check if search key is a valid account number.
		if (FormatMatch(account)) {
			// Generate an empty account to use to search the set.
			Account searchAcc = GenerateSearchAccountNumber(account);
			auto userAcc = accounts.find(searchAcc);

			// Check if the account was found.
			if (userAcc != accounts.end()) {
				// Display account information.
				std::cout << "Accountholder Name: " << userAcc->name << '\n';
				std::cout << "SSN: ### ## " << userAcc->account.substr(7, 4) << '\n';
				std::cout << "Date Opened: " << userAcc->date << '\n';
				return true;
			}
			else {
				std::cout << "Account with that SSN not found." << '\n';
				return false;
			}
		}
		else {
			std::cout << "Invalid SSN." << '\n';
			return false;
		}
	}

	void DisplayAccounts() {
		// Iterate through each Account in the accounts set, calling the DisplayAccount function to output their info.
		std::set<Account>::iterator it = accounts.begin();
		while (it != accounts.end()) {
			DisplayAccount(it->account);
			it++;
		}
	}

	void SearchName(std::string name) {
		std::set<Account>::iterator it = accounts.begin();
		typedef std::tuple<float, std::string, Account> pqAcc;
		std::priority_queue<pqAcc, std::vector<pqAcc>, std::greater<pqAcc>> accOIs;

		// Iterate through each account and check for similarity, storing ones with a certain limit of dissimilarity (currently 30%).
		while (it != accounts.end()) {
			float dissimilarity = LevenshteinDistance(it->name, name) / it->name.length();
			if (dissimilarity < 0.3) {
				accOIs.push(std::make_tuple(dissimilarity, it->name, *it));
			}
			it++;
		}

		// Declare an array for user selection of which account to view.
		Account selection[accOIs.size()];
		int arr = 0;

		// Iterate through each stored account, printing and storing them in printed order.
		while (!accOIs.empty()) {
			std::cout << arr + 1 << ". Accountholder Name: " << std::get<1>(accOIs.top()) << '\n';
			selection[arr] = std::get<2>(accOIs.top());
			accOIs.pop();
			arr++;
		}

		int input;
		bool inputInvalid = true;
		while (inputInvalid) {
			std::cin.clear();
			std::cout << "Enter Selection to View: ";
			std::cin >> input;
			if (std::cin.fail() || input > arr || input < 1) {
				std::cout << "Invalid input, enter a number listed!" << '\n';
				std::numeric_limits<std::streamsize>::max();
				continue;
			}
			inputInvalid = false;
		}
		DisplayAccount(selection[input - 1].account);
	}

	/* To-do: Play with iterators and find way of tracking account object making use of accounts' find() function.
	Account GetAccount(std::string account) {
		auto = accounts.find(GenerateSearchAccountNumber(account));
	}
	*/

	bool CloseAccount(std::string account) {
		if (FormatMatch(account)) {
			auto delAcc = accounts.find(GenerateSearchAccountNumber(account));
			if (delAcc != accounts.end()) {
				std::cout << "Are you sure you want to delete account " << account << " belonging to " << delAcc->name << "? (Y or N)" << '\n';
				std::string userInput;

				while (true) {
					std::cin >> userInput;
					if (userInput.compare("Y") == 0) {
						if (accounts.erase(delAcc) != accounts.end()) {
							std::cout << "Account deleted." << '\n';
							return true;
						}
						else {
							std::cout << "Potential error in deletion. Verify set elements." << '\n';
							return false;
						}
					}
					else if (userInput.compare("N") == 0) {
						std::cout << "Deletion aborted." << '\n';
						return false;
					}
					else {
						std::cout << "Invalid entry: Enter Y to delete, N to cancel." << '\n';
						return false;
					}
				}
			}
			else {
				std::cout << "Account " << account << " not found." << '\n';
				return false;
			}
		}
		else {
			std::cout << "Invalid account number- must contain only digits and spaces (### ## ####)." << '\n';
			return false;
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

	// Fuzzy string checking
	int LevenshteinDistance(std::string str1, std::string str2) {
		int strLenA = str1.length();
		int strLenB = str2.length();

		if (strLenA == 0) {
			return strLenB;
		}
		if (strLenB == 0) {
			return strLenA;
		}

		int cost;
		if (str1[strLenA - 1] == str2[strLenB - 1]) {
			cost = 0;
		}
		else {
			cost = 1;
		}

		return std::min(std::min(LevenshteinDistance(str1.substr(0, strLenA - 1), str2) + 1, LevenshteinDistance(str1, str2.substr(0, strLenB - 1)) + 1),
					LevenshteinDistance(str1.substr(0, strLenA - 1), str2.substr(0, strLenB - 1)) + cost);
	}

	/* To-do: Implement tracking for current user. Password encrpytion.
	bool LogOut() {
		std::cout << activeUser.accountNumber;
		return true;
	}

	// To-do: Password encrpytion.
	bool LogIn(std::string name, std::string password) {
		return true;
	}
	*/
};

void Line() {
	std::cout << "---------------" << '\n';
}

int main() {
	BankCLI bank;
	bool run = true;
	unsigned char state;

	state = 0;

	std::string input;
	while (run) {
		input = " ";

		switch (state) {
		// Login: username
		case 0:
			Line();
			std::cout << "Enter login username: ";
			std::cin >> input;

			if (input.compare("admin") == 0) {
				state = 1;
			}
			break;

		// Login: password
		case 1:
			Line();
			std::cout << "Enter login password: ";
			std::cin >> input;

			if (input.compare("default") == 0) {
				state = 2;
				std::cout << "Login successful!" << '\n';
			}
			break;

		// Using
		case 2:
			Line();
			std::cout << "Enter command (type 'help' for a list of commands): ";
			std::getline(std::cin, input);

			if (input.compare("help") == 0) {
				std::cout << "show accounts - List all accounts" << '\n';
				std::cout << "display account - Search a specific account's information" << '\n';
				std::cout << "search name - Search all accounts for a specific name" << '\n';
				std::cout << "new account - Create a new account" << '\n';
				std::cout << "close account - Close an existing account" << '\n';
				std::cout << "quit - Exit the Bank CLI" << '\n';
			}
			else if (input.compare("show accounts") == 0) {
				bank.DisplayAccounts();
			}
			else if (input.compare("display account") == 0) {
				bool cont = true;
				while (cont) {
					std::cout << "Enter account SSN to search (### ## ####) or cancel to exit: ";
					std::getline(std::cin, input);

					if (input.compare("cancel") == 0) {
						std::cout << "Cancelling account search." << '\n';
						break;
					}
					cont = !(bank.DisplayAccount(input));
				}
			}
			else if (input.compare("search name") == 0) {
				std::string name;
				std::cout << "Enter name to search: " << '\n';
				std::getline(std::cin, name);

				bank.SearchName(name);
			}
			else if (input.compare("new account") == 0) {
				bool cont = true;

				std::string name;
				std::string account;
				std::string password;
				while (cont) {
					std::cout << "Follow the prompts to create a new account, or, enter cancel to exit." << '\n';
					std::cout << "Enter the accountholder's name: ";
					std::getline(std::cin, name);
					if (name.compare("cancel") == 0) { std::cout << "Cancelling account creation." << '\n'; break; }
					std::cout << "Enter the accountholder's SSN: ";
					std::getline(std::cin, account);
					if (name.compare("cancel") == 0) { std::cout << "Cancelling account creation." << '\n'; break; }
					std::cout << "Enter the accountholder's password: ";
					std::getline(std::cin, password);
					cont = !(bank.NewAccount(name, password, account));
				}
			}
			else if (input.compare("close account") == 0) {
				bool cont = true;

				std::string account;
				while (cont) {
					std::cout << "Enter the SSN of the to-be-deleted account (### ## ####) or cancel to exit: ";
					std::getline(std::cin, input);

					if (input.compare("cancel") == 0) {
						std::cout << "Cancelling account closure." << '\n';
						break;
					}
					cont = !(bank.CloseAccount(input));
				}
			}
			else if (input.compare("quit") == 0) {
				std::cout << "Exiting..." << '\n';
				state = 3;
			}
			break;

		case 3:
			run = false;
			break;
		}
	}

	/*
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
	bank.LogOut();
	std::cout << '\n';
	bank.DisplayAccounts();
	*/

	return 0;
}

/*
Implemented Functions:
- Show accounts
- Display account
- New account
- Close account

Unimplemented Functions:
- Test cases
- Password system
- Menu system (and thus quit)
- Search name
*/
