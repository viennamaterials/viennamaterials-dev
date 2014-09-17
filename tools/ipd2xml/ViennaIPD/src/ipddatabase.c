/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Basic operations, algorithms for managing ipd structures.
          expression tree handling, inherited variables, inherited
          sections, function handling.

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  RK   Robert Klima,      Institute for Microelectronics, TU Vienna
          BR   Bernhard Reinisch, Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  BR  1997        first coding
  RK  1998-04-14  new from scratch
=======================================================================*/

/** @file ipddatabase.c
    @brief Definition of functions covering the core functionality of ViennaIPD.

    The implementations cover basic operations, algorithms for managing ipd structures, expression tree handling, inherited variables, inherited sections and function handling.
*/

/*=======================================================================
	Includes
=======================================================================*/

#include "ipdP.h"
#include <stdio.h>
#include <string.h>
#if IPD_USE_UNITS
#include "../core/include/unit.h"
#endif /* IPD_USE_UNITS */

/*=======================================================================
	DATA
=======================================================================*/

iiCCodedFunc_t *ipdCCodedFunctions   = NULL;   /* Functions written in C by the user         */
ipdTreeNode_t  *ipdParameterList     = NULL;   /* Parameterlist containing the parameters
						  of the function being parsed.              */
ipdBoolean        ipdEditorPrivileges  = ipdFALSE; /* If the IPD-Editor is used to edit inputdeck
						  Editor privileges must be introduced.
						  This can be set with this flag.            */
ipdBoolean        ipdApplicationSet    = ipdFALSE; /* Application sets/overwites ext variables.  */
ipdBoolean        ipdWarnOnLoseingStmt = ipdTRUE;  /* Warn, if statement will not be stored      */
ipdBoolean        ipdTolerantParsing   = ipdFALSE; /* Do not stop parsing if an error occurs     */
ipdBoolean        ipdAliasParsing      = ipdFALSE; /* Parsing an alias expression                */

/*********************************/
/*       Memory Management       */
/*********************************/
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
static long mallocCounter = 0;
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */

/*-------------------------------------------------------------------------
 * Fills memory of the given size (implicit) with zeros.
 */
void ipdMBclear(void *mem)
{  
   char             *cp   = (char *)mem;
   long             *lp   = (long *)mem;
   long              llen = sizeof (long);
   long              size = 0;

   size = *(long *)(((char *)mem) - IPD_MEM_ARCHITECTURE);

   /* Tries to fill memory with long-sized zeros. */
   if (!(((long)mem) & 3) && (!(llen % 4)))      /* address % 4 = 0 */
   {
      while (size >= llen)
      {
	 *lp++ = 0;
	 size -= llen;
      }
      cp = (char *)lp;
   }
   /* Fills the rest with byte-sized zeros. */
   while (size--)
      *cp++ = 0;

} /* ipdMBclear */

/***********************************
 * Hash for memory management!!!!! *
 * Only memory which must be       *
 * allocated by malloc!!!          *
 ***********************************/
       iiMemBlock_t           **iiMBlockHashLower  = NULL;
       iiMBufferHigherEntry_t **iiMBlockHashHigher = NULL;
static iiMBufferHigherEntry_t  *iiMBlockListHigher = NULL;

/*-------------------------------------------------------------------------
 * Computes a index key for the hash used for memory blocks.
 */
#define iiMBUFFERLENTIMES              (1 << 3)
#define iiMBlockComputeNormalKey(size) ((size) >> 3)
#define iiMBlockComputeHigherKey(key)  (((key) - IPD_MBLOCK_LOWER_HASH_SIZE) % IPD_MBLOCK_HIGHER_HASH_SIZE)

/*-------------------------------------------------------------------------
 * Frees the memory block pointed to by 's' if not NULL to the 
 * memory block buffer hash!!
 */
 
void ipdMBfree(const void *s)
{
   if ((s))
   {
#if IPD_USE_IPD_MALLOC
      long              mSize;
      long              index;

      mSize = *(long *)(((char *)s) - IPD_MEM_ARCHITECTURE);

#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
      mallocCounter--;
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
      index = iiMBlockComputeNormalKey(mSize);
      if (index < IPD_MBLOCK_LOWER_HASH_SIZE)
      {
      	 iiMBufferLowerEntry_t *snl = (iiMBufferLowerEntry_t *)(((char *)s) - sizeof(iiMBufferLowerEntry_t));
      #if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
      	 {
      	    iiMBufferLowerEntry_t *snlFree = iiMBlockHashLower[index]->first;

      	    while (snlFree)
      	    {
      	       if (snlFree == (iiMBufferLowerEntry_t *)snl)
      	       {
      		  fprintf(stderr, "\nfile: \"%s\", line: %ld, ERROR: ipdMBfree()"
      			  "called a second time for this structure(%ld bytes)!\n\n",
      			  snlFree->filename, snlFree->lineno, snlFree->m.mSize);
      		  return;
      	       }
      	       snlFree = snlFree->m.next;
      	    }
      	 }
      #endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
      	 snl->m.next                     = iiMBlockHashLower[index]->first;
      	 iiMBlockHashLower[index]->first = snl;
      }
      else
      {
      	 iiMBufferHigherEntry_t *snh = (iiMBufferHigherEntry_t *)(((char *)s) - sizeof(iiMBufferHigherEntry_t));

      	 index = iiMBlockComputeHigherKey(index);
      #if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
      	 {
      	    iiMBufferHigherEntry_t *snhFree = iiMBlockHashHigher[index];

      	    while (snhFree)
      	    {
      	       if (snhFree == (iiMBufferHigherEntry_t *)snh)
      	       {
      		  fprintf(stderr, "\nfile: \"%s\", line: %ld, ERROR: ipdMBfree()"
      			  "called a second time for this structure(%ld bytes)!\n\n",
      			  snhFree->filename, snhFree->lineno, snhFree->m.mSize);
      		  return;
      	       }
      	       snhFree = snhFree->m.next;
      	    }
      	 }
      #endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
      	 snh->m.next               = iiMBlockHashHigher[index];
      	 iiMBlockHashHigher[index] = snh;
      }
#else  
      free((void *)s);    
#endif 
   }
} /* ipdMBfree */

/*-------------------------------------------------------------------------
 * Malloc a string buffer. Gets some from the buffer hash or mallocs it.
 */
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
  void *ipdMBallocDB(unsigned long size, ipdConstString filename, ipdLong lineno)
  /* ipdMBalloc() defined in ipd.h */
#else  /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
  void *ipdMBalloc(unsigned long size)
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
{

#if IPD_USE_IPD_MALLOC
   long              index;

   /* Correct size: Round it to the next higher integer wich can be
      divided by 8!!! */
   if (size & (iiMBUFFERLENTIMES - 1))
      size = (size & ~((unsigned long)iiMBUFFERLENTIMES - 1)) +
	 iiMBUFFERLENTIMES;

   /* Find an old one. */
   index = iiMBlockComputeNormalKey(size);
   if (index < IPD_MBLOCK_LOWER_HASH_SIZE)
   {
      iiMBufferLowerEntry_t *snl, *snl2;
      iiMemBlock_t          *lp, *lpNew;
      long                   esize = sizeof(iiMBufferLowerEntry_t) + size;

      if (!(lp = iiMBlockHashLower[index]) ||
	  !(lp->first))
      {
	 /* Allocate new memory array block. */
	 if (!(lpNew = (iiMemBlock_t *)malloc(sizeof(iiMemBlock_t) + esize * IPD_MEM_BLOCK_NUM)))
	 {
	    ipdSetError(ipdERR_IPDNO_MEMORY);
	    return NULL;
	 }
	 /* Link it to the List of blocks. */
	 lpNew->next              = lp;
	 iiMBlockHashLower[index] = lpNew;
	 /* Store first element. */
	 snl  = lpNew->first = (iiMBufferLowerEntry_t *)(((char *)lpNew) + sizeof(iiMemBlock_t));
	 /* Generate a linked list of array elements. */
	 snl2 = (iiMBufferLowerEntry_t *)(((char *)snl) + esize * (IPD_MEM_BLOCK_NUM - 1));
	 for (; snl < snl2; snl = (iiMBufferLowerEntry_t *)(((char *)snl) + esize))
	 {
	    snl->m.next   = (iiMBufferLowerEntry_t *)(((char *)snl) + esize);
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
	    snl->m.mSize  = size;
	    snl->filename = NULL;
	    snl->lineno   = 0;
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
	 }
	 snl->m.next   = NULL;
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
	 snl->m.mSize  = size;
	 snl->filename = NULL;
	 snl->lineno   = 0;
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
	 lp = lpNew;
      }
      snl           = lp->first;
      lp->first     = lp->first->m.next;
      snl->m.mSize  = size;
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
      snl->m.next   = NULL;
      snl->filename = filename;
      snl->lineno   = lineno;
      mallocCounter++;
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
      return snl + 1;
   }
   else
   {
      iiMBufferHigherEntry_t   *snh, *snh2 = NULL;

      /* For big memory sizes we need another strategy, because we must not
	 allocate 100 times of big sizes at once: We build up a hash where
	 the memory size represents the key. */
      index = iiMBlockComputeHigherKey(index);
      snh   = iiMBlockHashHigher[index];
      while ((snh))
      {
	 if (snh->m.mSize == size)
	 {  /* We've found some free memory. */
	    if ((snh2))
	       snh2->m.next = snh->m.next;
	    else
	       iiMBlockHashHigher[index] = snh->m.next;
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
	    snh->filename = filename;
	    snh->lineno   = lineno;
	    mallocCounter++;
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
	    return snh + 1;
	 }
	 snh2 = snh;
	 snh  = snh->m.next;
      }
      /* Allocate a new one. */
      if (!(snh = (iiMBufferHigherEntry_t *)malloc(sizeof(iiMBufferHigherEntry_t) + size)))
      {
	 ipdSetError(ipdERR_IPDNO_MEMORY);
	 return NULL;
      }
      snh->m.mSize    = size;               /* Set mSize. */
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
      snh->filename = filename;
      snh->lineno   = lineno;
      mallocCounter++;
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */

      /* Add block to a global list. */
      snh->nextMB   = iiMBlockListHigher;
      iiMBlockListHigher = snh;

      return snh + 1;
   }
#else /* IPD_USE_IPD_MALLOC */
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
   mallocCounter++;
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
   
   /* [JW] DEVTEMP */
   iiMemBlock_t *mem_pnt = malloc(size);
   if(mem_pnt)
      memset(mem_pnt, 0, size);
   return mem_pnt;

#endif /* IPD_USE_IPD_MALLOC */
} /*ipdMBalloc */

/** @brief Tests, if all blocks have been freed
 *
 * Works only, if compiled with IPD_MEMORY_ALLOC_ipdDEBUG_INFO set
 */
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
void iiTestMalloc(void)
{
   if (mallocCounter == 0)
      ipdStdLog("\nAll blocks have been freed.\n");
   else if (mallocCounter > 0)
      ipdStdLog("\n%d some blocks not freed!!! Check ipd!\n", mallocCounter);
   else
      ipdStdLog("\n%d some blocks freed twice!!! Check ipd!\n", -mallocCounter);
} /* iiTestMalloc */
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */

/** @brief Frees the complete memory block hash table */
void iiFreeMBHash(void)
{
   iiMBufferHigherEntry_t *snh, *snh2;
   iiMemBlock_t           *mb, *mb2;
   long                    i;

   if ((iiMBlockHashHigher))
   {
      snh = iiMBlockListHigher;
      while ((snh))
      {
	       snh2 = snh;
	       snh  = snh->nextMB;
	       free(snh2);
      }
      iiMBlockListHigher = NULL;

      free(iiMBlockHashHigher);  /* Must be freed with original free() */
      iiMBlockHashHigher = NULL;
   }
   if ((iiMBlockHashLower))
   {
      i = IPD_MBLOCK_LOWER_HASH_SIZE;
      while (i--)
      {
	       mb = iiMBlockHashLower[i];
	       while ((mb))
	       {
	          mb2 = mb;
	          mb  = mb->next;
	          free(mb2);
	       }
	       iiMBlockHashLower[i] = NULL;
      }
      free(iiMBlockHashLower);  /* Must be freed with original free() */
      iiMBlockHashLower = NULL;
   }
} /* iiFreeMBHash */

/** @brief Initializes the memory block garbage hashes. */
void iiSetMBHash(void)
{
   if (!(iiMBlockHashLower) &&
       !(iiMBlockHashLower = (iiMemBlock_t **)calloc(1,
						     sizeof(iiMemBlock_t *) *
						     IPD_MBLOCK_LOWER_HASH_SIZE)))
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return;
   }
   if (!(iiMBlockHashHigher) &&
       !(iiMBlockHashHigher = (iiMBufferHigherEntry_t **)calloc(1,
								sizeof(iiMBufferHigherEntry_t *) *
								IPD_MBLOCK_HIGHER_HASH_SIZE)))
   {
      free(iiMBlockHashLower);
      iiMBlockHashLower = NULL;
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return;
   }
   iiMBlockListHigher = NULL;
} /* iiSetMBHash */

/*********************************/
/*          Check Name           */
/*********************************/
/** @brief Checks, if name is a part of fullname. We need this function to detect if a fullname has been temporaryly modified. */
ipdBoolean iiCheckName(ipdTreeNode_t *tn)
{
   if (tn == iiGetRoot())
      return ipdTRUE;
   if ((tn) && (tn->node.sv.fullname) && (tn->node.sv.name) &&
       (tn->node.sv.fullname < tn->node.sv.name) &&
       (tn->node.sv.name     < tn->node.sv.fullname + iiStrLen(tn->node.sv.fullname)) &&
       (*tn->node.sv.name) && (*(tn->node.sv.name - 1)))
      return ipdTRUE;
   else
      return ipdFALSE;
} /* iiCheckName */

/*********************************/
/*              Hash             */
/*********************************/

/** @brief Initializes hash table. */
ipdBoolean iiInitHash(ipdHash_t *hash, long hashTableSize)
{
   if (hashTableSize <= 0)
      hashTableSize = IPD_DEFAULT_HASH_SIZE;


   if (!(hash->hashTable = ipdMBalloc(sizeof(ipdHashEntry_t *) * hashTableSize)))
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return ipdFALSE;
   }
/* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not needed .. */   
#if IPD_USE_IPD_MALLOC  
   /* Clear hash memory */
   ipdMBclear(hash->hashTable);
#endif
   hash->hashTableSize = hashTableSize;

   return ipdTRUE;
} /* iiInitHash */

/** @brief Gets a new ipdHashEntry_t structure from the hash garbage. If the garbage is empty, a new structure will be allocated. */
ipdHashEntry_t *iiCreateHashEntry(void)
{
   ipdHashEntry_t *hn;

   /* Malloc a new memory block. */
   if (!(hn = (ipdHashEntry_t *)ipdMBalloc(sizeof(ipdHashEntry_t))))
      return NULL;
   /* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not needed .. */   
#if IPD_USE_IPD_MALLOC  
   /* Fill with zeros. Important, because structures will not be initialized. */
   ipdMBclear(hn);
#endif

   return hn;
} /* iiCreateHashEntry */

/** @brief Frees a hash entry to the garbage. */
void iiFreeHashNode(ipdHashEntry_t *hn)
{
   ipdMBfree(hn);
} /* iiFreeHashNode */

/** @brief Cleans a hash table: Frees all nodes */
void iiCleanHash(ipdHash_t *hash)
{
   if ((hash) && (hash->hashTable))
   {
      long            hashTableSize = hash->hashTableSize;
      ipdHashEntry_t *hn, *hn2, **ht = hash->hashTable;

      while (hashTableSize--)
      {
	 hn = *ht;
	 while ((hn))
	 {
	    hn2 = hn->next;
	    /* Do not free hn->node. It must has been freed before. */
	    iiFreeHashNode(hn);
	    hn = hn2;
	 }
	 *ht++ = NULL;
      }
   }
} /* iiCleanHash */

/** @brief Frees a hash table: Frees all nodes, frees the hash. */
void iiFreeHash(ipdHash_t *hash)
{
   if ((hash) && (hash->hashTable))
   {
      iiCleanHash(hash);
      ipdMBfree(hash->hashTable);
      hash->hashTable     = NULL;
      hash->hashTableSize = 0;
   }
} /* iiFreeHash */

/** @brief Returns the hashkey of a given fullname. 

    This fullname may be given in
    two parts. string1 must be a valid name. string2 may be NULL if no
    second name part is wanted. This two strings will be concated with a
    '.' between them (if the previous character is not a '~'). The second
     string must not begin with a '~' or a '.'.
 */
long iiHashComputeKey(register const char *string,
		      ipdHash_t           *hash)
{
   register unsigned long h = 0;
   register unsigned long c;

   if ((string))
   {
      if (*string == '~')
	      string++; /* skip this */
      while ((c = (long)*string))
      {
	      h <<= 4;
	      h += c;
	      string++;
      }
      return h % hash->hashTableSize;
   }
   return 0;
} /* iiHashComputeKey */

