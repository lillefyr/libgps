#ifndef _NMEA_H_
#define _NMEA_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define _EMPTY 0x00
#define NMEA_GPRMC 0x01
#define NMEA_GPRMC_STR "$GPRMC"
#define NMEA_GPGGA 0x02
#define NMEA_GPGGA_STR "$GPGGA"
#define NMEA_GPGSA 0x03
#define NMEA_GPGSA_STR "$GPGSA"
#define NMEA_GPZDA 0x04
#define NMEA_GPZDA_STR "$GPZDA"
#define NMEA_UNKNOWN 0x00
#define _COMPLETED 0x03

#define NMEA_CHECKSUM_ERR 0x80
#define NMEA_MESSAGE_ERR 0xC0

struct gpgga {
    // Latitude eg: 4124.8963 (XXYY.ZZKK.. DEG, MIN, SEC.SS)
    double latitude;
    // Latitude eg: N
    char lat;
    // Longitude eg: 08151.6838 (XXXYY.ZZKK.. DEG, MIN, SEC.SS)
    double longitude;
    // Longitude eg: W
    char lon;
    // Quality 0, 1, 2
    uint8_t quality;
    // Number of satellites: 1,2,3,4,5...
    uint8_t satellites;
    // Altitude eg: 280.2 (Meters above mean sea level)
    double altitude;
};
typedef struct gpgga gpgga_t;

struct gprmc {
    double latitude;
    char lat;
    double longitude;
    char lon;
    double speed;
    double course;
};
typedef struct gprmc gprmc_t;

struct gpzda {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t hundredths;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t local_zone_hours; // -13 .. 13
    uint8_t local_zone_minutes; // 0 .. 59
};
typedef struct gpzda gpzda_t;

struct gpgsa {
     uint8_t Satellitestatus;
     uint8_t Autoselection;
     uint8_t Dfix;
     uint8_t RPN01;
     uint8_t RPN02;
     uint8_t RPN03;
     uint8_t RPN04;
     uint8_t RPN05;
     uint8_t RPN06;
     uint8_t RPN07;
     uint8_t RPN08;
     uint8_t RPN09;
     uint8_t RPN10;
     uint8_t RPN11;
     uint8_t RPN12;
     double PDOP;
     double HDOP;
     double VDOP;
};
typedef struct gpgsa gpgsa_t;

uint8_t nmea_get_message_type(const char *);
uint8_t nmea_valid_checksum(const char *);
void nmea_parse_gpgga(char *, gpgga_t *);
void nmea_parse_gprmc(char *, gprmc_t *);
void nmea_parse_gpgsa(char *, gpgsa_t *);
void nmea_parse_gpzda(char *, gpzda_t *);

#endif

