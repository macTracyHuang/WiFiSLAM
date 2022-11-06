#!/usr/bin/env python
import subprocess
import os
import time
import re
import warnings
import rospy
from wifi_scan.msg import Fingerprint, AddressRSSI

class Collection:

    def __init__(self):
        # WiFi data collection commands
        self.cmd0 = 'sudo iwlist wlo1 scan | egrep "Cell|ESSID|Signal"'
        self.cmd1 = 'sudo iwlist wlo1 scan | egrep "Cell|ESSID|Signal"'
        self.cmd2 = 'sudo iwlist wlo1 scan | egrep "Cell|Signal"'
        self.cmd3 = 'sudo iw dev wlo1 scan'

    def collect(self):

        # User has pushed 'start'
        data = []
        print('WiFi started.')
        try:
            stream = os.popen(self.cmd3)
            data = stream.read()
            # p = subprocess.check_output(self.cmd3, shell=True)
            # data = str(p.decode())
        except:
            print("\tinterface was busy")
            time.sleep(0.1)

        print("WiFi data collection complete. ")
        return data
    
    def extract_data(self, rawdata):
        # Build list of MAC addresses sensed in this sample (item).
        # Note: re.findall() returns list of found re's in the order of occurrence in the source string.
        adr = re.compile(r'\w\w:\w\w:\w\w:\w\w:\w\w:\w\w\(.*\)')
        addresses = adr.findall(rawdata)
        for i in range(len(addresses)):
            sa      = addresses[i]
            sc      = sa[:17]
            addresses[i] = sc
        # Build list of signal strengths.
        sig = re.compile(r'signal:.* dBm')
        sigs = sig.findall(rawdata)
        # Convert sigs from list of strings to list of ints
        for i in range(len(sigs)):
            sa      = sigs[i]
            sb      = sa.split()
            sc      = sb[1]
            sigs[i] = int(float(sc))

        # Make an entry to self.wifi_data that includes addresses and sigs
        assert len(addresses) == len(sigs)
        entry = [addresses, sigs]
        return entry


    def pub_data(self,data):
        pub = rospy.Publisher('wifi_fp', Fingerprint, queue_size=10)
        rospy.init_node('wifiScanner', anonymous=True)
        # r = rospy.Rate(10) #10hz

        msg = Fingerprint()

        for bssid, rssi in zip(data[0], data[1]):
            addrssi = AddressRSSI()
            addrssi.address = bssid
            addrssi.rssi = rssi
            msg.list.append(addrssi)
        
        msg.header.stamp = rospy.get_rostime()
        # rospy.loginfo(msg)
        pub.publish(msg)
        print("Number of AP: ", len(data[0]))
        # r.sleep()

