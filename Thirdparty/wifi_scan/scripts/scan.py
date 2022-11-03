#!/usr/bin/env python
import subprocess
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
        data = ""
        print('WiFi started.')
        try:
            p = subprocess.check_output(self.cmd3, shell=True)
            data = p.decode()
        except subprocess.CalledProcessError:
            print("\tinterface was busy")
            time.sleep(0.1)

        print("WiFi data collection complete.")
        return data
    
    def extract_data(self, rawdata):
        for item in rawdata:
            # Build list of MAC addresses sensed in this sample (item).
            # Note: re.findall() returns list of found re's in the order of occurrence in the source string.
            adr = re.compile(r'\w\w:\w\w:\w\w:\w\w:\w\w:\w\w\(.*\)')
            addresses = adr.findall(item)
            for i in range(len(addresses)):
                sa      = addresses[i]
                sc      = sa[:17]
                addresses[i] = sc
            # Build list of signal strengths.
            sig = re.compile(r'signal:.* dBm')
            sigs = sig.findall(item)
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

        for bssid, rssi in data:
            addrssi = AddressRSSI()
            addrssi.address = bssid
            addrssi.rssi = rssi
            msg.list.append(addrssi)
        
        msg.header.stamp = rospy.get_rostime()
        rospy.loginfo(msg)
        pub.publish(msg)
        # r.sleep()

if __name__ == "__main__":
    a = Collection()

    try:
        for i in range(10):
            rawdata = a.collect()

            if not rawdata:
                print("no scan result")
                continue
            
            data = a.extract_data(rawdata)
            # data = [['xx:xx:xx:xx:xx:xx',-30]]
            a.pub_data(data)
    except KeyboardInterrupt:
        pass

    with warnings.catch_warnings():
        warnings.filterwarnings("ignore")