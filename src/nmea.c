#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

#include "nmea.h"

void nmea_parse_gpgga(char *nmea, gpgga_t *loc)
{
    char *p = nmea;
    //fprintf(stdout,"GPGGA\n");

    p = strchr(p, ',')+1; //skip time

    p = strchr(p, ',')+1;
    loc->latitude = atof(p);

    p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'N':
            loc->lat = 'N';
            break;
        case 'S':
            loc->lat = 'S';
            break;
        case ',':
            loc->lat = '\0';
            break;
    }

    p = strchr(p, ',')+1;
    loc->longitude = atof(p);

    p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'W':
            loc->lon = 'W';
            break;
        case 'E':
            loc->lon = 'E';
            break;
        case ',':
            loc->lon = '\0';
            break;
    }

    p = strchr(p, ',')+1;
    loc->quality = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->satellites = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;

    p = strchr(p, ',')+1;
    loc->altitude = atof(p);
    loc->valueSet = 'Y';
}

void nmea_parse_gprmc(char *nmea, gprmc_t *loc)
{
    char *p = nmea;
    //fprintf(stdout,"GPRMC\n");

    p = strchr(p, ',')+1; //skip time
    p = strchr(p, ',')+1; //skip status

    p = strchr(p, ',')+1;
    loc->latitude = atof(p);

    p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'N':
            loc->lat = 'N';
            break;
        case 'S':
            loc->lat = 'S';
            break;
        case ',':
            loc->lat = '\0';
            break;
    }

    p = strchr(p, ',')+1;
    loc->longitude = atof(p);

    p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'W':
            loc->lon = 'W';
            break;
        case 'E':
            loc->lon = 'E';
            break;
        case ',':
            loc->lon = '\0';
            break;
    }

    p = strchr(p, ',')+1;
    loc->speed = atof(p);

    p = strchr(p, ',')+1;
    loc->course = atof(p);
    loc->valueSet = 'Y';
}

void nmea_parse_gpgsa(char *nmea, gpgsa_t *loc)
{
    //fprintf(stdout,"GPGSA\n");
    char *p;
    // change * to , to avoid problems with *checksum
    p = strchr(nmea, '*');
    *p = ',';
    p = nmea;

    // skip Satellite status GSA tag
    p = strchr(p, ',')+1;

    p = strchr(p, ',')+1;
    loc->Autoselection = p[0];

    p = strchr(p, ',')+1;
    loc->Dfix = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN01 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN02 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN03 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN04 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN05 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN06 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN07 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN08 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN09 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN10 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN11 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->RPN12 = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    loc->PDOP = atof(p);

    p = strchr(p, ',')+1;
    loc->HDOP = atof(p);

    p = strchr(p, ',')+1;
    loc->VDOP = atof(p);
    loc->valueSet = 'Y';
}


void nmea_parse_gpzda(char *nmea, gpzda_t *loc)
{
    //fprintf(stdout,"GPZDA\n");
    char *p = nmea;
    fprintf(stdout, p); fprintf(stdout,"\n");
    char p2[2];

    p = strchr(p, ',')+1;
    fprintf(stdout, p); fprintf(stdout,"\n");

    p2[0] = p[0];
    p2[1] = p[1];
    loc->hour = (uint8_t)atoi(p2);
    fprintf(stdout, p2); fprintf(stdout,"\n");

    p2[0] = p[2];
    p2[1] = p[3];
    loc->minute = (uint8_t)atoi(p2);

    p2[0] = p[4];
    p2[1] = p[5];
    loc->second = (uint8_t)atoi(p2);

    p2[0] = p[6];
    p2[1] = p[7];
    loc->hundredths = (uint8_t)atoi(p2);

    p = strchr(p, ',')+1;
    loc->day = (uint8_t)atoi(p);
    p = strchr(p, ',')+1;
    loc->month = (uint8_t)atoi(p);
    p = strchr(p, ',')+1;
    loc->year = (uint8_t)atoi(p);
    p = strchr(p, ',')+1;
    loc->local_zone_hours = (uint8_t)atoi(p);
    p = strchr(p, ',')+1;
    loc->local_zone_minutes = (uint8_t)atoi(p);
    loc->valueSet = 'Y';
}

void nmea_parse_gpvtg(char *nmea, gpvtg_t *loc)
{
    //fprintf(stdout,"GPVTG\n");
    char *p = nmea;
    //fprintf(stdout, p); fprintf(stdout,"\n");

    p = strchr(p, ',')+1;
    //fprintf(stdout, p); fprintf(stdout,"\n");
    loc->valueSet = 'Y';
}

void nmea_parse_gpgsv(char *nmea, gpgsv_t *loc)
{
    //fprintf(stdout,"GPGSV\n");
    char *p = nmea;
    //fprintf(stdout, p); fprintf(stdout,"\n");

    p = strchr(p, ',')+1;
    //fprintf(stdout, p); fprintf(stdout,"\n");
    loc->valueSet = 'Y';
}

void nmea_parse_gpgll(char *nmea, gpgll_t *loc)
{
    //fprintf(stdout,"GPGLL\n");
    char *p = nmea;
    //fprintf(stdout, p); fprintf(stdout,"\n");

    p = strchr(p, ',')+1;
    //fprintf(stdout, p); fprintf(stdout,"\n");
    loc->valueSet = 'Y';
}

void nmea_parse_gptxt(char *nmea, gptxt_t *loc)
{
    //fprintf(stdout,"GPTXT\n");
    char *p = nmea;
    //fprintf(stdout, p); fprintf(stdout,"\n");

    p = strchr(p, ',')+1;
    //fprintf(stdout, p); fprintf(stdout,"\n");
    loc->valueSet = 'Y';
}

/**
 * Get the message type (GPGGA, GPRMC, etc..)
 *
 * This function filters out also wrong packages (invalid checksum)
 *
 * @param message The NMEA message
 * @return The type of message if it is valid
 */
uint8_t nmea_get_message_type(const char *message)
{
    uint8_t checksum = 0;

    if (strchr(message, '*') == NULL) { return NMEA_UNKNOWN; }

    if ((checksum = nmea_valid_checksum(message)) != _EMPTY) {
        return checksum;
    }

    if (strstr(message, NMEA_GPGGA_STR) != NULL) {
        return NMEA_GPGGA;
    }

    if (strstr(message, NMEA_GPRMC_STR) != NULL) {
        return NMEA_GPRMC;
    }

    if (strstr(message, NMEA_GPGSA_STR) != NULL) {
        return NMEA_GPGSA;
    }

    if (strstr(message, NMEA_GPZDA_STR) != NULL) {
        return NMEA_GPZDA;
    }

    if (strstr(message, NMEA_GPVTG_STR) != NULL) {
        return NMEA_GPVTG;
    }

    if (strstr(message, NMEA_GPGSV_STR) != NULL) {
        return NMEA_GPGSV;
    }

    if (strstr(message, NMEA_GPGLL_STR) != NULL) {
        return NMEA_GPGLL;
    }

    if (strstr(message, NMEA_GPTXT_STR) != NULL) {
        return NMEA_GPTXT;
    }

    return NMEA_UNKNOWN;
}

uint8_t nmea_valid_checksum(const char *message) {
    uint8_t checksum= (uint8_t)strtol(strchr(message, '*')+1, NULL, 16);

    char p;
    uint8_t sum = 0;
    ++message;
    while ((p = *message++) != '*') {
        sum ^= p;
    }

    if (sum != checksum) {
        return NMEA_CHECKSUM_ERR;
    }

    return _EMPTY;
}

