
#include <multitasking.h>

using namespace myos;
using namespace myos::common;

char* itoa(int x){
    char* buf = "    ";
    int i=0;
    if(x==0) return "0";
    while(x!=0){
        buf[i++]=x%10+48;
        x/=10;
    }
    buf[i]='\0';
    return buf;
}

void printf(char*);

Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
    
    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)entrypoint;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;
    terminated=false;
    joined=0;    
}

Task::~Task()
{
}

int TaskManager::numTasks = 0;

TaskManager::TaskManager()
{
    numYielded = 0;
    currentTask = -1;
    init=true;
}

TaskManager::~TaskManager()
{
}

void TaskManager::Terminate(Task* task){
    task->terminated=true;
}

bool TaskManager::AddTask(Task* task)
{
    task->terminated=false;
    task->joined=0;
    if(numTasks >= 256){
        return false;
        printf("There are 256 active threads. No more can be created or joined right now.\n");
    }
    tasks[numTasks++] = task;

    return true;
}

bool TaskManager::RemoveTask(Task* task){
    if(numTasks==0){
        printf("There are no threads to remove or yield.\n");
        return false;
    }
    for(int i=0;i<numTasks;i++){
        if(tasks[i]==task){
            for(int j=i;j<numTasks;j++){
                tasks[j] = tasks[j+1];

            }
            numTasks--;

            return true;
        }
    }
    for(int i=0;i<numYielded;i++){
        if(yieldedTasks[i]==task);
        for(int j=i;j<numYielded;j++){
            yieldedTasks[j] = yieldedTasks[j+1];
        }
    }
    printf("No such thread.\n");
    return false;
}

bool TaskManager::YieldTask(Task* task){
    
    if(!RemoveTask(task)){
        return false;
    }
    if(numYielded>=256){
        printf("There are 256 yielded processes. No more can be yielded.\n");
        return false;
    }
    yieldedTasks[numYielded++]=task;

    printf("Task yielded for 1 clock cycle.\n");
    return true;
}

bool TaskManager::JoinTask(Task* task1, Task* task2){
    if(numTasks<2){
        printf("There should be at least 2 active tasks to join\n");
        return false;
    }
    if(++currentTask >= numTasks)
        currentTask %= numTasks;
    task2->joined=task1;
    if(!RemoveTask(task1))return false;
    return true;
}

CPUState* TaskManager::Schedule(CPUState* cpustate)
{

    if(numYielded>0){
        printf("Task readded to ready-queue\n");
        AddTask(yieldedTasks[--numYielded]);
    }
    
    if(numTasks <= 0){
        if(init){
            initState = cpustate;
            init=false;
        }
        return cpustate;
    }

    if(currentTask >= 0){
        tasks[currentTask]->cpustate = cpustate;
    }
    
    if(++currentTask >= numTasks)
        currentTask %= numTasks;

    if(tasks[currentTask]->terminated && tasks[currentTask]->joined!=0){
        tasks[currentTask]=tasks[currentTask]->joined;
        tasks[currentTask]->joined=0;
    }
    if(tasks[currentTask]->terminated && tasks[currentTask]->joined==0){
        RemoveTask(tasks[currentTask]);
        if(numTasks<=0){
            cpustate=initState;
            currentTask=-1;
        }
        return Schedule(cpustate);
    }

    return tasks[currentTask]->cpustate;
}

