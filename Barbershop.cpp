#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include "Database.h"
#include "Person.h"
#include "Client.h"
#include "Employee.h"
#include "Administrator.h"
#include "Barber.h"
#include "Service.h"
#include "Appointment.h"
#include "AppointmentService.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    Database& db = Database::getInstance();
    db.executeSQLFile("./resources/schema.sql");

    std::vector<Client> clients = {
        Client(0, "Ivan", "I.", "Ivanov", "ivanov@barber.com", 3),
        Client(0, "Petr", "P.", "Petrov", "petrov@barber.com", 5),
        Client(0, "Semen", "S.", "Sidorov", "sidorov@barber.com", 1),
        Client(0, "Alex", "A.", "Alexandrov", "alex@barber.com", 2),
        Client(0, "Max", "M.", "Maximov", "max@barber.com", 4)
    };
    for (auto& client : clients) {
        client.create();
    }

    std::list<Employee> employees = {
        Employee(0, "Andrey", "A.", "Adminov", "andrey@shop.com", "Moscow", "Tverskaya", "1A", 10, "123456789011"),
        Employee(0, "Boris", "B.", "Barberov", "boris@shop.com", "Moscow", "Arbat", "22B", 15, "123456789012"),
        Employee(0, "Vitaly", "V.", "Vladimirov", "vitaly@shop.com", "Moscow", "Lenina", "5C", 8, "123456789013"),
        Employee(0, "Dmitry", "D.", "Dmitriev", "dmitry@shop.com", "Moscow", "Kutuzovsky", "9", 21, "123456789014"),
        Employee(0, "Fedor", "F.", "Fedorov", "fedor@shop.com", "Moscow", "Prospekt Mira", "18", 34, "123456789015")
    };
    for (auto& employee : employees) {
        employee.create();
    }

    std::vector<Barber> barbers = {
        Barber(0, "Boris", "B.", "Barberov", "boris_@shop.com", "Moscow", "Arbat", "22B", 15, "123456789016", 4),
        Barber(0, "Vitaly", "V.", "Vladimirov", "vitaly_@shop.com", "Moscow", "Lenina", "5C", 8, "123456789017", 6),
        Barber(0, "Dmitry", "D.", "Dmitriev", "dmitry_@shop.com", "Moscow", "Kutuzovsky", "9", 21, "123456789018", 3),
        Barber(0, "Fedor", "F.", "Fedorov", "fedor_@shop.com", "Moscow", "Prospekt Mira", "18", 34, "123456789019", 2),
        Barber(0, "Alexey", "A.", "Alekseev", "alexey_@shop.com", "Moscow", "Taganskaya", "3", 1, "123456789020", 5)
    };
    for (auto& barber : barbers) {
        barber.create();
    }

    std::deque<Administrator> admins = {
        Administrator(0, "Andrey", "A.", "Adminov", "andrey_@shop.com", "Moscow", "Tverskaya", "1A", 10, "123456789021", "+79998881111"),
        Administrator(0, "Lena", "L.", "Lenova", "lena_@shop.com", "Moscow", "Volgogradsky", "45", 12, "123456789022", "+79998882222")
    };
    for (auto& admin : admins) {
        admin.create();
    }

    std::vector<Service> services = {
        Service(0, "Haircut", 800),
        Service(0, "Beard Trim", 500),
        Service(0, "Shaving", 400),
        Service(0, "Hair Wash", 300),
        Service(0, "Face Mask", 700)
    };
    for (auto& service : services) {
        service.create();
    }

    std::map<int, Appointment> appointments;
    appointments[1] = Appointment(0, clients[0].getId(), barbers[0].getId(), "2025-04-08 10:00:00+03");
    appointments[2] = Appointment(0, clients[1].getId(), barbers[1].getId(), "2025-04-08 11:00:00+03");
    appointments[3] = Appointment(0, clients[2].getId(), barbers[2].getId(), "2025-04-08 12:00:00+03");
    appointments[4] = Appointment(0, clients[3].getId(), barbers[3].getId(), "2025-04-08 13:00:00+03");
    appointments[5] = Appointment(0, clients[4].getId(), barbers[4].getId(), "2025-04-08 14:00:00+03");
    for (auto& [key, appt] : appointments) {
        appt.create();
    }

    std::vector<AppointmentService> apptServices = {
        AppointmentService(0, appointments[1].getId(), services[0].getId()),
        AppointmentService(0, appointments[2].getId(), services[1].getId()),
        AppointmentService(0, appointments[3].getId(), services[2].getId()),
        AppointmentService(0, appointments[4].getId(), services[3].getId()),
        AppointmentService(0, appointments[5].getId(), services[4].getId())
    };
    for (auto& aps : apptServices) {
        aps.create();
    }

    std::ofstream out("output.txt");

    out << "=== Clients ===\n";
    for (auto& client : clients) {
        client.read();
        out << "Client ID: " << client.getId()
            << ", Name: " << client.getFirstName() << " " << client.getLastName()
            << ", Email: " << client.getEmail()
            << ", Visits: " << client.getNumberOfVisits() << "\n";
    }

    out << "\n=== Employees ===\n";
    for (auto& employee : employees) {
        employee.read();
        out << "Employee ID: " << employee.getId()
            << ", Name: " << employee.getFirstName() << " " << employee.getLastName()
            << ", Email: " << employee.getEmail()
            << ", Address: " << employee.getCity() << ", " << employee.getStreet()
            << " " << employee.getHouseNumber() << ", Apt " << employee.getApartmentNumber()
            << ", INN: " << employee.getINN() << "\n";
    }

    out << "\n=== Barbers ===\n";
    for (auto& barber : barbers) {
        barber.read();
        out << "Barber ID: " << barber.getId()
            << ", Name: " << barber.getFirstName() << " " << barber.getLastName()
            << ", Years of Experience: " << barber.getYearsOfExperience() << "\n";
    }

    out << "\n=== Administrators ===\n";
    for (auto& admin : admins) {
        admin.read();
        out << "Administrator ID: " << admin.getId()
            << ", Name: " << admin.getFirstName() << " " << admin.getLastName()
            << ", Phone: " << admin.getPhoneNumber() << "\n";
    }

    out << "\n=== Services ===\n";
    for (auto& service : services) {
        service.read();
        out << "Service ID: " << service.getId()
            << ", Name: " << service.getName()
            << ", Price: " << service.getPrice() << "\n";
    }

    out << "\n=== Appointments ===\n";
    for (auto& [key, appt] : appointments) {
        appt.read();
        out << "Appointment ID: " << appt.getId()
            << ", Client ID: " << appt.getClientId()
            << ", Barber ID: " << appt.getBarberId()
            << ", Time: " << appt.getTime() << "\n";
    }

    out << "\n=== Appointment Services ===\n";
    for (auto& aps : apptServices) {
        aps.read();
        out << "AppointmentService ID: " << aps.getId()
            << ", Appointment ID: " << aps.getAppointmentId()
            << ", Service ID: " << aps.getServiceId() << "\n";
    }

    out.close();
    return 0;
}