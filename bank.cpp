#include<iostream>
#include<stdlib.h>
#include<string>
#include<vector>
#include<algorithm>
#include <sstream> 
using namespace std;

// function prototypes
void printIntroMenu();
void printMainMenu();
void start();
void login();
void loginAdmin();
void createAccount();
void printAdminMenu();
bool userExists(string);
bool passwordMatch(string, string);

// global variable (use this variable to store the user’s menu selection)
char menuInput;

// users struct
struct user {
	string username, gender,accountType, userId, password;
	int age;
	double balance = 0, amountDonatedForDam = 0;

	void inputUser() {
		int choice;
		cout << "Please enter you user name: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, username);
		cout << "Please enter your age: ";
		cin >> age;
		cout << "Please enter your gender: ";
		cin >> gender;
		for_each(gender.begin(), gender.end(), [](char& c) {
			c = ::tolower(c);
		});
		cout << "Please select the type of account\n1->Current\n2->Default\n>";
		cin >> choice;
		switch (choice) {
		case 1:
			accountType = "current";
			break;
		case 2:
			accountType = "default";
			break;
		}
		while(1) {
			cout << "Please enter your user id: ";
			cin >> userId;
			if (userExists(userId)) {
				cout << "User id already exists. Try again.";
				system("pause");
			}
			else break;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		do {
			cout << "Please enter your password: ";
			getline(cin, password);
			if (password.length() < 8) {
				cout << "Password should consist of at least 8 alphanumeric characters\n";
			}
			else break;
		} while (1);
		cout << "Thank You! Your account has been created! To login please select option 1\n\n";
	}

	string login() {
		string id, pass;
		cout << "Please enter your user id: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, id);
		cout << "Please enter your password: ";
		getline(cin, pass);
		if (!userExists(id) || !passwordMatch(id, pass)) {
			cout << "\n\n******** LOGIN FAILED! ********\n\n";
			return "failed";
		}
		else {
			cout << "Access Granted! Please select an option\n\n";
			printMainMenu();
			return id;
		}
	}

	void deposit() {
		double amount;
		cout << "Amount of deposit: $";
		cin >> amount;
		balance += amount;
	}

	void withdraw() {
		double amount;
		cout << "\nYour balance is $" << balance<<endl;
		cout << "Please enter amount you want to withdraw\n1->$5\n2->$10\n3->$20\n4->Other Amount\n5->To Enter Previous Menu\n\n>";
		cin >> menuInput;
		switch (menuInput) {
		case '1':
			if (balance >= 5) {
				balance -= 5;
				cout << "\nYou have withdrawn $5: Your remaining balance is $" << balance;
			}
			else cout << "\nSorry You Don't Have Enough Balance\n";
			break;
		case '2':
			if (balance >= 10) {
				balance -= 10;
				cout << "\nYou have withdrawn $10: Your remaining balance is $" << balance;
			} else cout << "\nSorry You Don't Have Enough Balance\n";
			break;
		case '3':
			if (balance >= 20) {
				balance -= 20;
				cout << "\nYou have withdrawn $20: Your remaining balance is $" << balance;
			} else cout << "\nSorry You Don't Have Enough Balance\n";
			break;
		case '4':
			cout << "\nEnter amount you want to withdraw: $";
			cin >> amount;
			if (balance >= amount) {
				balance -= amount;
				cout << "\nYou have withdrawn $" << amount << ": Your remaining balance is $" << balance;
			}
			else cout << "\nSorry You Don't Have Enough Balance\n";
			break;
		case '5':
			return;
		}
	}

	void checkBalance() {
		cout << "Your balance is $"<<balance<<endl;
	}

	void bill() {
		double amount;
		cout << "Please Enter The Amount of Your Bill\n$";
		cin >> amount;
		if (balance >= amount) {
			balance -= amount;
			cout << "You Have Paid Your Electricity Bill:\nThank you" << endl;
		} else cout << "\nSorry You Don't Have Enough Balance";
	}

	void donate() {
		double amount;
		cout << "Please Enter Amount You Want to Donate\n$";
		cin >> amount;
		if (balance >= amount) { 
			balance -= amount; 
			amountDonatedForDam += amount;
		}
		else cout << "\nSorry You Don't Have Enough Balance";
	}
};

// more globals
vector<user> users;
user activeUser;
bool adminAuth = false;

bool compareCustomerAsc(user u1, user u2) {
	return u1.balance < u2.balance;
}

bool compareCustomerDesc(user u1, user u2) {
	return u1.balance > u2.balance;
}

// admin struct
struct admin {
	string username, password;

