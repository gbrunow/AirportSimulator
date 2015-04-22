/* External definitions for air traffic control system using simlib. */
#include "simlib.h"             /* Required for use of simlib.c. */
#include <string.h>

#define EVENT_ARRIVE_GATE       1  /* Event type for new call arrival. */
#define EVENT_ARRIVE_RUNWAY     2
#define EVENT_DEPART_RUNWAY     3

#define STREAM_GATE         1  /* Random-number stream for new arrivals. */
#define STREAM_RUNWAY       2  /* Random-number stream for handoff arrivals. */
#define STREAM_MAINTENANCE  3

#define TIMEST_UTIL      1    //for tracking time statistics

#define true             1
#define false            0

#define INTER_ARRIVAL_TIME  0
#define FREQ_MAINTENANCE    1
#define NUM_GATES           2
#define TRANSITION_TIME     3
#define MAINTENANCE_TIME    4
#define RUNWAY_TIME         5

#define LINES_NUMBER        11

#define TOTAL_DELAY         6
#define MAINTENANCE_DELAY   7
#define GATES_DELAY         8
#define RUNWAY_DELAY        9
#define DELAY_PER_PLANE     10

/* Declare non-simlib global variables. */

int   numGates, numGatesAvailable, numPlanes, numGateQ, numRunwayQ, runwayAvailable, simulationSize, varToVary;
float mst_transition, mst_maintenance, mst_runway, maintenanceFreq, interArrivalTime, time_last_event,
totalDelay, delayMaintenance, delayGates, delayRunway, avgDelayPerPlane, increment;
FILE  *infile, *outfile;

char *line[LINES_NUMBER];

/* Declare non-simlib functions. */

void init_model(void);
void scheduleArriveRunway(void);
void arriveGate(void);
void arriveRunway(void);
void departRunway(void);
void updateDelays(void);
void report(int);
void fillLine(int, float);
void closeLine(int);


int main()  /* Main function. */
//return 0 if successful
//return 1 if error loading files
//return 2 if error parsing input file
{
    /* Open input and output files. */
    //Make sure they are opened correctly
    if ((infile  = fopen("infile.in",  "r")) == false){
        printf("Error loading infile.in\n");
        return 1;
    }
    if ((outfile = fopen("outfile.m", "w")) == false){
        printf("Error loading outfile.m\n");
        return 1;
    }
    
    /* Read input parameters. */
    
    if (fscanf(infile, "%f %f %d %f %f %f %d %d %f", &interArrivalTime, &maintenanceFreq, &numGates, &mst_transition, &mst_maintenance, &mst_runway, &simulationSize, &varToVary, &increment) != 9) {
        printf("Error parsing infile.in\n");
        return 2;
    }
    
    line[INTER_ARRIVAL_TIME] = "interArrivalTime = [ ";
    line[FREQ_MAINTENANCE] = "maintenanceFreq = [ ";
    line[NUM_GATES] = "numGates = [ ";
    line[TRANSITION_TIME] = "mst_transition = [ ";
    line[MAINTENANCE_TIME] = "mst_maintenance = [ ";
    line[RUNWAY_TIME] = "mst_runway = [ ";
    
    line[TOTAL_DELAY] = "totalDelay = [ ";
    line[MAINTENANCE_DELAY] = "delayMaintenance = [ ";
    line[GATES_DELAY] = "delayGates = [ ";
    line[RUNWAY_DELAY] = "delayRunway = [ ";
    line[DELAY_PER_PLANE] = "avgDelayPerPlane = [ ";
    
    if(varToVary != INTER_ARRIVAL_TIME)
        fillLine(INTER_ARRIVAL_TIME, interArrivalTime);
    
    if(varToVary != FREQ_MAINTENANCE)
        fillLine(FREQ_MAINTENANCE, maintenanceFreq);
    
    if(varToVary != NUM_GATES)
        fillLine(NUM_GATES, numGates);
    
    if(varToVary != TRANSITION_TIME)
        fillLine(TRANSITION_TIME, mst_transition);
    
    if(varToVary != MAINTENANCE_TIME)
        fillLine(MAINTENANCE_TIME, mst_maintenance);
    
    if(varToVary != RUNWAY_TIME)
        fillLine(RUNWAY_TIME, mst_runway);
    
    /* Initialize simlib */
    int sim = 0;
    do
    {
        init_simlib();
        
        /* Set maxatr = max(maximum number of attributes per record, 4) */
        
        maxatr = 4;  /* NEVER SET maxatr TO BE SMALLER THAN 4. */
        
        /* Initialize the model. */
        
        init_model();
        
        /* Run the simulation while more calls are still needed. */
        
        while (numPlanes <= 10000) {
            
            /* Determine the next event. */
            
            timing();
            
            updateDelays();
            
            /* Invoke the appropriate event function. */
            
            switch (next_event_type) {
                case EVENT_ARRIVE_GATE:
                    arriveGate();
                    break;
                case EVENT_ARRIVE_RUNWAY:
                    arriveRunway();
                    break;
                case EVENT_DEPART_RUNWAY:
                    departRunway();
                    break;
            }
            
        }
        
        avgDelayPerPlane = (float)((float)totalDelay / (float)numPlanes);
        /* Invoke the report generator and end the current simulation. */
        report(varToVary);
        
        switch (varToVary){
            case INTER_ARRIVAL_TIME:
                interArrivalTime += increment;
                break;
            case FREQ_MAINTENANCE:
                maintenanceFreq += increment;
                break;
            case NUM_GATES:
                numGates += increment;
                break;
            case TRANSITION_TIME:
                mst_transition += increment;
                break;
            case MAINTENANCE_TIME:
                mst_maintenance += increment;
                break;
            case RUNWAY_TIME:
                mst_runway += increment;
                break;
        }
        ++sim;
    }while(sim < simulationSize);
    
    for(int i = 0; i<LINES_NUMBER; i++){
        closeLine(i);
    }
    
    fprintf(outfile,"clc;\n");
    fprintf(outfile,"clear all;\n");
    
    //Write all lines in file
    for(int i=0; i<LINES_NUMBER; i++){
        fprintf(outfile,line[i]);
    }
    
    fclose(infile);
    fclose(outfile);
    printf("Finished\n");
    
    return 0;
}


