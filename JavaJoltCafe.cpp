#include <iostream>
#include <iomanip> // Required for formatting
#include <string>
using namespace std;

// Constants
const double TAX_RATE = 0.02;
const double SOCIAL_SECURITY_TAX = 10.0;
const int MAX_EMPLOYEES = 100; // Maximum number of employees

// Employee class
class Employee
{
private:
    string id;
    string name;
    string position;
    string type;         // "fulltime" or "parttime"
    string start_date;
    int leaveDays;       // Only for full-time employees
    int overtimeHours;   // Only for full-time employees
    double workingHours; // Working hours for part-time employees
    double netSalary;

public:
    // Constructor
    Employee() : leaveDays(0), overtimeHours(0), workingHours(0) {}

    // Member function to input employee data
    void input()
    {
        cout << "\nEnter Employee ID   : ";
        cin >> id;

        cout << "Enter Employee Name : ";
        cin.ignore();
        getline(cin, name);

        // Show the menu before inputting position
        showPositionMenu();

        cout << "\nEnter Position      : ";
        getline(cin, position);

        cout << "Enter Employee Start Date (dd/mm/yy)   : ";
        cin >> start_date;

        cout << "Enter Employee Type (fulltime/parttime): ";
        cin >> type;

        // Input validation for employee type
        while (type != "fulltime" && type != "parttime")
        {
            cout << "Invalid type! Please enter 'fulltime' or 'parttime': ";
            cin >> type;
        }

        if (type == "parttime")
        {
            cout << "Enter Working Hours (Monthly)          : ";
            cin >> workingHours;

            while (workingHours < 0)
            {
                cout << "Invalid input! Working hours cannot be negative. Please re-enter: ";
                cin >> workingHours;
            }

            overtimeHours = 0;
            leaveDays = 0; // Reset leave days for part-time
        }

        else
        {                       // full-time
            workingHours = 160; // Default working hours for full-time employees

            cout << "Enter Leave Days    : ";
            cin >> leaveDays;

            while (leaveDays < 0)
            {
                cout << "Invalid input! Leave days cannot be negative. Please re-enter: ";
                cin >> leaveDays;
            }

            cout << "Enter Overtime Hours: ";
            cin >> overtimeHours;

            while (overtimeHours < 0)
            {
                cout << "Invalid input! Overtime hours cannot be negative. Please re-enter: ";
                cin >> overtimeHours;

                overtimeHours = static_cast<int>(overtimeHours); // Keep ensuring overtime is treated as an integer
            }
        }
    }

    // Getter methods
    string getId() const { return id; }
    string getName() const { return name; }
    string getType() const { return type; }
    string getPosition() const { return position; }
    string getStartDate() const { return start_date; }
    int getLeaveDays() const { return leaveDays; }
    double getOvertimeHours() const { return overtimeHours; }
    double getWorkingHours() const { return workingHours; }

    // Function to display available positions and basic salaries
    void showPositionMenu() const
    {
        cout << "\nAvailable Positions and Basic Salaries\n";
        cout << "-------------------------------------------\n";
        cout << "Barista                            : $1920\n";
        cout << "Cashier                            : $1790\n";
        cout << "Kitchen Staff (part-time only)     : $1850\n";
        cout << "Cleaner (part-time only)           : $1400\n";
        cout << "Waiter/waitress (part-time only)   : $1700\n";
        cout << "Supervisor                         : $2400\n";
        cout << "-------------------------------------------\n";
    }

    // Member function to calculate the employee's salary
    double calculateSalary(double &grossSalary, double &taxCost) const
    {
        double basicSalary = getBasicSalary();      // Fetch the basic salary
        double hourlyWage = (basicSalary / 30) / 8; // Hourly wage calculation

        double overtimeFees = 0.0;
        double totalAmount = 0.0;

        if (type == "fulltime")
        {
            // Calculate overtime fees
            overtimeFees = 2 * hourlyWage * overtimeHours;

            // Calculate total amount based on leave days and overtime
            if (leaveDays < 4 && overtimeHours >= 0)
            {
                totalAmount = basicSalary + getLeaveBonus() + overtimeFees;
            }
            else if (leaveDays >= 4 && overtimeHours > 0)
            {
                totalAmount = basicSalary - (basicSalary * 0.01) + overtimeFees - getLeaveBonus();
            }
            else
            {
                totalAmount = basicSalary - getLeaveBonus() + overtimeFees;
            }

            // Calculate tax and deductions
            taxCost = ((totalAmount /100)*2) + SOCIAL_SECURITY_TAX;

            // Net salary after tax
            grossSalary = totalAmount;
            double netSalary = totalAmount - taxCost;

            return max(0.0, netSalary); // Ensure salary is not negative
        }
        else if (type == "parttime")
        {
            // Calculate gross salary for part-time employees
            if (workingHours > 0)
            {
                grossSalary = hourlyWage * workingHours; // Gross salary
                taxCost = 0;   // Fixed security tax for part-time and tax 2% of income

                double netSalary = grossSalary - taxCost;
                return max(0.0, netSalary); // Ensure salary is not negative
            }
            else
            {
                cout << "Error: Invalid working hours for part-time employee." << endl;
                return 0.0;
            }
        }

        return 0.0; // Default case
    }

private:
    // Helper function to get basic salary based on position
    double getBasicSalary() const
    {
        if (type == "parttime")
        {
            if (position == "Kitchen Staff" || position == "kitchen staff")
                return 1850;
            if (position == "Cleaner" || position == "cleaner")
                return 1400;
            if (position == "Waiter" || position == "Waitress" || position == "waiter" || position == "waitress")
                return 1700;
        }
        else
        {
            if (position == "Barista" || position == "barista")
                return 1920;
            if (position == "Cashier" || position == "cashier")
                return 1790;
            if (position == "Supervisor" || position == "supervisor")
                return 2400;
        }
        return 0; // Default case: Invalid position returns 0
    }

