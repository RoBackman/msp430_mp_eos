typedef enum { OUT_OF_ORDER, RED, RED_AND_AMBER, GREEN, AMBER } light_state;

void trafic_lights_init(const light_state START_STATE);
void trafic_lights_update(void);
void trafic_lights_uart(char message);
