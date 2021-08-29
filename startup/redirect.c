#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <syscall.h>

#include "redirect.h"

void redirect_output_lines(
        char* buf, size_t len, void* ctx,
        void (*puts)(char* line, size_t len, void* ctx))
{
    char c;
    char* line;
    if (buf == NULL || len == 0) return;
    buf[--len] = '\0';

    while (len > 0)
    {
        line = buf;
        while (*buf != '\0' && *buf != '\n') ++buf;
        c = *buf;
        *buf = '\0';
        (*puts)(line, buf - line, ctx);
        *buf = c;
        ++buf;
        len -= (buf - line);
    }
}

static void putfile(char* line, size_t len, void* file)
{
    char c = line[len];
    line[len] = '\n';
    fwrite(line, sizeof(c), len + 1, file);
    line[len] = c;
}

void redirect_output_lines_to_file(
        char* buf, size_t len, FILE* file)
{
    return redirect_output_lines(buf, len, file, &putfile);
}

static void putfd(char* line, size_t len, void* fd)
{
    char c = line[len];
    line[len] = '\n';
    write((int)(intptr_t)fd, line, sizeof(c) * (len + 1));
    line[len] = c;
}

void redirect_output_lines_to_fd(
        char* buf, size_t len, int fd)
{
    return redirect_output_lines(buf, len, (void*)(intptr_t)fd, &putfd);
}


static void readfd(int fd, char** buf, size_t* len)
{
    char tmp[1024];
    char* p = tmp;
    size_t s = 0;
    while (1)
    {
        ssize_t n = read(fd, p, sizeof(buf) - s);
        if (n > 0)
        {
            p += n;
            s += n;
        }

        if ((n <= 0 && (s > 0 || *buf)) || (s == sizeof(buf)))
        {
            *buf = realloc(*buf, *len + s + (n <= 0));
            memcpy(*buf + *len, tmp, s);
            *len += s;

            if (n <= 0)
            {
                (*buf)[*len] = '\0';
                *len += 1;
            }
            else
            {
                p = tmp;
                s = 0;
            }
        }

        if (n <= 0) break;
    }
}

int redirect_by_pipe_start(struct redirect_by_pipe* r, int fd)
{
    r->old = -1;
    r->buf = NULL;
    r->len = 0;
    if (pipe(r->fd) == 0)
    {
        r->old = fd;
        fd = dup(r->old);
        dup2(r->fd[1], r->old);
        close(r->fd[1]);
        r->fd[1] = fd;
        return 0;
    }
    return 1;
}

void redirect_by_pipe_stop(struct redirect_by_pipe* r)
{
    if (r->old >= 0)
    {
        dup2(r->fd[1], r->old);
        close(r->fd[1]);
        r->old = -1;
        readfd(r->fd[0], &r->buf, &r->len);
        close(r->fd[0]);
    }
}

void redirect_by_pipe_cleanup(struct redirect_by_pipe* r)
{
    free(r->buf);
    r->buf = NULL;
    r->len = 0;
}


int redirect_by_memfd_start(struct redirect_by_memfd* r, int fd)
{
    char name[256];
    sprintf(name, "/%d", fd);
#if 0
    r->fd[0] = shm_open(name,O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    // The operation of shm_unlink() is analogous to unlink().
    // The file will remain in existence until the
    // last file descriptor referring to it is closed.
    if (r->fd[0] >= 0) shm_unlink(name);
#elif defined(SYS_memfd_create)
    r->fd[0] = (int)syscall(SYS_memfd_create, name, 0);
#else
    r->fd[0] = memfd_create(name, 0);
#endif
    r->old = -1;
    r->buf = NULL;
    r->len = 0;
    if (r->fd[0] >= 0)
    {
        r->old = fd;
        r->fd[1] = dup(fd);
        dup2(r->fd[0], r->old);
        return 0;
    }
    return 1;
}

void redirect_by_memfd_stop(struct redirect_by_memfd* r)
{
    char term[] = "";
    if (r->old >= 0)
    {
        dup2(r->fd[1], r->old);
        close(r->fd[1]);
        r->old = -1;
        r->len = lseek(r->fd[0], 0, SEEK_CUR);
        if (r->len > 0)
        {
            write(r->fd[0], term, sizeof(term));
            r->len += sizeof(term);
            r->buf = mmap(NULL, r->len, PROT_READ | PROT_WRITE, MAP_SHARED, r->fd[0], 0);
        }
        close(r->fd[0]);
        if (r->buf == MAP_FAILED)
        {
            r->buf = NULL;
            r->len = 0;
        }
    }
}

void redirect_by_memfd_cleanup(struct redirect_by_memfd* r)
{
    if (r->buf)
    {
        munmap(r->buf, r->len);
        r->buf = NULL;
        r->len = 0;
    }
}


int redirect_by_memstream_start(struct redirect_by_memstream* r, FILE** file)
{
    if ((r->mem = open_memstream(&r->buf, &r->len)))
    {
        r->file = file;
        r->old = *file;
        *file = r->mem;
        return 0;
    }
    return 1;
}

void redirect_by_memstream_stop(struct redirect_by_memstream* r)
{
    char term[] = "";
    if (r->mem)
    {
        *r->file = r->old;
        fflush(r->mem);
        if (r->len > 0)
        {
            fwrite(term, 1, sizeof(term), r->mem);
        }
        fclose(r->mem);
        r->mem = NULL;
        if (r->len == 0)
        {
            free(r->buf);
            r->buf = NULL;
        }
    }
}

void redirect_by_memstream_cleanup(struct redirect_by_memstream* r)
{
    free(r->buf);
    r->buf = NULL;
    r->len = 0;
}
