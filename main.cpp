#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <ctime>

using namespace std;

int testDataAmount = 60;

/**
 * Vector filter to filter the customers based to specified criteria
 *
 * @tparam T
 * @tparam Pred
 * @tparam ValueType
 * @param result
 * @param pred
 * @param member
 * @param model
 * @return vector<T>
 */
template<class T, class Pred, class ValueType>
vector<T> filter(vector<T> result, Pred pred, ValueType T::* member, ValueType model) {
    result.erase(remove_if(begin(result), end(result), [&](const auto &elem) {
        return !pred(elem.*member, model);
    }), end(result));

    return result;
}

/**
 * clears the console output "softly" as the system("clr") command does not work on mac :(
 */
void clear() {
    cout << string(30, '-') << endl;
}

/**
 * returns current date and time
 * @return string
 */
string currentDateTime() {
    time_t now = time(nullptr);
    struct tm tstruct{};
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d.%m.%Y um %H:%m", &tstruct);

    return buf;
}

/**
 * Customer class
 */
class Customer {
public:
    int id;
    string first_name;
    string last_name;
    string full_name;
    string address;
    string zip;
    string city;
    string phone;
    string birthday;
    bool license;
    bool has_active_reservation = false;

    Customer() : id(0),
                 first_name(""),
                 last_name(""),
                 full_name(""),
                 address(""),
                 zip(""), city(""),
                 phone(""),
                 birthday(""),
                 license(false),
                 has_active_reservation(false) {
    }

    /**
     * Constructor to create the customer with his details. Full name auto generated.
     * @param id
     * @param firstName
     * @param lastName
     * @param address
     * @param zip
     * @param city
     * @param phone
     * @param birthday
     * @param license
     */
    Customer(int id, string firstName, string lastName, string address, string zip,
             string city, string phone, string birthday, bool license) : id(id),
                                                                         first_name(firstName),
                                                                         last_name(lastName),
                                                                         address(address),
                                                                         zip(zip), city(city),
                                                                         phone(phone),
                                                                         birthday(birthday),
                                                                         license(license) {
        full_name = firstName + " " + lastName;
        has_active_reservation = false;
    }

    /**
     * Sets that the customer has an active reservation
     */
    void setHasReservation();

    /**
     * Sets that the customer does not have an active reservation
     */
    void setDoesNotHaveReservation();

    /**
     * Create new customer by reading data from istream
     */
    friend istream &operator>>(istream &is, Customer &a);

    /**
     * Show the details of the customer
     */
    friend ostream &operator<<(ostream &os, const Customer &a);

};

/**
     * Sets that the customer has an active reservation
     */
void Customer::setHasReservation() {
    has_active_reservation = true;
}

/**
 * Sets that the customer does not have an active reservation
 */
void Customer::setDoesNotHaveReservation() {
    has_active_reservation = false;
}

/**
 * Create new customer by reading data from istream
 */
istream &operator>>(istream &is, Customer &a) {
    string license_cin;

    cout << "Kunde erfassen" << endl << endl;

    cout << "Vorname: ";
    getline(is >> ws, a.first_name); // read the first name from input

    cout << "Nachname: ";
    getline(is >> ws, a.last_name); // read the last name from input

    a.full_name = a.first_name + " " + a.last_name;

    cout << "Straße: ";
    getline(is >> ws, a.address);// read the last name from input

    cout << "PLZ: ";
    getline(is >> ws, a.zip); // read the last name from input

    cout << "Ort: ";
    getline(is >> ws, a.city); // read the last name from input

    cout << "Geburtstag (TT.MM.JJJJ): ";
    is >> a.birthday; // read the birthday name from input

    cout << "Telefonnummer: ";
    getline(is >> ws, a.phone); // read the birthday name from input

    cout << "Im Besitz des Führerscheins A [j/n]: ";
    is >> license_cin; // read the birthday name from input

    if (license_cin == "j") {
        a.license = true;
    }

    return is;
}

/**
 * Show the details of the customer
 */
ostream &operator<<(ostream &os, const Customer &a) {
    os << "Nummer: " << a.id << endl;
    os << "Vorname: " << a.first_name << endl;
    os << "Nachname: " << a.last_name << endl;
    os << "Straße: " << a.address << endl;
    os << "PLZ: " << a.zip << endl;
    os << "Ort: " << a.city << endl;
    os << "Geburtsdatum: " << a.birthday << endl;
    os << "Telefonnummer: " << a.phone << endl;
    os << "Hat Führerschein: " << (a.license ? "Ja" : "Nein") << endl << endl;

    return os;
}

