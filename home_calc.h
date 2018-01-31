#ifndef _HOME_CALC_H_
#define _HOME_CALC_H_

/* constants */

#define EARTH_RADIUS                (6367449) /* meters */
#define RAD2DEG(x)                  ((x) * (180.0 / PI))
#define DEG2RAD(x)                  ((x) * (PI / 180.0))

#define M2FEET                      (3.2808399)
#define MILE2FEET                   (5280)
#define M2MILE                      (M2FEET / MILE2FEET)

#define CELSIUS2FAHR(x)             ((x * 18)/10 + 3200)

#define TRIM(_v, _b, _t)    ((_v < _b) ? _b : ((_v > _t) ? _t : _v))

#define ToRad(x) (x*0.01745329252)	// *pi/180
#define ToDeg(x) (x*57.2957795131) // *180/pi


#define HOME_CALC_INTERVAL 500          //Calling home calculation in (x)ms

enum home_lock {
    HOME_NONE = 0,
    HOME_WAIT,
    HOME_GOT,
    HOME_LOCKED,
    HOME_RESET,
    HOME_FORCE,
};

struct gps_coord_t {
    float lat, lon;
};

struct home_data_t {
    int altitude;
    int direction, uav_bearing;
    float distance;
    home_lock lock;
    unsigned long last_calc;
    gps_coord_t home_coord;
    int home_alt;
    gps_coord_t uav_coord;
    int uav_alt;
    int uav_heading;
};

float earth_distance(struct gps_coord_t *c1, struct gps_coord_t *c2);
float get_bearing(struct gps_coord_t *c1, struct gps_coord_t *c2);

void init_home(void);
void calc_home(void);



#endif