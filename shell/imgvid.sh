#!/bin/bash
set -x

function upscl()
{
	var=$(ls *.jpg)
	for p in $var;
	do
		waifu2x-ncnn-vulkan -i "$p" -o "$p" -n 2 &
		c=$((c+1))
		if [ $c == 2 ]
		then
			wait < <(jobs -p)
			c=0
		fi
	done
}

function tovid()
{
	ffmpeg -r 60 -i image-%03d.jpg -vcodec libx264 -crf 25  -pix_fmt yuv420p "$1"
}

function toimg()
{
	ffmpeg -i "$1" -r 60 image-%3d.jpg
}

function help()
{
        set +o xtrace
        echo " "
        echo "Target not found. Please specify target."
        echo " "
        echo "=====Command Line Options====="
        echo "* upscl            upscale and denoise images"
        echo "* tovid            images to video"
        echo "* toimg            video to images"
}

"$@"
help