class Model {
public:
    int id;
    bool in_use = false;
    string name;

    /**
     * Construct Model
     * @param id
     * @param name
     */
    Model(int id, string name) : id(id), name(name), in_use(false) {}

    /*
     * Set Model to be in use
     */
    void use();

    /*
     * Set Model to be NOT in use
     */
    void release();
};

/*
 * Set Model to be in use
 */
void Model::use() {
    in_use = true;
}

/*
 * Set Model to be NOT in use
 */
void Model::release() {
    in_use = false;
}

class Reservation {
public:
    int id;
    string created_at;
    Customer customer;
    Model model;

    Reservation(int id, Customer customer, Model model) : id(id), customer(customer), model(model) {
        created_at = currentDateTime();
    }

    /**
     * Show the details of the reservation
     */
    friend ostream &operator<<(ostream &os, const Reservation &a);

};

/**
 * Show the details of the reservation
 */
ostream &operator<<(ostream &os, const Reservation &a) {
    os << "---- Reservierungsdetails ----" << endl;
    os << "Reservierungsnummer: " << a.id << endl;
    os << "Modell: " << a.model.name << endl;
    os << "Ausgeliehen am: " << a.created_at << endl;
    os << "---- Kundendetails ----" << endl;
    os << "Vorname: " << a.customer.first_name << endl;
    os << "Nachname: " << a.customer.last_name << endl;
    os << "Straße: " << a.customer.address << endl;
    os << "PLZ: " << a.customer.zip << endl;
    os << "Ort: " << a.customer.city << endl;
    os << "Geburtsdatum: " << a.customer.birthday << endl;
    os << "Telefonnummer: " << a.customer.phone << endl;
    os << "Hat Führerschein: " << (a.customer.license ? "Ja" : "Nein") << endl << endl;

    return os;
}

vector<Customer> listOfCustomers;
vector<Reservation> listOfReservations;
vector<Model> listOfModels;


class CustomerManager {
public:
    int uuid = 0; // up counting id for creating customer data

    /**
     * Add new customer to store by reading data from istream
     */
    Customer createCustomer();

    /**
     * Search customer by different fields
     */
    int searchCustomer(bool printDetails = false);

    Customer findOrNew();

    /**
     * Lists customers in store
     */
    void listCustomers();

    /**
     * Creates sample customers (60)
     */
    void generateSampleCustomers();

    /**
     * Creates the menu
     *
     * @return int
     */
    int menu();

};

/**
* Add new customer to store by reading data from istream
*/
Customer CustomerManager::createCustomer() {
    Customer customer;
    cin >> customer;
    customer.id = uuid++;

    listOfCustomers.push_back(customer); // and push it to the vector

    cout << endl << "Kunde wurde erfasst."; // well done!

    return customer;
}

/**
 * Search customer by different fields
 */
int CustomerManager::searchCustomer(bool printDetails) {
    int field; // field to use for search
    string term; // the search term

    clear();

    cout << "In welchem Feld soll gesucht werden:" << endl << endl;
    cout << "[1] Nummer" << endl; // search by the customer id
    cout << "[2] Vorname" << endl; // search by the customer first name
    cout << "[3] Nachname" << endl; // search by the customer last name
    cout << "[4] Vor- und Nachname" << endl << endl; // search by the customer full name

    cout << "Feld [1-4]: "; // let the user choose where to search
    cin >> field;

    cout << "Zu suchender Wert: "; // the term to search for
    getline(cin >> ws, term);

    for (std::vector<int>::size_type i = 0;
         i != listOfCustomers.size(); i++) { // iterates over all data to find the one
        Customer s = listOfCustomers[i];

        // search in the set field, prints the action menu and reassigns the changed data to the "db"
        if (field == 1 && listOfCustomers[i].id == stoi(term)) { // if field is id and term equals id
            if (printDetails) {
                cout << s;
            }

            return i;
        } else if (field == 2 && listOfCustomers[i].first_name.find(term) !=
                                 string::npos) { // if field is first name and contains the term
            if (printDetails) {
                cout << s;
            }

            return i;
        } else if (field == 3 && listOfCustomers[i].last_name.find(term) !=
                                 string::npos) { // if field is last name and contains the term
            if (printDetails) {
                cout << s;
            }

            return i;
        } else if (field == 4 && listOfCustomers[i].full_name.find(term) !=
                                 string::npos) { // if field is last name and contains the term
            if (printDetails) {
                cout << s;
            }

            return i;
        }
    }

    cout << "! Kunde nicht gefunden" << endl;

    return -1;
}

