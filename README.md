# FixItNow -- Home Appliance Repair Service Management System

**FixItNow** is a **C++** application designed to manage the day-to-day operations of an authorized home appliance repair service. The application enables staff management, handles a catalog of accepted products for repair, and simulates real-time workflows using modern software design principles.

> 💡 **Note on Codebase Language:**
> The project's function and variable names are written in Romanian (reflecting its initial development phase/academic requirements). However, all code comments, documentation, and this README are provided in English to ensure global accessibility and easier code review.

------------------------------------------------------------------------

## 🏗️ Architecture and Design Patterns

The project adheres to **SOLID** principles and utilizes **Modern C++** concepts:

-   **Smart Pointers**
    -   `std::shared_ptr` for staff members and the payroll system
    -   `std::unique_ptr` for appliances and repair requests
-   **STL Containers**
    -   `std::vector` for dynamic lists
    -   `std::map` for the catalog hierarchy
-   **Lambda Expressions**
    -   Sorting requests chronologically by timestamp
    -   Identifying the most optimal technicians for specific tasks

### Design Patterns Used

-   **Singleton**  
    `ServiceManager` centralizes resource management and ensures a single, unique instance of the repair service system.

-   **Factory Method**  
    Dynamic creation of `Angajat` (Employee) and `Electrocasnic` (Appliance) objects through dedicated factory classes, ensuring proper decoupling of the instantiation logic.

-   **Decorator**  
    Extensible salary calculation; allows adding various bonuses (loyalty, transport, performance) on top of the base object dynamically without modifying the core `Angajat` class.

------------------------------------------------------------------------

## 📂 Project Structure

### 👷 Staff Management (`Muncitori/`)

-   **Angajati (Employees)**  
    Abstract base class; implements rigorous Romanian CNP (Personal Numeric Code) validation (control algorithm, leap years) and name length validation (3–30 characters).

-   **Tehnician (Technician)**  
    Manages repair specializations by appliance type/brand, enforces a limit of maximum 3 active requests simultaneously, and tracks total accumulated work hours.

-   **Receptioner (Receptionist)**  
    Registers incoming repair requests into the system and keeps track of processed unique IDs.

-   **Supervizor (Supervisor)**  
    Administrative role; has no additional technical attributes but automatically receives a leadership bonus.

-   **AngajatFactory**  
    Determines the specific type of employee object to instantiate based on an input string.

-   **Salarii (Payroll)**  
    System built around the `ICalculSalariu` interface:
    -   Base salary: **4000 RON**
    -   Loyalty bonus: **5%**
    -   Transport allowance: **400 RON**
    -   Technician bonus: **2% of the total value of completed repairs**
    -   Supervisor bonus: **20%**

------------------------------------------------------------------------

### 🔌 Appliance Management (`Electrocasnice/`)

-   **Electrocasnic (Appliance)**  
    Base class that stores the brand, model, base price, and contains logic to calculate the appliance's age.

-   **Frigider / Televizor / Masina_spalat (Refrigerator / TV / Washing Machine)**  
    Subclasses containing domain-specific details:
    -   Refrigerator: has freezer (`bool`)
    -   TV: screen diagonal (`double` / `string`)
    -   Washing Machine: load capacity (`kg`)

-   **CatalogElectrocasnice**  
    Internal database structured as `map<map<vector>>` for lightning-fast verification of repairable models.

------------------------------------------------------------------------

### 📝 Requests and Simulation (`Cereri/`)

-   **CereriReparatie (Repair Requests)**  
    Automatically computes:
    -   A unique request ID
    -   Estimated duration (**age × complexity**)
    -   Total cost (**price × duration**)
-   **ServiceManager**  
    Manages the life-cycle queues:
    -   Waiting
    -   Active
    -   Completed
    -   Rejected  
    It also handles the core *tic-toc* real-time simulation logic.

------------------------------------------------------------------------

## ⚙️ Features

-   **Complex Validation:** Enforces secure CNP checking and a minimum employment age of **16 years**.
-   **Minimum Operating Conditions:** The service requires at least 3 technicians, 1 receptionist, and 1 supervisor to successfully initialize and run.
-   **Repair Simulation:** Decrements the remaining repair time every second and automatically frees up technicians upon task completion.
-   **CSV Reporting:** Generates external reports detailing the Top 3 highest salaries, repair duration records, and history requests grouped by categories.

------------------------------------------------------------------------

## 🚀 Usage

-   **Compilation:** Requires a **C++17** compliant compiler or higher.
-   **Test Data:** Input CSV files (employees, product catalog, requests) must be properly placed inside the `tests/1` directory.
-   **Interface:** CLI console-based application with straightforward navigation via numeric menus.
-   **Makefile:** Included out of the box for quick compilation and automated execution.