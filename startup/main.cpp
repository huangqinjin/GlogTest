#include <iostream>
#include <filesystem>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "redirect.h"

#if defined(__ANDROID__)
#include <android/log.h>
#endif


namespace fs = std::filesystem;


static struct redirect_by_pipe redirect_by_pipe;
static struct redirect_by_memfd redirect_by_memfd;
static struct redirect_by_memstream redirect_by_memstream;

static void debug(char* line, size_t len, void*)
{
    char c = line[len];
    line[len] = '\n';
    write(STDERR_FILENO, line, sizeof(c) * (len + 1));
    line[len] = c;

#if defined(__ANDROID__)
    __android_log_write(ANDROID_LOG_FATAL, "stderr", line);
#endif
}

static void redirect_by_pipe_atexit()
{
    redirect_by_pipe_stop(&redirect_by_pipe);
    if (redirect_by_pipe.buf)
    {
        char msg[64];
        int n = snprintf(msg, sizeof(msg), "redirect_by_pipe: msg[%zu]", redirect_by_pipe.len);
        debug(msg, n, nullptr);
        redirect_output_lines(redirect_by_pipe.buf, redirect_by_pipe.len, nullptr, &debug);
    }
    redirect_by_pipe_cleanup(&redirect_by_pipe);
}

static void redirect_by_memfd_atexit()
{
    redirect_by_memfd_stop(&redirect_by_memfd);
    if (redirect_by_memfd.buf)
    {
        char msg[64];
        int n = snprintf(msg, sizeof(msg), "redirect_by_memfd: msg[%zu]", redirect_by_memfd.len);
        debug(msg, n, nullptr);
        redirect_output_lines(redirect_by_memfd.buf, redirect_by_memfd.len, nullptr, &debug);
    }
    redirect_by_memfd_cleanup(&redirect_by_memfd);
}

static void redirect_by_memstream_atexit()
{
    redirect_by_memstream_stop(&redirect_by_memstream);
    if (redirect_by_memstream.buf)
    {
        char msg[64];
        int n = snprintf(msg, sizeof(msg), "redirect_by_memstream: msg[%zu]", redirect_by_memstream.len);
        debug(msg, n, nullptr);
        redirect_output_lines(redirect_by_memstream.buf, redirect_by_memstream.len, nullptr, &debug);
    }
    redirect_by_memstream_cleanup(&redirect_by_memstream);
}


template<int Permissions>
static bool CheckFilePermissions(const char* name, const std::string& value)
{
    if (!value.empty() && access(value.c_str(), Permissions | F_OK))
    {
        perror(value.c_str());
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    gflags::RegisterFlagValidator(&FLAGS_log_dir, CheckFilePermissions<W_OK>);


    atexit(redirect_by_pipe_atexit);
    atexit(redirect_by_memfd_atexit);
    atexit(redirect_by_memstream_atexit);

#define REDIRECT 2
#if REDIRECT == 1
    redirect_by_pipe_start(&redirect_by_pipe, STDERR_FILENO);
#elif REDIRECT == 2
    redirect_by_memfd_start(&redirect_by_memfd, STDERR_FILENO);
#elif REDIRECT == 3
    redirect_by_memstream_start(&redirect_by_memstream, &stderr);
#endif


    gflags::SetVersionString(VERSION);
    gflags::SetUsageMessage(std::string(argv[0]) + " [OPTION]");
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    google::InitGoogleLogging(argv[0]);


    redirect_by_pipe_atexit();
    redirect_by_memfd_atexit();
    redirect_by_memstream_atexit();


    // Only create log file for INFO as it contains all severity levels.
    google::SetLogDestination(google::WARNING, "");
    google::SetLogDestination(google::ERROR, "");
    google::SetLogDestination(google::FATAL, "");


    for(int i = 0; i < argc; ++i)
    {
        LOG(INFO) << '[' << i << ']' << ':' << ' ' << '[' << argv[i] << ']';
    }

    LOG(INFO) << "Command Line Flags:\n" << gflags::CommandlineFlagsIntoString();


#define OUTPUT(severity) LOG(severity) << #severity
#define DOUTPUT(severity) DLOG(severity) << "DEBUG " #severity
#define VOUTPUT(verbose) VLOG(verbose) << "VERBOSE " #verbose

    VOUTPUT(1);
    VOUTPUT(2);
    DOUTPUT(INFO);
    OUTPUT(INFO);
    DOUTPUT(WARNING);
    OUTPUT(WARNING);
    DOUTPUT(ERROR);
    OUTPUT(ERROR);
    DOUTPUT(FATAL);
    OUTPUT(FATAL);

    return 0;
}