/** @brief Removes a ipdTreeNode_t structure from the given hash. */
void iiHashRemoveTreeNode(ipdTreeNode_t *tn)
{
   if ((tn))
   {
      ipdHash_t      *hash;
      ipdHashEntry_t *p, *p2 = NULL;
      long            index;

      hash = &ipdBase->hashTable;

      index = iiHashComputeKey(tn->node.sv.fullname, hash);
      p     = hash->hashTable[index];
      while ((p))
      {
	 if ((p->node == tn))
	 {  /* We've found it. */
	    if ((p2))
	       p2->next = p->next;
	    else
	       hash->hashTable[index] = p->next;
	    iiFreeHashNode(p);
	    return;
	 }
	 p2 = p;
	 p = p->next;
      }
   }
} /* iiHashRemoveTreeNode */

/** @brief Stores a given ipdTreeNode_t structure (a variable or a section) in the hash. */
void iiHashStoreTreeNode(ipdTreeNode_t *tn)
{
   ipdHash_t      *hash = &ipdBase->hashTable;
   long            index;
   ipdHashEntry_t *hn;

   hn                     = iiCreateHashEntry();
   ipd_RETURNonERRORv;
   index                  = iiHashComputeKey(tn->node.sv.fullname, hash);
   hn->node               = tn;
   hn->next               = hash->hashTable[index];
   hash->hashTable[index] = hn;
} /* iiHashStoreTreeNode */

/** @brief Finds a ipdTreeNode_t in the hash ipdBase->hashTable by its fullname. */
ipdTreeNode_t *iiHashFindTreeNode(const char *name)
{
   ipdHashEntry_t *p;
   ipdTreeNode_t  *tn;
   ipdHash_t      *hash = &ipdBase->hashTable;
   
   if ((name) && (hash->hashTable))
   {
      long cnt = 0;
      for (p = hash->hashTable[iiHashComputeKey(name, hash)]; p; p = p->next)
      {  
         tn = (ipdTreeNode_t *)p->node;
         if (!iiStrCmp(name, tn->node.sv.fullname) && iiCheckName(tn))
         {
   	      return tn;
   	   }

   	   cnt++;
      }
   }
   return NULL; /* no hash-entry found */
} /* iiHashFindTreeNode */

/** @brief  Searches for an item of the given type in the hash.

    Consist of two parts: name1 must be set. name2 may be set. If name2
    is set name1 and name2 are concated to a fullname, if necessary a '.'
    will be set between the parts to seperate them.
    @return: pointer to the item found or NULL if not found
 */
ipdTreeNode_t *iiHashFindTreeNodeType(const char *name,
				      ipdType_t   type)
{
   ipdTreeNode_t *tn;

   if ((tn = iiHashFindTreeNode(name)))
   {
      if (!(type) || (tn->type & type))
	 return tn;     /* item: found */
      else
	 return NULL;   /* item: wrong type */
   }
   else
      return NULL; /* item: nonexistent */
} /* iiHashFindTreeNodeType */

/** @brief Special function, which stores a given node in the hash. Does not store it, if a node with the given name exists. */
void iiHashStoreItem(ipdTreeNodeDll_t **dll_p,
		     ipdTreeNode_t    *tn,
		     ipdBoolean          isLocal,
		     ipdHash_t        *hash)
{
   long                      index;
   ipdHashEntry_t           *hn;
   static ipdTreeNodeDll_t **static_dll_p;

   if ((dll_p))
   {
      static_dll_p = dll_p;
      return;
   }
   if ((hash) && (hash->hashTable) && (tn))
   {
      /* Find an existing node. */
      index = iiHashComputeKey(tn->node.sv.name, hash);
      for (hn = hash->hashTable[index]; hn; hn = hn->next)
      {
	 if ((tn == ((ipdTreeNodeDll_t *)hn->node)->tn) ||
	     (!iiStrCmp(tn->node.sv.name,
			(((ipdTreeNodeDll_t *)hn->node)->tn)->node.sv.name)))
	 {
	    if (isLocal)
	       /* Overwrite the node. */
	       ((ipdTreeNodeDll_t *)hn->node)->tn = tn;
	    /* Node with the same name found. */
	    return;
	 }
      }

      /* Nothing found. So store tn. */
      hn               = iiCreateHashEntry();
      ipd_RETURNonERRORv;
      /* Get Dll entry and pop it to the given list dll_p. */
      hn->node         = iiAppendTNFifo(static_dll_p, tn);
      if (ipdGetErrorStatus())
      {
	 iiFreeHashNode(hn);
	 return;
      }
      hn->next         = hash->hashTable[index];
      hash->hashTable[index] = hn;
      if (((*static_dll_p)->next))
	 static_dll_p  = &((*static_dll_p)->next);
   }
   return;
} /* iiHashStoreItem */

/** @brief Add all items of the given section to the hash. */
void iiAddAllItemsToStack(ipdTreeNodeDll_t **dll_p,
			  ipdTreeNode_t    *tn,
			  ipdHash_t        *hash)
{
   if ((dll_p))
   {
      /* Initializes static pointer in iiHashStoreItem(). */
      iiHashStoreItem(dll_p,
		      tn,
		      ipdFALSE,
		      hash);
      return;
   }
   if ((hash) && (tn) && (tn->type & ipdSECTION))
   {
      ipdBoolean       isLocal;

      isLocal = (tn->node.sv.flags & ipdSV_IS_TEMP_LOKAL);
      tn = tn->node.sv.svu.sect.child;
      while ((tn))
      {
	 if (tn->type & (ipdVARIABLE | ipdSECTION))
	 {
	    iiHashStoreItem(dll_p,
			    tn,
			    isLocal,
			    hash);
	    ipd_RETURNonERRORv;
	 }
	 tn = tn->next;
      }
   }
} /* iiAddAllItemsToStack */

/** @brief Tests by its name if tn is in the hash. If nothing is found NULL will be returned, otherwise tn. */
ipdTreeNodeDll_t *iiHashGetItem(ipdHash_t     *hash,
				ipdTreeNode_t *tn)
{
   long            index;
   ipdHashEntry_t *hn;

   if ((hash) && (hash->hashTable) && (tn))
   {
      /* Find an existing nod. */
      index = iiHashComputeKey(tn->node.sv.name, hash);
      for (hn = hash->hashTable[index]; hn; hn = hn->next)
      {
	 if ((tn == ((ipdTreeNodeDll_t *)hn->node)->tn) ||
	     (!iiStrCmp(tn->node.sv.name,
			(((ipdTreeNodeDll_t *)hn->node)->tn)->node.sv.name)))
	    /* Node found. */
	    return (ipdTreeNodeDll_t *)hn->node;
      }
   }
   return NULL;
} /* iiHashGetItem */

/*********************************/
/*             FIFOs             */
/*********************************/
/** @brief Gets a new ipdTreeNodeDll_t structure from the dll garbage. If the garbage is empty, a new structure will be allocated. */
ipdTreeNodeDll_t *iiCreateTnDll(void)
{
   ipdTreeNodeDll_t *dll;

   /* Malloc a new memory block. */
   if (!(dll = (ipdTreeNodeDll_t *)ipdMBalloc(sizeof(ipdTreeNodeDll_t))))
      return NULL;

   /* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not needed .. */   
#if IPD_USE_IPD_MALLOC
   /* Fill with zeros. Important, because structures will not be initialized. */
   ipdMBclear(dll);
#endif
   return dll;
} /* iiCreateTnDll */

/** @brief Frees a dll to the garbage. */
void iiFreeTnDll(ipdTreeNodeDll_t *dll)
{
   if ((dll))
   {
      if ((dll->prev))
	 dll->prev->next = dll->next;
      if ((dll->next))
	 dll->next->prev = dll->prev;
      ipdMBfree(dll);
   }
} /* iiFreeTnDll */

/** @brief Uses DLLs. Appends an new entry. */
ipdTreeNodeDll_t *iiAppendTNFifo(ipdTreeNodeDll_t **tnDll_p,
				 ipdTreeNode_t     *tn)
{
   ipdTreeNodeDll_t *dll, *dllTmp;

   if ((tnDll_p) && (tn))
   {
      dll = iiCreateTnDll();
      ipd_RETURNonERRORp;
      dll->tn   = tn;
      if ((dllTmp = *tnDll_p))
      {
	 while (dllTmp->next)
	    dllTmp = dllTmp->next;
	 dllTmp->next = dll;
	 dll->prev    = dllTmp;
      }
      else
	 *tnDll_p = dll;
      return dll;
   }
   return NULL;
} /* iiAppendTNFifo */

/** @brief Uses DLLs. Pushes an new entry. */
ipdTreeNodeDll_t *iiPushTNFifo(ipdTreeNodeDll_t **tnDll_p,
			       ipdTreeNode_t     *tn)
{
   ipdTreeNodeDll_t *dll;

   if ((tnDll_p) && (tn))
   {
      dll = iiCreateTnDll();
      ipd_RETURNonERRORp;
      dll->tn    = tn;
      dll->next  = *tnDll_p;
      if (*tnDll_p)
	 (*tnDll_p)->prev = dll;
      *tnDll_p   = dll;
      return dll;
   }
   return NULL;
} /* iiPushTNFifo */

/** @brief Uses DLLs. Pops the first entry. */
ipdTreeNode_t *iiPopTNFifo(ipdTreeNodeDll_t **tnDll_p)
{
   if ((tnDll_p) && (*tnDll_p))
   {
      ipdTreeNode_t    *tn;
      ipdTreeNodeDll_t *dll;

      dll      = *tnDll_p;
      if ((dll->next))
	 dll->next->prev = NULL;
      *tnDll_p = dll->next;
      tn       = dll->tn;

      iiFreeTnDll(dll);

      return tn;
   }
   return NULL;
} /* iiPopTNFifo */

/** @brief Finds a element tn in the fifo and returns its fifo-node. */
ipdTreeNodeDll_t *iiFindTnInFifo(const ipdTreeNodeDll_t *tnDll,
				 const ipdTreeNode_t    *tn)
{
   ipdTreeNodeDll_t *dll;

   dll = (ipdTreeNodeDll_t *)tnDll;
   while (dll)
   {
      if (dll->tn == tn)
	 return dll;
      dll = dll->next;
   }
   return NULL;
} /* iiFindTnInFifo */

/** @brief Returns the length of a single linked list ( by using next ) */
long ipdListLen(void *list)
{
   ipdSLL_t *sll = (ipdSLL_t *)list;
   long      len = 0;

   while ((sll))
   {
      len++;
      sll = sll->next;
   }
   return len;
} /* ipdListLen */

/** @brief Returns whether the variable is an external one. */
ipdBoolean ipdIsExternal(ipdTreeNode_t *tn)
{
   return ((tn->type & ipdVARIABLE) && tn->node.sv.svu.var.flags & ipdVF_EXTERN);
}

/*********************************/
/*    Dynamic Name Structure     */
/*********************************/
/** @brief Gets a new ipdDynamicName_t structure from the garbage. If the garbage is empty, a new structure will be allocated. */
ipdDynamicName_t *iiCreateDNNode(void)
{
   ipdDynamicName_t *dnn;

   /* Malloc a new memory block. */
   if (!(dnn = (ipdDynamicName_t *)ipdMBalloc(sizeof(ipdDynamicName_t))))
      return NULL;

   /* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not needed .. */   
#if IPD_USE_IPD_MALLOC
   /* Fill with zeros. Important, because structures will not be initialized. */
   ipdMBclear(dnn);
#endif
   return dnn;
} /* iiCreateDNNode */

/** @brief Frees a dynamic name node to the garbage. */
void iiFreeDNNode(ipdDynamicName_t *dnn)
{
   if ((dnn))
   {
      ipdMBfree(dnn->name);
      ipdMBfree(dnn);
   }
} /* iiFreeDNNode */

/*********************************/
/*             Units             */
/*********************************/
#if IPD_USE_UNITS
static ipdUnit_t **ipdUnitHash = NULL;
/** @brief Get a new unit node from the garbage. */
ipdUnit_t *iiCreateUnitNode(void)
{
   ipdUnit_t *un;

   /* Malloc a new memory block. */
   if (!(un = (ipdUnit_t *)ipdMBalloc(sizeof(ipdUnit_t))))
      return NULL;

   /* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not needed .. */   
#if IPD_USE_IPD_MALLOC
   /* Fill with zeros. Important, because structures will not be initialized. */
   ipdMBclear(un);
#endif
   return un;
} /* iiCreateUnitNode */

/** @brief Initializes unit hash table. */
void iiInitUnitHash(void)
{
   unsigned long len;

   if (!(ipdUnitHash))
   {
      /* Allocate hash memory */
      len = sizeof(ipdUnit_t *) * IPD_UNIT_HASH_SIZE;
      if (!(ipdUnitHash = (ipdUnit_t**)ipdMBalloc(len)))
      {  
	      ipdSetError(ipdERR_IPDNO_MEMORY);
	      return;
      }

   /* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not needed .. */   
   #if IPD_USE_IPD_MALLOC 
      /* Clear hash memory */
      ipdMBclear(ipdUnitHash);
   #endif
   }
} /* iiInitUnitHash */

/** @brief Frees a unit hash entry to the garbage. */
void iiFreeUnitHashNode(ipdUnit_t *un)
{
   if ((un))
   {
      if ((un->unitStr))
	 ipdvFree(un->unitStr);
      ipdMBfree(un);
   }
} /* iiFreeUnitHashNode */

/** @brief Frees the complete unit hash table.
 
  - All ipdUnit_t structures
  - The hash table
 */
void iiFreeUnitHash(void)
{
   ipdUnit_t *un, *un2;
   long       i;

   if ((ipdUnitHash))
   {
      i = IPD_UNIT_HASH_SIZE;
      while (i--)
      {
	 un = ipdUnitHash[i];
	 while ((un))
	 {
	    un2 = un->next;
	    ipdMBfree(un);
	    un = un2;
	 }
      }
      ipdMBfree(ipdUnitHash);
      ipdUnitHash = NULL;
   }
} /* iiFreeUnitHash */

/** @brief Sets ipdUnitHash to NULL */
void iiSetNullUnitHash(void)
{
   ipdUnitHash = NULL;
} /* iiSetNullUnitHash */

/** @brief Returns the unit hashcode of a given string.

The hashcode is the sum of all base unit representations.
The reason for this is, that a hashkey of "VA" can be
computed from the hashkeys of "V" and "A".
 */
long iiUnitHashComputeKey(ipduntUnitRep *unitRep)
{
   unsigned long h = 0;
   int  i = 0;

   while (i++ < ipduntNUM_BASIC_UNITS)
   {
      h = h << 8;
      h += unitRep->unitBasics[i];
   }
   h %= IPD_UNIT_HASH_SIZE;

   return h;
} /* iiUnitHashComputeKey */

/** @brief Returns the hashkey code of the compound node, where the hashkeys of the single two unit nodes are given. */
long iiUnitHashComputeCompoundCode(long a, long b)
{
   return ((a + b) % IPD_UNIT_HASH_SIZE);
} /* iiUnitHashComputeCompoundCode */

/** @brief Frees a ipdUnit_t and deletes it from the unit hash. But if the unit is used by other nodes, only the link counter will be decreased. */
void iiUnitHashRemoveUnit(ipdUnit_t *unr)
{
   long index;
   ipdUnit_t *un, *un2;

   if ((unr))
   {
      if ((unr->uLinkCount))
      {
	 unr->uLinkCount--;
	 return;
      }
      index = iiUnitHashComputeKey(&(unr->unitRep));
      un    = ipdUnitHash[index];
      un2   = NULL;
      while ((un))
      {
	 /* Compare the unit with the unit stored. */
	 /* Compare all unit base vectors. */
	 if (un == unr)
	 {  /* We've found it. */
	    if ((un2))
	       un2->next = un->next;
	    else
	       ipdUnitHash[index] = un->next;
	    break;
	 }
	 un2 = un;
	 un  = un->next;
      }
   }
   iiFreeUnitHashNode(unr);
   return;
} /* iiUnitHashRemoveUnit */

/** Gets the corresponding string of a unit. If it does not exist the unit string will be created. */
char *iiGetUnitString(ipdUnit_t *un)
{
   if ((un))
   {
      if ((un->unitStr))
	 return un->unitStr;
      if (!(un->unitStr = (char *)ipduntCreateUnitString(&(un->unitRep),
						      ipdUNITFLAGS)))
      {
	 ipdSetError(ipdERR_IPDNO_MEMORY);
	 return NULL;
      }
      return un->unitStr;
   }
   return NULL;
} /* iiGetUnitString */

/** @brief Copies a unit by simply increasing the link counter of the existing one. */
ipdUnit_t *iiCopyUnit(ipdUnit_t *un)
{
   if ((un))
      un->uLinkCount++;
   return un;
} /* iiCopyUnit */

/** @brief Stores the given unit structure in the hash.

If an equivalent unit
does exist, the existing one will be returned. Otherwise the new unit
will be stored and returned.
 */
ipdUnit_t *iiUnitHashStoreUnit(ipdUnit_t *unr)
{
   long index;
   ipdUnit_t *un;

   if ((unr))
   {
      index = iiUnitHashComputeKey(&(unr->unitRep));
      un = ipdUnitHash[index];
      while ((un))
      {
	 /* Compare the unit with the unit stored. */
	 if (ipduntUnitCompareable(&(un->unitRep), &(unr->unitRep)))
	 {
	    /* We've found the unit in the hash. */
	    return iiCopyUnit(un);
	 }
	 un = un->next;
      }
      unr->next          = ipdUnitHash[index];
      ipdUnitHash[index] = unr;
   }
   return unr;
} /* iiUnitHashStoreUnit */

