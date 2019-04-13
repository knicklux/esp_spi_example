/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "esp_system.h"
#include "driver/spi_master.h"

#define PIN_NUM_MISO_V GPIO_NUM_19
#define PIN_NUM_MOSI_V GPIO_NUM_23
#define PIN_NUM_CLK_V  GPIO_NUM_18
#define PIN_NUM_CS_V GPIO_NUM_5

#define PIN_NUM_MISO_H 12
#define PIN_NUM_MOSI_H 13
#define PIN_NUM_CLK_H  14
#define PIN_NUM_CS_H 15


void app_main()
{
    printf("Hello world!\n");

    esp_err_t ret;
    spi_device_handle_t spi;
    memset(&spi, 0, sizeof(spi));
    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(buscfg));
    spi_device_interface_config_t devcfg;
    memset(&devcfg, 0, sizeof(devcfg));

    printf("VSPI\n");

    /*gpio_set_direction(PIN_NUM_MISO_V, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_NUM_MOSI_V, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_CLK_V, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_CS_V, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(PIN_NUM_MISO_V,  GPIO_PULLUP_PULLDOWN);
    gpio_set_pull_mode(PIN_NUM_MOSI_V,  GPIO_PULLUP_PULLDOWN);
    gpio_set_pull_mode(PIN_NUM_CLK_V,  GPIO_PULLUP_PULLDOWN);
    gpio_set_pull_mode(PIN_NUM_CS_V,  GPIO_PULLUP_PULLDOWN);*/

    buscfg.miso_io_num=PIN_NUM_MISO_V;
    buscfg.mosi_io_num=PIN_NUM_MOSI_V;
    buscfg.sclk_io_num=PIN_NUM_CLK_V;
    //buscfg.quadwp_io_num=-1;
    //buscfg.quadhd_io_num=-1;
    buscfg.flags |= SPICOMMON_BUSFLAG_MASTER;
    //buscfg.max_transfer_sz=0; // defaults to 4096

    devcfg.clock_speed_hz=10*1000*1000;           
    devcfg.mode=0;                                //SPI mode 0
    devcfg.spics_io_num=PIN_NUM_CS_V;               //CS pin
    devcfg.queue_size=10;                          //synchronous, only queue 16 at a time
    //devcfg.flags |= SPI_DEVICE_HALFDUPLEX;

    //Initialize the SPI bus
    ret=spi_bus_initialize(VSPI_HOST, &buscfg, 0);
    ESP_ERROR_CHECK(ret);

    //Attach the device to the SPI bus
    ret=spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);


    unsigned int len = 1;
    char send = 5;
    char recv = 0;
    char *sendBuf = &send;
    char *recBuf = &recv;

    while(1) {

        spi_transaction_t transaction;
        memset(&transaction, 0, sizeof(transaction));

        if (sendBuf) {
            printf("Send: %x\n", (char) *sendBuf);
        }

        transaction.length = len*8;
        transaction.rxlength = 0; // defaults to len
        transaction.user = (void *) 0;
        transaction.tx_buffer = (void *) sendBuf;
        transaction.rx_buffer = (void *) recBuf;

        ret = spi_device_transmit(spi, &transaction);
        printf("%x\n", ret);
        ESP_ERROR_CHECK(ret);

        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }

}
