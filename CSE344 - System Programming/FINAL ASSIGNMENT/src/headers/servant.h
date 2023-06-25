#ifndef SERVANT_H
#define SERVANT_H

#define REQUIRED_ARG_AMT 9
#include "common.h"
/*
Holds all the cities
*/
struct cities_root{
    struct city *cities;
    int city_amt;
};

/*
Holds the data for each city
*/
struct city{
    char city_name[FIXED_INFO_LEN];
    int day_amt;
    struct data_day* days;
};

/*
Holds the data for each day
*/
struct data_day{
    int day;
    struct transaction* transactions;
    long int transaction_amt;
};

/*
Holds the daata for each transaction
*/
struct transaction{
    int transaction_id;
    char real_estate_type[FIXED_INFO_LEN];
    char street[FIXED_INFO_LEN];
    int surface_area;
    int price;
};

/*
Handles requests from server
*/
void* servant_thread_function(void* arg);
/*
Searches its data to find transaction amount and returns it
*/
int search_transaction(char* city,int start,int end,char* transaction_type);
/*
Sets all the data for the cities
*/
void load_cities(char* directory_path,struct cities_root* cities,char* servants_info);

void servant_sigint_handler(int signum);
void servant_exit_handler();


#endif