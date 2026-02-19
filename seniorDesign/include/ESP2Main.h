#ifndef _ESP2MAIN_H
#define _ESP2MAIN_H

#include "MotorGroup.h"
#include "StateMachine.h"
#include "WebPage.h"

#define SPI_CHILD_INITIALIZE spi_slave_initialize
#define SPI_CHILD_TRANSMIT spi_slave_transmit
typedef spi_slave_transaction_t spi_child_transaction_t;
typedef spi_slave_interface_config_t spi_child_interface_config_t;

class ESP2Main {
public:
    void setup();
    void initMotorGroup();
    void initSPI();
    MOVE_COMMAND recieveMessageFromParent();
    WebPage webServer("ESP32-Access-Point", "123456789");

private:
};

#endif