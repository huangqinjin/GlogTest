#include <iostream>
#include <filesystem>

#include <gflags/gflags.h>
#include <glog/logging.h>

namespace fs = std::filesystem;


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


    gflags::SetVersionString(VERSION);
    gflags::SetUsageMessage(std::string(argv[0]) + " [OPTION]");
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    google::InitGoogleLogging(argv[0]);


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
