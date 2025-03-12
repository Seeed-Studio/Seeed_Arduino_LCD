#ifdef CHSCX6X_DRIVER
#include "Touch_Drivers/CHSCX6X.h"
#else
public:
bool getTouchRaw(int32_t *x, int32_t *y);

uint16_t getTouchRawZ(void);

void convertRawXY(int32_t *x, int32_t *y);

bool getTouch(int32_t *x, int32_t *y, uint16_t threshold = 600);

void calibrateTouch(uint16_t *data, uint32_t color_fg, uint32_t color_bg, uint8_t size);

void setTouch(uint16_t *data);

#endif