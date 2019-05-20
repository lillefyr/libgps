#include "gps.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "nmea.h"
#include "serial.h"

extern void gps_init(void) {
    serial_init();
    serial_config();

    //Write commands
}

extern void gps_on(void) {
    //Write on
}

// Compute the GPS location using decimal scale
extern void gps_location(loc_t *coord) {
    uint8_t status = _EMPTY;
    while(status != _COMPLETED) {
        gpgga_t gpgga;
        gprmc_t gprmc;
        char buffer[256];

        serial_readln(buffer, 256);
        switch (nmea_get_message_type(buffer)) {
            case NMEA_GPGGA:
                nmea_parse_gpgga(buffer, &gpgga);

                gps_convert_deg_to_dec(&(gpgga.latitude), gpgga.lat, &(gpgga.longitude), gpgga.lon);

                coord->latitude = gpgga.latitude;
                coord->longitude = gpgga.longitude;
                coord->altitude = gpgga.altitude;

                status |= NMEA_GPGGA;
                break;
            case NMEA_GPRMC:
                nmea_parse_gprmc(buffer, &gprmc);

                coord->speed = gprmc.speed;
                coord->course = gprmc.course;

                status |= NMEA_GPRMC;
                break;
        }
    }
}

// Get date and time
extern void gps_get_datetime(int *year, int *month, int *day, int *hour, int *minute, int *second, int *hundredths, int *age) {
    gpzda_t gpzda;
    char buffer[256];

    serial_readln(buffer, 256);
    nmea_parse_gpzda(buffer, &gpzda);

    hour = gpzda.hour;
    minute = gpzda.minute;
    second = gpzda.second;
    hundredths = gpzda.hundredths;
    day = gpzda.day;
    month = gpzda.month;
    year = gpzda.year;
    //gpzda.local_zone_hours; // -13 .. 13
    //gpzda.local_zone_minutes; // 0 .. 59
    age = 0;
}

// get hdop
extern int gps_hdop();
    gpgsa_t gpgsa;
    char buffer[256];

    serial_readln(buffer, 256);
    nmea_parse_gpgga(buffer, &gpgsa);
    return gpgsa.hdop;
}

extern void gps_off(void) {
    //Write off
    serial_close();
}

// Convert lat e lon to decimals (from deg)
void gps_convert_deg_to_dec(double *latitude, char ns,  double *longitude, char we)
{
    double lat = (ns == 'N') ? *latitude : -1 * (*latitude);
    double lon = (we == 'E') ? *longitude : -1 * (*longitude);

    *latitude = gps_deg_dec(lat);
    *longitude = gps_deg_dec(lon);
}

double gps_deg_dec(double deg_point)
{
    double ddeg;
    double sec = modf(deg_point, &ddeg)*60;
    int deg = (int)(ddeg/100);
    int min = (int)(deg_point-(deg*100));

    double absdlat = round(deg * 1000000.);
    double absmlat = round(min * 1000000.);
    double absslat = round(sec * 1000000.);

    return round(absdlat + (absmlat/60) + (absslat/3600)) /1000000;
}