    // Helper function to calculate leave bonus for full-time employees
    double getLeaveBonus() const
    {
        if (leaveDays == 0)
            return 300;
        if (leaveDays == 1)
            return 200;
        if (leaveDays == 2)
            return 100;
        return 0;
    }
};

// EmployeeManagement class to manage multiple employees
class EmployeeManagement
{
private:
    Employee employees[MAX_EMPLOYEES]; // Array to store employees
    int employeeCount;                 // Track the number of employees added

public:
    // Constructor
    EmployeeManagement() : employeeCount(0) {}

    // Function to add employees
    void addEmployees()
    {
        int numEmployees;
        cout << "\nHow many employees would you like to add? ";
        cin >> numEmployees;

        for (int i = 0; i < numEmployees; ++i)
        {
            if (employeeCount < MAX_EMPLOYEES)
            {
                employees[employeeCount].input();
                employeeCount++;
            }
            else
            {
                cout << "Employee list is full.\n";
                break;
            }
        }
        cout << "\nEmployee(s) Added!\n\n";
    }

    // Print employee report based on type
    void printEmployeeReport() const
    {
        string type;
        cout << "\nEnter Employee Type to view (fulltime/parttime): ";
        cin >> type;

        while (type != "fulltime" && type != "parttime")
        {
            cout << "Invalid input! Please enter 'fulltime' or 'parttime': ";
            cin >> type;
        }

        if (type == "fulltime")
        {
            printFullTimeReport();
        }
        else
        {
            printPartTimeReport();
        }
    }

    // Print full-time employees in a column format
    void printFullTimeReport() const
    {
        cout << "\nFull-Time Employee Report\n\n";
        cout << left << setw(6) << "ID"
             << setw(11) << "Name"
             << setw(14) << "Position"
             << setw(12) << "Type"
             << setw(12) << "StartDate"
             << setw(12) << "LeaveDays"
             << setw(10) << "Overtime"
             << setw(14) << "WorkingHours"
             << setw(12) << "BeforeTax"
             << setw(11) << "TaxCost"
             << "NetSalary\n";
        cout << "---------------------------------------------------------------------------------------------------------------------------\n";

        for (int i = 0; i < employeeCount; ++i)
        {
            if (employees[i].getType() == "fulltime")
            {
                double grossSalary = 0, taxCost = 0, bonus = 0, hourlyWage = 0;
                double netSalary = employees[i].calculateSalary(grossSalary, taxCost);

                cout << left << setw(6) << employees[i].getId()
                     << setw(11) << employees[i].getName()
                     << setw(14) << employees[i].getPosition()
                     << setw(12) << employees[i].getType()
                     << setw(12) << employees[i].getStartDate()
                     << setw(12) << employees[i].getLeaveDays()
                     << setw(10) << static_cast<int>(employees[i].getOvertimeHours()) // Cast to int for full-time
                     << "$" << setw(13) << static_cast<int>(employees[i].getWorkingHours())
                     << "$" << setw(10) << fixed << setprecision(2) << grossSalary
                     << "$" << setw(10) << fixed << setprecision(2) << taxCost
                     << "$" << fixed << setprecision(2) << netSalary << "\n";
            }
        }
        cout << "---------------------------------------------------------------------------------------------------------------------------\n\n";
    }

    // Print part-time employees in a column format
    void printPartTimeReport() const
    {
        cout << "\nPart-Time Employee Report\n\n";
        cout << left << setw(8) << "ID"
             << setw(12) << "Name"
             << setw(17) << "Position"
             << setw(12) << "Type"
             << setw(12) << "StartDate"
             << setw(17) << "Working Hours"
             << "Net Salary\n";
        cout << "-----------------------------------------------------------------------------------------\n";

        for (int i = 0; i < employeeCount; ++i)
        {
            if (employees[i].getType() == "parttime")
            {
                double grossSalary = 0.0;
                double taxCost = 0.0;
                double netSalary = employees[i].calculateSalary(grossSalary, taxCost);

                cout << left << setw(8) << employees[i].getId()
                     << setw(12) << employees[i].getName()
                     << setw(17) << employees[i].getPosition()
                     << setw(12) << employees[i].getType()
                     << setw(12) << employees[i].getStartDate()
                     << setw(17) << static_cast<int>(employees[i].getWorkingHours()) // Cast to int for part-time
                     << "$" << setw(10) << fixed << setprecision(2) << netSalary << "\n";
            }
        }
        cout << "-----------------------------------------------------------------------------------------\n\n";
    }
    // Print all employees (full-time and part-time) in a combined report
    // Add this function to implement heap sort on employee names
    void heapify(Employee arr[], int n, int i)
    {
        int largest = i;       // Initialize largest as root
        int left = 2 * i + 1;  // Left child
        int right = 2 * i + 2; // Right child

        // If left child is larger than root
        if (left < n && arr[left].getName() > arr[largest].getName())
            largest = left;

        // If right child is larger than largest so far
        if (right < n && arr[right].getName() > arr[largest].getName())
            largest = right;

        // If largest is not root
        if (largest != i)
        {
            swap(arr[i], arr[largest]); // Swap root with largest

            // Recursively heapify the affected sub-tree
            heapify(arr, n, largest);
        }
    }

