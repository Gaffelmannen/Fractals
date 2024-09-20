#!/bin/sh
set -e

filename="lab"
type="3"
target_file_type="ppm"
output="data/out.mp4"


echo "Begin"
exec ./fractals ${type} ${filename}

echo "Creating video"
exec ffmpeg -framerate 30 -pattern_type glob -i 'data/*.'${target_file_type} -c:v libx264 -pix_fmt yuv420p ${output}

echo "Done"