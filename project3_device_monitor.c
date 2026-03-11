/*
 * ============================================================
 *  PROJECT 3: Callback-Based Device Monitoring Simulator
 *  Student ID: jad6aR86d7N715dy
 *  Device Types : Temperature Sensor, Pressure Gauge,
 *                 Battery Monitor, Fan Speed Monitor
 *  Custom CB    : fan_speed_monitor()
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TYPE_TEMPERATURE  0
#define TYPE_PRESSURE     1
#define TYPE_BATTERY      2
#define TYPE_FAN_SPEED    3


struct Device {
    char name[30];
    int  type;
    union {
        float temperature;  
        int   pressure;     
        float voltage;     
        int   rpm;          
    } reading;
};



void temperature_monitor(struct Device *d) {
    float t = d->reading.temperature;
    const char *status;
    if      (t < 0)   status = "FREEZING  ❄";
    else if (t < 25)  status = "Normal    ✓";
    else if (t < 60)  status = "Warm      ⚠";
    else              status = "OVERHEAT  🔥";
    printf("  [TEMP ]  %-22s  %6.1f C     %s\n", d->name, t, status);
}

void pressure_monitor(struct Device *d) {
    int p = d->reading.pressure;
    const char *status;
    if      (p < 80)   status = "LOW       ⬇";
    else if (p <= 120) status = "Normal    ✓";
    else               status = "HIGH      ⬆";
    printf("  [PRES ]  %-22s  %6d kPa   %s\n", d->name, p, status);
}

void battery_monitor(struct Device *d) {
    float v = d->reading.voltage;
    const char *status;
    if      (v >= 4.0f)  status = "Full      🔋";
    else if (v >= 3.5f)  status = "Good      ✓";
    else if (v >= 3.0f)  status = "Low       ⚠";
    else                 status = "CRITICAL  🪫";
    printf("  [BATT ]  %-22s  %6.2f V     %s\n", d->name, v, status);
}


void fan_speed_monitor(struct Device *d) {
    int rpm = d->reading.rpm;
    const char *status;
    if      (rpm == 0)    status = "OFF / STALLED ✗";
    else if (rpm < 800)   status = "Too Slow      ⬇";
    else if (rpm <= 3000) status = "Normal        ✓";
    else                  status = "Overspeeding  ⬆";
    printf("  [FAN  ]  %-22s  %6d RPM   %s\n", d->name, rpm, status);
}


void process_device(struct Device *d, void (*callback)(struct Device *)) {
    callback(d);
}


void generate_reading(struct Device *d) {
    switch (d->type) {
        case TYPE_TEMPERATURE:
            d->reading.temperature = -10.0f + (float)(rand() % 900) / 10.0f;
            break;
        case TYPE_PRESSURE:
            d->reading.pressure = 60 + rand() % 100;
            break;
        case TYPE_BATTERY:
            d->reading.voltage = 2.5f + (float)(rand() % 20) / 10.0f;
            break;
        case TYPE_FAN_SPEED:
            /* occasionally simulate a stalled fan (rpm = 0) */
            d->reading.rpm = (rand() % 10 == 0) ? 0 : 400 + rand() % 3200;
            break;
    }
}


typedef void (*cb_fn)(struct Device *);

cb_fn callbacks[] = {
    temperature_monitor, 
    pressure_monitor,    
    battery_monitor,    
    fan_speed_monitor    
};


void display_all(struct Device *arr, int n) {
    printf("\n  %-5s  %-22s  %-10s  %s\n", "Idx", "Name", "Type", "Reading & Status");
    printf("  %s\n", "---------------------------------------------------------------------");
    struct Device *ptr = arr;
    for (int i = 0; i < n; i++, ptr++) {
        printf("  [%2d]  ", i);
        process_device(ptr, callbacks[ptr->type]);
    }
}


void simulate_readings(struct Device *arr, int n) {
    printf("\n  Simulating 10 random device readings...\n");
    printf("  %s\n", "---------------------------------------------------------------------");
    for (int i = 0; i < 10; i++) {
        int idx = rand() % n;
        struct Device *d = arr + idx;
        generate_reading(d);
        printf("  Reading %2d:  ", i + 1);
        process_device(d, callbacks[d->type]);
    }
}


