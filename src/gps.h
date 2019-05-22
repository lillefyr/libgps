#ifndef _GPS_H_
#define _GPS_H_

#include <nmea.h>

struct location {
    double latitude;
    char   lat;
    double longitude;
    char   lon;
    double speed;
    double altitude;
    double course;
};
typedef struct location loc_t;


struct gpsdatetime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int hundredths;
    int local_zone_hours;
    int local_zone_minutes;
    int age;
};
typedef struct gpsdatetime datetime_t;

struct satellitedata {
    int Satellitestatus;
    int Autoselection;
    int Dfix;
    int RPN01;
    int RPN02;
    int RPN03;
    int RPN04;
    int RPN05;
    int RPN06;
    int RPN07;
    int RPN08;
    int RPN09;
    int RPN10;
    int RPN11;
    int RPN12;
    double PDOP;
    double HDOP;
    double VDOP;
};
typedef struct satellitedata satellitedata_t;

struct common {
    gpgga_t gpgga;
    gprmc_t gprmc;
    gpzda_t gpzda;
    gpgsa_t gpgsa;
};
typedef struct common common_t;

//    loc_t location;
//    datetime_t gpsdatetime;
//    satellitedata_t satellitedata;

// Initialize device
extern void gps_init(void);
// Activate device
extern void gps_on(void);
// get actual values
extern void gps_data(void);

// Get the actual location
extern void gps_location(loc_t *);
// Get date and time
extern void gps_datetime(datetime_t *);
// get hdop
extern void gps_gpgsa(satellitedata_t *);


// Turn off device (low-power consumption)
extern void gps_off(void);

// -------------------------------------------------------------------------
// Internal functions
// -------------------------------------------------------------------------

// convert deg to decimal deg latitude, (N/S), longitude, (W/E)
void gps_convert_deg_to_dec(double *, char, double *, char);
double gps_deg_dec(double);

#endif
