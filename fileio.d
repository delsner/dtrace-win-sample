struct ustr{uint16_t buffer[256];};

BEGIN
{
    /* -1: trace this process; 0: do not trace process; */
    trackedpid[pid] = 0;  // do not trace `dtrace` pid.
    trackedpid[$target] = -1;  // call with -c or -p to allow using `$target` here.
    printf("Dtrace started.\n");
}

/* TODO: further reduce amount of probes here, e.g., by limiting to `thread_selfid` syscall or at least a set of syscalls? */
syscall:::entry
/(trackedpid[ppid] == -1) && (trackedpid[pid] == 0)/
{
    trackedpid[pid] = -1;
    trackedexecname[pid] = execname;
    printf("%d %s_%d spanwed process %s_%d \n", walltimestamp, trackedexecname[ppid], ppid, execname, pid);
}

syscall::NtCreateFile:entry
/trackedpid[pid] == -1/
{
	this->arg8 = (unsigned long) arg8;
	this->isFile = this->arg8 & 0x00000040; /* FILE_NON_DIRECTORY_FILE */
	if(this->isFile) {
		this->attr = (nt`_OBJECT_ATTRIBUTES*) copyin(arg2, sizeof(nt`_OBJECT_ATTRIBUTES));
		if (this->attr->ObjectName) {
				this->objectName = (nt`_UNICODE_STRING*)
					copyin((uintptr_t)this->attr->ObjectName,
						   sizeof(nt`_UNICODE_STRING));
				this->fname = (uint16_t*) copyin((uintptr_t)this->objectName->Buffer, this->objectName->Length);
				printf("%d %s_%d opened file %*ws \n", walltimestamp, execname, pid, this->objectName->Length / 2,
																					 ((struct ustr*)this->fname)->buffer);
		}
	}
}
