#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }

    ScheduleResult_t sr;
    memset(&sr,0,sizeof(ScheduleResult_t));
    

    //strcmp() returns zero on matching strings, which is falsy, so I use ! to make a match truthy
    if (!strcmp(argv[2], FCFS)) {	//First Come First Serve

    	//create pcbs data structure
    	dyn_array_t* pcbs = load_process_control_blocks(argv[1]);
    	if (!pcbs) {
    		dyn_array_destroy(pcbs);
	    	return EXIT_FAILURE;
	    }

	    if(!first_come_first_serve(pcbs,&sr)) {
	    	dyn_array_destroy(pcbs);
	    	return EXIT_FAILURE;
	    }

	    dyn_array_destroy(pcbs);

    } else if(!strcmp(argv[2], P)) {	//Priotity

    	//create pcbs data structure
    	dyn_array_t* pcbs = load_process_control_blocks(argv[1]);
    	if (!pcbs) {
    		dyn_array_destroy(pcbs);
	    	return EXIT_FAILURE;
	    }

	    if(!priority(pcbs,&sr)) {
	    	dyn_array_destroy(pcbs);
	    	return EXIT_FAILURE;
	    }

	    dyn_array_destroy(pcbs);

    } else if(!strcmp(argv[2], RR)) {
    	printf("Round Robin not implemented\n");
    	return EXIT_SUCCESS;
    }

    //print to stdout & readme.md
    printf("Average Latency Time: %f\n", sr.average_latency_time);
    printf("Average Wall Clock Time: %f\n", sr.average_wall_clock_time);
    printf("Total Run Time: %lu\n", sr.total_run_time);

    FILE* fp = fopen("../readme.md", "a");
    fprintf(fp, "\n== Time Information for: %s ==\n", argv[2]);
    fprintf(fp, "Average Latency Time: %f\n", sr.average_latency_time);
    fprintf(fp, "Average Wall Clock Time: %f\n", sr.average_wall_clock_time);
    fprintf(fp, "Total Run Time: %lu\n", sr.total_run_time);
    fclose(fp);

/*
    1. float Average Latency
	2. float Average Wall-Clock
	3. unsigned long Total Clock Time
*/
    return EXIT_SUCCESS;

}
