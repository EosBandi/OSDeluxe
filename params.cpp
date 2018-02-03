#include "osdeluxe.h"

#define MAX_CONFIG_PARAMS   (50)

/*
const struct param_def params_mavlink[] = {
    PARAM("MAV_UAVSYSID", MAV_PARAM_TYPE_UINT8, &config.mav.uav_sysid, NULL),
    PARAM("MAV_OSDSYSID", MAV_PARAM_TYPE_UINT8, &config.mav.osd_sysid, NULL),
    PARAM("MAV_HRTBEAT", MAV_PARAM_TYPE_UINT8, &config.mav.heartbeat, NULL),
    PARAM_END,
};
*/





const char *mavdata_type_name[] = {
    "CHAR", "UINT8_T", "INT8_T",
    "UINT16_T", "INT16_T",
    "UINT32_T", "INT32_T",
    "UINT64_T", "INT64_T",
    "FLOAT", "DOUBLE"
};


float cast2float(void *value, unsigned char type)
{
    switch (type) {
        case MAV_PARAM_TYPE_UINT8:
            return (float) *((unsigned char*) (value));
        case MAV_PARAM_TYPE_INT8:
            return (float) *((char*) (value));
        case MAV_PARAM_TYPE_UINT16:
            return (float) *((unsigned int*) (value));
        case MAV_PARAM_TYPE_INT16:
            return (float) *((int*) (value));
        case MAV_PARAM_TYPE_REAL32:
            return (float) *((float*) (value));
        default:
            return 0;
    }
}

static void cast2param(struct param_def *p, float v)
{
    switch (p->type) {
        case MAV_PARAM_TYPE_UINT8:
            *((unsigned char*) (p->value)) = (unsigned char) v;
            break;
        case MAV_PARAM_TYPE_INT8:
            *((char*) (p->value)) = (char) v;
            break;
        case MAV_PARAM_TYPE_UINT16:
            *((unsigned int*) (p->value)) = (unsigned int) v;
            break;
        case MAV_PARAM_TYPE_INT16:
            *((int*) (p->value)) = (int) v;
            break;
        case MAV_PARAM_TYPE_REAL32:
            *((float*) (p->value)) = (float) v;
            break;
        default:
            break;
    }
}
