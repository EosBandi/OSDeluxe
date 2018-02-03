#ifndef _PARAMS_H_
#define _PARAMS_H_

#define PARAM(n, t, p, c) { .name = n, .type = t, .value = (void *) p, .cbk = c }
#define PARAM_END { .name = "" }


struct param_value {
    union {
            float param_float;
            long param_int32;
            unsigned long param_uint32;
            int param_int16;
            unsigned int param_uint16;
            char param_int8;
            unsigned char param_uint8;
    };
};


struct param_def {
    char name[17];
    unsigned char type;
    void *value;
};




#endif