/** @brief Finds an existing an unit already stored in the unit hash. 

If it exits,
a link counter is increased. Otherwise a new unit will be created.
The unitFactor is important too: This factor will be changed in such a
manner, so that it only must be mulitplicated to the value of the 
ipdTreeNode_t, e.g 10mA -> .01 "A"
 */
ipdUnit_t *iiUnitHashFindUnitStr(const char *unitStr)
{
   ipdUnit_t *un = NULL;
   ipdUnit_t *unr;

   if ((unitStr))
   {
      unr = iiCreateUnitNode();
      ipd_RETURNonERRORp;
      if (!(ipduntParseUnitString(unitStr, &(unr->unitRep))))
      {
	 ipdSetError(ipdERR_INVALID_UNIT, unitStr);
	 iiFreeUnitHashNode(unr);
	 return NULL;
      }
      /* Find the unit in the hash. */
      un = iiUnitHashStoreUnit(unr);
      if (un == unr)
      {
	 /* It has been stored as a new unit. */
	 un->unitRep.unitFactor = (ipdDouble)(un->unitRep.unitFactor *
					    (ipdDouble)pow(10.0, (double)(un->unitRep.unitBasics[ipduntUNIT_EXPONENT] >> 1)));
      }
      else
      {
	 /* We found the same unit. */
	 un->unitRep.unitFactor = (ipdDouble)(unr->unitRep.unitFactor *
					    (ipdDouble)pow(10.0, (double)(unr->unitRep.unitBasics[ipduntUNIT_EXPONENT] >> 1)));
	 un->unitRep.unitBasics[ipduntUNIT_EXPONENT] = unr->unitRep.unitBasics[ipduntUNIT_EXPONENT];
	 /* Free the allocated unit structure. It exists already. */
	 iiFreeUnitHashNode(unr);
      }
   }
   return un;
} /* iiUnitHashFindUnitStr */

/** @brief Creats an new unit which is the product of unit 1 and unit 2: result = u1 * u2; */
void iiMultiplyUnits(ipdUnit_t **resUnit, ipdUnit_t *u2)
{
   int i;

   if (!(resUnit) && !(u2))
      return;
   if ((u2))
   {
      if ((*resUnit))
      {
	 for (i = 1; i <= ipduntNUM_BASIC_UNITS; i++)
	    /* Sum up new units */
	    (*resUnit)->unitRep.unitBasics[i] += u2->unitRep.unitBasics[i];
      }
      else
      {
	 *resUnit = iiCreateUnitNode();
	 ipd_RETURNonERRORv;
	 for (i = 1; i <= ipduntNUM_BASIC_UNITS; i++)
	    (*resUnit)->unitRep.unitBasics[i] = u2->unitRep.unitBasics[i]; /* Invert unit */
      }
   }
   if ((*resUnit))
      for (i = 1; i <= ipduntNUM_BASIC_UNITS; i++)
	 if (((*resUnit)->unitRep.unitBasics[i]))
	    return;

   /* It is equal to "1". */
   /* Free the allocated unit structure. It exists already. */
   iiFreeUnitHashNode((*resUnit));
   *resUnit = NULL;
} /* iiMultiplyUnits */

/** @brief Creates an new unit which is the result of the division of unit 1 and unit 2: result = u1 / u2; */
void iiDivideUnits(ipdUnit_t **resUnit, ipdUnit_t *u2)
{
   int i;

   if (!(resUnit) && !(u2))
      return;
   if ((u2))
   {
      if ((*resUnit))
      {
	 for (i = 1; i <= ipduntNUM_BASIC_UNITS; i++)
	    /* Sum up new units */
	    (*resUnit)->unitRep.unitBasics[i] -= u2->unitRep.unitBasics[i];
      }
      else
      {
	 *resUnit = iiCreateUnitNode();
	 ipd_RETURNonERRORv;
	 for (i = 1; i <= ipduntNUM_BASIC_UNITS; i++)
	    (*resUnit)->unitRep.unitBasics[i] = - u2->unitRep.unitBasics[i]; /* Invert unit */
      }
   }
   if ((*resUnit))
      for (i = 1; i <= ipduntNUM_BASIC_UNITS; i++)
	 if (((*resUnit)->unitRep.unitBasics[i]))
	    return;

   /* It is equal to "1". */
   /* Free the allocated unit structure. It exists already. */
   iiFreeUnitHashNode(*resUnit);
   *resUnit = NULL;
} /* iiDivideUnits */

/** @brief Compares two units. */
ipdBoolean iiUnitCompareable(ipdUnit_t *u1, ipdUnit_t *u2)
{
   int i;

   if (!(u1) && !(u2))
      return ipdTRUE;
   if ((u1))
   {
      if ((u2))
	 return (ipduntUnitCompareable(&(u1->unitRep), &(u2->unitRep))) ? ipdTRUE : ipdFALSE;
      else
      {
	 u2 = u1;
	 u1 = NULL;
      }
   }
   if (!(u1) && (u2))
      for (i = 1; i <= ipduntNUM_BASIC_UNITS; i++)
	 if ((u2->unitRep.unitBasics[i]))
	    return ipdFALSE;

   return ipdTRUE;
} /* iiUnitCompareable */
#endif /* IPD_USE_UNITS */

/*********************************/
/*           TreeNodes           */
/*********************************/

/** @brief Connects node newChild as lastChild of the parent node */
ipdTreeNode_t *iiConnectTreeNode(ipdTreeNode_t *parent, ipdTreeNode_t *newChild)
{
   if ((newChild))
   {
      newChild->node.sv.parent = parent;   /* The parent section  */
      newChild->next           = NULL;     /* It's the last child */
      if ((parent))
      {
	 /* The parent is a section! */
	 if ((parent->node.sv.svu.sect.child)) /* Do the parents have children? */
	 {                      /* IPDYES */
	    parent->node.sv.svu.sect.lastChild->next = newChild;
	    newChild->node.sv.prev                   = parent->node.sv.svu.sect.lastChild;
	 }
	 else
	 {                      /* IPDNO */
	    parent->node.sv.svu.sect.child           = newChild;
	    newChild->node.sv.prev                   = NULL;
	 }
	 parent->node.sv.svu.sect.lastChild = newChild; /* The new lastChild */
      }
      return parent;
   }
   return NULL;
} /* iiConnectTreeNode */

/** @brief iiUnlinkTreeNode() doesn't delete anything. The given ipdTreeNode_t structure (a section or a variable) will be simply unlinked of the whole section-tree. */
void iiUnlinkTreeNode(ipdTreeNode_t *tn)
{
   if ((tn))
   {
      if ((tn->node.sv.prev))
      {
	 tn->node.sv.prev->next = tn->next;
      }
      else
      {
	 /* No previous node. So: It's the first child of a parent section. */
	 if ((tn->node.sv.parent))
	 {
	    if (tn->node.sv.parent->node.sv.svu.sect.child == tn)
	       tn->node.sv.parent->node.sv.svu.sect.child     = tn->next;
	 }
      }

      if ((tn->next))
      {
	 tn->next->node.sv.prev = tn->node.sv.prev;
      }
      else
      {
	 /* No next node. So: It's the last child of a parent section. */
	 if ((tn->node.sv.parent))
	 {
	    if (tn->node.sv.parent->node.sv.svu.sect.lastChild == tn)
	       tn->node.sv.parent->node.sv.svu.sect.lastChild = tn->node.sv.prev;
	 }
      }
      tn->node.sv.prev       = NULL;
      tn->next               = NULL;
      tn->node.sv.parent     = NULL;
   }
} /* iiUnlinkTreeNode */

/** @brief Get a new treenode from the garbage. */
ipdTreeNode_t *iiCreateTreeNode(ipdType_t type)
{
   ipdTreeNode_t *tn;

   if (!(tn = (ipdTreeNode_t *)ipdMBalloc(sizeof(ipdTreeNode_t))))
      return NULL;

   /* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not required .. */   
#if IPD_USE_IPD_MALLOC  
   /* Fill with zeros. Important, because structures will not be initialized. */
   ipdMBclear(tn);
#endif

   tn->type = type;
   return tn;
} /* iiCreateTreeNode */

/** @brief Cleans a ipdTreeNode_t structure. Frees all strings, units, arrays, ...

Frees all strings, units, arrays, ...
This means: All memories allocated to represent values will be freed. So
if the structure to be freed represents a '+/-'-operator (iiSum) which
represents a value, only the value is cleared. The operator still
remains, its operands leave unchanged.
 */
void iiClearValue(ipdTreeNode_t *tn)
{
   if (tn->type & (ipdARRAYnVALUE))
   {
      if (tn->type & ipdSTRING)
      {
	 ipdMBfree(tn->node.expr.val.string);
	 tn->node.expr.val.string = NULL;
      }
      else if (tn->type & ipdQUANTITY)
      {
#if IPD_USE_UNITS
	 iiUnitHashRemoveUnit(tn->node.expr.val.quantity.unit);
	 tn->node.expr.val.quantity.unit = NULL;
#endif /* IPD_USE_UNITS */
      }
      else if (tn->type & ipdARRAYVAL)
      {
	 ipdMBfree(tn->node.expr.val.arrayval.tree);
	 tn->node.expr.val.arrayval.tree = NULL;
      }
      tn->type &= ~(ipdARRAYnVALUE);
   }
} /* iiClearValue */

/** @brief Checks whether the given item is constant for sure. */
ipdBoolean iiIsConstant(ipdTreeNode_t *tn)
{
   return (!(tn->type & (~ipdARRAYnVALUE)) ||         /* A pure value */
#if !IPD_EVAL_DELETES_EXPR
	   (tn->type & ipdEVAL) ||                    /* An evaluated expr. */
#endif
	   ((tn->type & (ipdARRAY |                   /* A constant expr. */
			 ipdOPERATOR |
			 ipdINTERNFUNCTION |
			 ipdCCODEDFUNCTION)) &&
	    (tn->node.expr.eFlags & ipdEF_CONSTANT)));
} /* iiIsConstant */

/** @brief Cleans a ipdTreeNode_t structure if it consists of temporary values created at calculation time.

Values stored in a treenode containing only this value must not be freed.
 */
void iiClearTemporaryValue(ipdTreeNode_t *tn)
{
   if (iiIsConstant(tn))
      return;
   else
      iiClearValue(tn); /* Clean the value. */
} /* iiClearTemporaryValue */

/** @brief Copys a only the value of a node. If docopy is ipdTRUE, the value will be copied else it will be moved.
 */
void iiCopyValue(ipdTreeNode_t *copy, ipdTreeNode_t *tree, ipdBoolean docopy)
{
   if (!(copy) || !(tree))
      return;

   iiClearValue(copy);

#if IPD_ipdDEBUG
   printf("iiCopyValue of type %s\n", ipdGetTypeStr(tree->type, ipdTRUE));
#endif

   if (tree->type & ipdINTEGER)
      copy->node.expr.val.integer     = tree->node.expr.val.integer;
   else if (tree->type & ipdREAL)
      copy->node.expr.val.quantity.re = tree->node.expr.val.quantity.re;
   else if (tree->type & ipdCOMPLEX)
   {
      copy->node.expr.val.quantity.re = tree->node.expr.val.quantity.re;
      copy->node.expr.val.quantity.im = tree->node.expr.val.quantity.im;
   }
   else if (tree->type & ipdSTRING)
   {
      if (docopy)
      {
	 if ((tree->node.expr.val.string))
	 {
	    if (!(copy->node.expr.val.string = iiStrDup(tree->node.expr.val.string)))
	       return;
	 }
	 else
	    copy->node.expr.val.string = NULL;
      }
      else /* MOVE */
      {
	 copy->node.expr.val.string = tree->node.expr.val.string;
	 tree->node.expr.val.string = NULL;
      }
   }
   else if (tree->type & ipdBOOLEAN)
      copy->node.expr.val.boolean = tree->node.expr.val.boolean;
   else if (tree->type & ipdARRAYVAL)
   {
      if (docopy)
      {
	 ipdTreeNode_t **lFrom, **lTo;
	 long len;

	 copy->node.expr.val.arrayval.len = len = tree->node.expr.val.arrayval.len;
	 if (!(copy->node.expr.val.arrayval.tree = (ipdTreeNode_t **)ipdMBalloc(sizeof(ipdTreeNode_t *) * len)))
	 {
	    ipdSetError(ipdERR_IPDNO_MEMORY);
	    return;
	 }
	 lFrom = tree->node.expr.val.arrayval.tree;
	 lTo   = copy->node.expr.val.arrayval.tree;
	 while (len--)
	    *lTo++ = *lFrom++;
      }
      else /* MOVE */
      {
	 copy->node.expr.val.arrayval.tree = tree->node.expr.val.arrayval.tree;
	 copy->node.expr.val.arrayval.len  = tree->node.expr.val.arrayval.len;
	 tree->node.expr.val.arrayval.tree = NULL;
	 tree->node.expr.val.arrayval.len  = 0;
      }
   }
   if (tree->type & ipdQUANTITY)
   {
      if (docopy)
      {
#if IPD_USE_UNITS
	 copy->node.expr.val.quantity.unit = iiCopyUnit(tree->node.expr.val.quantity.unit);
#endif /* IPD_USE_UNITS */
      }
      else /* MOVE */
      {
#if IPD_USE_UNITS
	 copy->node.expr.val.quantity.unit = tree->node.expr.val.quantity.unit;
	 tree->node.expr.val.quantity.unit = NULL;
#endif /* IPD_USE_UNITS */
      }
   }

   copy->type |= (tree->type & ipdARRAYnVALUE);
   if (!docopy)
      tree->type &= (~ipdARRAYnVALUE);
} /* iiCopyValue */

/** @brief If the value stored in tree is a constant, copy the value, else move it */
void iiAssignValue(ipdTreeNode_t *copy, ipdTreeNode_t *tree)
{
   iiCopyValue(copy, tree, iiIsConstant(tree));
} /* iiAssignValue */

/** @brief Frees a ipdTreeNode_t structure and its contents.

The flag removeFromHash should be used only, when a regular variable or
section has to be deleted. If the flag is set, items like variables and
sections are removed from the internal Hash.
If the whole ipd database has to be removed, it is recommended to set
removeFromHash to ipdFALSE and to delete the hash afterwards.
 */
void iiFreeTreeNodeRec(ipdTreeNode_t *tn, ipdBoolean removeFromHash)
{
   if ((tn))
   {
      /* Remove an expression */

      /* Delete a variable */
      if (tn->type & ipdVARIABLE)
      {
	 /* remove from hash, remove from section tree */
	 if (!(removeFromHash))
	    iiHashRemoveTreeNode(tn);
	 iiUnlinkTreeNode(tn);

	 /* Delete the tree of a variable */
	 if ((tn->node.sv.svu.var.tree))
	    iiFreeTreeNodeRec(tn->node.sv.svu.var.tree, ipdFALSE);

	 iiFreeTreeNodeRec(tn->node.sv.svu.var.traceTree, ipdFALSE);
	 ipdMBfree(tn->node.sv.fullname);
      }

      /* Delete a section */
      else if (tn->type & ipdSECTION)
      {
	 if (!(removeFromHash))
	 {
	    iiHashRemoveTreeNode(tn);
	 }
	 /* Unlink from neighbours. */
	 iiUnlinkTreeNode(tn);

	 iiFreeTreeNodeRec(tn->node.sv.svu.sect.ancestor, ipdFALSE);

	 /* Delete all entries in this section */
	 while ((tn->node.sv.svu.sect.child))
	    iiFreeTreeNodeRec(tn->node.sv.svu.sect.child, removeFromHash);

	 ipdMBfree(tn->node.sv.fullname);
	 if (tn == ipdBase->root_p)
	    ipdBase->root_p = NULL;
      }

      /* Remove the condition expression */
      else if (tn->type & ipdSECTION_REF)
      {
	 ipdMBfree(tn->node.sectRef.name);
	 iiFreeTreeNodeRec(tn->node.sectRef.condTree, ipdFALSE);
      }

      /* Remove the parameters of an ipdINTERNFUNCTION */
      else if (tn->type & ipdPARAMETER)
      {
	 ipdMBfree(tn->node.parameter.name);
      }
      /* ipdPARAMETER_REF: do nothing
      else if (tn->type & ipdPARAMETER_REF)
      {
      }
      */
      /* Remove optional parameters. */
      else if (tn->type & ipdOPTION)
      {
	 iiFreeTreeNodeRec(tn->node.option.tree, ipdFALSE);
	 ipdMBfree(tn->node.option.name);
      }

      /* Delete a ipdINCLUDE structure. */
      else if (tn->type & ipdINCLUDE)
      {
	 ipdMBfree(tn->node.sv.fullname);
	 iiUnlinkTreeNode(tn);
      }

      /* Delete a ipdALIAS structure. */
      else if (tn->type & ipdALIAS)
      {
	 iiFreeTreeNodeRec(tn->node.sv.svu.alias.condTree, ipdFALSE);
	 ipdMBfree(tn->node.sv.fullname);
	 iiUnlinkTreeNode(tn);
      }

      else
      {
	 /* Clean values */
	 iiClearValue(tn);

	 /* Removes an array of expression trees. */
	 /* Delete an inherited variable */
	 if (tn->type & ipdVARIABLE_REF)
	 {
	    ipdMBfree(tn->node.expr.func.varref.name);
	 }

	 else if (tn->type & ipdARRAY)
	 {
	    iiFreeTreeNodeRec(tn->node.expr.func.array, ipdFALSE);
	 }
	 /* Recursive remove operands */
	 else if (tn->type & ipdOPERATOR)
	    iiFreeTreeNodeRec(tn->node.expr.func.op.operand, ipdFALSE);

	 /* Remove intenal function. */
	 else if ((tn->type & ipdINTERNFUNCTION))
	 {
	    /* Don't free the intern function. */
	    /* Free the parameter nodes. */
	    iiFreeTreeNodeRec(tn->node.expr.func.internFunc.param, ipdFALSE);
	 }

	 /* Remove C-coded functions. */
	 else if ((tn->type & ipdCCODEDFUNCTION))
	 {
	    if ((tn->node.expr.func.cCodedFunc.userDestructor))
	       tn->node.expr.func.cCodedFunc.userDestructor(tn);
	    iiFreeTreeNodeRec(tn->node.expr.func.cCodedFunc.param, ipdFALSE);
	 }

	 else if (tn->type & ipdINDEX)
	 {
	    iiFreeTreeNodeRec(tn->node.expr.func.arrayIndex.tree, ipdFALSE);
	    iiFreeTreeNodeRec(tn->node.expr.func.arrayIndex.index, ipdFALSE);
	    iiFreeTreeNodeRec(tn->node.expr.func.arrayIndex.next, ipdFALSE);
	 }
	 /* Delete an 'eval' node. */
#if !IPD_EVAL_DELETES_EXPR
	 else if (tn->type & ipdEVAL)
	 {
	    if ((tn->node.expr.func.evalTree))
	       iiFreeTreeNodeRec(tn->node.expr.func.evalTree, ipdFALSE);
	 }
#endif	
      }

      ipdMBfree(tn->comment);
      if (tn->next)
	 iiFreeTreeNodeRec(tn->next, removeFromHash);
      ipdMBfree(tn);
   }
} /* iiFreeTreeNodeRec */

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

