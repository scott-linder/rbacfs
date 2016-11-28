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