if __name__ == "__main__":
    a = Collection()

    try:
        while True:
            rawdata = a.collect()

            if not rawdata:
                print("===== Warning: no scan result =====")
                continue
            
            data = a.extract_data(rawdata)
            # print(data)
            # print("=" * 50)
            # print(len(data[0]),len(data[1]))
            # data = [['3c:7c:3f:3e:a1:bc', '30:87:d9:31:72:4c', '30:87:d9:71:72:4c', '30:87:d9:b1:72:4c', 'c4:e9:0a:1d:37:92', '30:87:d9:32:06:4c', '30:87:d9:72:06:4c', '30:87:d9:31:7c:48', '30:87:d9:71:7c:48', '30:87:d9:b1:7c:48', '30:87:d9:f1:7c:48', '76:40:bb:09:fa:ad', '30:87:d9:31:7c:4c', '30:87:d9:71:7c:4c', '30:87:d9:b1:7c:4c', 'fa:8f:ca:89:71:7b', '30:87:d9:b2:06:4c', '30:87:d9:31:72:48', '30:87:d9:71:72:48', '30:87:d9:b1:72:48', '30:87:d9:f1:72:48', '30:87:d9:31:7f:6c', '30:87:d9:71:7f:6c', '30:87:d9:b1:7f:6c', '30:87:d9:72:38:8c', '30:87:d9:32:38:8c', '30:87:d9:71:58:a8', '30:87:d9:b1:7f:68', '30:87:d9:71:82:c8', '30:87:d9:72:06:48', '30:87:d9:31:9a:68', 'b0:6e:bf:3b:a8:38', '30:87:d9:31:97:a8', '30:87:d9:71:7f:68', '30:87:d9:f1:7f:68', '30:87:d9:71:59:a8', '30:87:d9:b1:59:a8', '30:87:d9:f1:59:a8', '30:87:d9:31:82:c8', '30:87:d9:31:59:a8', '30:87:d9:31:7f:68', '3c:7c:3f:3e:a1:b8', '30:87:d9:b2:06:48', '30:87:d9:f2:06:48', '30:87:d9:71:9a:68', '30:87:d9:b1:9a:68', 'fc:34:97:90:b0:d8', '00:24:6c:2d:1a:00', '00:24:6c:2d:1a:02', '00:24:6c:2d:1a:08', '00:24:6c:2d:1a:0a', '30:87:d9:b1:58:a8', '30:87:d9:f1:58:a8', '30:87:d9:31:58:a8', 'fc:34:97:90:b0:dc', '78:d2:94:7f:eb:14', '30:87:d9:31:58:ac', '30:87:d9:71:58:ac', '30:87:d9:b1:58:ac', '30:87:d9:b2:38:88', '78:d2:94:7f:eb:10', '30:87:d9:f1:97:a8', '30:87:d9:f1:82:c8', '30:87:d9:b1:82:c8', '30:87:d9:72:38:88', '30:87:d9:f2:38:88', '30:87:d9:31:81:68', '00:24:6c:26:89:21', '30:87:d9:b2:38:8c', '34:97:f6:6b:eb:a4', '2c:30:33:d1:95:88', '00:24:6c:2d:1a:01', '00:24:6c:26:8b:58', '00:24:6c:26:89:22', '30:87:d9:f1:d3:68', '00:24:6c:26:8b:59', '34:97:f6:6b:eb:a0', '30:87:d9:31:76:08', '30:87:d9:f1:9a:68', '30:87:d9:32:06:48', 'a0:ab:1b:89:a7:9e', '7c:10:c9:5d:99:d4', '36:0a:33:43:77:60', '30:87:d9:32:38:88', '00:24:6c:2d:1a:09', '30:87:d9:b1:97:a8', '30:87:d9:71:97:a8', 'e8:48:b8:7f:19:ce', 'c4:e9:0a:1d:37:90', '34:0a:33:03:77:60', '30:87:d9:31:99:08', '30:87:d9:b1:76:08', '30:87:d9:f1:76:08', '30:87:d9:71:76:08', '2c:30:33:d1:95:89', '30:87:d9:31:54:88', '30:87:d9:f1:54:88', '36:0a:33:63:77:60', '00:24:6c:26:8b:5a', '30:87:d9:31:99:0c', '30:87:d9:71:99:0c', '30:87:d9:b1:99:0c', '30:87:d9:31:9a:6c', '30:87:d9:71:9a:6c', '30:87:d9:b1:9a:6c', '50:c7:bf:af:dd:61', '30:87:d9:31:76:0c', '30:87:d9:71:76:0c', '30:87:d9:b1:76:0c', '30:87:d9:31:54:8c', '30:87:d9:71:54:8c', '30:87:d9:b1:54:8c', '50:c7:bf:af:dd:62', '30:87:d9:f1:55:88', '30:87:d9:71:99:08', '30:87:d9:b1:99:08', '30:87:d9:f1:99:08', '7c:10:c9:5d:99:d0', 'b0:6e:bf:3b:a8:3c', '30:87:d9:31:76:ac', '30:87:d9:71:76:ac', '30:87:d9:b1:76:ac'], [-45, -69, -70, -69, -74, -80, -80, -41, -41, -41, -41, -58, -57, -57, -57, -58, -80, -60, -60, -60, -60, -63, -63, -63, -88, -88, -59, -56, -67, -63, -69, -57, -71, -56, -56, -67, -69, -69, -69, -69, -54, -40, -64, -64, -68, -67, -53, -52, -67, -73, -73, -58, -58, -59, -77, -75, -79, -79, -79, -70, -54, -59, -69, -69, -70, -71, -80, -80, -89, -85, -85, -51, -89, -78, -83, -88, -67, -71, -67, -63, -76, -84, -70, -70, -73, -74, -73, -71, -59, -56, -70, -73, -73, -72, -63, -74, -74, -70, -90, -77, -76, -75, -72, -72, -73, -90, -90, -90, -90, -87, -88, -88, -78, -84, -70, -70, -70, -66, -81, -88, -89, -88]]
            a.pub_data(data)
    except KeyboardInterrupt:
        exit()

    with warnings.catch_warnings():
        warnings.filterwarnings("ignore")