/*=======================================================================
	String operations
=======================================================================*/

/** @brief Converts a long to an ascii string. */
char *ltoa2(char *str, long num)
{
   if (num < 0)
   {
      *str++ = '-';
      num = -num;
   }
   if (num > 10)
      str = ltoa2(str, num / 10);
   *str++ = (char)(num % 10) + '0';
   *str  = 0;
   return str;
} /* ltoa2 */

/** @brief Compares the two given strings. */
int iiStrCmp(const char *s1, const char *s2)
{
   if ((s1) && (*s1 == '\0'))
      s1 = NULL;
   if ((s2) && (*s2 == '\0'))
      s2 = NULL;

   if ((s1))
   {
      if ((s2))
	return strcmp(s1, s2);
      else
	 return 1;
   }
   else if ((s2))
      return -1;

   return 0;
} /* iiStrCmp */

/** @brief Compares the two name-strings. The first name-string is splitted into two parts. */
int iiStrCmpNamePart(const char *p1, const char *p2, const char *s)
{
   const char *s1 = p1;

   if (!(p1))
      p1 = "";
   if (!(p2))
      p2 = "";
   if (!(s))
      s = "";

   while ((*s) && (*p1 == *s))
   {
      p1++;
      s++;
   }
   if ((*p1) || (!(*s) && (*p2)))
      return 1;
   if ((*s1 == '~') && (*(s1 + 1) != '\0') && (*p2) && (*s++ != '.'))
      return 1;
   while ((*s) && (*p2 == *s))
   {
      p2++;
      s++;
   }
   if (*p2 == *s)
      return 0;

   return 1;
} /* iiStrCmpNamePart */

/** @brief Test, if str1 equals to str2. If equal, 0 will be returned. But 0 will be returned too, if the end of one of the two string has bee reached. */
int iiStrCmpSoft(char *str1, char *str2)
{
   if (!str1)
      str1 = (char *)"";
   if (!str2)
      str2 = (char *)"";
   while ((*str1) && (*str2))
      if (*str1 != *str2)
	 return (*str1 > *str2) ? 1 : -1;
   return 0;
} /* iiStrCmpSoft */

/** @brief Gets the lenght of a string. */
long iiStrLen(const char *s1)
{
   if ((s1))
      return strlen(s1);
   return 0;
} /* iiStrLen */

/** @brief Concates the two given strings. Nothing is freed. */
char *iiStrConcate(char *s1, char *s2)
{
   char *s;
   unsigned long  len1 = (s1) ? strlen(s1) : 0;
   unsigned long  len2 = (s2) ? strlen(s2) : 0;

   if (!(len1 + len2))
      return NULL;
   if (!(s = (char *)ipdMBalloc( 1 + len1 + len2 )))
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return NULL;
   }

   *s = '\0';
   if ((s1))
      strcpy(s, s1);

   if ((s2))
      strcpy(s + len1, s2);

   return s;
} /* iiStrConcate */

/** @brief Appends the second string to the first. The first string is freed(!!!). The second string is untouched. */
char *iiStrAppend(char *s1, char *s2)
{
   char *s;

   s = iiStrConcate(s1, s2);
   ipdMBfree(s1);
   return s;
} /* iiStrAppend */

/** @brief Appends a character c to the string s1. The new string will be returned. */
char *iiStrAppendChar(ipdChar *s1, ipdLong c)
{
   char *s;
   unsigned long len;

   if (!(s1))
   {
      if (!(s = (char *)ipdMBalloc(2)))
      {
	 ipdSetError(ipdERR_IPDNO_MEMORY);
	 return NULL;
      }
      s[0] = (char)c;
      s[1] = 0;
      return s;
   }
   len = strlen(s1);
   if (!(s = (char *)ipdMBalloc(len + 2)))
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return NULL;
   }
   strcpy(s, s1);
   ipdMBfree(s1);
   s[len]=(char)c;         /* the appended char */
   s[len+1]='\0';    /* null-termination */
   return s;
} /* iiStrAppendChar */

/** @brief Copies the string. Allocates necessary memory. */
char *iiStrDup(const char *s)
{
   char *dup = NULL;

   if (!(s))
   {
      if ((dup = (char *)ipdMBalloc(1)))
      {
	 *dup = '\0';
	 return dup; /* PFUI,PFUI */
      }
      else
      {
	 ipdSetError(ipdERR_IPDNO_MEMORY);
	 return NULL;
      }
   }
   else if (!(dup = (char *)ipdMBalloc(strlen(s) + 1)))
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return NULL;
   }
  
   strcpy(dup, s);

   return dup;
} /* iiStrDup */

/** @brief Appends the string sFrom to the string *sTo_p if append == ipdTRUE. 

If not,
sFrom will be compied to *sTo_p. sTo_p is a pointer to a buffer of the
length *len_p, which may be overflowed. In this case the buffer is
reallocated with a increased lenght.
 */
#define IPD_STRING_LEN_INC  80
ipdBoolean iiStrExtend(char       **sTo_p,
		     long        *len_p,
		     const char  *sFrom,
		     ipdBoolean     append)
{
   if ((sTo_p) && (sFrom) && (len_p))
   {
      long  l1, l2;
      char *str, *tmp;

      if ((*sTo_p))
	 l1 = strlen(*sTo_p);
      else
	 l1 = 0;

      if (!(sFrom))
	 sFrom = (char *)"";
      l2 = strlen(sFrom) + 3; /* + 2 is necessary ('.' + '\0'). + 3 is save. */
      while (*sFrom == '.')
	 sFrom++;

      if (append)
      {
	 if (l1 + l2 > *len_p)
	 {
	    *len_p = ((l1 + l2 + (IPD_STRING_LEN_INC << 1)) /
		      IPD_STRING_LEN_INC) * IPD_STRING_LEN_INC;

	    if (!(tmp = (char *)ipdMBalloc((unsigned long)*len_p)))
	    {
	       ipdSetError(ipdERR_IPDNO_MEMORY);
	       return ipdFALSE;
	    }
	    strcpy(tmp, *sTo_p);
	    ipdMBfree(*sTo_p);
	    *sTo_p = tmp;
	 }
	 if ((*sFrom))
	 {
	    str = *sTo_p + l1;
	    if (*(str - 1) != '~')
	       *str++ = '.';
	 }
	 else
	    return ipdTRUE;
      }
      else
      {
	 if (l2 > *len_p)
	 {
	    *len_p = ((l2 + (IPD_STRING_LEN_INC << 1)) /
		      IPD_STRING_LEN_INC) * IPD_STRING_LEN_INC;
	    ipdMBfree(*sTo_p);
	    if (!(*sTo_p = (char *)ipdMBalloc((unsigned long)*len_p)))
	    {
	       ipdSetError(ipdERR_IPDNO_MEMORY);
	       return ipdFALSE;
	    }
	 }
	 str = *sTo_p;
      }
      strcpy(str, sFrom);
      return ipdTRUE;
   }
   return ipdFALSE;
} /* iiStrExtend */

/** @brief Returns the total number of the character of ostr in the string string. */
long iiStrocc(const char *string, const char *ostr)
{
   const char *s, *o;
   char        c;
   long        l = 0;

   if (!(string) || !(ostr))
      return 0;

   o = (char *)ostr;

   while ((c = *o++))
   {
      s = (char *)string;
      while (*s)
      {
	 if (*s == c)
	    l++;
	 s++;
      }
   }
   return l;
} /* iiStrocc */

/*=======================================================================
	QUERY
=======================================================================*/

/** @brief Returns the tree root */
ipdTreeNode_t *iiGetRoot(void)
{
   return ipdBase->root_p;
} /* iiGetRoot */

/** @brief  Returns a pointer to the CurrentSection */
ipdTreeNode_t *iiGetCS(void)
{
   return ipdBase->CS_p;
} /* iiGetCS */

/** @brief Make the passed pointer the CurrentSection */
void iiSetCS(ipdTreeNode_t *newCS)
{
   ipdBase->CS_p = newCS;
} /* iiSetCS */

/** @brief Creates a full name

snp is a string, containing an absolute or relative path and the
name itself. The fullname will be returned (allocates memory).
 */
char *iiCreateName(const char *name, const char *CSFullName)
{
   char          *fullname; /* Memory space for the resulting name to be returned */
   const char    *snp;      /* Source name pointer */
   char          *tnp;      /* Temporary name pointer */
   ipdTreeNode_t *CS;

#if IPD_ipdDEBUG
   printf("iiCreateName(%s, %s)\n", name, CSFullName);
#endif

   if (!(name))
      return NULL;
   /* For speed up test, if name is simply the root section or
      the current section. */
   if (!(strcmp(name, "~")))
      return iiStrDup("~");
   else if ((ipdBase->CSFullName) && !(strcmp(name, ".")))
      return iiStrDup(ipdBase->CSFullName);

   snp = name;
   if (!(snp) || !(*snp))
      return NULL;

   if (!(CSFullName))
   {
      if ((ipdBase->CSFullName))
	 CSFullName = ipdBase->CSFullName;
      else if ((CS = iiGetCS()))
      {
	 /* get CurrentSection */
	 CSFullName = CS->node.sv.fullname;
      }
      else
	 CSFullName = (char *)"~";
   }

   /* Never mind if there'll be some bytes more allocated than needed. */
   /* Better than difficult calculating the exact length */
   /* Allocate Memory: Secure length is name-length + CS-name-length + 1 !!! */
   if (!(fullname = (char *)ipdMBalloc(strlen(CSFullName) + strlen(snp) + 2) ))
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return NULL;
   }
   /* Copy only the current section fullname as relative start path */
   strcpy(fullname, CSFullName);  /* path */
   tnp = fullname + strlen (fullname) - 1;
   if (*tnp != '~')
      tnp++;
   /* Helpfull for following algorithm: Overwrite leading '~' with a '.'*/
   *fullname = '.';

   /* Now compose fullname */
   while ((*snp))
   {
      switch (*snp)
      {
      case '~': /* Jump to the beginning */
	 tnp = fullname;
	 *tnp = 0;
	 snp++;
	 break;
      case '^': /* Jump back one section */
	 while ((tnp > fullname) && (*tnp == '.'))
	    tnp--;
	 while ((tnp > fullname) && (*tnp != '.'))
	    tnp--;
	 *tnp = 0; /* Delete a '.', if there is one. */
	 snp++;
	 break;
      case '.': /* Do nothing */
	 snp++;
	 break;
      default: /* Copy the name */
	 *tnp++ = '.'; /* leading '.' */
	 /* Now copy the name */
	 while ((*snp) && (*snp != '~') && (*snp != '^') && (*snp != '.'))
	 {
	    if (((*snp >= 'A') && (*snp <= 'Z')) ||
		((*snp >= 'a') && (*snp <= 'z')) ||
		((*snp >= '0') && (*snp <= '9')) || (*snp == '_'))
	       *tnp++ = *snp++;
	    else
	    {
	       ipdMBfree(fullname);

	       ipdSetError(ipdERR_INVALID_NAME, name);
	       return NULL;
	    }
	 }
	 break;
      }
   }
   if (tnp == fullname)
      tnp++;
   *tnp = '\0';     /* End string end. */
   /* Write back leading '~' at the Beginning of the fullname */
   *fullname = '~';

#if IPD_ipdDEBUG
   printf("iiCreateName result: %s\n", fullname);
#endif

   return fullname;
} /* iiCreateName */

/** @brief Creates a relative name.

snp is a string, containing an absolute or relative path and the
name itself. The relative name will be returned (allocates memory).
 */
char *iiCreateRelName(const char *name)
{
   char          *relName; /* Memory space for the resulting name to be returned */
   const char    *snp;      /* Source name pointer */
   char          *tnp;      /* Temporary name pointer */

   if (!(name) || !(*name))
      return NULL;

   snp = name;

   /* Allocate Memory:  length is name-length + 1 */
   if (!(relName = (char *)ipdMBalloc(strlen(snp) + 1)))
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return NULL;
   }
   tnp = relName;

   /* Now compose relName */
   while ((*snp))
   {
      switch (*snp)
      {
      case '~': /* Jump to the beginning */
	 tnp    = relName;
	 *tnp++ = '~';
	 snp++;
	 break;
      case '^': /* Jump back one section */
	 if ((tnp == relName) || (*(tnp - 1) == '^'))
	    *tnp++ = '^';
	 else if (*(tnp - 1) != '~')
	 {
	    tnp--;
	    while ((tnp > relName) && (*tnp != '~') &&
		   (*tnp != '^') && (*tnp != '.'))
	       tnp--;
	    if ((*tnp == '~') || (*tnp == '^'))
	       tnp++;
	 }
	 snp++;
	 break;
      case '.': /* Do nothing */
	 snp++;
	 break;
      default: /* Copy the name */
	 if ((tnp > relName) && (*(tnp - 1) != '~') &&
	     (*(tnp - 1) != '^'))
	    *tnp++ = '.'; /* leading '.' */
	 /* Now copy the name */
	 if (isalpha(*snp) || (*snp == '_'))
	    *tnp++ = *snp++;
	 else
	 {
	    ipdMBfree(relName);

	    ipdSetError(ipdERR_INVALID_NAME, name);
	    return NULL;
	 }
	 while ((*snp) && (*snp != '~') && (*snp != '^') && (*snp != '.'))
	 {
	    if ((isalnum(*snp)) || (*snp == '_'))
	       *tnp++ = *snp++;
	    else
	    {
	       ipdMBfree(relName);
	      
	       ipdSetError(ipdERR_INVALID_NAME, name);
	       return NULL;
	    }
	 }
	 break;
      }
   }
   *tnp = 0;     /* End string end. */

#if IPD_ipdDEBUG
   printf("iiCreateRelName result: %s\n", relName);
#endif

   return relName;
} /* iiCreateRelName */

/** @brief Corrects a given full name. Nothing will be allocated. */
char *iiCorrectName(char *name)
{
   char *snp;      /* Source name pointer */
   char *tnp;      /* Temporary name pointer */

   tnp = snp = name;
   if (!(snp) || !(*snp))
      return NULL;

   /* Now compose fullname */
   while ((*snp))
   {
      switch (*snp)
      {
      case '~': /* Jump to the beginning */
	 tnp    = name;
	 *tnp++ = '~';
	 snp++;
	 break;
      case '^': /* Jump back one section */
	 if ((tnp > name) && *(tnp - 1) != '~')
	 {
	    tnp--;
	    if ((tnp > name) && *(tnp - 1) == '.')
	       tnp--;
	    while ((tnp > name) && (*tnp != '.'))
	       tnp--;
	    if ((tnp == name))
	       *tnp++ = '~';
	 }
	 snp++;
	 break;
      case '.': /* Do nothing */
	 snp++;
	 break;
      default: /* Copy the name */
	 if ((tnp > name) && (*(tnp - 1) != '~'))
	    *tnp++ = '.'; /* leading '.' */
	 /* Now copy the name */
	 while ((*snp) && (*snp != '~') && (*snp != '^') && (*snp != '.'))
	    *tnp++ = *snp++;
	 break;
      }
   }
   *tnp = 0;     /* End string end. */

#if IPD_ipdDEBUG
   printf("iiCorrectName result: %s\n", name);
#endif

   return name;
} /* iiCorrectName */