	string login() {
		string id, pass;
		cout << "Enter your administrative user id: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, id);
		cout << "Enter your Password: ";
		getline(cin, pass);
		if (id == "admin" && pass == "admin123") {
			cout << "Access Granted Admin! Please select an option\n\n";
			printAdminMenu();
			return id;
		}
		else {
			cout << "\n\n******** LOGIN FAILED! ********\n\n";
			return "failed";

		}
	}

	void totalAmount() {
		double amount = 0;
		for (int i = 0; i < users.size(); i++) {
			amount += users[i].balance;
		}
		cout << "Total amount of all the customers is $" << amount;
	}

	void sortCustomers() {
		sort(users.begin(), users.end(), compareCustomerAsc);
		cout << "Customers with acsending order w.r.t balance are:\n";
		showUsers();
		sort(users.begin(), users.end(), compareCustomerDesc);
		cout << "Customers with descending order w.r.t balance are:\n";
		showUsers();
	}

	void showUsers() {
		for (int i = 0; i < users.size(); i++) {
			cout << "Username: " << users[i].username << ", Balance: " << users[i].balance<<endl;
		}
	}

	void customerWithHighestBalance() {
		user u = users[0];
		for (int i = 1; i < users.size(); i++) {
			if (users[i].balance > u.balance) u = users[i];
		}
		cout << "Customer with highest balance is " << u.username << " with balance of $" << u.balance<<endl;
	}

	void customerWithLowestBalance() {
		user u = users[0];
		for (int i = 1; i < users.size(); i++) {
			if (users[i].balance < u.balance) u = users[i];
		}
		cout << "Customer with lowest balance is " << u.username << " with balance of $" << u.balance << endl;
	}

	void searchCustomer() {
		string name;
		cout << "Enter a name to search: ";
		getline(cin, name);
		for (int i = 0; i < users.size(); i++) {
			if (users[i].username == name) {
				cout << "Customer Found.\nUsername: "<<users[i].username<<"\nBalance: $"<<users[i].balance<<endl;
				system("pause");
				return;
			}
		}
		cout << "\nNo user found with such name.\n"<<endl;
	}

	void customersWithTenBalance() {
		int count = 0;
		for (int i = 0; i < users.size(); i++) {
			if (users[i].balance > 10) count++;
		}
		cout << "\nTotal Customers having balance greater than $10 are " << count<<endl;
	}

	void femaleCUstomersWithAge2030() {
		int count = 0;
		for (int i = 0; i < users.size(); i++) {
			if (users[i].gender == "female")
				if (users[i].age > 20 && users[i].age < 30) count++;
		}
		cout << "\nTotal female customers with age greater than 20 and less than 30 are " << count << endl;
	}

	void customersWithAge3045() {
		int count = 0;
		for (int i = 0; i < users.size(); i++) {
			
				if (users[i].age > 30 && users[i].age < 45) count++;
		}
		cout << "\nTotal customers with age greater than 30 and less than 45 are " << count << endl;
	}

	void customerDonatedHighestForDam() {
		user u = users[0];
		for (int i = 1; i < users.size(); i++) {
			if (users[i].amountDonatedForDam > u.amountDonatedForDam) u = users[i];
		}
		cout << "Customer with highest donation for dam is done by " << u.username << " with donation amount of $" << u.amountDonatedForDam << endl;
	}

