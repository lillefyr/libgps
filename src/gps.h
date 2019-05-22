#ifndef _GPS_H_
#define _GPS_H_

struct location {
    double latitude;
    double longitude;
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

union common_u {
    loc_t location;
    datetime_t gpsdatetime;
    satellitedata_t satellitedata;
};
typedef union common_u common_t;

// Initialize device
extern void gps_init(void);
// Activate device
extern void gps_on(void);
// Get the actual location
extern void gps_location(common_t *);
// Get date and time
extern void gps_get_datetime(common_t *);
// get hdop
extern void gps_gpgsa(common_t *);


// Turn off device (low-power consumption)
extern void gps_off(void);

// -------------------------------------------------------------------------
// Internal functions
// -------------------------------------------------------------------------

// convert deg to decimal deg latitude, (N/S), longitude, (W/E)
void gps_convert_deg_to_dec(double *, char, double *, char);
double gps_deg_dec(double);

#endif
