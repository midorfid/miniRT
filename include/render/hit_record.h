#ifndef HIT_RECORD_H
#define HIT_RECORD_H

typedef struct hit_record_s {
    point3_t    p;
    vec3_t      normal;
    double      t;
} hit_record_t;



#endif
