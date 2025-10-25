#ifndef SRC_INCLUDE_APP_SWITCH_H_
#define SRC_INCLUDE_APP_SWITCH_H_

typedef enum {
    ACTION_HOLD = 0,
    ACTION_SINGLE,
    ACTION_DOUBLE,
    ACTION_TRIPLE,
    ACTION_QUADRUPLE,
    ACTION_QUINTUPLE,
    ACTION_CLEAR = 250,
    ACTION_RELEASE = 255,
    ACTION_EMPTY = 300,
} switch_action_t;

void switch_handler();
bool switch_idle();

#endif /* SRC_INCLUDE_APP_SWITCH_H_ */
