/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"

#define PIN_NUM_MISO_V 19
#define PIN_NUM_MOSI_V 23
#define PIN_NUM_CLK_V  18
#define PIN_NUM_CS_V 5

#define PIN_NUM_MISO_H 12
#define PIN_NUM_MOSI_H 13
#define PIN_NUM_CLK_H  14
#define PIN_NUM_CS_H 15


void app_main()
{
    printf("Hello world!\n");
    esp_err_t ret;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg = {0};
    spi_device_interface_config_t devcfg = {0};
    printf("VSPI\n");
    buscfg.miso_io_num=PIN_NUM_MISO_V;
    buscfg.mosi_io_num=PIN_NUM_MOSI_V;
    buscfg.sclk_io_num=PIN_NUM_CLK_V;
    buscfg.quadwp_io_num=-1;
    buscfg.quadhd_io_num=-1;
    //buscfg.flags |= SPICOMMON_BUSFLAG_MASTER;
    //buscfg.max_transfer_sz=0; // defaults to 4096
    devcfg.clock_speed_hz=10*1000*1000;           
    devcfg.mode=0;                                //SPI mode 0
    devcfg.spics_io_num=PIN_NUM_CS_V;               //CS pin
    devcfg.queue_size=100;                          //synchronous, only queue 16 at a time
    //devcfg.flags |= SPI_DEVICE_HALFDUPLEX;
    //Initialize the SPI bus
    ret=spi_bus_initialize(VSPI_HOST, &buscfg, 1);
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

        uint32_t send_len = len;

        if (!(*sendBuf)) {
            printf("read only\n");
            //sendBuf = 0;
            send_len = len;
        } 
        else {
            //recBuf = 0;
            len = send_len;
        }

        spi_transaction_t transaction = {0};
        if (sendBuf) {
            printf("Send: %x\n", (char) *sendBuf);
        }
        transaction.length = send_len*8;
        transaction.rxlength = len*8; // defaults to len
        transaction.user = (void *) 0;
        transaction.tx_buffer = (void *) sendBuf;
        transaction.rx_buffer = (void *) recBuf;
        ret = spi_device_transmit(spi, &transaction);
        printf("%x\n", ret);
        ESP_ERROR_CHECK(ret);

        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }

}