Customer CustomerManager::findOrNew() {
    string term;

    cout << "> Kunden suchen oder erstellen" << endl;
    cout << "Vor- und Nachname: ";
    getline(cin >> ws, term); // get first and last name

    auto results = filter(listOfCustomers, std::equal_to<>(), &Customer::full_name,
                          term); // get all customers that matches the criteria

    if (results.size() == 1) { // if there is exact one entry use it and go on
        return results[0];
    } else if (results.size() > 1) {
        // here could be some more action to select the correct customer if there were multiple customers found.
        // but I did not implement that - the whole ea2 took very long...
    }

    cout << "> Kunde nicht gefunden" << endl;

    return createCustomer();
}

/**
 * Lists customers in store
 */
void CustomerManager::listCustomers() {
    printf("%6s|%14s|%14s|%12s\n", "Nummer", "Vorname", "Nachname", "Geburtsdatum"); // prints the table header

    for (Customer i: listOfCustomers) { // iterates over all customer and prints a "table row" with the details
        printf("%6i|%14s|%14s|%12s\n", i.id, i.first_name.c_str(), i.last_name.c_str(),
               i.birthday.c_str());
    }

    cout << "Insgesamt " << listOfCustomers.size() << " Datensätze."
         << endl; // shows the total amount of created customer
}

/**
 * Creates sample customers (60)
 */
void CustomerManager::generateSampleCustomers() {
    int yB = 1990; // year base from where to count down

    for (int i = 0; i < testDataAmount; ++i) { // creates 60 sample "Jan" data with different birthday values
        Customer customer = *new Customer(++uuid, "Jan " + to_string(i), "Mueller " + to_string(1), "Straße",
                                          "20251", "Hamburg", "04046812364",
                                          "05.09." + to_string(yB - i), true);
        listOfCustomers.push_back(customer); // and push them to the global "db"
    }

    // And me :)
    Customer customer = *new Customer(++uuid, "Jonas", "Imping", "Appener Weg 7",
                                      "20251", "Hamburg", "04046812364",
                                      "19.05.1995", true);
    listOfCustomers.push_back(customer); // and push them to the global "db"
}

/**
 * Creates the menu
 *
 * @return int
 */
int CustomerManager::menu() {
    int choice;

    clear();

    // Choose action
    cout << "Bitte wählen Sie Ihre Aktion:" << endl << endl;
    cout << "[1] Kunde erfassen" << endl;
    cout << "[2] Liste aller Kunden" << endl;
    cout << "[3] Kunde suchen" << endl;
    cout << "[4] Zurück" << endl;
    cout << string(10, '-') << endl;
    cout << "[5] Beispieldaten einfügen" << endl;

    cout << "Was darf es sein? [1-7]: ";
    cin >> choice;

    bool quit = false;

    clear(); // Clear the console

    switch (choice) { // Choose action from menu
        case 1:
            createCustomer(); // Create a new member
            break;
        case 2:
            listCustomers(); // List all customer
            break;
        case 3:
            searchCustomer(true); // List customer by id, first name or last name
            break;
        case 4:
            quit = true; // quit programm
            break;
        case 5:
            generateSampleCustomers(); // Adds sample customer to "db"
            break;
        default:
            break;
    }

    return quit;
}

class ReservationManager {
private:
    CustomerManager customerManager;
public:
    int uuid = 0; // up counting id for creating reservation data

    explicit ReservationManager(const CustomerManager &customerManager) : customerManager(customerManager) {}

    /**
     * Add new reservation to store
     */
    void createReservation();

    /**
     * Shows the reservation details for a selected reservation id
     */
    void reservationDetails();

    /**
     * Lists reservations in store
     */
    void listReservations();

    /**
     * Releases reservation
     */
    void releaseReservation();

    /**
     * Creates sample reservations
     */
    void generateSampleReservations();

    /**
     * Save reservation data to file in CSV format
     */
    void saveReservations();


