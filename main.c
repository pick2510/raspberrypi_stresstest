/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: root
 *
 * Created on 4. November 2016, 11:06
 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
/*
 * 
 */
static bool endFlag = false;

void handle_exit() {
    printf("Ok, stopping.......\n");
    endFlag = true;
}

void print_usage() {
    printf("Welcome to Raspberry Pi Stresstest\n");
    printf("This Program executes several threads and moves data from\n");
    printf("/dev/zero to /dev/null to generate some stress\n");
    printf("Please use stress -t [Number of threads] -b [size of Read/Write Buffer\n");
    exit(EXIT_FAILURE);
}

void *workerThread(void *buffersize) {
    ssize_t bytesRead, bytesWritten;;
    char *buffer = calloc(*(int*) buffersize, sizeof(char));
    //char buffer[4096];
    int in_fd, out_fd;
    ssize_t readBytes, writtenBytes;
    in_fd = open("/dev/zero", O_RDONLY);
    if (in_fd < 0) {
        printf("Error opening /dev/zero as Input. Aborting...");
        exit(EXIT_FAILURE);
    }
    out_fd = open("/dev/null", O_WRONLY);
    if (out_fd < 0) {
        printf("Error opening /dev/null as Output. Aborting...");
        exit(EXIT_FAILURE);
    }
    while (endFlag == false) {
        readBytes = read(in_fd, &buffer, sizeof (buffer));
        if (readBytes != sizeof (buffer)) {
            printf("Some reading error happened, exiting.....");
            exit(EXIT_FAILURE);
        }
        writtenBytes = write(out_fd, &buffer, sizeof (buffer));
        if (writtenBytes != sizeof (buffer)) {
            printf("Some wiritng error happened, exiting......");
            exit(EXIT_FAILURE);
        }
    }
    free(buffer);
    close(in_fd);
    close(out_fd);

}

int main(int argc, char** argv) {
    pthread_t *threads;
    int terr = 0;
    int c, num_threads, buffersize;
    char *c_threads = NULL;
    char *c_buffersize = NULL;
    while ((c = getopt(argc, argv, "t:b:")) != -1) {
        switch (c) {
            case 't':
                c_threads = optarg;
                break;
            case 'b':
                c_buffersize = optarg;
                break;
            default: print_usage();
        }
    }
    if (c_threads == NULL || c_buffersize == NULL) {
        print_usage();
    }
    if (signal(SIGINT, handle_exit) == SIG_ERR) {
        printf("Can't catch SIGINT\n");
        exit(EXIT_FAILURE);
    }
    num_threads = atoi(c_threads);
    buffersize = atoi(c_buffersize);
    threads = calloc(num_threads, sizeof (pthread_t));
    for (int i = 0; i < num_threads; i++) {
        terr = pthread_create(&threads[i], NULL, workerThread, &buffersize);
        printf("Starting thread %d\n", i);
        fflush(stdout);
        if (terr) {
            printf("Couldn't create thread %d, exiting.....", i + 1);
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_threads; i++) {
        terr = pthread_join(threads[i], NULL);
        if (terr) {
            printf("Couldn't join thread %d, aborting....", i);
            exit(EXIT_FAILURE);
        }
    }
    free(threads);
    printf("All threads stopped\n");
    return (EXIT_SUCCESS);
}