void init_model(void)  /* Initialization function. */
{
    numPlanes = 0;
    numGatesAvailable = numGates;
    delayRunway = 0;
    delayGates = 0;
    delayMaintenance = 0;
    totalDelay = 0;
    numGateQ = 0;
    numRunwayQ = 0;
    time_last_event = 0;
    
    event_schedule(sim_time + expon(1/interArrivalTime, STREAM_GATE), EVENT_ARRIVE_GATE);
    
}

void scheduleArriveRunway(void){
    float timeToRunway = 0;
    float random = (float)rand()/ (float)RAND_MAX;
    if(random <= maintenanceFreq){
        float thisMaintenanceDelay = expon(mst_maintenance, STREAM_MAINTENANCE);
        timeToRunway += thisMaintenanceDelay;
        delayMaintenance += thisMaintenanceDelay;
    }
    timeToRunway += expon(mst_transition, STREAM_GATE);
    
    event_schedule(sim_time + timeToRunway, EVENT_ARRIVE_RUNWAY);
}


void arriveGate(void)  /* Arrival event function. */
{
    /* Schedule next arrival. */
    event_schedule(sim_time + expon(1/interArrivalTime, STREAM_GATE), EVENT_ARRIVE_GATE);
    
    //increment counters
    ++numPlanes;
    
    //if a gate is available: reduce num available gates, schedule finish event
    if (numGatesAvailable > 0){
        --numGatesAvailable;
        scheduleArriveRunway();
    }
    //else: increment gate queue
    else{
        ++numGateQ;
    }
    
}

void arriveRunway(void)
{
    ++numGatesAvailable;
    if(numGateQ > 0){
        --numGateQ;
        --numGatesAvailable;
    }
    
    if(runwayAvailable == true){
        runwayAvailable = false;
        event_schedule(sim_time + expon(mst_runway, STREAM_RUNWAY), EVENT_DEPART_RUNWAY);
    }
    else{
        ++numRunwayQ;
    }
}


void departRunway(void)  /* Departure event function. */
{
    if(numRunwayQ > 0){
        --numRunwayQ;
        event_schedule(sim_time + expon(mst_runway, STREAM_RUNWAY), EVENT_DEPART_RUNWAY);
    }
    else{
        runwayAvailable = true;
    }
}

void updateDelays(void)  /* Update area accumulators for time-average
                          statistics. */
{
    float time_since_last_event;
    
    /* Compute time since last event, and update last-event-time marker. */
    
    time_since_last_event = sim_time - time_last_event;
    time_last_event       = sim_time;
    
    /* Update area under number-in-queue function. */
    delayGates += numGateQ * time_since_last_event;
    delayRunway += numRunwayQ * time_since_last_event;
    totalDelay += (numGateQ + numRunwayQ) * time_since_last_event;
    
}

void fillLine(int ln, float value){
    char *str;
    char str2[100];
    
    sprintf(str2,"%.5f ", value);
    str = &str2[0];
    
    size_t len1 = strlen(line[ln]), len2 = strlen(str);
    char *concat = (char*) malloc(len1 + len2 + 1);
    
    memcpy(concat, line[ln], len1);
    memcpy(concat+len1, str, len2+1);
    
    line[ln] = concat;
    
}

void closeLine(int ln){
    
    char *str = "];\n\n";
    
    size_t len1 = strlen(line[ln]), len2 = strlen(str);
    char *concat = (char*) malloc(len1 + len2 + 1);
    
    memcpy(concat, line[ln], len1);
    memcpy(concat+len1, str, len2+1);
    
    line[ln] = concat;
}


void report(int varyingInput)  /* Report generator function. */
{
    //negative puts stats into transfer; transfer[1] is time average
    switch (varToVary){
        case INTER_ARRIVAL_TIME:
            fillLine(varyingInput, interArrivalTime);
            break;
        case FREQ_MAINTENANCE:
            fillLine(varyingInput, maintenanceFreq);
            break;
        case NUM_GATES:
            fillLine(varyingInput, numGates);
            break;
        case TRANSITION_TIME:
            fillLine(varyingInput, mst_transition);
            break;
        case MAINTENANCE_TIME:
            fillLine(varyingInput, mst_maintenance);
            break;
        case RUNWAY_TIME:
            fillLine(varyingInput, mst_runway);
            break;
    }
    
    fillLine(TOTAL_DELAY, totalDelay);
    fillLine(MAINTENANCE_DELAY, delayMaintenance);
    fillLine(GATES_DELAY, delayGates);
    fillLine(RUNWAY_DELAY, delayRunway);
    fillLine(DELAY_PER_PLANE, avgDelayPerPlane);
}
