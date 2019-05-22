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

void gps_data(int datarequested, common_t *common) {
    uint8_t status = _EMPTY;
    while(status != _COMPLETED) {
        gpgga_t gpgga;
        gprmc_t gprmc;
        gpgsa_t gpgsa;
        gpzda_t gpzda;
        char buffer[256];

        serial_readln(buffer, 256);
        switch (nmea_get_message_type(buffer)) {
            case NMEA_GPGGA:
                nmea_parse_gpgga(buffer, &gpgga);

                gps_convert_deg_to_dec(&(gpgga.latitude), gpgga.lat, &(gpgga.longitude), gpgga.lon);

                common->location.latitude = gpgga.latitude;
                common->location.longitude = gpgga.longitude;
                common->location.altitude = gpgga.altitude;

                if ( datarequested == NMEA_GPGGA ) { status |= NMEA_GPGGA; }
                break;
            case NMEA_GPRMC:
                nmea_parse_gprmc(buffer, &gprmc);

                common->location.speed = gprmc.speed;
                common->location.course = gprmc.course;

                if ( datarequested == NMEA_GPRMC ) { status |= NMEA_GPRMC; }
                break;

            case NMEA_GPGSA:
                nmea_parse_gpgsa(buffer, &gpgsa);
                if ( datarequested == NMEA_GPGSA ) { status |= NMEA_GPGSA; }
                break;

            case NMEA_GPZDA:
                nmea_parse_gpzda(buffer, &gpzda);

                common->gpsdatetime.hour = gpzda.hour;
                common->gpsdatetime.minute = gpzda.minute;
                common->gpsdatetime.second = gpzda.second;
                common->gpsdatetime.hundredths = gpzda.hundredths;
                common->gpsdatetime.day = gpzda.day;
                common->gpsdatetime.month = gpzda.month;
                common->gpsdatetime.year = gpzda.year;
                common->gpsdatetime.local_zone_hours = gpzda.local_zone_hours; // -13 .. 13
                common->gpsdatetime.local_zone_minutes = gpzda.local_zone_minutes; // 0 .. 59
                common->gpsdatetime.age = 0;
                if ( datarequested == NMEA_GPZDA ) { status |= NMEA_GPZDA; }
                break;
        }
    }
}

// Compute the GPS location using decimal scale
extern void gps_location(common_t *coord) {
    gps_data(NMEA_GPGGA, coord);
    gps_data(NMEA_GPRMC, coord);
}

extern void gps_get_datetime(common_t *datetime) {
    gps_data(NMEA_GPZDA, datetime);
}

// get hdop
extern void gps_hdop(common_t *sat) {
    gps_data(NMEA_GPGSA, sat);
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

