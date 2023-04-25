#!/bin/bash

# parent_dir="/root/p2slam/TUM/Handheld/"

# # Use find command to get a list of all subdirectories under the parent directory
# directories=($(find "$parent_dir" -mindepth 1 -maxdepth 1 -type d))

# # Iterate over each directory and execute the commands
# for dir in "${directories[@]}"; do

#   echo "$dir Start"
#   # Get the directory name from the path
#   dir_name=$(basename "$dir")
  
#   if [[ $dir_name == *"freiburg1"* ]]; then
#     yaml_file="./Examples/RGB-D/TUM1.yaml"
#   elif [[ $dir_name == *"freiburg2"* ]]; then
#     yaml_file="./Examples/RGB-D/TUM2.yaml"
#   elif [[ $dir_name == *"freiburg3"* ]]; then
#     yaml_file="./Examples/RGB-D/TUM3.yaml"
#   fi

#   python ./evaluation/associate.py "$dir"/rgb.txt "$dir"/depth.txt > "$dir"/association.txt
#   ./Examples/RGB-D/rgbd_tum Vocabulary/ORBvoc.txt "$yaml_file" "$dir"/ "$dir"/association.txt
#   mv /root/Thesis_code/evaluateEigen/Result/${dir_name}_framePose.txt "$dir"/framePose.txt
#   mv /root/Thesis_code/evaluateEigen/Result/${dir_name}_frameEigen.txt "$dir"/frameEigen.txt
  
#   echo "$dir_name done"
# done


# # generate error files
# parent_dir="/root/p2slam/TUM/Handheld/"

# # Iterate over each directory and execute the commands
# for dir in "${directories[@]}"; do

#   echo "$dir Start Calculate Error"
#   python ./evaluateEigen/evaluate.py "$dir"/groundtruth.txt "$dir"/framePose.txt --offset 0 --scale 1 --verbose
#   mv trans_error.txt "$dir"/trans_error.txt
#   echo "$dir_name done"
# done


# parent_dir="/root/p2slam/TUM/Texture/"

# # Use find command to get a list of all subdirectories under the parent directory
# directories=($(find "$parent_dir" -mindepth 1 -maxdepth 1 -type d))

# # Iterate over each directory and execute the commands
# for dir in "${directories[@]}"; do

#   echo "$dir Start"
#   # Get the directory name from the path
#   dir_name=$(basename "$dir")
  
#   if [[ $dir_name == *"freiburg1"* ]]; then
#     yaml_file="./Examples/RGB-D/TUM1.yaml"
#   elif [[ $dir_name == *"freiburg2"* ]]; then
#     yaml_file="./Examples/RGB-D/TUM2.yaml"
#   elif [[ $dir_name == *"freiburg3"* ]]; then
#     yaml_file="./Examples/RGB-D/TUM3.yaml"
#   fi

#   python ./evaluation/associate.py "$dir"/rgb.txt "$dir"/depth.txt > "$dir"/association.txt
#   ./Examples/RGB-D/rgbd_tum Vocabulary/ORBvoc.txt "$yaml_file" "$dir"/ "$dir"/association.txt
#   mv /root/Thesis_code/evaluateEigen/Result/${dir_name}_framePose.txt "$dir"/framePose.txt
#   mv /root/Thesis_code/evaluateEigen/Result/${dir_name}_frameEigen.txt "$dir"/frameEigen.txt
  
#   echo "$dir_name done"
# done

# # Iterate over each directory and execute the commands
# for dir in "${directories[@]}"; do

#   echo "$dir Start Calculate Error"
#   python ./evaluateEigen/evaluate.py "$dir"/groundtruth.txt "$dir"/framePose.txt --offset 0 --scale 1 --verbose
#   mv trans_error.txt "$dir"/trans_error.txt
#   echo "$dir_name done"
# done


parent_dir="/root/p2slam/TUM/Robot"

# Use find command to get a list of all subdirectories under the parent directory
directories=($(find "$parent_dir" -mindepth 1 -maxdepth 1 -type d))

# Iterate over each directory and execute the commands
for dir in "${directories[@]}"; do

  echo "$dir Start"
  # Get the directory name from the path
  dir_name=$(basename "$dir")
  
  if [[ $dir_name == *"freiburg1"* ]]; then
    yaml_file="./Examples/RGB-D/TUM1.yaml"
  elif [[ $dir_name == *"freiburg2"* ]]; then
    yaml_file="./Examples/RGB-D/TUM2.yaml"
  elif [[ $dir_name == *"freiburg3"* ]]; then
    yaml_file="./Examples/RGB-D/TUM3.yaml"
  fi

  python ./evaluation/associate.py "$dir"/rgb.txt "$dir"/depth.txt > "$dir"/association.txt
  ./Examples/RGB-D/rgbd_tum Vocabulary/ORBvoc.txt "$yaml_file" "$dir"/ "$dir"/association.txt
  mv /root/Thesis_code/evaluateEigen/Result/${dir_name}_framePose.txt "$dir"/framePose.txt
  mv /root/Thesis_code/evaluateEigen/Result/${dir_name}_frameEigen.txt "$dir"/frameEigen.txt
  
  echo "$dir_name done"
done

# Iterate over each directory and execute the commands
for dir in "${directories[@]}"; do

  echo "$dir Start Calculate Error"
  python ./evaluateEigen/evaluate.py "$dir"/groundtruth.txt "$dir"/framePose.txt --offset 0 --scale 1 --verbose
  mv trans_error.txt "$dir"/trans_error.txt
  echo "$dir_name done"
done