/** @brief Appends the given name to a given fullname. */
char *iiComposeFullName(const char *fullname, const char *name)
{
   char *newFullName, *s;
   long  fullnameLen;

   if ((fullname))
   {
      fullnameLen = strlen(fullname);
      if (!(newFullName = (char *)ipdMBalloc(fullnameLen +
					     ((name) ? strlen(name) + 2 : 1))))
      {
	 ipdSetError(ipdERR_IPDNO_MEMORY);
	 return NULL;
      }
      s = newFullName;
      strcpy(s, fullname);
      if ((name))
      {
	 s += fullnameLen;
	 if ((*(s - 1) != '~') && (*(s - 1) != '.'))
	    *s++ = '.';
	 strcpy(s, name);
      }
      return newFullName;
   }
   return NULL;
} /* iiComposeFullName */

/** @brief Kills the last name of a valid (!!) path name.

If the pathname is equal to "~", nothing will be done.
Otherwise hanges on pathname will be done: If a delimiter ('.' or '~')
is found it'll be removed by a zero-byte.
Warning! Not using this function with duplicates of the original
path string will change the original! If this is wanted, finally
iiExtractNameFromFullname() must be used to restore a valid name.
The deleted character will be returned.
 */
char iiRemoveNameFromPath(char *pathname)
{
   if ((pathname))
   {
      char *pos, c;

      /* Remove variable name. Only the section it is located in is needed. */
      pos = pathname + strlen(pathname);
      while ((pos > pathname) && (*pos != '~') && (*pos != '.') &&
	     (*pos != '^'))
	 pos--;
      if ((*pos == '~') || (*pos == '^'))
	 pos++;
      c = *pos;
      *pos = 0;

      return c;
   }
   return '\0';
} /* iiRemoveNameFromPath */

/** @brief Extracts the name out of a given fullname.

For each variable or section its name and its fullname is stored. But
name is a part (the last part) of the fullname. So we need not allocate
new memory for the same string. iiExtractNameFromFullname() extracts
the name out of a given fullname.
fullname and name (the return value) can now be stored in an
ipdTreeNode_t structure.
 */
char *iiExtractNameFromFullname(char *fullname)
{
   if ((fullname))
   {
      char *s;

      /* Now extract the name of the fullname */
      s = fullname + strlen (fullname) - 1; /* To the last char */
      while ((s >= fullname) && (*s != '~') && (*s != '^') && (*s != '.'))
	 s--;
     
      /** Now we're at a '~', '^' or a '.', so extract the name */
      s++;
      /** Name name extracted */

      return s;
   }
   return NULL;
} /* iiExtractNameFromFullname */

/** @brief Prints conditional base names */
static void iiPrintConditionalBaseNames(ipdTreeNode_t *tn)
{
   ipdTreeNode_t *sanc = tn->node.sv.svu.sect.ancestor;

   if (!ipdPrivate.inheritanceInfo)
      return;

   /* Test if the section is inherited form itself. */
   /* Search in all ancestors. */
   while ((sanc))
   {
      if ((sanc->node.sectRef.condTree))
      {
	 ipdStdLog("IPD-Info: section \"%s\" %s derived from section \"%s\"\n",
		   tn->node.sv.fullname,
		   (sanc->node.sectRef.isActive) ? "IS" : "IS IPDNOT",
		   sanc->node.sectRef.name);
      }
      sanc = sanc->next;
   }
} /* iiPrintConditionalBaseNames */

/** @brief Steps into the section with the given pathName. This section will become the current section. 

If the section doesn't exist, it will be created. pathName may be a complete path. All not existing sections will be created.
But when a new section is created, the parameter flags is or-ed.
sectName is a valid path, including the sectionnames that might be created at the end.
 @return The return value of type boolean shows if a new section had to be created to generate the full path (pathName).
 */
static ipdBoolean iiCreateSubsections(ipdTreeNode_t  **found,
				    char            *pathName,
				    ipdSectFlags_t   flags,
				    ipdBoolean         forceCreation)
{
   ipdTreeNode_t  *sect = NULL;   /* the new section.                    */
   ipdTreeNode_t  *sectPath; /* a path (of all linked subsections). */
   long            len;
   char           *pos, c;

   if (!(pathName))
   {
      if ((found))
	 *found = NULL;
      return ipdFALSE;
   }
   else if (!(iiStrCmp(pathName, iiGetCS()->node.sv.fullname)))
   {
      if ((found))
	 *found = iiGetCS();
      return ipdFALSE;
   }

   len = strlen(pathName);
   sectPath = iiFindNearestItem(pathName, &pos);
   if (!(sectPath->type & ipdSECTION))
   {
      ipdSetError(ipdERR_IPDNO_SECTION, pathName);
      if ((found))
	 *found = NULL;
      return ipdFALSE;
   }
   if ((*pos))
   {
      if (!forceCreation &&
	  !ipdEditorPrivileges &&
	  (sectPath->node.sv.svu.sect.flags & ipdSF_CLOSED))
      {
	 if (*(pos - 1) == '.')
	    pos--;
	 c = *pos;
	 *pos = '\0'; /* Overwrite the point */
	 ipdSetError(ipdERR_REOPEN_SECTION, pathName);
	 *pos = c;    /* Write back.         */
	 if ((found))
	    *found = NULL;
	 return ipdFALSE;
      }
   }
   else
   {
      if ((found))
	 *found = sectPath;
      return ipdFALSE;
   }
   while ((*pos))
   {
      if (*pos == '.')
	 pos++;
      while ((*pos) && (*pos != '.'))
	 pos++;

      /* an item with this section name doesn't exist in the CurrentSection,
	 so we have to create a new subsection */
      if (!(sect = iiCreateTreeNode(ipdSECTION)))
	 goto iiCreateSubsectionsError;

      /* Copy fullname */
      c = *pos;
      *pos = '\0'; /* Overwrite the point */
      sect->node.sv.fullname = iiStrDup (pathName);
      *pos = c;    /* Write back.         */
      if (!sect->node.sv.fullname)
	 goto iiCreateSubsectionsError;

      /* Now extract the name of the fullname */
      sect->node.sv.name = iiExtractNameFromFullname(sect->node.sv.fullname);

      /* let the hash know about the new section */
      iiHashStoreTreeNode(sect);
      if (ipdGetErrorStatus())
	 goto iiCreateSubsectionsError;
      iiConnectTreeNode(sectPath, sect);

      sect->node.sv.svu.sect.flags = sectPath->node.sv.svu.sect.flags | ipdSF_CLOSED;
      sectPath = sect;
   }
   if (!(sect))
      sect = sectPath;
   sect->node.sv.svu.sect.flags |= flags;

   if ((found))
      *found = sect;

   return ipdTRUE; /* New section created.*/

 iiCreateSubsectionsError:
   iiFreeTreeNodeRec(sect, ipdFALSE);
   if ((found))
      *found = NULL;
   return ipdFALSE;
} /* iiCreateSubsections */

/** @brief Gets a ipdTreeNode_t structure (a complete tree) with tn->name set.

If not set iiSave() adds the fullname.
Then it tries to add the structure in the current section. But tn->name
is allowed to be a path using '~', '^' and subsectionnames. In this 
case the node tn will be stored in the right section, the name will
be reduced to the structure name itself, e. g.
  -  tn->fullname = "~SectionA.SectionB.x"
  -  tn->name     will be set to "x".
  -  tn->fullname will be set to "~SectionA.SectionB.x"
  -  ~SectionA    will be created if nonexistent
  -  ~SectionA.SectionB will be created as well.
Finally the structure named x will be stored.
 */
ipdTreeNode_t *iiSave(ipdTreeNode_t *tn,
		      ipdBoolean       hasValidFullname,
		      long           lineno,
		      ipdIFL_t      *file)
{
   char           c, *pos;
   char          *searchForName;
   ipdTreeNode_t *tmp, *sect;

   /* Test, if an error occured. */
   if ((!(tn->node.sv.fullname)) || (!tn->node.sv.fullname[0]) ||
       (ipdGetErrorStatus()))
   {
      iiFreeTreeNodeRec(tn, ipdFALSE);
      return NULL;
   }

   /* Is there a node named fullname? First get a valid fullname. */
   /* Did we get a valid fullname from iiRecallByName()? */
   if (hasValidFullname)
   {
      /* Yes */
      searchForName        = tn->node.sv.fullname;
      tn->node.sv.fullname = NULL;
   }
   else
   {
      searchForName = iiCreateName(tn->node.sv.fullname, NULL);
      ipd_RETURNonERRORp;
   }

   if ((tmp = iiHashFindTreeNode(searchForName)))
   {
      /* It exists. */

      if (!hasValidFullname)
			ipdMBfree(tn->node.sv.fullname);
		tn->node.sv.fullname = searchForName;
      if ((tmp->type & ipdVARIABLE) &&	(tn->type  & ipdVARIABLE))
      {
			 if (!(tmp->node.sv.svu.var.flags & ipdVF_EXTERN) &&
			     !ipdEditorPrivileges)
			 {
			    ipdSetError(ipdERR_OVERWRITING_EXPRESSION, tn->node.sv.fullname);
			    iiFreeTreeNodeRec(tn, ipdFALSE);
			    return NULL;
			 }
			 if (!ipdApplicationSet)
			 {
			    ipdSetError(ipdERR_APPLICATION_SET_VAR, tmp->node.sv.fullname);
			    iiFreeTreeNodeRec(tn, ipdFALSE);
			    return NULL;
			 }

			 /* not protected   */
			 /* overwrite the found item with the value of the new item tmp,
			  * modify flags:
			  */
			 if ((tn->node.sv.svu.var.tree))
			 {
			    /* Trace variable. */
			    iiTraceVar(tn, "overwriting");

			    /* Saves the tree of tn in tmp and returns tmp (the found one) */
			    iiFreeTreeNodeRec(tmp->node.sv.svu.var.tree, ipdFALSE);
			    tmp->node.sv.svu.var.tree = tn->node.sv.svu.var.tree;
			    tn->node.sv.svu.var.tree  = NULL;

			    /* Remember the file and the line where this statement is located. */
			    if ((tn->node.sv.file))
			    {
			       tmp->node.sv.lineno = tn->node.sv.lineno;
			       tmp->node.sv.file   = tn->node.sv.file;
			    }
			 }
			 iiFreeTreeNodeRec(tn, ipdFALSE);
			 return tmp;
      }
      else if ((tmp->type & ipdSECTION) &&
	       (tn->type & ipdSECTION))
      {
			 if (!(tmp->node.sv.svu.sect.child))
			 {
			    if ((tn->node.sv.svu.sect.ancestor))
			    {
			       ipdTreeNode_t *parent;
			       ipdTreeNode_t *ancestor;
			       ipdTreeNode_t *sanc = tn->node.sv.svu.sect.ancestor;
			       char          *ancestorFullName;

			       /* There are new ancestors defined. */
			       /* Test if the section is inherited form itself. */
			       /* Search in all ancestors. */
			       while ((sanc))
			       {
						  if (!(ancestorFullName = iiCreateName((char *)sanc->node.sectRef.name,
											tmp->node.sv.parent->node.sv.fullname)))
						  {
						     iiFreeTreeNodeRec(tn, ipdFALSE);
						     return NULL;
						  }
						  if ((ancestor = iiHashFindTreeNodeType(ancestorFullName, ipdSECTION)))
						  {
						     ipdMBfree(ancestorFullName);
						     parent = tmp;
						     while ((parent))
						     {
									if (parent == ancestor)
									{
									   ipdSetError(ipdERR_INHERIT_PARENT, tn->node.sv.fullname);
									   iiFreeTreeNodeRec(tn, ipdFALSE);
									   return NULL;
									}
									parent = parent->node.sv.parent;
						     }
						  }
						  sanc = sanc->next;
			       }
			       /* Take new ancestors. */
			       iiFreeTreeNodeRec(tmp->node.sv.svu.sect.ancestor, ipdFALSE);
			       tmp->node.sv.svu.sect.ancestor = tn->node.sv.svu.sect.ancestor;
			       tn->node.sv.svu.sect.ancestor  = NULL;
			    }
			    iiFreeTreeNodeRec(tn, ipdFALSE);

			    iiPrintConditionalBaseNames(tmp);

			    return tmp;
			 }
			 iiFreeTreeNodeRec(tn, ipdFALSE);

			 return tmp;
      }
      if ((tmp->type & ipdALIAS) && (tn->type  & ipdALIAS))
      {
			 if (!ipdEditorPrivileges)
			 {
			    ipdSetError(ipdERR_OVERWRITING_EXPRESSION, tn->node.sv.fullname);
			    iiFreeTreeNodeRec(tn, ipdFALSE);
			    return NULL;
			 }
			 if ((tn->node.sv.svu.alias.condTree))
			 {
			    /* Saves the tree of tn in tmp and returns tmp (the found one) */
			    iiFreeTreeNodeRec(tmp->node.sv.svu.alias.condTree, ipdFALSE);
			    tmp->node.sv.svu.alias.condTree = tn->node.sv.svu.alias.condTree;
			    tn->node.sv.svu.alias.condTree  = NULL;

			    /* Remember the file and the line where this statement is located. */
			    if ((tn->node.sv.file))
			    {
			       tmp->node.sv.lineno = tn->node.sv.lineno;
			       tmp->node.sv.file   = tn->node.sv.file;
			    }
			 }
			 iiFreeTreeNodeRec(tn, ipdFALSE);
			 return tmp;
      }
      else if (tn->type & ipdVARIABLE)
      {
			 ipdSetError(ipdERR_IPDNO_VARIABLE, tn->node.sv.fullname);
			 iiFreeTreeNodeRec(tn, ipdFALSE);
			 return NULL;
      }
      else if (tn->type & ipdSECTION)
      {
			 ipdSetError(ipdERR_IPDNO_SECTION, tn->node.sv.fullname);
			 iiFreeTreeNodeRec(tn, ipdFALSE);
			 return NULL;
      }
      else
			 return NULL;
   }

   tn->node.sv.name = NULL;
   ipdMBfree(tn->node.sv.fullname);

   /* Set the fullname ascertained. */
   tn->node.sv.fullname = searchForName;
   /* Now extract the name of the fullname */
   tn->node.sv.name = iiExtractNameFromFullname (tn->node.sv.fullname);

   /* Now create all necessary sections (and subsections) we want
      tn to be stored in. We need the path.
      Extract the path by temporaryly modifying tn->node.sv.fullname. */
   if (tn->node.sv.fullname == tn->node.sv.name)
      pos = tn->node.sv.name;
   else
      pos = tn->node.sv.name - 1;
   c = *pos;    /* Remember old char. */
   if (c != '.')
   { /* c == '~' */
      pos  = tn->node.sv.name;
      c    = *pos;    /* Remember the first char of tn->node.sv.name */
   }
   *pos = 0; /* Kill point in tn->node.sv.fullname */

   /* Now create all necessary subsections */
   if (!(iiCreateSubsections(&sect,
			     tn->node.sv.fullname,
			     ipdSF_NOFLAG,
			     ipdApplicationSet)))
   {
      if (!(sect))
      {
	 ipdSetError(ipdERR_REOPEN_SECTION, tn->node.sv.fullname);
	 iiFreeTreeNodeRec(tn, ipdFALSE);

	 return NULL;
      }
   }
#if 0
   if (ipdGetErrorStatus())
   {
      iiFreeTreeNodeRec(tn, ipdFALSE);
      return NULL;
   }
#endif

   /* Now restore original tn->node.sv.fullname */
   *pos = c; /* Restore the delete char  */

   /* Test, if there is an inherited section. If so, test if this sections
      protection-flag is set. This means then, all descendant sections
      may only consist of those items stored in the ancestor section. */

   /* Find the section which stores the basesection list. */
   tmp = NULL;
   if ((iiGetAncestorSection(sect)))
   {
      ipdTreeNode_t *sectext;

      sectext = sect;
      while ((sectext))
      {
	 if ((sectext->node.sv.svu.sect.flags & ipdSF_EXTENDABLE) ||
	     (sectext->node.sv.flags & ipdSV_FORCE_APPEND)) /* sections which have been created by force append ('+') are extendable too. */
	    break;
	 sectext = sectext->node.sv.parent;
      }
      if ((sectext)) /* Is appendable? */
      {
	 /* Yes. */
	 if (tn->type & ipdVARIABLE)
	 {
	    /* Find the variable itself. We need it later on. */
	    iiGetInheritedItemRec(&tmp,
				  tn->node.sv.fullname,
				  NULL,
				  ipdFALSE,
				  NULL);
	 }
      }
      else
      {
	 /* No.
	    The ancestor section is protected. So test, if the item
	    we want to store does exist inherently within this section.
	    If not --> Error! */
	 /* Find the variable itself. */
	 iiGetInheritedItemRec(&tmp,
			       tn->node.sv.fullname,
			       NULL,
			       ipdFALSE,
			       NULL);
	 if ((tmp))
	 {
	    if (tmp->type != tn->type)
	    {
	       ipdSetError(ipdERR_DIFFERENT_ITEM_TYPE,
			   tn->node.sv.fullname,
			   ipdGetTypeStr(tmp->type, ipdFALSE));
	       iiFreeTreeNodeRec(tn, ipdFALSE);
	       return NULL;
	    }
	    if (tn->node.sv.flags & ipdSV_FORCE_APPEND)
	       ipdSetError(ipdWARN_FORCE_APPEND_EXISTS, tn->node.sv.fullname);
	 }
	 else
	 {
	    if (ipdEditorPrivileges)
	    { /* Allow editors to append items */
	    }
	    /* Warn, if an application tries to set a new item which is not declared extern */
	    else if (ipdApplicationSet)
	    {
	       ipdSetError(ipdWARN_APPENDED_BY_APPLICATION,
			   tn->node.sv.name,
			   sect->node.sv.fullname);
	    }
	    /* Else: Error? */
	    else if (!(tn->node.sv.flags & ipdSV_FORCE_APPEND) && /* ipdSV_FORCE_APPEND allows appending explicitely */
		     (((tn->type == ipdVARIABLE) && /* only auxiliary variables may be appended */
		       !(tn->node.sv.svu.var.flags & ipdVF_AUXILIARY)) ||
		      (tn->type == ipdSECTION))) /* sections may not be appended without ipdApplicationSet set */
	    {    
	       ipdSetError(ipdERR_APPEND_PROTECTED_SECTION,
			   tn->node.sv.fullname,
			   sect->node.sv.fullname);
            
         /* [JW] skipping the dump of the ipd, because
            the dump can not handle this problem 
            proably. the terminal is being flooded with whitespaces.
            problem: inheriting a section from another, 
            appending a new variable without the use of the prefix plus.
            
            this approach still dumps thelog file with an indication what
            error occured and where. except the ipd DB is not dumped. 
            good tradeoff imho*/            
	       iiFreeTreeNodeRec(tn, ipdFALSE);            
          return NULL;  
         
         
          /*            
	       // Dump the whole section with all its inherited items      
	       // Temporaryly reset error                                  
	       ipdResetErrorStatus();
	       // Dump the whole section including all inherited items     
	       // Dump the section head and the names of inherited secions 
	       ipdPushCS();
	       ipdLogPrintf(ipdPrivate.logFile, sect->node.sv.name);
	       if ((sect->node.sv.svu.sect.ancestor))
	       {
		        ipdTreeNode_t *sanc = sect->node.sv.svu.sect.ancestor;
		        ipdBoolean       notFirstParam = ipdFALSE;

		        ipdLogPrintf(ipdPrivate.logFile, " : ");
		        while ((sanc))
		        {
		           if (notFirstParam)
			      ipdLogPrintf(ipdPrivate.logFile, ", ");
		           else
			      notFirstParam = ipdTRUE;
		           ipdLogPrintf(ipdPrivate.logFile, "%s", (char *)(sanc->node.sectRef.name));
		           sanc = sanc->next;
		        }
	       }
	       ipdLogPrintf(ipdPrivate.logFile, "\n{\n");

	       // Dump the content of the section sect                     
	       if (!ipdIteratorNewByTN(&iNode, sect, ipdSECTION, ipdANY))
          {
		      return NULL;
	       }
	       if (ipdIteratorDoStep(iNode))
          {
		       do
		       {
		         ipdLogPrintf(ipdPrivate.logFile, "   ");
		         ipdDumpRec(ipdPrivate.logFile,
			         iNode->tn,
			         (iNode->tn->type & ipdVARIABLE),
				      ipdFALSE,
				      (ipdPrintf_f)ipdLogPrintf);
		       } while (ipdIteratorDoNext(iNode));
	       }
	       ipdPopCS();
	       ipdLogPrintf(ipdPrivate.logFile, "}\n\n");
	       // Set error number again to previous error if none occured meanwhile 
	       iiSetErrorStatus();
	       ipdIteratorFree(iNode);
	       iiFreeTreeNodeRec(tn, ipdFALSE);
	       return NULL;
	       */
	    }
	 }
      }
   }
   else
   {
      if (ipdEditorPrivileges)
      { /* Allow editors to append items */
      }
      /* Warn, if an application tries to set a new item which is not declared before (thus not declared extern) */
      else if (ipdApplicationSet)
	 ipdSetError(ipdWARN_APPENDED_BY_APPLICATION,
		     tn->node.sv.name,
		     sect->node.sv.fullname);
      else if (tn->node.sv.flags & ipdSV_FORCE_APPEND)
	 ipdSetError(ipdWARN_FORCE_APPEND_IPDNOT_INHERITED, tn->node.sv.fullname);
      tmp = NULL;
   }

   /* Remember the file and the line where this statement is located. */
   tn->node.sv.lineno = lineno;
   tn->node.sv.file   = file;

   /* tmp stores the inherited variable, if there is one. */
   if (tn->type & ipdVARIABLE)
   {
      /* First, correct the flags. */
      ipdApplicationSet = ipdFALSE;
      if (!(tn->node.sv.svu.var.flags & ipdVF_AUXILIARY))
      {
			 if (sect->node.sv.svu.sect.flags & ipdSF_EXTERN)
			    tn->node.sv.svu.var.flags |= ipdVF_EXTERN;
      }
      else if (!(tn->node.sv.svu.var.flags & ipdVF_EXTERN))
      {
			 if (sect->node.sv.svu.sect.flags & ipdSF_AUXILIARY)
			    tn->node.sv.svu.var.flags |= ipdVF_AUXILIARY;
      }
      /* Check, if both variables are of the same variable type. */
      if ((tmp))
      {
			 if ((tmp->node.sv.svu.var.flags & (ipdVF_AUXILIARY | ipdVF_EXTERN)) !=
			     (tn->node.sv.svu.var.flags  & (ipdVF_AUXILIARY | ipdVF_EXTERN)))
			 {
		           if (ipdEditorPrivileges) /* [OB] Editor is allowed to overwrite non-externs too */
		           {  /*Keep original flag, if ipdEditorPrivileges are deactivated again*/
		             tn->node.sv.svu.var.flags = tmp->node.sv.svu.var.flags;   
		           }
		           else
		           {
		             ipdSetError(ipdERR_VAR_TYPE, tn->node.sv.fullname);
			     iiFreeTreeNodeRec(tn, ipdFALSE);
			     return NULL;
		           }
			 }
      }
      tn->node.sv.svu.var.lock = 0;
   }

   /* Link node to tree, insert in hash */
   iiHashStoreTreeNode(tn);
   ipd_RETURNonERRORp;
   iiConnectTreeNode(sect, tn);
   if (ipdGetErrorStatus())
   {
      iiFreeTreeNodeRec(tn, ipdFALSE);
      return NULL;
   }

   if (tn->type & ipdSECTION)
   {
      ipdTreeNode_t *parent;
      ipdTreeNode_t *ancestor;
      ipdTreeNode_t *sanc = tn->node.sv.svu.sect.ancestor;
      char          *ancestorFullName;

      /* Test if the section is inherited form itself. */
      /* Search in all ancestors. */
      while ((sanc))
      {
	 if (!(ancestorFullName = iiCreateName((char *)sanc->node.sectRef.name,
					       sect->node.sv.fullname)))
	 {
	    iiFreeTreeNodeRec(tn, ipdFALSE);
	    return NULL;
	 }
	 if ((ancestor = iiHashFindTreeNodeType(ancestorFullName, ipdSECTION)))
	 {
	    ancestor->node.sv.svu.sect.flags |= ipdSF_IS_BASESECTION;
	    ipdMBfree(ancestorFullName);
	    parent = tn;
	    while ((parent))
	    {
	       if (parent == ancestor)
	       {
				  ipdSetError(ipdERR_INHERIT_PARENT, tn->node.sv.fullname);
				  iiFreeTreeNodeRec(tn, ipdFALSE);
				  return NULL;
	       }
	       parent = parent->node.sv.parent;
	    }
	 }
	 else
	    ipdMBfree(ancestorFullName);
	 sanc = sanc->next;
      }
      iiPrintConditionalBaseNames(tn);
   }

   return tn;
} /* iiSave */

