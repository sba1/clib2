/*
 * $Id$
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2005 by Olaf Barthel <olsen@sourcery.han.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Neither the name of Olaf Barthel nor the names of contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _STDLIB_HEADERS_H
#include "stdlib_headers.h"
#endif /* _STDLIB_HEADERS_H */

/****************************************************************************/

#ifndef _STDLIB_MEMORY_H
#include "stdlib_memory.h"
#endif /* _STDLIB_MEMORY_H */

/****************************************************************************/

#ifndef _STDLIB_CONSTRUCTOR_H
#include "stdlib_constructor.h"
#endif /* _STDLIB_CONSTRUCTOR_H */

/****************************************************************************/

#undef alloca

extern void * alloca(size_t size);

/****************************************************************************/

struct MemoryContextNode
{
	struct MinNode	mcn_MinNode;
	void *			mcn_StackPointer;
	void *			mcn_Memory;
};

/****************************************************************************/

static struct MinList alloca_memory_list;

/****************************************************************************/

CLIB_DESTRUCTOR(alloca_exit)
{
	ENTER();

	/* Clean this up, too, just to be safe. */
	NewList((struct List *)&alloca_memory_list);

	LEAVE();
}

/****************************************************************************/

void
__alloca_cleanup(const char * file,int line)
{
	/* Initialize this if it hasn't been taken care of yet. */
	if(alloca_memory_list.mlh_Head == NULL)
		NewList((struct List *)&alloca_memory_list);

	/* Is this worth cleaning up? */
	if(NOT IsListEmpty((struct List *)&alloca_memory_list))
	{
		void * stack_pointer = __get_sp();
		struct MemoryContextNode * mcn_prev;
		struct MemoryContextNode * mcn;

		/* The assumption is that the stack grows downwards. If this function is
		   called, we must get rid off all the allocations associated with stack
		   pointers whose addresses are smaller than the current stack pointer.
		   Which so happen to be stored near the end of the list. The further
		   we move up from the end to the top of the list, the closer we get
		   to the allocations made in the context of a stack frame near to
		   where were currently are. */
		for(mcn = (struct MemoryContextNode *)alloca_memory_list.mlh_TailPred ;
		    mcn->mcn_MinNode.mln_Pred != NULL && mcn->mcn_StackPointer < stack_pointer ;
		    mcn = mcn_prev)
		{
			mcn_prev = (struct MemoryContextNode *)mcn->mcn_MinNode.mln_Pred;

			Remove((struct Node *)mcn);

			__force_free(mcn->mcn_Memory,file,line);
			__free(mcn,file,line);
		}
	}
}

/****************************************************************************/

__static void *
__alloca(size_t size,const char * file,int line)
{
	void * stack_pointer = __get_sp();
	struct MemoryContextNode * mcn;
	void * result = NULL;

	__alloca_cleanup(file,line);

	assert( (int)size >= 0 );

	if(size == 0)
	{
		#ifdef __MEM_DEBUG
		{
			kprintf("[%s] ",__program_name);

			if(file != NULL)
				kprintf("%s:%ld:",file,line);

			kprintf("alloca(0) called.\n");
		}
		#endif /* __MEM_DEBUG */

		goto out;
	}

	mcn = __allocate_memory(sizeof(*mcn),FALSE,file,line);
	if(mcn == NULL)
	{
		SHOWMSG("not enough memory");
		goto out;
	}

	/* Allocate memory which cannot be run through realloc() or free(). */
	mcn->mcn_Memory = __allocate_memory(size,TRUE,file,line);
	if(mcn->mcn_Memory == NULL)
	{
		SHOWMSG("not enough memory");

		__free(mcn,file,line);
		goto out;
	}

	mcn->mcn_StackPointer = stack_pointer;

	assert( alloca_memory_list.mlh_Head != NULL );

	AddTail((struct List *)&alloca_memory_list,(struct Node *)mcn);

	result = mcn->mcn_Memory;

 out:

	return(result);
}

/****************************************************************************/

void *
alloca(size_t size)
{
	void * result;

	result = __alloca(size,NULL,0);

	return(result);
}
