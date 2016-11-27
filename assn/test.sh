#!/bin/bash

doas() {
	user="$1"
	path="$2"
	succeed="$3"
	cmd="$4"
	verb="$5"
	sudo -u "$user" bash -c "$cmd$path" >/dev/null 2>&1
	status=$?
	if test "$succeed" = "succeed"; then
		if test $status -ne 0; then
			echo "$user cannot $verb $path" >&2
			exit 1
		fi
	else
		if test $status -eq 0; then
			echo "$user can $verb $path" >&2
			exit 1
		fi
	fi
	
}

readas() {
	doas "$@" "cat mount" "read"
}

writeas() {
	doas "$@" "echo >>mount" "write"
}

readas bob /etc/lpd.conf succeed
writeas bob /etc/lpd.conf succeed
readas bob /var/syslog succeed
writeas bob /var/syslog fail
readas bob /dev/lp0 fail
writeas bob /dev/lp0 fail
readas bob /var/reports/report1 fail
writeas bob /var/reports/report1 fail
readas bob /var/home/file1 fail
writeas bob /var/home/file1 fail

readas john /etc/lpd.conf fail
writeas john /etc/lpd.conf fail
readas john /var/reports/report1 succeed
writeas john /var/reports/report1 succeed
readas john /var/reports/report2 succeed
writeas john /var/reports/report2 succeed
readas john /home/file1 succeed
writeas john /home/file1 succeed
readas john /home/file2 succeed
writeas john /home/file2 succeed

readas lpd /etc/lpd.conf succeed
writeas lpd /etc/lpd.conf fail
readas lpd /var/syslog fail
writeas lpd /var/syslog succeed
readas lpd /dev/lp0 succeed
writeas lpd /dev/lp0 succeed
readas lpd /var/reports/report1 fail
writeas lpd /var/reports/report1 fail
readas lpd /var/home/file1 fail
writeas lpd /var/home/file1 fail
