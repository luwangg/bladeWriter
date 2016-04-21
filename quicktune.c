#include "quicktune.h"

int example (struct bladerf *dev, bladerf_module module) {
    /** [example] */
    int status;
    unsigned int i, j, r;

    printf("Entering quick_tune table generation\n");
    struct bladerf_quick_tune quick_tunes[COLS];
    /* Get our quick tune parameters for each frequency we'll be using */
    for (i = 0; i < COLS; i++) {
        status = bladerf_set_frequency (dev, module, frequencies[i]);
        if (status != 0) {
            fprintf (stderr, "Failed to set frequency to %u Hz: %s\n",
                     frequencies[i], bladerf_strerror (status));
            return status;
        }
        status = bladerf_get_quick_tune (dev, module, &quick_tunes[i]);

        if (status != 0) {
            fprintf (stderr, "Failed to get quick tune for %u Hz: %s\n",
                     frequencies[i], bladerf_strerror (status));
            return status;
        }
    }

    printf("+quick_tune table generated\n");
    for (r = 0; r < ROWS; r++) {
        for (i = 0; i < waitRow; i++) {	// This is our middle loop. it is row height.
            for (j = 0; j < COLS; j++){

                //fprintf(stdout, "logo[%u][%u] iter: %u\n", r, j, i);
                if(!csh_logo[r][j]) {
                    status = bladerf_schedule_retune (dev, module, BLADERF_RETUNE_NOW, 0, &quick_tunes[0]);
                }
                else {
                    status = bladerf_schedule_retune (dev, module, BLADERF_RETUNE_NOW, 0, &quick_tunes[j]);
                }
                if (status != 0) return checkStatus (status);
            }
        }
    }

    return status;
}

int main (int argc, char *argv[]) {
//    signal(SIGKILL, *sig_handler);
//    signal(SIGINT, *sig_handler);

    int status = 0;

    printf("Using :");
    if(argc > 1) {
        printf("arguments");
        genTable(argc, argv);
    }
    else {
        printf("default settings");
        defaultTable();
    }
    printf("\n");

    struct bladerf *dev = NULL;
    struct bladerf_devinfo dev_info;

    /* Initialize the information used to identify the desired device
     * to all wildcard (i.e., "any device") values */
    bladerf_init_devinfo (&dev_info);

    /* Request a device with the provided serial number.
     * Invalid strings should simply fail to match a device. */
    //if (argc >= 2) {
    //    fprintf (stdout, "dev_info.serial: %s", dev_info.serial);
    //    strncpy (dev_info.serial, argv[1], sizeof (dev_info.serial) - 1);
    //}

    status = bladerf_open_with_devinfo (&dev, &dev_info);
    if (status != 0) {
        fprintf (stderr, "Unable to open device: %s\n",
                 bladerf_strerror (status));
        return 1;
    }

    /* A quick check that this works is to watch LO leakage on a VSA */

    status = bladerf_enable_module (dev, BLADERF_MODULE_TX, true);
    if (status != 0) {
        fprintf (stderr, "Failed to enable TX module: %s\n",
                 bladerf_strerror (status));
        return status;
    }

    bladerf_set_tuning_mode(dev, BLADERF_TUNING_MODE_FPGA);
    status = example (dev, BLADERF_MODULE_TX);

    bladerf_enable_module (dev, BLADERF_MODULE_TX, false);
    bladerf_close (dev);
    return status;
}

void defaultTable () {
  // table->freqTable = frequencies;
   //table->colWidth  = COLS;
   //table->rowHeight = ROWS;
   //table->decade    = 6;
}

unsigned int checkStatus (int status) {
    return fprintf (stderr, "Failed to apply quick tune: %s\n",
                    bladerf_strerror (status));
}

void sig_handler(int sig) {
    printf("Caught signal %d", sig);
    //bladerf_close(dev);
    abort();
}

void genTable(int argc, char *args[]) {
    int limit, startFreq, endFreq, colWidth, rowHeight, decade, step;

    startFreq = atof(args[1]);
    endFreq   = atof(args[2]);
    colWidth  = COLS;
    rowHeight = ROWS;
    step      = (startFreq-endFreq)/colWidth;
    limit     = 11;
    decade    = 6;

    //for(int a = 1; a < argc; a++) {
    //printf("char: %c\n", (char)*args[1]);
    //printf("a2i: %i\n", atoi(args[1]));

    table = malloc(sizeof(struct freqGroup));
    table->limit     = limit;
    table->startFreq = startFreq;
    table->endFreq   = endFreq;
    table->colWidth  = colWidth;
    table->decade    = decade;

    // allocate array of uint *s as wide as colWidth. Each uint is a frequency.
    table->freqTable = (unsigned int *) malloc( sizeof(unsigned int)*table->colWidth );

    //do table logic here. Should be left with a nice table of all values.
    int i;
    for(i = 0; i < table->colWidth; i++) {
        table->freqTable[i] = ((i * step) + startFreq) * pow(10, decade);
    }
}