	void youngestCustomer() {
		user uMale, uFemale;
		int maleIndex = 0, femaleIndex = 0;
		for (int i = 0; i < users.size(); i++) {
			if (users[i].gender == "male") {
				uMale = users[i];
				maleIndex = i;
				break;
			}
		}
		for (int i = 0; i < users.size(); i++) {
			if (users[i].gender == "female") {
				uFemale = users[i];
				femaleIndex = i;
				break;
			}
		}
		// for lowest aged male
		for (int i = maleIndex + 1; i < users.size(); i++) {
			if (users[i].gender == "male") {
				if (users[i].age < uMale.age)
					uMale = users[i];
			}
		}
		// for lowest aged female
		for (int i = femaleIndex + 1; i < users.size(); i++) {
			if (users[i].gender == "female") {
				if (users[i].age < uFemale.age)
					uFemale = users[i];
			}
		}
		cout << "\nYoungest Male is " << uMale.username << " with age " << uMale.age << endl;
		cout << "\nYoungest Female is " << uFemale.username << " with age " << uFemale.age << endl;
	}

	void totalDonationAmount() {
		double amount = 0;
		for (int i = 0; i < users.size(); i++) {
			amount += users[i].amountDonatedForDam;
		}
		cout << "Total amount your customers have donated for \"Diamer Basha and Mohmind Dam is $\""<<amount<<endl;
	}

	void startWithA() {
		int count = 0;
		for (int i = 0; i < users.size(); i++) {
			if (users[i].username[0] == 'A' || users[i].username[0] == 'a') {
				count++;
			}
		}
		cout << "Total customers whose name start with 'a' or 'A' are " << count << endl;
	}

	void palindrome() {
		for (int i = 0; i < users.size(); i++) {
			string name = users[i].userId;
			bool isPalindrome = true;
			for (int j = 0; j < name.length() / 2; j++) {
				if (name[j] == name[name.length() - 1 - j]) continue;
				else {
					isPalindrome = false;
					break;
				};
			}
			if (isPalindrome) {
				cout << "\nCustomer having palindrome user id is " << name<<endl;
				return;
			}

		}
	}

