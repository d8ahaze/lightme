#!/bin/bash

while true
do
	/root/zxcvx/packs/lightme/dems/run.sh

	# if the demon's exit code not equal 0
	if [ $? -ne 0 ]
	then
		echo "Demon crashed. Restarting..."
	else
		echo "Demon exited normally."
		break
	fi

	sleep 5
done
