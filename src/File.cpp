#include "File.h"
#include "VisualStudioWarnings.h"

#include <assert.h>  // <----+ this shit doesn't work with it
                     //     \|/
#define _CRT_SECURE_NO_WARNINGS 
#include <cstdlib>
#include <cstdio>



bool fls_free   (void *ptr)  { std::free   (ptr);  return false; }
bool fls_fclose (FILE *file) { std::fclose (file); return false; }

File::File (const char *path) :
    file_path        (path),
    buff             (nullptr),
    buff_size        (0),
    is_exlusive_buff (true)     
{
    assert (path && "file path is nullptr");
}

File::~File()
{
    if (is_exlusive_buff)
        std::free (buff);
}
                  
#define FCLOSE   fls_fclose (file) 
#define FREE     fls_free   (buff)     

bool File::read()
{
    if (!file_path) return false;

    FILE *file = std::fopen (file_path, "rb");
    if (!file) return false;

    const size_t file_size = getFileSize (file);
    if (file_size == 0) return FCLOSE;

    if (!buff)
    {
        buff = (char *)calloc (file_size + 1 , sizeof (buff[0]));
        if (!buff) return FCLOSE;
    }
    else if (!is_exlusive_buff)
    {
        if (buff_size < file_size)
        {
            char *reallocated_buff = (char *)realloc (buff, (file_size + 1) * sizeof (buff[0]));
            if (!reallocated_buff) return FCLOSE;
            buff = reallocated_buff;
        }
    }
    else 
        return FCLOSE;

    buff_size = file_size;
    if (buff_size !=
        fread (buff, sizeof (buff[0]), buff_size, file))
        return is_exlusive_buff && FREE || FCLOSE;

    FCLOSE; /* FCLOSE will not be called with true || */
    return true;
}

bool File::write()
{                                                 
    if (!file_path) return false;                 
    if (!buff)      return false;

    if (!buff) return false;

    FILE *file = std::fopen (file_path, "wb");
    if (!file) return false;

    if (buff_size !=
        fwrite (buff, sizeof (buff[0]), buff_size, file))
        return FCLOSE;

    FCLOSE;
    return true;
}

#undef FCLOSE
#undef FREE


void File::setBuffer (char *external_buff, size_t buff_sz)
{
    is_exlusive_buff = false;
    this->buff       = external_buff;
    this->buff_size  = buff_sz;
}

long File::getFileSize (FILE * file)
{
    if (!file) return 0;

    long cur_pos = ftell (file);  
    if (cur_pos == -1) return 0;

    long size = 0;

    fseek (file, 0, SEEK_END);
    size = ftell (file);
    /* error case */
    if (size == -1) return 0;

    /* returns old file position */
    fseek (file, cur_pos, SEEK_SET);

    return size;
}