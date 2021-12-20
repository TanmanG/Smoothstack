
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
#include <fstream>
#include "bankdata.pb.h"


/*
To-Do (Must):
Implement add transaction <- Specifically flesh out what to use, refactor and bugtest implementation
Implement security (encrpytion to gain access) <- Encryption on all stored data preferred, encryption on sensistive minimum
Implement data persistence (Using google protobuf) <- Bugtest and finish refactoring existing code
Implement protobuf serialization/deserialization <- Same as above
Implement activity logging (log every function performed/attempted) <- Implement Event class
Implement/Find Date class <- Implement into each class, replacing current "date" string

To-Do (Preferred)
Implement proper unique ID system (for transactions, accounts, and events) <- Implement to: Transactions, Events, Accounts,
and Customers
Implement proper login privilege tracking system <- Re-implement 'active account', implement more expanisve log in/out
*/

class Transaction;
class Account;
class BankCLI;
class Customer;
class Event;

class Customer { public:
	std::string ssn;
	std::string name;
	int ssnNumber;
};

class Event { public:
	std::string description;
};

class Account { public:
	std::string ownerSSN;
	std::string name; // = owner->name;
	std::string account; // = owner->ssn;
	std::string date;
	std::string username;
	std::string password;
	std::set<Transaction> transactions;
	std::set<Event> events;
	double balance;
	int accountNumber; // = owner->ssnNumber;

	// Privelege: 0 = user, 1 = admin.
	int privilege : 1;

	// Overload less-than comparison to allow sets to sort.
	friend bool operator< (const Account& acc1, const Account& acc2);
	friend bool operator== (const Account& acc1, const Account& acc2);
};

bool operator< (const Account& acc1, const Account& acc2) {
	return acc1.accountNumber < acc2.accountNumber;
}

bool operator== (const Account& acc1, const Account& acc2) {
	return acc1.accountNumber == acc2.accountNumber;
}

class Transaction { public:
	long int transactionID;
	int firstPartyID;
	int secondPartyID;
	enum {
		CREDIT,
		DEBIT
	} type;
};

class BankCLI {
	private:
	bankdata::BankDB bank_data;
	std::vector<Account> accountsUnsorted;
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

	bool NewTransaction(double amount, bankdata::Transaction::transactiontype type,
			int firstAccountID, int secondAccountID) {
		RepopulateLists();
		// Check transaction customers are valid (find from accounts then check balance)
		if (std::find(accounts.begin(), accounts.end(), GenerateSearchAccountNumber(std::to_string(firstAccountID))) != accounts.end()) {
			// Get an index and copy of the first account within the bank_data database.
			auto searchIT = std::find(accountsUnsorted.begin(), accountsUnsorted.end(), GenerateSearchAccountNumber(std::to_string(firstAccountID)));
			const bankdata::Account& firstAcc = bank_data.accounts(searchIT - accountsUnsorted.begin());
			if (std::find(accounts.begin(), accounts.end(), GenerateSearchAccountNumber(std::to_string(secondAccountID))) != accounts.end()) {
				// Repeat the search, getting an index and copy of the second account within the bank_data database.
				auto searchIT = std::find(accountsUnsorted.begin(), accountsUnsorted.end(), GenerateSearchAccountNumber(std::to_string(firstAccountID)));
				const bankdata::Account& secondAcc = bank_data.accounts(searchIT - accountsUnsorted.begin());
				if (secondAcc.balance() - amount >= 0) {
					// Create a mutable pointer for modifying the data on account one and two.
					bankdata::Account* secondAccPtr = bank_data.mutable_accounts(searchIT - accountsUnsorted.begin());
					secondAccPtr->set_balance(secondAcc.balance() - amount);
					bankdata::Transaction* newTrans = secondAccPtr->add_transactions();
					// REQUIRES PROPER UNIQUE ID, THIS IS NOT A GOOD IMPLEMENTATION
					newTrans->set_transactionid(rand());
					newTrans->set_firstpartyid(firstAccountID);
					newTrans->set_secondpartyid(secondAccountID);
					newTrans->set_type(type);
					return true;
				}
				else {
					std::cout << "Error: Cannot overdraw account: " << secondAccountID << '\n';
					return false;
				}
			}
			else {
				std::cout << "Error: Cannot find account with ID: " << secondAccountID << '\n';
				return false;
			}
		}
		else {
			std::cout << "Error: Cannot find account with ID: " << firstAccountID << '\n';
			return false;
		}
	}

