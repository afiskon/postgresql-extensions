#include <postgres.h>
#include <miscadmin.h>
#include <storage/ipc.h>
#include <storage/shmem.h>
#include <storage/lwlock.h>
#include <utils/builtins.h>

static shmem_request_hook_type prev_shmem_request_hook = NULL;
static shmem_startup_hook_type prev_shmem_startup_hook = NULL;

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(experiment_get_message);
PG_FUNCTION_INFO_V1(experiment_set_message);

#define MESSAGE_BUFF_SIZE 128
typedef struct SharedStruct {
	char message[MESSAGE_BUFF_SIZE];
} SharedStruct;

LWLock* sharedStructLock;
SharedStruct *sharedStruct;

static void
experiment_shmem_request(void)
{
	if(prev_shmem_request_hook)
		prev_shmem_request_hook();

	RequestAddinShmemSpace(MAXALIGN(sizeof(SharedStruct)));

	RequestNamedLWLockTranche("experiment", 1);
}

static void
experiment_shmem_startup(void)
{
	bool found;

	if(prev_shmem_startup_hook)
		prev_shmem_startup_hook();

	/*
	 * This callback can be called several times, see the comments for
	 * CreateSharedMemoryAndSemaphores(). In order to play it save we have
	 * to take the lock.
	 */
	LWLockAcquire(AddinShmemInitLock, LW_EXCLUSIVE);

	sharedStruct = ShmemInitStruct("SharedStruct", sizeof(SharedStruct), &found);
	if(!found) {
		sharedStruct->message[0] = '\0';

		sharedStructLock = &(GetNamedLWLockTranche("experiment"))->lock;
	}

	LWLockRelease(AddinShmemInitLock);
}

void
_PG_init(void)
{
	if(!process_shared_preload_libraries_in_progress)
		elog(FATAL, "Please use shared_preload_libraries");

	elog(LOG, "extension loaded");

	prev_shmem_request_hook = shmem_request_hook;
	shmem_request_hook = experiment_shmem_request;

	prev_shmem_startup_hook = shmem_startup_hook;
	shmem_startup_hook = experiment_shmem_startup;
}

Datum
experiment_get_message(PG_FUNCTION_ARGS)
{
	text* result;

	LWLockAcquire(sharedStructLock, LW_SHARED);
	result = cstring_to_text(sharedStruct->message);
	LWLockRelease(sharedStructLock);

	PG_RETURN_TEXT_P(result);
}

Datum
experiment_set_message(PG_FUNCTION_ARGS)
{
	const char* msg = TextDatumGetCString(PG_GETARG_DATUM(0));

	LWLockAcquire(sharedStructLock, LW_EXCLUSIVE);
	strncpy(sharedStruct->message, msg, MESSAGE_BUFF_SIZE-1);
	LWLockRelease(sharedStructLock);

	PG_RETURN_VOID();
}