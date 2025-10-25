#ifndef SRC_INCLUDE_APP_ONOFF_H_
#define SRC_INCLUDE_APP_ONOFF_H_

void cmdOnOff_toggle(uint8_t ep);
void cmdOnOff_on(uint8_t ep);
void cmdOnOff_off(uint8_t ep);
void remoteCmdOnOff(uint8_t ep, uint8_t cmd);

#endif /* SRC_INCLUDE_APP_ONOFF_H_ */
