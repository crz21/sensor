#ifndef __WIFI_H__
#define __WIFI_H__

typedef enum
{
    SET_AP_MODE = 0,
    SET_AP_INFO,
    QUERY_IP,
    START_MUX_LINK,
    START_SERVER,
};

void wifi_init(void);

#endif



