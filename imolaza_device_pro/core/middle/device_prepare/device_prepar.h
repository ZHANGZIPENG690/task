#if !defined(DEVICE_PREPAR_H)
#define DEVICE_PREPAR_H
// #include "esp_err.h"
// #include "nvs.h"
// #include "nvs_flash.h"
// #include "string.h"
#include "core/fset_lib.h"
// #include "../../../drive/drive.h"

#define DEVICETYPE_HEAD "N.{"
#define DEVICETYPE_END "}.N"
#define SERVERNUM_HEAD "SN.{"
#define SERVERNUM_END "}.SN"
#define ENCRYPTED_KEY_HEAD "KEY.{"
#define ENCRYPTED_KEY_END "}.KEY"

#define NVS_KEY_SN "*SN"
#define NVS_KEY_EN_KEY "*encrKey"

#define TEMP_WORD_SYSTEM_DETYPE "dev_type"

typedef struct
{
    char sn[15];
    char key[35];
} device_prepar;

stat_m m_cal_device_prepar_set(device_prepar device_prepar);


#endif // DEVICE_PREPAR-H
