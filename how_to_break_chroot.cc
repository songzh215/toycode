#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <sys/wait.h>
#include <boost/filesystem.hpp>
#include <sys/mount.h>


int main() {
    boost::filesystem::path old_root("/");
    boost::filesystem::path new_root("/tmp/");
    boost::filesystem::path new_root_file("/tmp/cr_test");
    int old_root_fd = open;

    if (!boost::filesystem::exists(new_root)) {
        boost::filesystem::create_directory(new_root);
    }
    // try to chroot to /tmp
    exit(0);
}
