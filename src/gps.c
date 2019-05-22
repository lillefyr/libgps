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

// all GPS data
common_t common;

extern void gps_data(void) {
    int cnt = 20;
    int NMEA_Status = 0;
    char buffer[256];

    char buffer2[16]; // unused, debugging bufferoverflow, seems to save the code
    for ( int cnt2=1; cnt2<15; cnt2++ ) { buffer2[cnt2] = 'X'; }
    buffer2[15]= 0;
    buffer2[0]='Y';

    while ((cnt > 0) && ( NMEA_Status != ( NMEA_GPGGA && NMEA_GPRMC && NMEA_GPGSA && NMEA_GPZDA ))) {
        
        //fprintf(stdout, "start read\n"); fflush(stdout);
        serial_readln(buffer, 254);
        //fprintf(stdout, "end read\n"); fflush(stdout);

        // extract the buffers we find
        switch (nmea_get_message_type(buffer)) {
            case NMEA_GPGGA:
                fprintf(stdout, "GPGGA\n"); fflush(stdout);
                nmea_parse_gpgga(buffer, &common.gpgga);
                gps_convert_deg_to_dec(&(common.gpgga.latitude), common.gpgga.lat, &(common.gpgga.longitude), common.gpgga.lon);
                fprintf(stdout, "GPGGA\n"); fflush(stdout);
                NMEA_Status |= NMEA_GPGGA; // set the bit
                break;

            case NMEA_GPRMC:
                fprintf(stdout, "GPRMC\n"); fflush(stdout);
                nmea_parse_gprmc(buffer, &common.gprmc);
                fprintf(stdout, "GPRMC\n"); fflush(stdout);
                NMEA_Status |= NMEA_GPRMC; // set the bit
                break;

            case NMEA_GPGSA:
                fprintf(stdout, "GPGSA\n"); fflush(stdout);
                nmea_parse_gpgsa(buffer, &common.gpgsa);
                fprintf(stdout, "GPGSA\n"); fflush(stdout);
                NMEA_Status |= NMEA_GPGSA; // set the bit
                break;

            case NMEA_GPZDA:
                fprintf(stdout, "GPZDA\n"); fflush(stdout);
                nmea_parse_gpzda(buffer, &common.gpzda);
                fprintf(stdout, "GPZDA\n"); fflush(stdout);
                NMEA_Status |= NMEA_GPZDA; // set the bit
                break;
            default:
                //fprintf(stdout, "NOTHING found\n"); fflush(stdout);
                break;
        }
        cnt--;
//        fprintf(stdout, "remaining %d\n", cnt); fflush(stdout);
//        fprintf(stdout, "buffer2=%s", buffer2); fflush(stdout);
    }
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

// Compute the GPS location using decimal scale
extern void gps_location(loc_t *coord) {
    coord->latitude = common.gpgga.latitude;
    coord->lat = common.gpgga.lat;
    coord->longitude = common.gpgga.lon;
    coord->lon = common.gpgga.longitude;
    coord->altitude = common.gpgga.altitude;
    coord->speed = common.gprmc.speed;
    coord->course = common.gprmc.course;
}

extern void gps_datetime(datetime_t *datetime) {

    datetime->year = common.gpzda.year;
    datetime->month = common.gpzda.month;
    datetime->day = common.gpzda.day;
    datetime->hour = common.gpzda.hour;
    datetime->minute = common.gpzda.minute;
    datetime->second = common.gpzda.second;
    datetime->hundredths = common.gpzda.hundredths;
    datetime->local_zone_hours = common.gpzda.local_zone_hours;
    datetime->local_zone_minutes = common.gpzda.local_zone_minutes;
    datetime->age = 0;
}

// get satellite info
extern void gps_gpgsa(satellitedata_t *sat) {
    sat->Satellitestatus = common.gpgsa.Satellitestatus;
    sat->Autoselection = common.gpgsa.Autoselection;
    sat->Dfix = common.gpgsa.Dfix;
    sat->RPN01 = common.gpgsa.RPN01;
    sat->RPN02 = common.gpgsa.RPN02;
    sat->RPN03 = common.gpgsa.RPN03;
    sat->RPN04 = common.gpgsa.RPN04;
    sat->RPN05 = common.gpgsa.RPN05;
    sat->RPN06 = common.gpgsa.RPN06;
    sat->RPN07 = common.gpgsa.RPN07;
    sat->RPN08 = common.gpgsa.RPN08;
    sat->RPN09 = common.gpgsa.RPN09;
    sat->RPN10 = common.gpgsa.RPN10;
    sat->RPN11 = common.gpgsa.RPN11;
    sat->RPN12 = common.gpgsa.RPN12;
    sat->PDOP = common.gpgsa.PDOP;
    sat->HDOP = common.gpgsa.HDOP;
    sat->VDOP = common.gpgsa.VDOP;
}

