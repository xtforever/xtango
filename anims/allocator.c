/***********************************************
*  Animation Package for Allocating resources  *
*                David Carlson                 *
*                                              *
*                 allocator.c                  *
***********************************************/
/*

This program graphically models a simple resource allocation strategy.


This program was inspired by Richar Holt's article "Some Deadlock
Properties of Computer Systems."  The idea is that given a number of
processes and a number of reusable resources, one could animate the
requesting, allocation and the release of resources, as well as the
blocking and unblocking of a process.

The animation screen depicts a general resource graph with processes
as squares across the top of the window and resources as circles below
the processes.  This was to stay as close to the notation in the Holt
article as possible without increasing complexity to a level beyond
what I could do in a limited amount of time.  Requests for resources
are depicted by arcs from a process to a resource.  When a resource is
granted, the arc reverses and points from the resource to the process
in posession of the resource.  The Holt article provides no graphical
depiction for a blocked process, but for purposes of this
demonstration, when a process is blocked, it is filled to be a solid,
and if it ever unblocks, the rectangle returns to being unfilled.
Blocked processes are of course not allowed to do anything (request or
release any resources), so the model does a fair job of representing
deadlocks.  Therefore, this program also can also be used as a basis
for deadlock detection and prevention.

The algorithm for allocation is based on the expedient single request
model.  A model can only request one resource at a time, and if the
resource is available, it is immediately allocated.  Otherwise, the
process is blocked.  This implies that if a process has a request arc
coming from it, it is blocked until the process is granted the desired
resource.  However for the sake of simplicity, the depiction of
blocking is delayed until the resource is not immediately granted.
This is a shortcoming of the program that in future versions could be
fixed.

At startup, the user is prompted for the number of processes (<6),
the number of` resources(<6) and the total number of units for each
resource.  From this information, allocator draws the initial state of
the GRG with no resources requested or allocated.  The user is then
asked to select either to request a resource, release a resource or
quit.  If either request or release is chosen, the user is prompted
for the process to request or release.  If the process is blocked, the
program will back up and ask again for an appropriate action.
Otherwise, the user will be asked for a resource.  The processes and
resources are numbered 0 through n-1 (where n is the total number).

When this infomation is given, xtango animates the specified action
and prompts the user for the next instruction.

There are many areas that could be improved, the processes should be
labeled and the resourses should represent the number of available
units at any given time.  The blocking should occur at the beinning
process and not at the end, unblocking if the resource is granted.


*/




#include <stdio.h>
#include "xtango.h"

void init(), request(), allocate(), release(), block(), unblock();
static NAME_FUNCT map[] ={{"Init", 1, {VOID, (FPTR)init}},
			{"Request",1, {VOID, (FPTR)request}},
			{"Allocate",1,{VOID, (FPTR)allocate}},
			{"Release", 1,{VOID, (FPTR)release}},
			{"Block", 1, {VOID, (FPTR)block}},
			{"Unblock", 1, {VOID, (FPTR)unblock}}};

#define MAX_PROCS 5
#define MAX_RESOURCES 5
#define MAX_HOLDINGS 30
#define NADA -1
#define FREE 1
#define BLOCKED 0

struct Queue{
	int Head;
	int Tail;
	int Buffer[MAX_PROCS];};

struct Process{
	int Holding[MAX_HOLDINGS];
	int Status;} Process_Array[MAX_PROCS];

struct Resource{
	int Total;
	int Available;
	struct Queue Process_Queue;}Resource_Array[MAX_RESOURCES];

int heap_add(Proc,Res)
int Proc;
int Res;
{
	int i = 0;

	while((Process_Array[Proc].Holding[i] != NADA) && (i < MAX_HOLDINGS))
		i++;
	if(i < MAX_HOLDINGS)
	{
		Process_Array[Proc].Holding[i] = Res;
		return(Res);
	}
	else
		return(NADA);
}

int heap_remove(Proc,Res)
int Proc;
int Res;
{
	int i;

	for(i=0;i<MAX_HOLDINGS;i++)
	{
		if(Process_Array[Proc].Holding[i] == Res)
		{
			Process_Array[Proc].Holding[i] = NADA;
			return(Res);
		}
	}
	return(NADA);
}