    /**
     * Creates the menu
     *
     * @return int
     */
    int menu();

};

/**
* Add new reservation to store
*/
void ReservationManager::createReservation() {
    int model_index;
    cout << "Reservierung erfassen" << endl << endl;

    Customer customer = customerManager.findOrNew(); // find or create a customer

    if (!customer.license) { // abort if customer does not have a license
        cout << endl << "!!!! Kunde hat keinen Führerschein -- abbruch !!!!" << endl;

        return; // break
    }

    if (customer.has_active_reservation) { // abort if customer already has an active reservation
        cout << endl << "!!!! Kunde hat bereits eine Reservierung -- abbruch !!!!" << endl;

        return; // break
    }

    cout << endl << "Welches Modell?" << endl << endl;

    for (int i = 0; i < listOfModels.size(); ++i) { // choose the model
        cout << "[" << i + 1 << "] " << listOfModels[i].name << endl; // choose the model
    }

    cout << "Model [1-" << listOfModels.size() << "]: "; // choose the model
    cin >> model_index; // choose the model

    if (!(model_index > 0 && model_index <= listOfModels.size())) { // abort if model index not in range of models
        cout << "!!!! Modell nicht gefunden -- abbruch !!!!" << endl;

        return;
    }

    Model model = listOfModels[model_index - 1]; // get the model

    if (model.in_use) { // if the model is already in use do not approve reservation and abort
        cout << "!!!! Modell bereits reserviert -- abbruch !!!!" << endl;
        return;
    }

    model.use(); // set the model to be in use

    for (auto &listOfCustomer: listOfCustomers) { // set that the customer has an active reservation
        if (listOfCustomer.id == customer.id) {
            listOfCustomer.setHasReservation();
        }
    }

    Reservation reservation = *new Reservation(++uuid, customer, model); // creates reservation
    listOfReservations.push_back(reservation); // and push it to the vector
    listOfModels[model_index - 1] = model; // refresh model -- it's in use now!

    cout << endl << "Reservierung wurde erfasst."; // well done!
}

/**
 * Shows the reservation details for a selected reservation id
 */
void ReservationManager::reservationDetails() {
    int id;

    cout << "Reservierungsnummer: ";
    cin >> id;

    auto results = filter(listOfReservations, std::equal_to<>(), &Reservation::id,
                          id); // filter the reservation by id

    if (results.size() == 1) { // if found than show details
        cout << results[0];
    }
}

/**
 * Shows the reservation details for a selected reservation id
 */
void ReservationManager::releaseReservation() {
    int id;

    cout << "Reservierungsnummer: ";
    cin >> id;

    auto results = filter(listOfReservations, std::equal_to<>(), &Reservation::id,
                          id); // filter the reservation by id

    if (results.size() == 1) { // if found than show details
        Reservation res = results[0];

        for (auto &listOfModel: listOfModels) { // release the model
            if (res.model.id == listOfModel.id) {
                listOfModel.release();
            }
        }

        for (auto &customer: listOfCustomers) { // release the model
            if (res.customer.id == customer.id) {
                customer.setDoesNotHaveReservation();
            }
        }
    }

    listOfReservations = filter(listOfReservations, std::not_equal_to<>(), &Reservation::id,
                                id); // release reservation and refresh list
}

/**
 * Lists reservations in store
 */
void ReservationManager::listReservations() {
    printf("%6s|%30s|%20s\n", "Nummer", "Kunde", "Modell"); // prints the table header

    for (const Reservation &i: listOfReservations) { // iterates over all reservations and prints a "table row" with the details
        printf("%6i|%30s|%20s\n", i.id, i.customer.full_name.c_str(), i.model.name.c_str());
    }

    cout << "Insgesamt " << listOfReservations.size() << " Datensätze."
         << endl; // shows the total amount of created reservations
}

/**
 * Creates sample reservations
 */
void ReservationManager::generateSampleReservations() {

    // generate some customer samples if there are not enough
    if (listOfCustomers.size() < listOfModels.size()) {
        customerManager.generateSampleCustomers();
    }

    for (int i = 0; i < listOfModels.size(); ++i) { // creates some reservation samples for all models
        listOfModels[i].use();
        Reservation reservation = *new Reservation(++uuid, listOfCustomers[i], listOfModels[i]);
        listOfReservations.push_back(reservation); // and push them to the global "db"
    }
}

