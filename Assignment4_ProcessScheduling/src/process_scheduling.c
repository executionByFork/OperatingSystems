#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"
#define QUANTUM 4 // Used for Robin Round for process as the run time limit

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) {
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    if (!ready_queue || !result) {  //check for NULL values
        return false;
    }
    size_t numEl;
    if ( !(numEl = dyn_array_size(ready_queue)) ) {
        return false;
    }

    //ensure result has been zeroed out
    if (result->average_latency_time || result->average_wall_clock_time || result->total_run_time) {
        return false;
    }

    ProcessControlBlock_t pcb;  //run loop until no processes left in queue
    while(dyn_array_extract_back(ready_queue, &pcb)) {
        if (!pcb.remaining_burst_time) {    //initial burst time should be > 1
            return false;
        }

        result->average_latency_time += result->total_run_time;
        while(pcb.remaining_burst_time) {   //exhaust burst time before moving to next process
            virtual_cpu(&pcb);
            ++result->total_run_time;
        }
        result->average_wall_clock_time += result->total_run_time;
    }

    result->average_wall_clock_time /= (float)numEl;
    result->average_latency_time /= (float)numEl;
    return true;
}

int comparator(const void *a, const void *b) {
    //subtract b.priority from a.priority
    //if result is positive, a must be greater than b
    //if result is negative, a must be less than b
    //if result is 0, they must be equal
    return (((ProcessControlBlock_t *)a)->priority - ((ProcessControlBlock_t *)b)->priority);
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    if (!ready_queue || !result) {  //check for NULL values
        return false;
    }
    size_t numEl;
    if ( !(numEl = dyn_array_size(ready_queue)) ) {
        return false;
    }

    //ensure result has been zeroed out
    if (result->average_latency_time || result->average_wall_clock_time || result->total_run_time) {
        return false;
    }

    //sort array by priority
    if(!dyn_array_sort(ready_queue, &comparator)) {
        return false;
    }

    ProcessControlBlock_t pcb;  //run loop until no processes left in queue
    while(dyn_array_extract_front(ready_queue, &pcb)) {
        if (!pcb.remaining_burst_time) {    //initial burst time should be > 1
            return false;
        }

        result->average_latency_time += result->total_run_time;
        while(pcb.remaining_burst_time) {   //exhaust burst time before moving to next process
            virtual_cpu(&pcb);
            ++result->total_run_time;
        }
        result->average_wall_clock_time += result->total_run_time;
    }

    
    result->average_wall_clock_time /= (float)numEl;
    result->average_latency_time /= (float)numEl;

    return true;
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) {
    if (ready_queue || result || quantum) {}

    return false;
}

dyn_array_t *load_process_control_blocks(const char *input_file) {
    if (input_file == NULL) {   //check for an address
        return NULL;
    }
    if (strlen(input_file) == 0) {  //check for empty string
        return NULL;
    }
    for (size_t i = 0; i < strlen(input_file); ++i) {   //checks for the occurance of \n in the string
        if (input_file[i] == '\n') {
            return NULL;
        }
    }

    //open file
    int fp = open(input_file, O_RDONLY);
    if (fp <= 0) {  //bad filename / error
        return NULL;
    }

    uint32_t numP;  //read in number of processes from file
    if( read(fp, &numP, sizeof(uint32_t)) != sizeof(uint32_t)) {
        return NULL;
    }
    if (numP == 0) {    //zero processes
        return NULL;
    }
    ProcessControlBlock_t pcb;
    dyn_array_t* pcbs = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);

    //read in process information for each & add to array
    for (uint32_t i = 0; i < numP; ++i) {
        if ( read(fp, &(pcb.remaining_burst_time), sizeof(uint32_t)) != sizeof(uint32_t) ) {    //read in burst time
            return NULL;    //error reading file
        }
        if ( read(fp, &(pcb.priority), sizeof(uint32_t)) != sizeof(uint32_t) ) {    //read in priority
            return NULL;    //error reading file
        }
        pcb.started = 0;

        //copy block onto end of array
        if( !dyn_array_push_back(pcbs, &pcb) ) {
            return NULL;
        }
    }

    return pcbs;
}