/** @brief Deletes an variable. A variable can be deleted only, if no other variables refer to it.

Deleting a variable is useful, when using inherited variables.
Variables, which are inherited from an other section, can be locally
overwritten in the inherited section. The inherited variable represents
the default value, when no locally modification has been done. To get
rid of this modification and to get back this default value, we may
simply delete this variable.
 */
void iiDeleteVariable(const char *name)
{
   ipdTreeNode_t *var;
   char          *searchForName;

   searchForName = iiCreateName(name, NULL);
   ipd_RETURNonERRORv;
   if ((var = iiHashFindTreeNode(searchForName)))
   {
      if (var->type & ipdVARIABLE)
      {
	 /* Trace variable. */
	 iiTraceVar(var, "delete");

	 /* variable locked? */
	 if (var->node.sv.svu.var.lock)
	 {
	    ipdSetError(ipdERR_VAR_LOCKED, name);
	    return;
	 }

	 /* Delete it. */
	 iiFreeTreeNodeRec(var, ipdFALSE);
      }
      else if (var->type & ipdSECTION)
	 ipdSetError(ipdERR_IPDNO_VARIABLE, name);
      else
	 ipdSetError(ipdERR_VAR_IPDNOT_FOUND, name);
   }
   else
      ipdSetError(ipdERR_VAR_IPDNOT_FOUND, name);

   ipdMBfree(searchForName);
} /* iiDeleteVariable */

/** @brief Generates an inherited section.

iiGenerateInheritedSection() searches for an already defined section
named sectName. If it finds one, this section will be added to sect's ancestor list.
 */
ipdTreeNode_t *iiGenerateInheritedSection(ipdTreeNode_t *sect, char *sectName)
{
   ipdTreeNode_t *tn, *newTn;

   if (!(sect) || !(sectName) || !(newTn = iiCreateTreeNode(ipdSECTION_REF)))
   {
      iiFreeTreeNodeRec(sect, ipdFALSE);
      ipdMBfree(sectName);
      return NULL;
   }

   newTn->node.sectRef.name   = sectName;
   newTn->node.sectRef.isActive = ipdTRUE;
   tn = sect->node.sv.svu.sect.ancestor;
   if (tn)
   {
      while (tn->next)
	 tn = tn->next;
      tn->next = newTn;
   }
   else
      sect->node.sv.svu.sect.ancestor = newTn;

   return sect;
} /* iiGenerateInheritedSection */

/** @brief Preserves history to enable iiReturnsub(). */ 
void iiSetCSHistory(ipdTreeNode_t *sect)
{
   if ((sect))
   {
      if ((sect->node.sv.svu.sect.history))
	 ipdSetError(ipdWARN_LOOSE_LAST_CUR_SECT);
      sect->node.sv.svu.sect.history = iiGetCS();  /* Save last current section for return */

      iiSetCS(sect); /* Set the current section */
   }
} /* iiSetCSHistory */

/** @brief Performs a 'cd ..' at the current section.

Current section becomes the parent section.
Returns the previous current section.
 */
ipdTreeNode_t *iiReturnsub(void)
{
   ipdTreeNode_t *CS;

   if ((CS = iiGetCS()) && (CS->node.sv.svu.sect.history))
   {
      iiSetCS(CS->node.sv.svu.sect.history);
      CS->node.sv.svu.sect.history = NULL;
   }
   return CS;
} /* iiReturnsub */

/** @brief Returns true if the given section has an active(!) base section.  */
static ipdBoolean iiHasBaseSection(ipdTreeNode_t *tn)
{
   ipdTreeNode_t *sanc;

   if ((tn->type & ipdSECTION) && (sanc = tn->node.sv.svu.sect.ancestor))
   {
      /* Search in all ancestors. */
      while ((sanc))
      {
	 if (sanc->node.sectRef.isActive)
	    return ipdTRUE;
	 sanc = sanc->next;
      }
   }
   return ipdFALSE;
} /* iiHasBaseSection */

/** @brief Finds an inherited variable and returns it in *result_p if result_p != NULL.

All ancestor sections will be scanned. First call: item = NULL  !!
If tnDll_p points to a DLL not only the first item but all items which
can be found will be returned.
The boolean return value is used for the recursive calls. ipdTRUE shows,
that no further recursions needs to be done.
 */
ipdBoolean iiGetInheritedItemRec(ipdTreeNode_t    **result_p,
			       char              *iName,
			       ipdTreeNodeDll_t **tnDll_p,
			       ipdBoolean           getLocalsToo,
			       ipdTreeNode_t     *oldTn)
{
   ipdTreeNode_t *item, *pushItem = NULL;
   char          *pos;
   ipdTreeNode_t *sanc;
   long           len     = 0;
   char          *newName = NULL;
   ipdBoolean       foundAPart = ipdFALSE;

   if (!(iName)||
       !(*iName))
   {
      if ((result_p))
	 *result_p = NULL;
      return ipdTRUE;
   }

   /* Now find the nearest section of iName and store it in item.
      Adjust iName. */
   /* pos is set in iiFindNearestItem() to the rest of the name
      string pointing to the relative name of the item not found. */
   if (!(item = iiFindNearestItem(iName, &pos)))
   {
      if ((result_p))
	 *result_p = NULL;
      return ipdTRUE;
   }

   if (!(*pos) && (getLocalsToo))
   {
      if ((tnDll_p))
	 pushItem = item;
      else
      {
	 if ((result_p) && !(*result_p))
	    *result_p = item;
	 return ipdTRUE;
      }
   }
   if (item != oldTn)
   {
      if (!getLocalsToo && (item->type & (ipdVARIABLE)))
      {	 /* We don't want to find a local variable. */
	 item = item->node.sv.parent;
	 /* Get the name of the previous section. */
	 do
	 {
	    pos--;
	 } while ((pos > iName) && (*(pos - 1) != '.'));
      }
      foundAPart = ipdTRUE;
   }

   while ((item))
   {
      /* Do not test for an ipdVARIABLE, because we only search for
	 INHERITED items !!! So search for the name in the ancestors. */
      if (item->type & ipdSECTION)
      {
	 if ((sanc = item->node.sv.svu.sect.ancestor))
	 {
	    /* Search in all ancestors. */
	    while ((sanc))
	    {
	       if (!sanc->node.sectRef.isActive)
	       {
		  sanc = sanc->next;
		  continue;
	       }
	       iiStrExtend(&newName,
			   &len,
			   item->node.sv.parent->node.sv.fullname,
			   ipdFALSE);
	       iiStrExtend(&newName,
			   &len,
			   sanc->node.sectRef.name,
			   ipdTRUE);
	       while (1)
	       {
		  if (ipdGetErrorStatus())
		  {
		     if ((result_p))
			*result_p = NULL;
		     ipdMBfree(newName);
		     return ipdTRUE;
		  }
		  iiCorrectName(newName); /* Name of the new base section. */
		  oldTn = NULL;
		  /* Get the new base section and store it in oldTn. */
		  iiGetInheritedItemRec(&oldTn,
					newName,
					NULL,
					ipdTRUE,
					NULL);
		  if (!(oldTn))
		  {
		     ipdSetError(ipdERR_ANCESTOR_IPDNOT_FOUND,
				 newName,
				 item->node.sv.fullname);
		     ipdMBfree(newName);
		     return ipdTRUE;
		  }
		  if (oldTn->type == ipdSECTION)
		     break;
		  else if (oldTn->type & ipdALIAS)
		  {
		     iiStrExtend(&newName,
				 &len,
				 item->node.sv.parent->node.sv.fullname,
				 ipdFALSE);
		     if (!(oldTn->node.sv.svu.alias.sectRef))
		     {
			ipdMBfree(newName);
			ipdSetError(ipdERR_UNKIPDNOWN);
			return ipdTRUE;

		     }
		     iiStrExtend(&newName,
				 &len,
				 oldTn->node.sv.svu.alias.sectRef->node.sectRef.name,
				 ipdTRUE);
		  }
		  else
		  {
		     ipdSetError(ipdERR_ANCESTOR_IPDNO_SECTION,
				 newName,
				 item->node.sv.fullname);
		     ipdMBfree(newName);
		     return ipdTRUE;
		  }
	       }
	       iiStrExtend(&newName,
			   &len,
			   oldTn->node.sv.fullname,
			   ipdFALSE);
	       iiStrExtend(&newName,
			   &len,
			   pos,
			   ipdTRUE);
	       if (ipdGetErrorStatus())
	       {
		  if ((result_p))
		     *result_p = NULL;
		  ipdMBfree(newName);
		  return ipdTRUE;
	       }
	       iiCorrectName(newName); /* Name of the item in the base section. */
	       if (iiGetInheritedItemRec(result_p,
					 newName,
					 tnDll_p,
					 ipdTRUE,
					 oldTn))
	       {
		  if ((pushItem))
		  {
		     iiPushTNFifo(tnDll_p, pushItem);
		     /* The section pushItem set above the outer loop
			is a locally modified section. */
		     pushItem->node.sv.flags |= ipdSV_IS_TEMP_LOKAL;
		  }
		  /* Don't store any item found in the DLL now!
		     This has been already done. */
		  ipdMBfree(newName);
		  return ipdTRUE;
	       }
	       if (ipdGetErrorStatus())
	       {
		  if ((result_p))
		     *result_p = NULL;
		  ipdMBfree(newName);
		  return ipdTRUE;
	       }
	       sanc = sanc->next;
	    }
	    /* We've found an ancestor. But we havn't found an item in it.
	       So stop scanning: Return ipdTRUE !! We store do not return the
	       item. It needs not be stored in the calling routine. */
	    if ((result_p))
	       *result_p = NULL;
	    ipdMBfree(newName);
	    if ((tnDll_p))
	    {
	       if ((!(*pos) && (getLocalsToo)) ||
		   (pushItem))
	       {
		  iiPushTNFifo(tnDll_p, pushItem);
		  /* The section pushItem set above the outer loop
		     is a locally modified section. */
		  pushItem->node.sv.flags |= ipdSV_IS_TEMP_LOKAL;
		  if (pushItem != oldTn)
		     foundAPart = ipdTRUE;
	       }
	       return foundAPart;
	    }
	    return ipdTRUE;
	 }
	 else
	 {
	    if ((item = item->node.sv.parent))
	    {
	       /* Get the name of the previous section. */
	       if (pos == iName)
		  break;
	       do
	       {
		  pos--;
	       } while ((pos > iName) && (*(pos - 1) != '.'));
	    }
	 }
      }
      else if (item->type & ipdALIAS)
      {
	 iiStrExtend(&newName,
		     &len,
		     item->node.sv.parent->node.sv.fullname,
		     ipdFALSE);
	 iiStrExtend(&newName,
		     &len,
		     item->node.sv.svu.alias.sectRef->node.sectRef.name,
		     ipdTRUE);
	 iiStrExtend(&newName,
		     &len,
		     pos,
		     ipdTRUE);
	 if (ipdGetErrorStatus())
	 {
	    if ((result_p))
	       *result_p = NULL;
	    ipdMBfree(newName);
	    return ipdTRUE;
	 }
	 iiCorrectName(newName); /* Name of the item in the base section. */
	 if (iiGetInheritedItemRec(result_p,
				   newName,
				   tnDll_p,
				   ipdTRUE,
				   NULL))
	 {
	    if ((pushItem))
	    {
	       iiPushTNFifo(tnDll_p, pushItem);
	       /* The section pushItem set above the outer loop
		  is a locally modified section. */
	       pushItem->node.sv.flags |= ipdSV_IS_TEMP_LOKAL;
	    }
	    /* Don't store any item found in the DLL now!
	       This has been already done. */
	    ipdMBfree(newName);
	    return ipdTRUE;
	 }
      }
      else
      {
	 /* No local variable found */
	 if ((result_p))
	    *result_p = NULL;
	 return ipdTRUE;
      }
   }

   if ((pushItem))
   {
      iiPushTNFifo(tnDll_p, pushItem);
      /* The section pushItem set above the outer loop
	 is IPDNOT a locally modified section. */
      pushItem->node.sv.flags &= ~ipdSV_IS_TEMP_LOKAL;
      if (pushItem != oldTn)
	 foundAPart = ipdTRUE;
   }
   if ((result_p))
      *result_p = NULL;
   return foundAPart;
} /* iiGetInheritedItemRec */

