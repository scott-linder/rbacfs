#!/bin/bash

. ../funcs.sh

readas user1 /etc/lpd.conf succeed
writeas user1 /etc/lpd.conf succeed
readas user1 /var/syslog succeed
writeas user1 /var/syslog fail
readas user1 /dev/lp0 fail
writeas user1 /dev/lp0 fail
readas user1 /var/reports/report1 fail
writeas user1 /var/reports/report1 fail
readas user1 /var/home/file1 fail
writeas user1 /var/home/file1 fail

readas user2 /etc/conf.conf fail
writeas user2 /etc/conf.conf fail
readas user2 /var/reports/report1 succeed
writeas user2 /var/reports/report1 succeed
readas user2 /var/reports/report2 succeed
writeas user2 /var/reports/report2 succeed
readas user2 /home/file1 succeed
writeas user2 /home/file1 succeed
readas user2 /home/file2 succeed
writeas user2 /home/file2 succeed
readas user2 /etc/lpd.conf succeed
writeas user2 /etc/lpd.conf fail
readas user2 /var/syslog fail
writeas user2 /var/syslog succeed
readas user2 /dev/lp0 succeed
writeas user2 /dev/lp0 succeed

readas user3 /etc/lpd.conf fail
writeas user3 /etc/lpd.conf fail
readas user3 /var/syslog fail
writeas user3 /var/syslog fail
readas user3 /dev/lp0 fail
writeas user3 /dev/lp0 fail
readas user3 /var/reports/report1 fail
writeas user3 /var/reports/report1 fail
readas user3 /var/home/file1 fail
writeas user3 /var/home/file1 fail