int enqueue(Res,Proc)
int Res;
int Proc;
{
	int Head;
	Head = Resource_Array[Res].Process_Queue.Head;

	if(((Head + 1) % MAX_PROCS) == (Resource_Array[Res].Process_Queue.Tail))
		return(-1);
	Resource_Array[Res].Process_Queue.Buffer[Head] = Proc;
	Resource_Array[Res].Process_Queue.Head = (Head + 1) % MAX_PROCS;
	return (Proc);
}

int dequeue(Res)
int Res;
{
	int Tail, Data;
	Tail = Resource_Array[Res].Process_Queue.Tail;

	if( Tail == Resource_Array[Res].Process_Queue.Head)
		return(-1);
	Data = Resource_Array[Res].Process_Queue.Buffer[Tail];
	Resource_Array[Res].Process_Queue.Tail = (Tail + 1) % MAX_PROCS;
	return(Data);
}

main()
{
	int n_Processes, n_Resources;
	int Proc = 0, Res = 0, Waiter = 0;
	int i,j;
	int Selection;

        printf("Illustrating resource allocation strategies\n");

	TANGOalgoOp(map,"BEGIN");
	printf("# of processes (<6):");
	scanf("%d", &n_Processes);
	printf("# of resources (<6):");
	scanf("%d", &n_Resources);
/* consider checking these input for validity */
/*
Initialize Process and Resource Structures
*/
	for(i=0;i<n_Processes;i++)
	{ 
		Process_Array[i].Status = FREE;
		for(j=0;j<MAX_HOLDINGS;j++)
		{
			Process_Array[i].Holding[j] = NADA;
		}
	}

	for(i=0;i<n_Resources;i++)
	{
		printf("# of resource %d available:",i);
		scanf("%d",&(Resource_Array[i].Total));

		Resource_Array[i].Available = Resource_Array[i].Total;
	}
	TANGOalgoOp(map,"Init",n_Processes,n_Resources);
/*
Main Program Loop
*/
	printf("(1) request, (2) release, or (3) quit:");
	scanf("%d",&Selection);
	while((Selection==1)||(Selection==2))
	{
		printf("For process #:");
		scanf("%d",&Proc);
		if((Proc >= 0) && (Proc < n_Processes) &&
				(Process_Array[Proc].Status == FREE))
		{
			printf("For resource #:");
			scanf("%d",&Res);
			if((Res >= 0)&&(Res < n_Resources)&&(Selection == 1))
			{
/*
Handle Requests
*/
				/*Animate Request of Resource*/
				TANGOalgoOp(map,"Request",Proc,Res);
				if(Resource_Array[Res].Available <= 0)
				{
					/*Animate Block*/
					TANGOalgoOp(map,"Block",Proc);
					Process_Array[Proc].Status = BLOCKED;
					enqueue(Res,Proc);
				}

				else
				{
					/*Animate Allocation*/
					TANGOalgoOp(map,"Allocate",Proc,Res);
					--Resource_Array[Res].Available;
					heap_add(Proc,Res);
				}
			}
			else if((Res>=0)&&(Res<n_Resources)&&(Selection == 2))
			{
/*
Handle Release
*/
				if(heap_remove(Proc,Res)!=NADA)
				{
					/*Animate Resease*/
					TANGOalgoOp(map,"Release",Proc,Res);
					++Resource_Array[Res].Available;
					if((Waiter = dequeue(Res))!=NADA)
					{
/*
Allocate Resource to first waiting Process
*/
						/*Animate unblock*/
						TANGOalgoOp(map,"Unblock",Waiter);
						Process_Array[Waiter].Status=FREE;
						/*Animate Allocation*/
						TANGOalgoOp(map,"Allocate",Waiter,Res);
						--Resource_Array[Res].Available;
						heap_add(Waiter,Res);
					}
				}
			}
		}
		printf("(1) request, (2) release, or (3) quit:");
		scanf("%d",&Selection);
	}
	TANGOalgoOp(map,"END");
	return(0);
}