	void armstrong() {
		for (int i = 0; i < users.size(); i++) {
			int bal = (int)users[i].balance;
			string strBal = to_string(bal);
			int num = 0;
			for (int j = 0; j < strBal.length(); j++) {
				int digit = 0;
				stringstream geek(strBal[j]);
				geek >> digit;
				num += digit * digit * digit;
			}
			if (num == bal) {
				cout << "Customer having Armstrong balance has name " << users[i].username << " with balance of $" << bal<<endl;
				break;
			}
		}
	}
};

admin adminUser;

// the main function
int main()
{
	// TO WRITE A WELCOME MESSAGE HERE
	// call the function start
	while(1) start();
	return 0;
}

bool userExists(string id) {
	for (int i = 0; i < users.size(); i++) {
		if (users[i].userId == id) return true;
	}
	return false;
}

bool passwordMatch(string id, string pass) {
	for (int i = 0; i < users.size(); i++) {
		if (users[i].userId == id)
			if (users[i].password == pass) return true;
			else return false;
	}
	return false;
}

void printIntroMenu()
{
	cout << "1 -> Login\n2 -> Login as Admin\n3 -> Create New Account\n4 -> Quit\n\n> ";
}

void printMainMenu()
{
	cout << "1 -> Deposit Money\n2 -> Withdraw Money\n3 -> Request Balance\n4 -> Pay Electricity Bill\n5 -> Press 5 to Donate for \"Diamer Bhasha and Mohmind Dam\"\n6 -> Quit\n\n> ";
}

void printAdminMenu() {
	cout << " 1 -> Check Total number of Customers\n 2 -> Total amount of all the customers\n 3 -> Sort Customers w.r.t their balance (in increasing order and decreasing order)\n 4 -> Find the customer with highest balance\n 5 -> Search Customer by name and print its information\n 6 -> Find total customers having balance greater than $10\n 7 -> Find all the female customers with age greater than 20 and less than 30\n 8 -> Find all the customers with age greater than 30 and less 45\n 9 -> Find the customer who have donated highest amount for \"Diamer Bhasha and Mohmind Dam\"\n10 -> Find the Customer with lowest balance\n11 -> Find the youngest male and female customer\n12 -> Find the Customer having palindrome user id\n13 -> Find the customer having Armstrong balance\n14 -> Find total customers whose name starts with 'a' or 'A'\n15 -> Find total amount, your customers have donated for \"Diamer Bhasha and Mohmind Dam\"\n16 -> Quit\n\n> ";
}

void start()
{
	cout << "Please select an option from the menu below\n\n\n";
	printIntroMenu();
	cin >> menuInput;
	switch (menuInput) {
	case '1':
		login();
		break;
	case '2':
		loginAdmin();
		break;
	case '3': 
		createAccount();
		break;
	case '4': 
		exit(0);
		break;
	}
}

void createAccount()
{
	activeUser.inputUser();
	users.push_back(activeUser);
}

void loginAdmin() {
	string result = adminUser.login();
	if (!(result == "failed")) {
		adminAuth = true;
	}
	else return;
	bool exitFromLoop = false;
	int choice;
	while (1) {
		cin >> choice;
		switch (choice) {
		case 1:
			cout << "\nTotal numbers of customers are " << users.size()<<endl;
			system("pause");
			break;
		case 2:
			adminUser.totalAmount();
			system("pause");
			break;
		case 3:
			adminUser.sortCustomers();
			system("pause");
			break;
		case 4:
			adminUser.customerWithHighestBalance();
			system("pause");
			break;
		case 5:
			adminUser.searchCustomer();
			system("pause");
			break;
		case 6:
			adminUser.customersWithTenBalance();
			system("pause");
			break;
		case 7:
			adminUser.femaleCUstomersWithAge2030();
			system("pause");
			break;
		case 8:
			adminUser.customersWithAge3045();
			system("pause");
			break;
		case 9:
			adminUser.customerDonatedHighestForDam();
			break;
		case 10:
			adminUser.customerWithLowestBalance();
			system("pause");
			break;
		case 11:
			adminUser.youngestCustomer();
			system("pause");
			break;
		case 12:
			adminUser.palindrome();
			break;
		case 13:
			adminUser.armstrong();
			break;
		case 14:
			adminUser.startWithA();
			system("pause");
			break;
		case 15:
			adminUser.totalDonationAmount();
			break;
		case 16:
			exitFromLoop = true;
			break;
		}
		if (exitFromLoop) break;
		cout << "\n\nSelect an option\n\n"<<endl;
		printAdminMenu();
	}
}

void login() {
	string result = activeUser.login();
	if (!(result == "failed")) {
		for (int i = 0;i < users.size(); i++) {
			if (users[i].userId == result) {
				activeUser = users[i];
				break;
			}
		}
	}
	else {
		return;
	}
	bool breakFromLoop = false;
	while (1) {
		cin >> menuInput;
		switch (menuInput) {
			case '1':
				activeUser.deposit();
				system("pause");
				break;
			case '2':
				activeUser.withdraw();
				system("pause");
				break;
			case '3':
				activeUser.checkBalance();
				system("pause");
				break;
			case '4':
				activeUser.bill();
				system("pause");
				break;
			case '5':
				activeUser.donate();
				system("pause");
				break;
			case '6':
				cout << "\nThanks for Using Our Services!\n\n";
				breakFromLoop = true;
				break;
		}
		if (breakFromLoop) break;
		printMainMenu();
	}
}
// CODE ENDS HERE