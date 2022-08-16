 
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace myos
{
    
    struct CPUState
    {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        common::uint32_t error;

        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;        
    } __attribute__((packed));
    
    
    class Task
    {
    friend class TaskManager;
    private:
        common::uint8_t stack[4096]; // 4 KiB
        CPUState* cpustate;
        bool terminated;
        Task* joined;

    public:
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        ~Task();
    };
    
    
    class TaskManager
    {
    private:
        Task* tasks[256];
        Task* yieldedTasks[256];
        static int numTasks;
        int numYielded;
        int currentTask;
        bool init;
        CPUState* initState;
        bool RemoveTask(Task* task);
    public:
        TaskManager();
        ~TaskManager();
        bool AddTask(Task* task);
        void Terminate(Task* task);
        bool YieldTask(Task* task1);
        bool JoinTask(Task* task1,Task* task2);
        CPUState* Schedule(CPUState* cpustate);
    };
    
    
    
}


#endif