/**
 * Save reservation data to file in CSV format
 */
void ReservationManager::saveReservations() {
    ofstream fileWriter; // this is the writer

    fileWriter.open("reservierungen.txt",
                    std::ios::out | ios_base::app); // open file and append data or create file if not exists

    if (fileWriter.good()) {
        fileWriter << "Stand;" << currentDateTime() << ";;;" << endl; // current date and time in header
        fileWriter << "Reservierung Nr.;Vorname;Nachname;Modell;ausgeliehen am"
                   << endl; // current date and time in header
        for (const Reservation &i: listOfReservations) { // iterates over all reservations and prints a "table row" with the details
            cout << "Reservierung Nr. " << i.id << " gespeichert";
            fileWriter << i.id << ";"
                       << i.customer.first_name.c_str() << ";"
                       << i.customer.last_name.c_str() << ";"
                       << i.model.name.c_str() << ";"
                       << i.created_at.c_str()
                       << endl;
        }
    }

    fileWriter.close();
}


/**
 * Creates the menu
 *
 * @return int
 */
int ReservationManager::menu() {
    int choice;

    clear();

    // Choose action
    cout << "Bitte wählen Sie Ihre Aktion:" << endl << endl;
    cout << "[1] Reservierung erfassen" << endl;
    cout << "[2] Liste aller Reservierungen" << endl;
    cout << "[3] Reservierungsdetails einsehen" << endl;
    cout << "[4] Motorrad herausgeben" << endl;
    cout << "[5] Aktuellen Stand speichern" << endl;
    cout << "[6] Zurück" << endl;
    cout << string(10, '-') << endl;
    cout << "[7] Beispieldaten einfügen" << endl;

    cout << "Was darf es sein? [1-7]: ";
    cin >> choice;

    bool quit = false;

    clear(); // Clear the console

    switch (choice) { // Choose action from menu
        case 1:
            createReservation(); // Create a new member
            break;
        case 2:
            listReservations(); // List all customer
            break;
        case 3:
            reservationDetails(); // Show details of a reservation
            break;
        case 4:
            releaseReservation(); // Release reservation
            break;
        case 5:
            saveReservations(); // Save all to file
            break;
        case 6:
            quit = true; // quit programm
            break;
        case 7:
            generateSampleReservations(); // Adds sample customer to "db"
            break;
        default:
            break;
    }

    return quit;
}

/**
 * Main App Class
 */
class App {
private:
    CustomerManager customerManager;
    ReservationManager reservationManager;
public:
    /**
     * Initializes the App with the Reservation and Customer Manager
     * @param reservationManager
     */
    explicit App(ReservationManager reservationManager) : reservationManager(reservationManager) {
        this->initializeModels();

        this->customerManager = *new CustomerManager();
        this->reservationManager = *new ReservationManager(this->customerManager);
    }

    /**
     * Initial Model DB
     */
    void initializeModels();

    /**
     * Creates the menu
     *
     * @return int
     */
    int menu();
};


/**
 * Initial Model DB
 */
void App::initializeModels() {
    listOfModels.push_back(*new Model(1, "Suzuki Bandit"));
    listOfModels.push_back(*new Model(2, "Honda TransAlp"));
    listOfModels.push_back(*new Model(3, "BMW F 650 GS"));
    listOfModels.push_back(*new Model(4, "Kawasaki ZZR1400"));
}

/**
 * Creates the menu
 *
 * @return int
 */
int App::menu() {
    int choice;

    // Choose action
    cout << "Bitte wählen Sie Ihr Modul:" << endl << endl;
    cout << "[1] Kundenverwaltung" << endl;
    cout << "[2] Reservierungsverwaltung" << endl;
    cout << "[3] Beenden" << endl;

    cout << "Was darf es sein? [1-3]: ";
    cin >> choice;

    bool quit = false;

    if (choice == 1) {
        while (!this->customerManager.menu()) {} // Loops the menu until user quits it
    } else if (choice == 2) {
        while (!this->reservationManager.menu()) {} // Loops the menu until user quits it
    } else {
        quit = true;
    }

    return quit;
}

/**
 * Open menu until quits
 *
 * @return int
 */
int main() {
    App app = *new App(ReservationManager(CustomerManager()));

    while (!app.menu()) { // Loops the menu until the users action is "quit"
        // clear
    }

    return 0;
}