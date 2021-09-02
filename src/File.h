#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <cstdio>

/* @todo TEST read / write */

struct File
{
    File (const char * path);
   ~File ();

    bool read();
    bool write();
    void setBuffer (char * buff, size_t buff_sz);

    static long getFileSize (FILE * file);

    const char * file_path;

    char       * buff;
    size_t       buff_size;
    bool         is_exlusive_buff;                        
};

#endif // !FILE_H_INCLUDED
