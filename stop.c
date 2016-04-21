#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libbladeRF.h>

int main(int argc, char *argv[])
{
    int status = 0;

    struct bladerf *dev = NULL;
    struct bladerf_devinfo dev_info;

    /* Initialize the information used to identify the desired device
     * to all wildcard (i.e., "any device") values */
    bladerf_init_devinfo(&dev_info);

    /* Request a device with the provided serial number.
     * Invalid strings should simply fail to match a device. */
    if (argc >= 2) {
        fprintf(stdout, "dev_info.serial: %s", dev_info.serial);
        strncpy(dev_info.serial, argv[1], sizeof(dev_info.serial) - 1);
    }

    status = bladerf_open_with_devinfo(&dev, &dev_info);
   
    bladerf_enable_module(dev, BLADERF_MODULE_TX, false);
    bladerf_close(dev);
    return status;
}