/** @brief Finds a variable by a fullname. The variable might be inherited.

The variable to search for can be specified in two ways.
 -# A given ipdTreeNode_t structure tn, where its name is taken as the
    searchname.
 -# The name of the variable itself. The name can be given in two parts
    searchForName1 and searchForName2.
 This funcion will return the variable or NULL if not found.
 */
static ipdTreeNode_t *iiGetVariable(char          *searchForName,
				    ipdBoolean       getLocalsToo)
{
   ipdTreeNode_t *found;

   if (!(searchForName))
      return NULL;
   if (getLocalsToo &&
       (found = iiHashFindTreeNode(searchForName)))
   {
      if (found->type & ipdVARIABLE)
	 return found;
      ipdSetError(ipdERR_IPDNO_VARIABLE, searchForName);
      return NULL;
   }

   /* Test if there are ancestors, if tn is inherited!! */
   found = NULL;
   iiGetInheritedItemRec(&found,
			 searchForName,
			 NULL,
			 ipdFALSE,
			 NULL);
   ipd_RETURNonERRORp;
   if ((found))
   {
      if (found->type & ipdVARIABLE)
	 return found;
      ipdSetError(ipdERR_IPDNO_VARIABLE, searchForName);
      return NULL;
   }
   return NULL;
} /* iiGetVariable */

/** @brief Finds the given parent section if it exists. */
ipdTreeNode_t *iiFindParentSection(ipdTreeNode_t *tn,
				   ipdTreeNode_t *tryParent)
{
   if ((tn))
   {
      tn = tn->node.sv.parent;
      while ((tn))
      {
	 if (tn == tryParent)
	    return tn;
	 tn = tn->node.sv.parent;
      }
   }
   return NULL;
} /* iiFindParentSection */

/** @brief Finds the nearest item

 fullname1 must be a valid fullname. fullname2 may be NULL. Otherwise
 * it must be a valid relative name without '~' and '^'.
 * The ipdTreeNode_t returned will be the item itself or - if it does not
 * exist - the first parent section found.
 */
ipdTreeNode_t *iiFindNearestItem(char  *fullname,
				 char **pos_p)
{
   ipdTreeNode_t *tn;
   char          *pos, c;

   /* Set nearest-existing-parent-section-pointer. */
   if (!(fullname))
      return NULL;
   pos = fullname + strlen(fullname);

   while (1)
   {
      if (pos == fullname)
      {
	 tn = iiGetRoot();
	 break;
      }
      c    = *pos;
      *pos = '\0';
      tn   = iiHashFindTreeNode(fullname);
      *pos = c;
      if ((tn))
      {
/*
	 if ((tn->type & ipdVARIABLE))
	    tn = tn->node.sv.parent;
*/
	 break;
      }
      while ((--pos > fullname) && (*pos != '.'))
	 /* Nothing */;
   }
   if ((pos_p))
   {
      if ((*pos == '.') || (*pos == '~'))
	 pos++;
      *pos_p = pos;
   }
   return tn;
} /* iiFindNearestItem */

/** @brief Finds the parent section which has ancestor sections. */
ipdTreeNode_t *iiGetAncestorSection(ipdTreeNode_t *tn)
{
   while (tn)
   {
      if (iiHasBaseSection(tn))
	 return tn;
      tn = tn->node.sv.parent;
   }
   return NULL;
} /* iiGetAncestorSection */

/** @brief Finds those variable, which might be used when evaluating an expression tree of an inherited variable.
 *
 * Example:
 * A
 * {
 *   a = 1;
 *   b = a;
 * }
 * B : A
 * {
 *   a = 2;
 * }
 * A.a is equal to 1;
 * A.b is equal to 1;
 * B.a is equal to 2;
 * Important:
 * B.b must be equal to 2!!!! but IPDNOT equal to 1!!!!
 * B.b is an inherited variable. Inherited from section A, where b is set
 * equal to a; But when a is locally modified in an inherited Section (B)
 * This locally modification will cause B.b to change itself!
 * So when evaluating an expression we must take care, if the variable
 * to be evaluated is located in an inherited section or not.
 * baseSect is the section, which has anscestors and where the
 * variable to be evaluated is located in.
 */
ipdTreeNode_t *iiGetRealVariable(ipdTreeNode_t     *tn,
				 ipdDynamicName_t **baseSect_p)
{
   ipdTreeNode_t    *found;
   char             *fullname = NULL, *name = NULL;
   ipdDynamicName_t *dnn;

   if (!(tn) || !(baseSect_p))
      return NULL;

   /* Do initializations. Store current basesection, where calculations
      have to be done from now on in a seperate structure.
      This structure will be stored in baseSect_p. */
   /* Storage of basesection initialized? */
   if (!(*baseSect_p) && (tn->type & ipdVARIABLE))
   {
      dnn = iiCreateDNNode();
      ipd_RETURNonERRORp;
      if (!(dnn->name = iiStrDup(tn->node.sv.parent->node.sv.fullname)))
      {
	 iiFreeDNNode(dnn);
	 return NULL;
      }
      dnn->len  = iiStrLen(dnn->name) + 1;
      found = tn;
   }
   else
   {
      /* Get fullname and name of ipdVARIABLE and ipdVARIABLE_REF respectivly. */
      if (tn->type & ipdVARIABLE)
      {
	 fullname = tn->node.sv.fullname;
	 name     = tn->node.sv.name;
      }
      else
	 name     = tn->node.expr.func.varref.name;

      if (name == fullname)
      {
	 /* There is no absolute or relative path. Only the name. This
	    means, the new variable must be in the same section. So
	    leave it unchanged. */
	 dnn = *baseSect_p;
	 iiStrExtend(&(dnn->name),
		     &(dnn->len),
		     name,
		     ipdTRUE);
      }
      else
      {
	 /* Get empty node. */
	 dnn = iiCreateDNNode();
	 ipd_RETURNonERRORp;

	 /* Store fullname of the parent section. */
	 /* First copy the current section fullname. */
	 if ((*baseSect_p))
	 {
	    iiStrExtend(&(dnn->name),
			&(dnn->len),
			(*baseSect_p)->name,
			ipdFALSE);
	 }
	 else
	 {
	    ipdTreeNode_t *CS;
	    char          *CSFullName;

	    if ((ipdBase->CSFullName))
	       CSFullName = ipdBase->CSFullName;
	    else if ((CS = iiGetCS()))
	    {
	       /* get CurrentSection */
	       CSFullName = CS->node.sv.fullname;
	    }
	    else
	       CSFullName = (char *)"~";

	    iiStrExtend(&(dnn->name),
			&(dnn->len),
			CSFullName,
			ipdFALSE);
	 }
	 /* Append the relative name of the inherited variable. */
	 iiStrExtend(&(dnn->name),
		     &(dnn->len),
		     name,
		     ipdTRUE);
	 if (ipdGetErrorStatus())
	 {
	    iiFreeDNNode(dnn);
	    return NULL;
	 }

	 /* Correct the name.*/
	 iiCorrectName(dnn->name);
      }
      /* Get the item (inherited or not). */
      found = iiGetVariable(dnn->name,
			    ipdTRUE);

      iiRemoveNameFromPath(dnn->name);

      if (!(found))
	 ipdSetError(ipdERR_VAR_IPDNOT_FOUND, fullname);
   }
   *baseSect_p = dnn;

   return found;
} /* iiGetRealVariable */

/** @brief Recalls a variable - finds the existing one. Makes a link to existing variable */
ipdTreeNode_t *iiRecallByName(char *relativename)
{
   ipdTreeNode_t *item = NULL, *reference = NULL;
   ipdTreeNode_t *param;

   ipd_RETURNonERRORp;
   if (!(relativename))
      return NULL;

   /* If it's a parameter, get it. */
   if ((param = ipdParameterList))
   {
      /* ipdParameterList is a list of function parameters
	 which is hopefully ;-) beeing parsed now. */
      while ((param))
      {
	 if (!(iiStrCmp(param->node.parameter.name, relativename)))
	    break;
	 param = param->next;
      }
      if (!(param))
      {
	 ipdSetError(ipdERR_PARA_IPDNOT_FOUND, relativename);
	 ipdMBfree(relativename);
	 return NULL;
      }

      ipdMBfree(relativename);
      if (!(reference = iiCreateTreeNode(ipdPARAMETER_REF)))
	 return NULL;

      reference->node.expr.func.pararef.tree  = param;
      reference->node.expr.func.pararef.name  = param->node.parameter.name;

      return reference;
   }
   else
   {
      char          *fullname;

      /* Create relative name. */
      if (!(fullname = iiCreateName(relativename, NULL)))
      {
	 ipdMBfree(relativename);
	 return NULL;
      }

      item = NULL;
      iiGetInheritedItemRec(&item,
			    fullname,
			    NULL,
			    ipdTRUE,
			    NULL);
      if (ipdGetErrorStatus())
      {
	 ipdMBfree(relativename);
	 return NULL;
      }

      ipdMBfree(fullname);
#if 0
      if (!(item))
      {
	 ipdSetError(ipdERR_VAR_IPDNOT_FOUND, relativename);
	 ipdMBfree(relativename);
	 return NULL;
      }
#endif

      /* Create an ipdVARIABLE_REF or ipdSECTION_REF ipdTreeNode_t */
      if (!item || item->type & ipdVARIABLE)
      {
	 if ((reference = iiCreateTreeNode(ipdVARIABLE_REF)))
	    reference->node.expr.func.varref.name = relativename;
      }
      else if (item->type & ipdSECTION)/* ipdSECTION_REF */
      {
	 if (ipdAliasParsing)
	 {
	    if ((reference = iiCreateTreeNode(ipdSECTION_REF)))
	       reference->node.sectRef.name = relativename;
	 }
	 else
	 {
	    ipdSetError(ipdERR_SECTION_IN_EXPRESSION, relativename);
	 }
      }
      else
      {
	 ipdSetError(ipdERR_UNKIPDNOWN);
      }

      if ((!reference))
      {
	 ipdMBfree(relativename);
	 iiFreeTreeNodeRec(item, ipdFALSE);
	 return NULL;
      }

      return reference;
   }
} /* iiRecallByName */

/** @brief Evaluates an expression. Suited for use in ipdparse.y. */
ipdTreeNode_t *iiEval(ipdTreeNode_t *tn)
{
   if ((tn))
   {
      ipdTreeNode_t    *ret, *res;
      ipdDynamicName_t *baseSect;

      /* Create a structure, necessary for calculations in iiCalculate().
	 Store the name of the current section in this structure. */
      /* Get empty node. */
      baseSect = iiCreateDNNode();
      ipd_RETURNonERRORp;

      /* Store fullname of the parent section.    */
      /* First copy the current section fullname. */
      iiStrExtend(&(baseSect->name), &(baseSect->len),
		  iiGetCS()->node.sv.fullname, ipdFALSE);
      if (ipdGetErrorStatus())
      {
	 iiFreeDNNode(baseSect);
	 iiFreeTreeNodeRec(tn, ipdFALSE);
	 return NULL;
      }
      ipdBase->calcVar = NULL;
      ipdBase->topBaseSect = ipdBase->currBaseSect = baseSect;
      ret = iiCalculate(tn, baseSect);
      ipdBase->topBaseSect = ipdBase->currBaseSect = NULL;
      ipdBase->calcVar = NULL;
      if ((ret))
      {
	 if ((res = iiCreateTreeNode(ipdINVALID)))
	 {
	    if (IPD_EVAL_DELETES_EXPR)
	       /* move */
	       iiCopyValue(res, ret, ipdFALSE);
	    else
	    {
	       /* copy */
	       iiAssignValue(res, ret);
	    }
	    iiFreeDNNode(baseSect);
	    if (ipdGetErrorStatus())
	    {
	       ipdSetError(ipdERR_EVAL_EXPRESSION);
	       iiFreeTreeNodeRec(res, ipdFALSE);
	       iiFreeTreeNodeRec(tn, ipdFALSE);
	       return NULL;
	    }
	 }
	 else
	 {
	    iiFreeDNNode(baseSect);
	    ipdSetError(ipdERR_EVAL_EXPRESSION);
	    iiFreeTreeNodeRec(tn, ipdFALSE);
	    return NULL;
	 }
      }
      else
      {
	 iiFreeDNNode(baseSect);
	 ipdSetError(ipdERR_EVAL_EXPRESSION);
	 iiFreeTreeNodeRec(tn, ipdFALSE);
	 return NULL;
      }
#if IPD_EVAL_DELETES_EXPR
      iiFreeTreeNodeRec(tn, ipdFALSE);
#else
      res->type               |= ipdEVAL;
      res->node.expr.func.evalTree  = tn;
#endif
      return res;
   }
   ipdSetError(ipdERR_EVAL_EXPRESSION);
   return NULL;
} /* iiEval */

/** @brief Converts a parsed list to an array.

The parser builds an array by creating linked lists.
But evaluating such lists is very slow (to find an array element with a given index). So
after parsing the array and building the linked lists, we convert this lists to an array.
 */
ipdTreeNode_t *iiCreateArray(ipdTreeNode_t *tn)
{
   ipdTreeNode_t *arr = NULL;

   if ((tn))
   {
      if ((arr = iiCreateTreeNode(ipdARRAY)))
	 arr->node.expr.func.array = tn;
      else
	 iiFreeTreeNodeRec(tn, ipdFALSE);

      /* Store the array */
   }

   return arr;
} /* iiCreateArray */

/** @brief Gets the element of the array with the index given.
 *
 * Let's say arr is an array, i is the index, so arr[i] will be returned.
 * But arr[i] then is not a pointer to this structure: An ipdTreeNode_t
 * structure of the type ipdINDEX will be created. The element will
 * be picked out at run time.
 */
