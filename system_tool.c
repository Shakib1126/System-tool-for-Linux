#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <fcntl.h>

// Function to get memory information
void displayMemoryInfo() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long long totalMem = memInfo.totalram * memInfo.mem_unit;
    long long freeMem = memInfo.freeram * memInfo.mem_unit;

    printf("\nMemory Information:\n");
    printf("  Total Memory: %lld bytes (%.2f MB)\n", totalMem, (double)totalMem / (1024 * 1024));
    printf("  Free Memory: %lld bytes (%.2f MB)\n", freeMem, (double)freeMem / (1024 * 1024));
}

// Function to get disk space information
void displayDiskSpaceInfo() {
    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        unsigned long long totalSpace = vfs.f_blocks * vfs.f_frsize;
        unsigned long long freeSpace = vfs.f_bfree * vfs.f_frsize;

        printf("\nDisk Space Information:\n");
        printf("  Total Disk Space: %llu bytes (%.2f GB)\n", totalSpace, (double)totalSpace / (1024 * 1024 * 1024));
        printf("  Free Disk Space: %llu bytes (%.2f GB)\n", freeSpace, (double)freeSpace / (1024 * 1024 * 1024));
    }
}

// Function to get network information
void displayNetworkInfo() {
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    printf("\nNetwork Information:\n");
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            char addrBuf[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &sa->sin_addr, addrBuf, sizeof(addrBuf));
            printf("  Interface: %s, IP Address: %s\n", ifa->ifa_name, addrBuf);
        }
    }

    freeifaddrs(ifaddr);
}

// Function to get CPU usage
void displayCPUUsage() {
    int fd;
    char buf[128];
    double user, nice, system, idle;
    
    if ((fd = open("/proc/stat", O_RDONLY)) == -1) {
        perror("open");
        return;
    }
    
    read(fd, buf, sizeof(buf));
    sscanf(buf, "cpu %lf %lf %lf %lf", &user, &nice, &system, &idle);

    printf("\nCPU Usage:\n");
    printf("  User: %.2f%%\n", user / (user + nice + system + idle) * 100);
    printf("  System: %.2f%%\n", system / (user + nice + system + idle) * 100);
    printf("  Idle: %.2f%%\n", idle / (user + nice + system + idle) * 100);
    
    close(fd);
}

// Function to get load average
void displayLoadAverage() {
    double loadavg[3];
    if (getloadavg(loadavg, 3) != -1) {
        printf("\nLoad Average:\n");
        printf("  1-minute: %.2f\n", loadavg[0]);
        printf("  5-minute: %.2f\n", loadavg[1]);
        printf("  15-minute: %.2f\n", loadavg[2]);
    }
}

// Function to get system uptime
void displayUptime() {
    struct sysinfo info;
    if (sysinfo(&info) != -1) {
        printf("\nSystem Uptime:\n");
        printf("  %ld days %ld hours %ld minutes\n", info.uptime / (3600 * 24), (info.uptime % (3600 * 24)) / 3600, (info.uptime % 3600) / 60);
    }
}

int main() {
    struct utsname unameData;
    uname(&unameData);

    printf("System Information:\n");
    printf("  OS: %s %s %s\n", unameData.sysname, uname