	bool NewAccount(std::string customerSSN, std::string username, std::string password) {
		// Pass the string account to verify it meets the required parameters (i.e. "### ## ####" with only numbers and
		// whitespace characters).
		if (FormatMatch(customerSSN)) {
			RepopulateLists();

			// Declare a new account.
			Account newAcc = *std::find(accounts.begin(), accounts.end(), GenerateSearchAccountNumber(customerSSN));

/*
			// Create an integer representation of the account's number for sorting into a set to allow high efficiency
			// with reading (finding).
			account.erase(std::remove(account.begin(), account.end(), ' '), account.end());
			newAcc.accountNumber = std::atoi(account.c_str());
*/

			// Attempt to add a new account, tracking the returned pair if a new element was made or if it was already found.
			if (std::find(accountsUnsorted.begin(), accountsUnsorted.end(), newAcc) != accountsUnsorted.end()) {
				bankdata::Account* newAccount = bank_data.add_accounts();
				newAccount->set_account(customerSSN);
				customerSSN.erase(std::remove(customerSSN.begin(), customerSSN.end(), ' '), customerSSN.end());
				int newAccNum = std::atoi(customerSSN.c_str());
				newAccount->set_accountnumber(newAccNum);
				newAccount->set_balance(0);
				newAccount->set_username(username);
				newAccount->set_password(password);
				newAccount->set_creationdate(GenerateDate());
				return true;
			}
			else {
				std::cout << "Account already exists." << '\n'; return false;
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
			RepopulateLists();
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
			float dissimilarity = float(LevenshteinDistance(it->name, name)) / float(it->name.length());
			if (dissimilarity < 0.3) {
				std::cout << dissimilarity << '\n';
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
			else {
				inputInvalid = false;
			}
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
			RepopulateLists();
			auto delAcc = accounts.find(GenerateSearchAccountNumber(account));
			if (delAcc != accounts.end()) {
				std::cout << "Are you sure you want to delete account " << account << " belonging to " << delAcc->name << "? (Y or N)" << '\n';
				std::string userInput;

				while (true) {
					std::cin >> userInput;
					if (userInput.compare("Y") == 0) {
						bank_data.mutable_accounts()->DeleteSubrange(find(accountsUnsorted.begin(), accountsUnsorted.end(), delAcc) - accountsUnsorted.end(), 1);
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

	/* To-do: Implement tracking for current user. Also, password/data encrpytion.
	bool LogOut() {
		std::cout << activeUser.accountNumber;
		return true;
	}

	// To-do: Password encrpytion.
	bool LogIn(std::string name, std::string password) {
		return true;
	}
	*/

	bool ReadData(std::string dataFile) {
		std::fstream input(dataFile, std::ios::in | std::ios::binary);
		if (!input) {
			std::cout << dataFile << " not found!" << '\n';
			return false;
		}
		else if (!bank_data.ParseFromIstream(&input)) {
			std::cout << "Error: Could not parse database." << '\n';
		}
		else {
			std::cout << "File read." << '\n';
			return true;
		}
	}
	void RepopulateLists();
/*
	void RepopulateLists() {

		accounts.clear();
		accountsUnsorted.clear();
		for (int i = 0; i < bank_data.accounts_size(); i++) {
			accounts.insert(bank_data.accounts(i));
			accountsUnsorted.push_back(bank_data.accounts(i));
		}
	}

	Account ConvertAccount(const bankdata::Account& oldAcc) {
		Account returnAcc;
		returnAcc.ownerSSN = oldAcc.account;
		returnAcc.account = oldAcc.account;
		returnAcc.name = oldAcc.name;
		returnAcc.date = oldAcc.datecreated;
		returnAcc.username = oldAcc.username;
		returnAcc.password = oldAcc.password;
		returnAcc.balance = oldAcc.balance;
		for (int i = 0; i < oldAcc.size_transactions) {
			returnAcc.transactions.insert(ConvertTransaction(oldAcc.transactions(i)));
		}
		for (int i = 0; i < oldAcc.size_events) {
			returnAcc.events.insert(ConvertEvent(oldAcc.events(i)));
		}
	}

	Transaction ConvertTransaction(const bankdata::Transaction& oldTrans);
	Transaction ConvertEvent(const bankdata::Transaction& oldEvent);
*/
};

void Line() {
	std::cout << "---------------" << '\n';
}

int main() {
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	// Declare bank CLI class and bank_data object
	BankCLI bank;
	std::string dataFile = "bankdat";

	bank.ReadData(dataFile);


	// Declare/Initialize starting run loop variables.
	bool run = true;
	unsigned char state = 0;
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

		// Logged In
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
				std::cout << "Enter name to search: ";
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
