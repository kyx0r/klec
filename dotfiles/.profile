#!/bin/sh
[ -z "$ENVSET" ] && if [ -f "$HOME"/.rc ]; then
	export ENV="$HOME"/.rc
	. "$HOME"/.rc
fi
