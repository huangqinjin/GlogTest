#ifndef REDIRECT_H
#define REDIRECT_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// `buf` is always null-terminated and `len` also counts null-terminators.
//

void redirect_output_lines(
        char* buf, size_t len, void* ctx,
        void (*puts)(char* line, size_t len, void* ctx));

void redirect_output_lines_to_file(
        char* buf, size_t len, FILE* file);

void redirect_output_lines_to_fd(
        char* buf, size_t len, int fd);


struct redirect_by_pipe
{
    char* buf;
    size_t len;
    int old;
    int fd[2];
};

int redirect_by_pipe_start(struct redirect_by_pipe* r, int fd);
void redirect_by_pipe_stop(struct redirect_by_pipe* r);
void redirect_by_pipe_cleanup(struct redirect_by_pipe* r);

struct redirect_by_memfd
{
    char* buf;
    size_t len;
    int old;
    int fd[2];
};

int redirect_by_memfd_start(struct redirect_by_memfd* r, int fd);
void redirect_by_memfd_stop(struct redirect_by_memfd* r);
void redirect_by_memfd_cleanup(struct redirect_by_memfd* r);


struct redirect_by_memstream
{
    char* buf;
    size_t len;
    FILE* old;
    FILE* mem;
    FILE** file;
};

// https://man7.org/linux/man-pages/man3/stdout.3.html
// > Since the symbols stdin, stdout, and stderr are specified to be
// > macros, assigning to them is nonportable.
// https://en.cppreference.com/w/c/io/std_streams
// > These macros may be expanded to modifiable lvalues.
// > If any of these FILE* lvalue is modified, subsequent
// > operations on the corresponding stream result in
// > unspecified or undefined behavior.
int redirect_by_memstream_start(struct redirect_by_memstream* r, FILE** file);
void redirect_by_memstream_stop(struct redirect_by_memstream* r);
void redirect_by_memstream_cleanup(struct redirect_by_memstream* r);


#ifdef __cplusplus
}
#endif
#endif
