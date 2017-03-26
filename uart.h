#define TXD BIT2
#define RXD BIT1

void uart_init(void);
void uart_write_char(char byte);
void uart_write(char *str);
char uart_read(void);