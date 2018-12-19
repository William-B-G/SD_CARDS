//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                            FAT16/32 File IO Library
//                                    V2.6
//                                 Ultra-Embedded.com
//                            Copyright 2003 - 2012
//
//                         Email: admin@ultra-embedded.com
//
//                            License: Commercial
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003-2012 Ultra-Embedded.com.
// All rights reserved. 
// 
// The author grants permission for use of this software in closed source & 
// commercial projects.
// This license implies no requirement to release any source code that uses this
// project.
// Please do not openly redistribute this software in source form, use the open 
// source version instead.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
// SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
// OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <string.h>
#include "fat_cache.h"

// Per file cluster chain caching used to improve performance.
// This does not have to be enabled for architectures with low
// memory space.

//-----------------------------------------------------------------------------
// fatfs_cache_init:
//-----------------------------------------------------------------------------
int	fatfs_cache_init(struct fatfs *fs, FL_FILE *file)
{
#ifdef FAT_CLUSTER_CACHE_ENTRIES
	int i;
	
	for (i=0;i<FAT_CLUSTER_CACHE_ENTRIES;i++)
	{
		file->cluster_cache_idx[i] = 0xFFFFFFFF; // Not used
		file->cluster_cache_data[i] = 0; 
	}
#endif
	
	return 1;
}
//-----------------------------------------------------------------------------
// fatfs_cache_get_next_cluster:
//-----------------------------------------------------------------------------
int	fatfs_cache_get_next_cluster(struct fatfs *fs, FL_FILE *file, uint32 clusterIdx, uint32 *pNextCluster)
{
#ifdef FAT_CLUSTER_CACHE_ENTRIES
	uint32 slot = clusterIdx % FAT_CLUSTER_CACHE_ENTRIES;

	if (file->cluster_cache_idx[slot] == clusterIdx)
	{
		*pNextCluster = file->cluster_cache_data[slot];
		return 1;
	}
#endif
	
	return 0;
}
//-----------------------------------------------------------------------------
// fatfs_cache_set_next_cluster:
//-----------------------------------------------------------------------------
int	fatfs_cache_set_next_cluster(struct fatfs *fs, FL_FILE *file, uint32 clusterIdx, uint32 nextCluster)
{
#ifdef FAT_CLUSTER_CACHE_ENTRIES
	uint32 slot = clusterIdx % FAT_CLUSTER_CACHE_ENTRIES;

	if (file->cluster_cache_idx[slot] == clusterIdx)
		file->cluster_cache_data[slot] = nextCluster;
	else
	{
		file->cluster_cache_idx[slot] = clusterIdx;
		file->cluster_cache_data[slot] = nextCluster;
	}
#endif

	return 1;
}
