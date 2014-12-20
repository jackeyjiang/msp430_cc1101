#define moto_1st 0x01
#define moto_2nd 0x02
typedef enum {false = 0, true = 1} bool;
extern void PWM_set(INT8U moto_num,INT16U pwm_t);