ipdTreeNode_t *iiCreateArrayIndex(ipdTreeNode_t *tn, ipdTreeNode_t *index)
{
   ipdTreeNode_t *ax;

   ipd_RETURNonERRORp;
   if ((tn) && (index))
   {
      if (tn->type & ipdINDEX)
      {
	 ax = tn;
	 while ((ax->node.expr.func.arrayIndex.next))
	    ax = ax->node.expr.func.arrayIndex.next;
	 if ((ax->node.expr.func.arrayIndex.index))
	 {
	    if (!(ax = ax->node.expr.func.arrayIndex.next = iiCreateTreeNode(ipdINDEX)))
	    {
	       iiFreeTreeNodeRec(tn, ipdFALSE);
	       iiFreeTreeNodeRec(index, ipdFALSE);
	       tn = NULL;
	    }
	 }
	 ax->node.expr.func.arrayIndex.index = index;
	 return tn;
      }
      else
      {
	 /* Now create an array index node. */
	 if (!(ax = iiCreateTreeNode(ipdINDEX)))
	 {
	    iiFreeTreeNodeRec(tn, ipdFALSE);
	    iiFreeTreeNodeRec(index, ipdFALSE);
	    return NULL;
	 }
	 ax->node.expr.func.arrayIndex.tree = tn;
	 ax->node.expr.func.arrayIndex.index    = index;
	 return ax;
      }
   }
   iiFreeTreeNodeRec(tn, ipdFALSE);
   iiFreeTreeNodeRec(index, ipdFALSE);
   return NULL;
} /* iiCreateArrayIndex */

/** @brief Finds an element of an array. The only parameter expected is a ipdTreeNode_t structure of type ipdINDEX.
 *
 * The pointer to this
 * element &arr[i] will be returned, so that other functions can change
 * this element, e.g iiStoreInArray.
 */
ipdTreeNode_t *iiFindArrayElement(ipdTreeNode_t    *ax,
				  ipdDynamicName_t *baseSect)
{
   if ((ax))
   {
      ipdTreeNode_t *arr, *tmp;
      long           xi;

      xi = -1;
      if ((ax->type & ipdINDEX) && (arr = ax->node.expr.func.arrayIndex.tree))
      {
	 /* Find the array and calculate it. */
	 if (!(arr = iiCalculate(arr, baseSect)))
	    return NULL;
	 /* Calculate the index (offset) in an even array. */
	 while ((ax))
	 {
	    if (!(ax->node.expr.func.arrayIndex.index) ||
		!(arr) ||
		!(arr->type & ipdARRAYVAL) ||
		!(arr->node.expr.val.arrayval.tree) ||
		(arr->node.expr.val.arrayval.len == 0))
	    {
	       ipdSetError(ipdERR_EXPECT_ARRAY);
	       return NULL;
	    }
	    /* Calculate the index. */
	    if (!(tmp = iiCalculate(ax->node.expr.func.arrayIndex.index,
				    baseSect)))
	       return NULL;
	    /* Get the index. */
	    if (tmp->type & ipdINTEGER)
	       xi = tmp->node.expr.val.integer;
	    else if (tmp->type & ipdBOOLEAN)
	       xi = (long)(tmp->node.expr.val.boolean);
#if 0 /* do not use real values as indices! */
	    else if (tmp->type & ipdREAL)
	       xi = (long)(tmp->node.expr.val.quantity.re /* + 0.5 */);
#endif
	    else
	    {
	       ipdSetError(ipdERR_ARRAY_INDEX_TYPE);
	       return NULL;
	    }
	    /* Check the index. */
	    if (xi < 0)
	    {
	       ipdSetError(ipdERR_INDEX2LOW);
	       return NULL;
	    }
	    if (xi >= arr->node.expr.val.arrayval.len)
	    {
	       ipdSetError(ipdERR_INDEX2HIGH, xi, arr->node.expr.val.arrayval.len);
	       return NULL;
	    }
	    /* Get the array element of the one-dimensional array. */
	    arr = arr->node.expr.val.arrayval.tree[xi];

	    ax = ax->node.expr.func.arrayIndex.next;
	 }
	 return arr;
      }
   }
   return NULL;
} /* iiFindArrayElement */

/** @brief Deletes all intern functions. */
void iiFreeAllInternFunctions(void)
{
   iiInternFunc_t *fn, *tmp;

   fn = ipdBase->funcList;
   while ((fn))
   {
      ipdMBfree(fn->name);
      iiFreeTreeNodeRec(fn->tree, ipdFALSE);
      iiFreeTreeNodeRec(fn->paramList, ipdFALSE);
      ipdMBfree(fn->comment);

      tmp = fn->next;
      ipdMBfree(fn);
      fn  = tmp;
   }
   ipdBase->funcList = NULL;
} /* iiFreeAllInternFunctions */

/** @brief Deletes all C-coded functions. */
void iiFreeAllCCodedFunctions(void)
{
   iiCCodedFunc_t *fn, *tmp;

   fn = ipdCCodedFunctions;
   while ((fn))
   {
      tmp = fn->next;
      ipdMBfree(fn);
      fn  = tmp;
   }
   ipdCCodedFunctions = NULL;
} /* iiFreeAllCCodedFunctions */

/** @brief Searches for the intern function with the given name. */
static iiInternFunc_t *iiFindInternFunctionByName(const ipdChar *funcName)
{
   iiInternFunc_t *fn;

   fn = ipdBase->funcList;
   while ((fn))
   {
      if (!(iiStrCmp (fn->name, funcName)))
	 return fn;
      fn = fn->next;
   }
   return NULL;
} /* iiFindInternFunctionByName */


void iiPrintInternFunctions(ipdTreeNode_t *tn) // [JW] 
{
	if (!(tn))
	{	
		iiInternFunc_t *fn;
		ipdTreeNode_t  *param;
		ipdBoolean    notFirstParam;
		
		fn = ipdBase->funcList;
		printf("// User-defined functions:\n\n");
		while ((fn))
		{
			printf("%s(", (fn->name) ? fn->name : "?functionname?(");
			notFirstParam = ipdFALSE;

			param = fn->paramList;
			while ((param))
			{  
				 if (notFirstParam)
				    printf(", ");
				 else
				    notFirstParam = ipdTRUE;
				 printf("%s",
				 (param->node.parameter.name) ?
				 param->node.parameter.name :
					    " ?parametername?");
			   param = param->next;
			}
			printf(") = ");
			if ((fn->tree))
				iiPrintInternFunctions(fn->tree);
			//   ipdDumpRec(fOut, fn->tree, recursive, onlyValues, thisPrintf);
			else
			   printf("?tree?");
			printf(";");
			if ((fn->comment))
			   printf("%s", fn->comment);
			   //ipdPrintComment(fOut, fn->comment, indent, thisPrintf);
			else
			   printf("\n");
			fn = fn->next;
		}
		printf("\n");
	}
}




/** @brief Searches for the C-coded function with the given name. */
static iiCCodedFunc_t *iiFindCCodedUserFunctionByName(const ipdChar *funcName)
{
   iiCCodedFunc_t *fn;

   fn = ipdCCodedFunctions;
   while ((fn))
   {
      if (!(iiStrCmp (fn->name, funcName)))
	 return fn;
      fn = fn->next;
   }
   return NULL;
} /* iiFindCCodedUserFunctionByName */

/** @brief Finds a already defined function and initializes an ipdTreeNode_t structure which references this function.
 *
 * The number of parameters is checked, except for user-defined C-coded
 * functions. This has to be done by the user.
 */
ipdTreeNode_t *iiGetFunction(const ipdChar *funcName, ipdTreeNode_t *parameter)
{
   ipdTreeNode_t       *tn;
   iiInternFunc_t      *funcI;
   iiCCodedFunc_t      *funcC;
   iiBuiltInFunc_t     *funcB;

   if (!(funcName))
   {
      iiFreeTreeNodeRec(parameter, ipdFALSE);
      return NULL;
   }
   /* Temporary create an ipdINVALID tree node. We'll replace that soon. */
   if (!(tn = iiCreateTreeNode(ipdINVALID)))
   {
      iiFreeTreeNodeRec(parameter, ipdFALSE);
      return NULL;
   }
   /* We need no name (tn->node.sv.fullname or tn->node.sv.name) */
   /* We just store the functionpointers */

   /* Now get the function */
   if ((funcI = iiFindInternFunctionByName(funcName)))
   {
      /* Intern functions are user defined functions, defined using the
       * input deck programming language (ipl).
       */
      tn->type                                     = ipdINTERNFUNCTION;
      tn->node.expr.func.internFunc.func           = funcI;
      tn->node.expr.func.internFunc.param          = parameter;
      tn->node.expr.func.internFunc.funcName       = funcI->name; /* no strcpy()!*/

      if (ipdListLen(parameter) != funcI->parameterNum)
      {
	 if (funcI->parameterNum == 1)
	    ipdSetError(ipdERR_PARAM_NUM1, funcName);
	 else
	    ipdSetError(ipdERR_PARAM_NUMN, funcName, funcI->parameterNum);
	 iiFreeTreeNodeRec(tn, ipdFALSE);
	 tn = NULL;
      }
   }
   else if ((funcC = iiFindCCodedUserFunctionByName(funcName)))
   {
      /* C-coded functions are functions the user can add tho the input deck
       * function list by writing his own functions in c-language. The input
       * deck does not know anything about these functions. The input does
       * not even know how many parameter these functions will need. But the
       * user (the writer of this c-coded functions) has to know.
       */
      tn->type                                     = ipdCCODEDFUNCTION;
      tn->node.expr.func.cCodedFunc.func           = funcC->func;
      tn->node.expr.func.cCodedFunc.param          = parameter;
      tn->node.expr.func.cCodedFunc.userData       = funcC->userData;
      tn->node.expr.func.cCodedFunc.funcName       = funcC->name;
      tn->node.expr.func.cCodedFunc.userDestructor = funcC->userDestructor;
   }
   else if ((funcB = iiFindBuiltinFunctionByName(funcName)))
   {
      /* The standard functions like sin(), cos(), sqrt(), ... */
      tn->type                                     = ipdCCODEDFUNCTION;
      tn->node.expr.func.cCodedFunc.func           = funcB->func;
      tn->node.expr.func.cCodedFunc.param          = parameter;
      tn->node.expr.func.cCodedFunc.userData       = NULL;
      tn->node.expr.func.cCodedFunc.funcName       = (char *)funcB->name;
      tn->node.expr.func.cCodedFunc.userDestructor = funcB->userDestructor;
      tn->node.expr.eFlags                         |= ipdEF_BUILTIN_FUNC;

      if (ipdListLen(parameter) != funcB->parameterNum)
      {
	 if (funcB->parameterNum == 1)
	    ipdSetError(ipdERR_PARAM_NUM1, funcName);
	 else
	    ipdSetError(ipdERR_PARAM_NUMN, funcName, funcB->parameterNum);
	 iiFreeTreeNodeRec(tn, ipdFALSE);
	 tn = NULL;
      }
   }
   else
   {
      ipdSetError(ipdERR_FUNC_IPDNOT_DEFINED, funcName);
      iiFreeTreeNodeRec(tn, ipdFALSE);
      tn = NULL;
   }
   return tn;
} /* iiGetFunction */

/** @brief Creates an intern function. Defined with IPL. */
iiInternFunc_t *iiCreateInternFunction(char          *funcName,
				       ipdTreeNode_t *parameterList,
				       ipdTreeNode_t *tree)
{
   if ((tree) && (funcName))
   {
      iiInternFunc_t *fn, *ftmp;

      if ((iiFindInternFunctionByName(funcName)))
      {
	 ipdSetError(ipdERR_FUNC_ALREADY_DEFINED, funcName);
	 iiFreeTreeNodeRec(parameterList, ipdFALSE);
	 iiFreeTreeNodeRec(tree, ipdFALSE);
	 ipdMBfree(funcName);
	 return NULL;
      }
      if (!(fn = (iiInternFunc_t *)ipdMBalloc(sizeof(iiInternFunc_t))))
      {
	 iiFreeTreeNodeRec(parameterList, ipdFALSE);
	 iiFreeTreeNodeRec(tree, ipdFALSE);
	 ipdMBfree(funcName);
	 return NULL;
      }

      /* No ipdMBclear() not called, so set unused pointers to NULL! */
      fn->next                                = NULL;
      fn->name                                = funcName;
      fn->tree                                = tree;
      fn->paramList                           = parameterList;
      fn->parameterNum                        = ipdListLen(parameterList);
      fn->comment                             = NULL;

      if ((ftmp = ipdBase->funcList))
         while ((ftmp->next))
	         ftmp = ftmp->next;
      if ((ftmp))
	      ftmp->next        = fn;
      else
	      ipdBase->funcList = fn;

      return fn;
   }
   iiFreeTreeNodeRec(parameterList, ipdFALSE);
   iiFreeTreeNodeRec(tree, ipdFALSE);
   ipdMBfree(funcName);
   return NULL;
} /* iiCreateInternFunction */

/*=======================================================================
	EXTERN FUNCTIONS
=======================================================================*/

/*-------------------------------------------------------------------------
 * Creates a C-coded function.
 * funcName: No Path!! Only the Name.
 * func    : The C-function
 */
void ipdCreateExternFunction(char          *funcName,
			     ipdFunc_t      func,
			     void          *userData,
			     ipdUserFunc_t  userDestructor)
{
   iiCCodedFunc_t *fn;

   if (!(fn = (iiCCodedFunc_t *)ipdMBalloc(sizeof(iiCCodedFunc_t))))
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return;
   }

   fn->name            = funcName;
   fn->func            = func;
   fn->userData        = userData;
   fn->userDestructor  = userDestructor;

   fn->next            = ipdCCodedFunctions;
   ipdCCodedFunctions  = fn;
} /* ipdCreateExternFunction */

/*-------------------------------------------------------------------------
 * Returns the optional parameter with the given name
 */
ipdTreeNode_t *ipdGetOptionByName(ipdTreeNode_t *tn,
				  const char *name)
{
   if (tn && name)
   {
      while ((tn))
      {
	 if ((tn->type & ipdOPTION) &&
	     !(iiStrCmp(tn->node.option.name, name)))
	    return tn;
	 tn = tn->next;
      }
   }
   return NULL;
} /* ipdGetOptionByName */

/*-------------------------------------------------------------------------
 * Returns the parameter with the given index
 */
ipdTreeNode_t *ipdGetParamByIndex(ipdTreeNode_t *tn,
				  ipdLong          index)
{
   if (tn)
   {
      while ((tn))
      {
	 if (index == 0)
	    return tn;
	 index--;
	 tn = tn->next;
      }
   }
   return NULL;
} /* ipdGetParamByIndex */

/*-------------------------------------------------------------------------
 * Prints list of valid options for a function.
 */
ipdVoid ipdPrintOptions(const char *funcName,
		      const char *validOptions[])
{
   const char **vo;

   ipdStdLog("\nFunction %s: Valid options are ",
	     funcName);
   for (vo = validOptions; (*vo); vo++)
      if (*(vo+1))
	 ipdStdLog("'%s', ", *vo);
      else
	 ipdStdLog("'%s'.\n", *vo);
} /* ipdPrintOptions */

/*-------------------------------------------------------------------------
 * Checks options. If a wrong option has been given in the input files an error
 * is thrown. All valid options known to the current function must be given by
 * an NULL-terminated array of strings in validOptions.
 */
ipdBoolean ipdCheckOptions(const char    *funcName,
			 ipdTreeNode_t *param,
			 const char    *validOptions[])
{
   if (param && validOptions)
   {
      const char    **vo;
      ipdTreeNode_t  *tn;

      for (tn = param; (tn); tn = tn->next)
	 if (tn->type & ipdOPTION)
	 {
	    for (vo = validOptions; (*vo); vo++)
	       if (!strcmp(tn->node.option.name, *vo))
		  break;
	    if (!*vo)
	    {
	       ipdSetError(ipdERR_UNKIPDNOWN_OPTION,
			   funcName,
			   tn->node.option.name);
	       ipdPrintOptions(funcName, validOptions);
	    }
	 }
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdCheckOptions */

/*-------------------------------------------------------------------------
 * Counts the number of parameters in a parameter list. Options are ignored.
 */
ipdLong ipdGetParamNum(ipdTreeNode_t *tn)
{
   ipdLong num = 0;

   for (; (tn); tn = tn->next)
      if (!(tn->type & ipdOPTION))
	 num++;
   return num;
} /* ipdGetParamNum */

/*=======================================================================
	Tracing
=======================================================================*/

/** @brief Set a trace variable */
ipdTreeNode_t *iiSetTraceVar(const char *fullname, ipdTreeNode_t *expr)
{
  ipdTreeNode_t *tn;

  if ((fullname) && (fullname = iiCreateName(fullname, NULL)))
  {
     tn = iiHashFindTreeNode(fullname);
     ipdMBfree(fullname);
     if (!(tn))
     {
	ipdSetError(ipdERR_VAR_IPDNOT_FOUND, fullname);
	iiFreeTreeNodeRec(expr, ipdFALSE);
	return NULL;
     }
     if (!(tn->type & ipdVARIABLE))
     {
	ipdSetError(ipdERR_IPDNO_VARIABLE, fullname);
	iiFreeTreeNodeRec(expr, ipdFALSE);
	return NULL;
     }
     iiFreeTreeNodeRec(tn->node.sv.svu.var.traceTree, ipdFALSE);
     tn->node.sv.svu.var.traceTree  = expr;
     tn->node.sv.svu.var.flags     |= ipdVF_TRACE;
     return tn;
  }
  return NULL;
} /* iiSetTraceVar */
