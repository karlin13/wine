#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "xmalloc.h"
#include "windows.h"
#include "winbase.h"
#include "dos_fs.h"
#include "heap.h"
#include <ctype.h>

#define PATH_LEN 260

struct FindFileContext32 {
    DIR *	dir;
    char mask[PATH_LEN];
    char path[PATH_LEN];
};

typedef struct FindFileContext32 FindFileContext32;

const char *DOSFS_Hash(const char *, int);

/* example D:\*.dbs */

static BOOL32 MatchWildCard(LPCSTR file, LPCSTR mask)
{
    int	len;

    /* We should check volume information to see if long filenames possible.
     */

    len = strlen(file);

    while (*file) {
	if (*mask == '*') {
	    if (*(mask+1)) {
		while (*file && (toupper(*file) != *(mask+1))) file++;
		if (!*file)
		    return FALSE;
	    }
	    else
		break;
	}
	else {
	    if (*mask != '?' && *mask != toupper(*file)) {
		return FALSE;
	    }
	    file++;
	}
	mask++;
    }
    return (TRUE);
}

/*************************************************************************
 *              FindNextFile32A             (KERNEL32.126)
 */

BOOL32 FindNextFile32A(HANDLE32 handle, LPWIN32_FIND_DATA32A data)
{
    FindFileContext32 *context;
    struct dirent *dirent;
    char  dosname[14];

    memset(data, 0, sizeof(WIN32_FIND_DATA32A));
    context = (FindFileContext32 *) handle;

    while ((dirent = readdir(context->dir)) != NULL) {
	if (strcmp(dirent->d_name, "..") == 0 ||
	    strcmp(dirent->d_name, ".") == 0)
	    continue;

	strcpy(dosname, DOSFS_Hash(dirent->d_name, FALSE));

	if (MatchWildCard(dirent->d_name, context->mask)) {
	    /* Full file name - is this a long file name?
	     * If it is, we should probably use the dirent
	     * instead of the dos hashed name.
	     */
	    strcpy(data->cFileName, dosname);

	    /* file name expressed in 8.3 format */
	    strcpy(data->cAlternateFileName, dosname);
	    return (TRUE);
	}
    }
    
    return (FALSE);
}

/*************************************************************************
 *              FindFirstFile32A             (KERNEL32.123)
 */

HANDLE32 FindFirstFile32A(LPCSTR lpfilename, 
			  LPWIN32_FIND_DATA32A lpFindFileData)
{
    const char *unixpath;
    char *slash, *p;
    FindFileContext32 *context;

    context = HeapAlloc(SystemHeap, 0, sizeof(FindFileContext32));
    if (!context)
	return (INVALID_HANDLE_VALUE);

    slash = strrchr(lpfilename, '\\');
    if (slash) {
	lstrcpyn32A(context->path, lpfilename, slash - lpfilename + 1);
	context->path[slash - lpfilename + 1] = '\0';
	unixpath = DOSFS_GetUnixFileName(context->path, FALSE);
	if (!unixpath) {
	    /* FIXME: SetLastError(??) */
	    HeapFree(SystemHeap, 0, context);
	    return INVALID_HANDLE_VALUE;
	}
	lstrcpy32A(context->mask, slash+1);
    }
    else {
	context->path[0] = '\0';
	unixpath = ".";
	lstrcpy32A(context->mask, lpfilename);
    }

    context->dir = opendir(unixpath);
    if (!context->dir) {
	/* FIXME: SetLastError(??) */
	HeapFree(SystemHeap, 0, context);
	return INVALID_HANDLE_VALUE;
    }

    strcpy(context->mask, slash+1);

    /* uppercase mask in place */
    for (p = context->mask ; *p; p++)
	*p = toupper(*p);

    if (!FindNextFile32A((HANDLE32) context, lpFindFileData))
	return (INVALID_HANDLE_VALUE);
    return ((HANDLE32) context);
}

/*************************************************************************
 *              FindClose             (KERNEL32.119)
 */
BOOL32 FindClose(HANDLE32 handle)
{
    FindFileContext32 *context;

    context = (FindFileContext32 *) handle;
    if (context->dir)
	closedir(context->dir);
    HeapFree(SystemHeap, 0, context);
    return (TRUE);
}
