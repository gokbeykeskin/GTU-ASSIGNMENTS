
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


// #define GRAPHICSMODE


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

void funcA();
void funcB();
void funcC();
void consumer();
void producer();
void petersonsConsumer();
void petersonsProducer();

TaskManager taskManager;
GlobalDescriptorTable gdt;
InterruptManager interrupts(0x20, &gdt, &taskManager);


Task taskA(&gdt, funcA);
Task taskB(&gdt, funcB);
Task taskC(&gdt, funcC);
Task taskCons(&gdt, consumer);
Task taskProd(&gdt, producer);
Task taskPetersonCons(&gdt, petersonsConsumer);
Task taskPetersonsProd(&gdt, petersonsProducer);


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

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}


//---for testing Creating,Removing,Joining threads---//
void funcA(void){
    while(true){
        printf("Thread A Running\n");
    }
}

void funcB(void){
    while(true){
        printf("Thread B Running\n");
    }  
}
//--for testing Yield--//
void funcC(void){
    while(true){
            printf("Thread C Running.\n");
            for(int i=0;i<999999999;i++); //sleep for testing.

    }
}

//---for testing consumer-producer with race condition---//
#define N 1000
int count = 0;
char buffer[N];
bool prodSleeping=false;
bool consSleeping=true;

void producer(){
    char item='A';
    while(true){
        if(count==N){
            prodSleeping = true;
        }
        if(!prodSleeping){
            buffer[count++] = item++;
            if(item-1>='Z') item = 'A';
            if(count==1) consSleeping = false;
        }
    }
}

void consumer(){
    char item;
    while(true){
        if(count==0){
            consSleeping=true;
        }
        if(!consSleeping){
            item = buffer[--count];
            if(count==N-1) prodSleeping = false;
            printf(&item);
            printf("\n");
        }
    }
}
//---for testing consumer-producer without race condition (Peterson's Solution)---//

#define PAMT 2 //process amt;
int turn;
bool interested[PAMT];
void enter_region(int process){
    int other = 1-process;
    interested[process]=true;
    turn = process;
    while(turn==process && interested[other]==true)/*BUSY WAIT*/;
}
void leave_region(int process){
    interested[process]=false;
}

void petersonsProducer(){
    char item='A';
    while(true){
        if(count==N) prodSleeping = true;
        if(!prodSleeping){
            enter_region(0);
                buffer[count++] = item++;
                if(item-1>='Z') item = 'A';
                if(count==1) consSleeping = false;
            leave_region(0);

        }
    }
}

void petersonsConsumer(){
    char item;
    while(true){
        if(count==0) consSleeping=true;
        if(!consSleeping){
            enter_region(1);
                item = buffer[--count];
                if(count==N-1) prodSleeping = false;
                printf(&item);
                printf("\n");
            leave_region(1);
        }
    }
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        int testNum=0;
        if(c=='1') testNum=1;
        if(c=='2') testNum=2;
        if(c=='3') testNum=3;
        if(c=='4') testNum=4;
        if(testNum==1){
            printf("TEST1:\n");
            switch(c){
                case 's':
                    taskManager.AddTask(&taskA);
                    taskManager.AddTask(&taskB);
                    break;
                case 'j':
                    taskManager.JoinTask(&taskA,&taskB);
                    break;
                case 't':
                    taskManager.Terminate(&taskB);
                    break;

                case 'q':
                    taskManager.Terminate(&taskA);
                    taskManager.Terminate(&taskB);
                    break;
            }
        }
        else if(testNum==2){
            printf("TEST2:\n");
            switch(c){
                case 's':
                    taskManager.AddTask(&taskC);
                    break;
                case 'y':
                    taskManager.YieldTask(&taskC);
                    break;
                case 'q':
                    taskManager.Terminate(&taskC);
                    break;
            }
        }
        else if(testNum==3){
            printf("TEST3:\n");
            switch(c){
                case 's':
                    taskManager.AddTask(&taskProd);
                    taskManager.AddTask(&taskCons);
                    break;
                case 'q':
                    taskManager.Terminate(&taskProd);
                    taskManager.Terminate(&taskCons);
                    break;
            }
        }
        else if(testNum==4){
            printf("TEST4:\n");
            switch(c){
                case 's':
                    taskManager.AddTask(&taskPetersonsProd);
                    taskManager.AddTask(&taskPetersonCons);
                    break;
                case 'q':
                    taskManager.Terminate(&taskPetersonsProd);
                    taskManager.Terminate(&taskPetersonCons);
                    break;
            }
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
