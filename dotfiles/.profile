#!/bin/sh
[ -z "$ENVSET" ] && if [ -f ~/.rc ]; then
	export ENV=~/.rc
	. ~/.rc
fi
