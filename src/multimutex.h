/* Copyright (c) 2020-2022, ellie/@ell1e & HVM Team (see AUTHORS.md).

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Alternatively, at your option, this file is offered under the Apache 2
license, see accompanied LICENSE.md.
*/

#ifndef HVM_MULTIMUTEX_H_
#define HVM_MULTIMUTEX_H_

typedef struct multimutex multimutex;
typedef uint64_t mmutexentry_t;


multimutex *multimutex_New();

int multimutex_AddEntry(
    multimutex *mm, mmutexentry_t *out_entry
);

int multimutex_DelEntry(
    multimutex *mm, mmutexentry_t entry
);

int multimutex_IsEntryLocked(
    multimutex *mm, mmutexentry_t entry
);

void multimutex_LockEntry(
    multimutex *mm, mmutexentry_t entry
);

int multimutex_UnlockEntry(
    multimutex *mm, mmutexentry_t entry
);

void multimutex_WaitForAnyUnlocked(
    multimutex *mm
);

void multimutex_UnregisterLockEntryWaiter(
    multimutex *mm, mmutexentry_t entry,
    void *ptrinfo
);

int multimutex_RegisterLockEntryWaiter(
    multimutex *mm, mmutexentry_t entry,
    void *ptrinfo
);

mmutexentry_t *multimutex_GetNewlyMaybeUnlockedEntries(
    multimutex *mm, uint64_t *count
);

void **multimutex_GetRegisteredWaitersForEntry(
    multimutex *mm, mmutexentry_t entry, uint64_t *count
);

void multimutex_Destroy(multimutex *mm);

#endif  // HVM_MULTIMUTEX_H_