    // Function to sort employees array by name using heap sort
    void heapSort(Employee arr[], int n)
    {
        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        // Extract elements from heap one by one
        for (int i = n - 1; i > 0; i--)
        {
            // Move current root to end
            swap(arr[0], arr[i]);

            // Call max heapify on the reduced heap
            heapify(arr, i, 0);
        }
    }

    // Modify the printAllEmployeesReport function to sort by name using heap sort
    // Remove the 'const' keyword from the function declaration
    void printAllEmployeesReport()
    {
        // Sort employees by name using heap sort
        Employee sortedEmployees[MAX_EMPLOYEES];
        for (int i = 0; i < employeeCount; ++i)
        {
            sortedEmployees[i] = employees[i];
        }
        heapSort(sortedEmployees, employeeCount); // Sort by name

        // Now print the sorted employee list
        cout << "\nAll Employees Report (Sorted by Name)\n\n";
        cout << left << setw(8) << "ID"
             << setw(12) << "Name"
             << setw(15) << "Position"
             << setw(12) << "Type"
             << "Net Salary\n";
        cout << "---------------------------------------------------------\n";

        for (int i = 0; i < employeeCount; ++i)
        {
            double grossSalary = 0, taxCost = 0;
            double netSalary = sortedEmployees[i].calculateSalary(grossSalary, taxCost);

            cout << left << setw(8) << sortedEmployees[i].getId()
                 << setw(12) << sortedEmployees[i].getName()
                 << setw(15) << sortedEmployees[i].getPosition()
                 << setw(12) << sortedEmployees[i].getType()
                 << "$" << setw(10) << fixed << setprecision(2) << netSalary << "\n";
        }
        cout << "---------------------------------------------------------\n\n";
    }

    // Calculate total monthly payment for all employees
    // Calculate total monthly payment for all employees with breakdown
    void calculateTotalMonthlyPayment() const
    {
        int fullTimeCount = 0, partTimeCount = 0;
        double fullTimeTotal = 0, partTimeTotal = 0;

        for (int i = 0; i < employeeCount; ++i)
        {
            double grossSalary = 0, taxCost = 0;
            double netSalary = employees[i].calculateSalary(grossSalary, taxCost);

            if (employees[i].getType() == "fulltime")
            {
                fullTimeCount++;
                fullTimeTotal += netSalary;
            }
            else if (employees[i].getType() == "parttime")
            {
                partTimeCount++;
                partTimeTotal += netSalary;
            }
        }

        // Display the results in a table format
        cout << "\nMonthly Payment Summary\n";
        cout << "---------------------------------------------------\n";
        cout << left << setw(25) << "Employee Type"
             << setw(10) << "Count"
             << "Total Net Salary\n";
        cout << "---------------------------------------------------\n";
        cout << left << setw(25) << "Full-time Employees"
             << setw(10) << fullTimeCount
             << "$" << fixed << setprecision(2) << fullTimeTotal << "\n";
        cout << left << setw(25) << "Part-time Employees"
             << setw(10) << partTimeCount
             << "$" << fixed << setprecision(2) << partTimeTotal << "\n";
        cout << "---------------------------------------------------\n";
        cout << left << setw(25) << "Total Employees"
             << setw(10) << (fullTimeCount + partTimeCount)
             << "$" << fixed << setprecision(2) << (fullTimeTotal + partTimeTotal) << "\n";
        cout << "---------------------------------------------------\n\n";
    }
};

// Main program
int main()
{   
    int opt;
    cout << endl;
    cout << "\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "\t\t\t\t|              Java Jolt Cafe             |" << endl;
    cout << "\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << endl;

    EmployeeManagement empManagement;

    int choice;
    do
    {
        cout << "Employee Management System\n";
        cout << "==========================\n";
        cout << "1. Add Employees\n";
        cout << "2. View Employee Reports\n";
        cout << "3. View All Employees\n";
        cout << "4. Calculate Total Monthly Payment for All Employees\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            empManagement.addEmployees();
            break;
        case 2:
            empManagement.printEmployeeReport();
            break;
        case 3:
            empManagement.printAllEmployeesReport(); // Combined report for all employees
            break;
        case 4:
            empManagement.calculateTotalMonthlyPayment();
            break;
        case 5:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}