void summary_stats(struct Device *arr, int n) {
    int   tc=0, pc=0, bc=0, fc=0;
    float t_sum=0, bv_sum=0;
    int   p_sum=0, rpm_sum=0;

    struct Device *ptr = arr;
    for (int i = 0; i < n; i++, ptr++) {
        switch (ptr->type) {
            case TYPE_TEMPERATURE: tc++; t_sum   += ptr->reading.temperature; break;
            case TYPE_PRESSURE:    pc++; p_sum   += ptr->reading.pressure;    break;
            case TYPE_BATTERY:     bc++; bv_sum  += ptr->reading.voltage;     break;
            case TYPE_FAN_SPEED:   fc++; rpm_sum += ptr->reading.rpm;         break;
        }
    }
    printf("\n  Summary Statistics:\n");
    if (tc) printf("    Avg Temperature : %.1f C    (%d sensors)\n",  t_sum/tc,          tc);
    if (pc) printf("    Avg Pressure    : %.0f kPa  (%d gauges)\n",   (float)p_sum/pc,   pc);
    if (bc) printf("    Avg Battery     : %.2f V    (%d monitors)\n", bv_sum/bc,         bc);
    if (fc) printf("    Avg Fan Speed   : %.0f RPM  (%d fans)\n",     (float)rpm_sum/fc, fc);
}


void pointer_traversal_demo(struct Device *arr, int n) {
    printf("\n  Pointer traversal — memory address of each device:\n");
    struct Device *ptr = arr;
    for (int i = 0; i < n; i++, ptr++) {
        printf("    devices[%2d]  @  %p  ->  %-22s  type=%d\n",
               i, (void *)ptr, ptr->name, ptr->type);
    }
}

/*
 *  MAIN */
int main(void) {
    setbuf(stdout, NULL);
    srand((unsigned)time(NULL));

    printf("\n");
    printf("  +----------------------------------------------------------+\n");
    printf("  |     CALLBACK-BASED DEVICE MONITORING SIMULATOR          |\n");
    printf("  |     Student ID : jad6aR86d7N715dy                       |\n");
    printf("  |     Custom CB  : Fan Speed Monitor                      |\n");
    printf("  +----------------------------------------------------------+\n");

    /* dynamically allocate memory for 12 devices */
    int n = 12;
    struct Device *devices = malloc(n * sizeof(struct Device));
    if (!devices) { fprintf(stderr, "Memory allocation failed\n"); return 1; }

    /* device names and types */
    const char *dnames[] = {
        "Server Room A", "Server Room B", "Boiler Unit",   "Compressor 1",
        "Phone Bank A",  "Phone Bank B",  "CPU Fan",       "Case Fan 1",
        "Cold Storage",  "Engine Bay",    "UPS Bank",      "Case Fan 2"
    };
    int dtypes[] = {
        TYPE_TEMPERATURE, TYPE_TEMPERATURE, TYPE_PRESSURE, TYPE_PRESSURE,
        TYPE_BATTERY,     TYPE_BATTERY,     TYPE_FAN_SPEED, TYPE_FAN_SPEED,
        TYPE_TEMPERATURE, TYPE_PRESSURE,    TYPE_BATTERY,   TYPE_FAN_SPEED
    };

    /* use pointer traversal to initialise each device */
    struct Device *ptr = devices;
    for (int i = 0; i < n; i++, ptr++) {
        strncpy(ptr->name, dnames[i], 29);
        ptr->name[29] = '\0';
        ptr->type = dtypes[i];
        generate_reading(ptr);
    }

    printf("\n  %d devices allocated at memory address: %p\n", n, (void *)devices);

    int choice;
    do {
        printf("\n  +--------------------------------------+\n");
        printf("  |             MAIN MENU               |\n");
        printf("  +--------------------------------------+\n");
        printf("  |  1. Display All Devices             |\n");
        printf("  |  2. Simulate 10 Random Readings     |\n");
        printf("  |  3. Refresh All Readings            |\n");
        printf("  |  4. Summary Statistics              |\n");
        printf("  |  5. Pointer Traversal Demo          |\n");
        printf("  |  0. Exit                            |\n");
        printf("  +--------------------------------------+\n");
        printf("  Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: display_all(devices, n);             break;
            case 2: simulate_readings(devices, n);        break;
            case 3:
                ptr = devices;
                for (int i = 0; i < n; i++, ptr++) generate_reading(ptr);
                printf("\n  All readings refreshed.\n");
                break;
            case 4: summary_stats(devices, n);           break;
            case 5: pointer_traversal_demo(devices, n);  break;
            case 0: break;
            default: printf("  Invalid option. Please enter 0-5.\n");
        }
    } while (choice != 0);

    free(devices);
    printf("\n  Memory released. Goodbye.\n");
    return 0;
}