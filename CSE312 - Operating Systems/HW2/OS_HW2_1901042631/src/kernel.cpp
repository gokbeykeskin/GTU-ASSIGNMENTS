
#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>
#include "queue.h"


// #define GRAPHICSMODE


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;



TaskManager taskManager;
GlobalDescriptorTable gdt;
char lastPressedKey='0';
InterruptManager interrupts(0x20, &gdt, &taskManager);


#define DISK_SIZE 64
int hit_amt = 0;
int miss_amt = 0;
int used_register = 0;
queue q;
struct page_entry{
    bool present;
    bool ref;
    bool modified;
    int disk_address;
    int frame;
    int last_access;
};

struct page_entry page_table[DISK_SIZE/8] = {0};
int disk[DISK_SIZE];


void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Function to reverse `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}

int abs(int num){
    if(num < 0){
        return -num;
    }
    return num;
}

char* itoa(int value, char* buffer)
{
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % 10;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / 10;
    }
 
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    if (value < 0)
        buffer[i++] = '-';
 
    buffer[i] = '\0'; 
 
    return reverse(buffer, 0, i - 1);
}





void restart_disk(){
    int temp[DISK_SIZE] = {33195, 24980, 30243, 76161, 81471, 66095, 14999, 76030,
                        50133, 34987, 61626, 95771, 24088, 95003, 45372, 82614,
                        97916, 24817, 38132, 23526, 87364, 62143, 73553, 74054,
                        52115, 6852, 99279, 42562, 63930, 5632, 77868, 51302,
                        93042, 41163, 23890, 21030, 22194, 82929, 12276, 58274,
                        53856, 96610, 42799, 62479, 55873, 95274, 25622, 13765,
                        84195, 83495, 2884, 21504, 64220, 53441, 75078, 7983,
                        96691, 30896, 61258, 74188, 79456, 58417, 87679, 87723};

    for(int i = 0; i < DISK_SIZE; i++){
        disk[i] = temp[i];
    }
}


void init_mem(){
    hit_amt=0;
    miss_amt=0;
    restart_disk();
    for(int i=0;i<DISK_SIZE/8;i++){
        page_table[i].frame = disk[i];
        page_table[i].last_access = used_register++;
        page_table[i].disk_address = i;
        page_table[i].present = true;
        page_table[i].ref = false;
        page_table[i].modified = false;
        q.enqueue(i);
    }
}

void write_to_mem(int disk_addr, int frame){
    disk[disk_addr] = frame;
}

void fifo_paging(int disk_addr){
    int page_addr = q.dequeue();
    if(page_table[page_addr].modified){
        write_to_mem(page_table[page_addr].disk_address,page_table[page_addr].frame);
        page_table[page_addr].modified = false;

    }
    page_table[page_addr].disk_address = disk_addr;
    page_table[page_addr].frame = disk[disk_addr];
    page_table[page_addr].present = true;
    page_table[page_addr].ref = true;
    q.enqueue(page_addr);
}

void second_chance_paging(int disk_addr){
    int page_addr;
    bool repeat=false;
    do{
        page_addr = q.dequeue();
        if(page_table[page_addr].ref){
            repeat=true;
            page_table[page_addr].ref = false;
            q.enqueue(page_addr);
        }
        else repeat = false;
    }while(repeat);
    if(page_table[page_addr].modified){
        write_to_mem(page_table[page_addr].disk_address,page_table[page_addr].frame);
        page_table[page_addr].modified = false;

    }
    page_table[page_addr].disk_address = disk_addr;
    page_table[page_addr].frame = disk[disk_addr];
    page_table[page_addr].present = true;
    page_table[page_addr].ref = true;
    q.enqueue(page_addr);
}

void least_recently_used_paging(int disk_addr){
    int last_access = 999999999;
    int current_page=0;
    for(int i=0;i<DISK_SIZE/8;i++){
        if(page_table[i].last_access < last_access){
            last_access = page_table[i].last_access;
            current_page = i;
        }
    }
    if(page_table[current_page].modified){
        write_to_mem(page_table[current_page].disk_address,page_table[current_page].frame);
        page_table[current_page].modified = false;

    }
    page_table[current_page].disk_address = disk_addr;
    page_table[current_page].frame = disk[disk_addr];
    page_table[current_page].present = true;
    page_table[current_page].ref = true;
}
int testNum=0;



int set_page(int disk_address, int frame){
    if(disk_address > DISK_SIZE){
        return -1;
    }
    for(int i=0;i<DISK_SIZE/8;i++){
        if(page_table[i].disk_address == disk_address){
            page_table[i].frame = frame;
            page_table[i].present = true;
            page_table[i].ref = true;
            page_table[i].modified = true;
            page_table[i].last_access = used_register++;
        }
    }
    return 0;
}





void save_to_mem_dirty(){
    for(int i=0;i<DISK_SIZE/8;i++){
        if(page_table[i].modified){
            write_to_mem(page_table[i].disk_address,page_table[i].frame);
            page_table[i].modified = false;
        }
    }
}




void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}
int request_page(int disk_address){
    if(disk_address > DISK_SIZE){
        return -1;
    }
    for(int i=0;i<DISK_SIZE/8;i++){
        if(page_table[i].disk_address == disk_address){
            page_table[i].ref = true;
            page_table[i].last_access = used_register++;
            hit_amt++;
            return page_table[i].frame;
        }
    }
    if(testNum==1){
        fifo_paging(disk_address);
    }
    if(testNum==2){
        second_chance_paging(disk_address);
    }
    if(testNum==3){
        least_recently_used_paging(disk_address);
    }
    miss_amt++;
    return request_page(disk_address);

}

void bubble_sort(int arr[], int n){
    for(int i=0; i<n-1; i++){
        for(int j=0; j<n-i-1; j++){
            if(request_page(j) > request_page(j+1)){
                int temp = request_page(j);
                set_page(j,request_page(j+1));
                set_page(j+1,temp);
            }
        }
    }
}

void insertion_sort(int arr[], int n){
    for(int i=1; i<n; i++){
        int key = request_page(i);
        int j = i-1;
        while(j>=0 && request_page(j) > key){
            set_page(j+1,request_page(j));
            j = j-1;
        }
        set_page(j+1,key);
    }
}

int partition(int arr[], int low, int high){
    int pivot = request_page(high);
    int i = (low-1);
    for(int j=low; j<high; j++){
        if(request_page(j) <= pivot){
            i++;
            int temp = request_page(i);
            set_page(i,request_page(j));
            set_page(j,temp);
        }
    }
    int temp = request_page(i+1);
    set_page(i+1,request_page(high));
    set_page(high,temp);
    return (i+1);
}

void quick_sort(int arr[], int low, int high){
    if(low < high){
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi-1);
        quick_sort(arr, pi+1, high);
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

char buffer[128];

void bubble_test_1(){
    init_mem();
    printf("Memory Initialized\n");
    printf("Before Bubble Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("\n---------------------------------------------\n");
    bubble_sort(disk,DISK_SIZE);
    printf("After Bubble Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("\nMiss amount: ");printf(itoa(miss_amt,buffer));printf("\n");
    printf("Hit amount: ");printf(itoa(hit_amt,buffer));printf("\n");
}

void insertion_test_1(){
    init_mem();
    printf("Memory Initialized\n");
    printf("Before Insertion Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("\n---------------------------------------------\n");
    insertion_sort(disk,DISK_SIZE);
    printf("After Insertion Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("\nMiss amount: ");printf(itoa(miss_amt,buffer));printf("\n");
    printf("Hit amount: ");printf(itoa(hit_amt,buffer));printf("\n");
}

void quick_test_1(){
    init_mem();
    printf("Memory Initialized\n");
    printf("Before Quick Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("\n---------------------------------------------\n");
    quick_sort(disk,0,DISK_SIZE-1);
    printf("After Quick Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("\nMiss amount: ");printf(itoa(miss_amt,buffer));printf("\n");
    printf("Hit amount: ");printf(itoa(hit_amt,buffer));printf("\n");
}


class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        if(c=='1'){
            printf("FIFO Replacement Test Mode\n");
            testNum=1;
        }
        if(c=='2'){
            printf("Second Chance Test Mode\n");
            testNum=2;
        }
        if(c=='3'){
            printf("LRU Test Mode\n");
            testNum=3;
        }


            
        switch(c){
            case 'b':
                bubble_test_1();
                break;
            case 'i':
                insertion_test_1();
                break;
            case 'q':
                quick_test_1();
                break;

        }

        
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};



typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    interrupts.Activate();
    printf("Welcome to GokOS\n");
        /*printf("\n---------------------------------------------\n--------------------------------------------------\n");
    printf("Memory reinitialized\n");
    init_mem();
    printf("Before Insertion Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("\n---------------------------------------------\n");
    insertion_sort(disk,DISK_SIZE);
    printf("After Insertion Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("Miss Amount:");printf(itoa(miss_amt,buffer));printf("\n");
    printf("Hit Amount:");printf(itoa(hit_amt,buffer));printf("\n");
    printf("\n---------------------------------------------\n--------------------------------------------------\n");
    printf("Memory reinitialized\n");
    init_mem();
    printf("Before Quick Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("Miss Amount:");printf(itoa(miss_amt,buffer));printf("\n");
    printf("Hit Amount:");printf(itoa(hit_amt,buffer));printf("\n");
    printf("\n---------------------------------------------\n");
    quick_sort(disk,0,DISK_SIZE-1);
    printf("After Quick Sort:\n");
    for(int i=0;i<DISK_SIZE;i++){
        printf(itoa(disk[i],buffer));
        printf("|");
    }
    printf("Miss Amount:");printf(itoa(miss_amt,buffer));printf("\n");
    printf("Hit Amount:");printf(itoa(hit_amt,buffer));printf("\n");*/
 
    #ifdef GRAPHICSMODE
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif
    
    DriverManager drvManager;
    
        #ifdef GRAPHICSMODE
            KeyboardDriver keyboard(&interrupts, &desktop);
        #else
            PrintfKeyboardEventHandler kbhandler;
            KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif
        drvManager.AddDriver(&keyboard);
        
    
        #ifdef GRAPHICSMODE
            MouseDriver mouse(&interrupts, &desktop);
        #else
            MouseToConsole mousehandler;
            MouseDriver mouse(&interrupts, &mousehandler);
        #endif
        drvManager.AddDriver(&mouse);
        
        PeripheralComponentInterconnectController PCIController;
        PCIController.SelectDrivers(&drvManager, &interrupts);

        VideoGraphicsArray vga;
        
        drvManager.ActivateAll();
        

    #ifdef GRAPHICSMODE
        vga.SetMode(320,200,8);
        Window win1(&desktop, 10,10,20,20, 0xA8,0x00,0x00);
        desktop.AddChild(&win1);
        Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
        desktop.AddChild(&win2);
    #endif


    
    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.Draw(&vga);
        #endif
    }
}
