# wifi_scan

## Install iw tool
```sh
sudo apt-get update
sudo apt-get install iw
```

## Build ,Compile, Install
```sh
mkdir build
cd build
cmake ..
make
make install
```

## Add access to py file
```sh
chmod +x scripts/scan.py
```

## Run
```sh
rosrun wifi_scan scan.py
```