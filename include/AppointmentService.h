#pragma once

class AppointmentService {
private:
    int id;
    int appointmentId;
    int serviceId;

public:
    AppointmentService() = default;
    AppointmentService(int id, int appointmentId, int serviceId);

    int getId() const;
    void setId(int);

    int getAppointmentId() const;
    void setAppointmentId(int);

    int getServiceId() const;
    void setServiceId(int);

    void create();
    void read();
    void update();
    void remove();
};